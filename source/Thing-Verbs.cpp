///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/Thing.hpp>
#include <Langulus/Temporal.hpp>
#include <Langulus/Verbs/Conjunct.hpp>
#include <Langulus/CppAPI/AI.hpp>
#include "Thing.inl"


namespace Langulus::Things
{
   /// Interpret and execute a natural language prompt wherever possible in   
   /// the current context.                                                   
   ///   @param text text to execute                                          
   ///   @return the results of the execution                                 
   Many Thing::Say(Text const& text) {
      if (not text)
         return {};

      // Get minds from the current context                             
      auto minds = GatherParts<Things::Mind, Seek::HereAndAbove>();
      if (not minds) {
         Logger::Error(this, ": No minds - can't interpret prompt: ", text);
         return {};
      }

      // Get a flow for executing the interpreted messages              
      auto flow = GetFlow();
      if (not flow) {
         Logger::Error(this, ": No flow - can't execute prompt: ", text);
         return {};
      }

      // Interpret by each mind and then execute in flow                
      Many results;
      for (auto& mind : minds) {
         auto interpretation = mind->Interpret(text);
         if (interpretation)
            results << flow->Push(interpretation);
      }

      return Abandon(results);
   }
   
   /// Executes a piece of code in the current context                        
   ///   @param code code to execute                                          
   ///   @return the results of the execution, if any                         
   Many Thing::Run(Code const& code) {
      if (not code)
         return {};

      // Parse the code                                                 
      auto parsed = code.Parse();
      if (not parsed)
         return {};

      // Push and execute in the active flow                            
      auto flow = GetFlow();
      if (not flow) {
         Logger::Error(this, ": No flow - can't Run code: ", code);
         return {};
      }

      return flow->Push(Verbs::Do(Abandon(parsed)).In(this));
   }

   /// Custom dispatcher.                                                     
   /// Entity is a composite type, and its behavior depends on its parts,     
   /// so the custom dispatcher forwards verbs to these parts                 
   ///   @param verb any verb                                                 
   void Thing::Do(Verb& verb) {
      // Execute in the thing itself first. This will satisfy tag       
      // selection, part selection, etc.                                
      bool unusedSkipVerbs = false;
      Many flow = verb.GetRHS();
      Many output;
      if (Flow::Execute(flow, this, output, false, unusedSkipVerbs)) {
         verb.SetOutput(Abandon(output));
         verb.Done();
         return;
      }

      // If verb is still not satisfied, dispatch to ALL parts.         
      for (auto& part : mPartsList) {
         const Many concrete_part {Annies::Absorb, part->Downcast()};
         if (Flow::Execute(flow, concrete_part, output, false, unusedSkipVerbs)) {
            // The execution may or may not have an output, but a       
            // success should always be carried over                    
            verb << Move(output);
            verb.Done();
         }
      }
   }

   /// Execute verb in the hierarchy, searching for valid context in the      
   /// given direction                                                        
   ///   @tparam SEEK the direction in which to seek a valid context          
   ///   @param verb the verb to execute                                      
   ///   @return verb output                                                  
   /*template<Seek SEEK>
   void Thing::DoIn(Verb& verb) {
      if constexpr (SEEK & Seek::Here) {
         // Execute here                                                
         Do(verb);

         if (verb.IsDone())
            return;
      }

      if constexpr (SEEK & Seek::Above) {
         // Execute in parents up to root, if requested                 
         if (mOwner) { 
            mOwner->template DoIn<Seek::HereAndAbove>(verb);
            if (verb.IsDone())
               return verb;
         }
      }

      if constexpr (SEEK & Seek::Below) {
         // Execute in children, if requested                           
         for (auto& child : mChildren) {
            auto local = verb;
            child->template RunIn<Seek::HereAndBelow>(local);

            if (local.IsDone()) {
               verb << Abandon(local.GetOutput());

               // Some verbs are short-circuited                        
               if (local.IsShortCircuited())
                  return;
            }
         }
      }
   }*/

   /// Create/Destroy stuff inside entity's context                           
   ///   @param verb creation verb                                            
   void Thing::Create(Verb& verb) {
      if (verb)
         CreateInner(verb.GetOutput(), verb.GetRHS());
   }

   /// Pick something from the entity - children, tags, parts, modules        
   /// If this entity doesn't satisfy the query, a search will be performed   
   /// in all parents, climbing the hierarchy                                 
   ///   @param verb selection verb                                           
   void Thing::Select(Verb& verb) {
      // Probe every part of the argument and check if it matches       
      TMany<Tag>    selectedTags;
      TMany<Part*>  selectedParts;
      TMany<Thing*> selectedThings;
      bool mismatch = false;

      const auto selectTag = [&](TMeta const& tag) {
         auto found = GetTag(tag);
         if (not found) {
            mismatch = true;
            return Loop::Break;
         }

         selectedTags << Abandon(found);
         return Loop::Continue;
      };

      const auto selectPart = [&](DMeta const& type) {
         if (type.Is<Thing>())
            return Loop::Continue;

         auto found = GetPartMeta(type);
         if (not found) {
            mismatch = true;
            return Loop::Break;
         }

         selectedParts << found;
         return Loop::Continue;
      };

      const auto selectRecipe = [&](Recipe const& recipe) {
         if (recipe.GetTarget().Is<Thing>()) {
            // Find a thing containing recipe arguments.                
            // Start with this one:                                     
            Verbs::Select selector {recipe.GetDescriptor()};
            Select(selector);
            if (selector.GetOutput()) {
               selectedThings << this;
               return Loop::Continue;
            }
         }
         else if (recipe.CastsTo<Part>()) {
            // Find a part containing recipe arguments                  
            if (not selectPart(recipe.GetTarget()))
               return Loop::Break;

            // selectedComponents has been populated with results       
            // Filter them additionally by recipe arguments          
            TMany<Part*> filteredSelectedComponents;
            for (auto& part : selectedParts) {
               bool localMismatch = false;
               auto resolved = part->GetResolved();
               recipe->ForEach(
                  [&](Many const& part) {
                     for (size_t i = 0; i < part.GetCount(); ++i) {
                        auto element = part.GetElementResolved(i);
                        Verbs::Select selector {element};
                        if (not Flow::DispatchFlat(resolved, selector)) {
                           // Abort on first failure                    
                           localMismatch = true;
                           return Loop::Break;
                        }
                     }

                     return Loop::Continue;
                  }
               );

               if (not localMismatch) {
                  // The part passes the test                           
                  filteredSelectedComponents << part;
               }
            }

            // Substitute the selected parts with the filtered ones     
            selectedParts = Move(filteredSelectedComponents);
         }

         return Loop::Break;
      };

      verb.ForEachDeep(
         [&](Recipe const& recipe) {
            return selectRecipe(recipe);
         },
         [&](Tag const& tag) {
            return selectTag(tag.GetTag());
         },
         [&](TMeta const& tag) {
            return selectTag(tag);
         },
         [&](DMeta const& type) {
            return selectPart(type);
         }
      );

      if (not mismatch) {
         // We're not seeking an entity, but components/tags          
         if (selectedTags) {
            THINGS_SELECTION_VERBOSE_SELF(Logger::Green,
               "Tag(s) selected: ", selectedTags);
            verb << selectedTags;
         }

         if (selectedParts) {
            THINGS_SELECTION_VERBOSE_SELF(Logger::Green,
               "Part(s) selected: ", selectedParts);
            verb << selectedParts;
         }

         if (selectedThings) {
            THINGS_SELECTION_VERBOSE_SELF(Logger::Green,
               "Entity(s) selected: ", selectedThings);
            verb << selectedThings;
         }
      }

      // Climb the hierarchy                                            
      if (not verb.IsDone() and mOwner)
         mOwner->Select(verb);
   }
}
