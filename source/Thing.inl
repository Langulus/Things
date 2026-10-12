///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Thing.hpp>
#include <Langulus/Part.hpp>
#include <Langulus/Neat.hpp>


namespace Langulus::Things
{
   /// Create a root thing (convenience function)                             
   ///   @tparam CREATE_FLOW should a temporal flow be created in the root?   
   ///   @param modules... names of modules to load                           
   ///   @return a root thing, with a runtime, a flow, and a name "ROOT"      
   template<bool CREATE_FLOW>
   Thing Thing::Root(CT::Text auto&&...modules) {
      Thing root;
      root.SetName("ROOT");
      root.CreateRuntime();
      if constexpr (CREATE_FLOW)
         root.CreateFlow();
      (root.LoadMod(modules), ...);
      root.Reference(-1);
      return Abandon(root);
   }

   /// Create a child Thing with the provided arguments                       
   ///   @param arguments instructions for the thing's creation               
   ///   @return the new child instance                                       
   template<class...T>
   Ref<Thing> Thing::CreateChild(T&&...arguments) {
      THINGS_VERBOSE_SELF_TAB(
         "Producing child (at ", GetReferences(), " references): ");
      Ref<Thing> newThing;
      newThing.Emplace(this, Many {LglsFwd(arguments)...});
      return Abandon(newThing);
   }

   /// Inner creation routine                                                 
   ///   @tparam T type of instructions for creation                          
   ///   @param verb original create verb to output to                        
   ///   @param stuff creation instructions                                   
   template<class T>
   void Thing::CreateInner(Verb& verb, T const& stuff) {
      if constexpr (CT::Deep<T> or CT::Neat<T>) {
         // Nest if deep/neat                                           
         stuff.ForEachDeep(
            [this, &verb](Tag const& trait) {
               verb << AddTrait(trait);
            },
            [this, &verb](Recipe const& construct) {
               CreateInner(verb, construct);
            },
            [this, &verb](Neat const& neat) {
               CreateInner(verb, neat);
            },
            [this, &verb](DMeta const& type) {
               CreateInner(verb, type);
            }
         );
      }
      else if constexpr (Exact<T, DMeta>) {
         // Instantiate a type without any arguments                    
         if (stuff->template Is<Thing>()) {
            // Instantiate a child Thing                                
            verb << CreateChild();
         }
         else if (stuff->template Is<Runtime>()) {
            // Instantiate a runtime                                    
            verb << CreateRuntime();
         }
         else if (stuff->template Is<Temporal>()) {
            // Instantiate a temporal flow                              
            verb << CreateFlow();
         }
         else if (stuff->template CastsTo<Things::Module>()) {
            // Instantiate a module from the runtime                    
            auto& runtime = GetRuntime();
            auto dependency = runtime->GetDependency(stuff);
            verb << runtime->InstantiateModule(dependency);
         }
         else {
            // Instantiate anything else                                
            verb << CreateData(Recipe {stuff});
         }
      }
      else if constexpr (CT::Recipe<T>) {
         // Instantiate a type, with charge and arguments               
         const auto count = static_cast<int>(std::ceil(stuff.GetCharge().mMass));
         for (int i = 0; i < count; ++i) {
            if (count != 1) {
               THINGS_CREATION_VERBOSE_SELF(Logger::Yellow,
                  "Charged creation - creating ", i + 1, " of ", count);
            }

            if (stuff.template Is<Thing>()) {
               // Instantiate a child Thing                             
               verb << CreateChild(stuff.GetDescriptor());
            }
            else if (stuff.template Is<Runtime>()) {
               // Instantiate a runtime                                 
               verb << CreateRuntime();
            }
            else if (stuff.template Is<Temporal>()) {
               // Instantiate a temporal flow                           
               verb << CreateFlow();
            }
            else if (stuff.template CastsTo<Things::Module>()) {
               // Instantiate a module from the runtime                 
               auto& runtime = GetRuntime();
               auto dependency = runtime->GetDependency(stuff.GetType());
               verb << runtime->InstantiateModule(dependency, stuff.GetDescriptor());
            }
            else {
               // Instantiate anything else                             
               verb << CreateData(stuff);
            }
         }
      }
      else static_assert(false, "Unsupported descriptor");
   }

   /// Add a child                                                            
   ///   @attention assumes entity is a valid pointer                         
   ///   @tparam TWOSIDED true to also set the entity's owner;                
   ///      used mainly internally to avoid endless loops                     
   ///   @param entity instance to add as child                               
   ///   @return the number of added children                                 
   template<bool TWOSIDED>
   size_t Thing::AddChild(Thing* entity) {
      LglsAssumeUser(entity, "Bad entity pointer");

      const auto added = mChildren.Merge(entity);
      if constexpr (TWOSIDED) {
         if (added) {
            if (entity->mOwner != this) {
               if (entity->mOwner)
                  entity->mOwner->RemoveChild<false>(entity);

               entity->mOwner = this;
               entity->mRefreshRequired = true;
               THINGS_VERBOSE_SELF(entity, "'s owner overwritten");
            }

            THINGS_VERBOSE_SELF(entity, " added as child");
         }
      }

      return added;
   }
      
   /// Remove a child that matches pointer                                    
   ///   @attention assumes entity is a valid pointer                         
   ///   @tparam TWOSIDED true to also remove entity's owner;                 
   ///      used mainly internally to avoid endless loops                     
   ///   @param entity instance to remove from children                       
   ///   @return the number of removed children                               
   template<bool TWOSIDED>
   size_t Thing::RemoveChild(Thing* entity) {
      LglsAssumeUser(entity, "Bad entity pointer");
      
      const auto removed = mChildren.Erase(entity);
      if constexpr (TWOSIDED) {
         if (removed) {
            if (entity->mOwner == this) {
               entity->mOwner = nullptr;
               entity->mRefreshRequired = true;
               THINGS_VERBOSE_SELF(entity, "'s owner overwritten");
            }

            THINGS_VERBOSE_SELF(entity, " removed from children");
         }
      }

      return removed;
   }

   /// Execute verb in the hierarchy, searching for valid context in the      
   /// given direction                                                        
   ///   @tparam SEEK the direction in which to seek a valid context          
   ///   @param verb the verb to execute                                      
   ///   @return verb output                                                  
   template<Seek SEEK, CT::Executable V>
   V& Thing::RunIn(V& verb) {
      if constexpr (SEEK & Seek::Here) {
         // Execute here                                                
         if (Run(verb).IsDone())
            return verb;
      }

      if constexpr (SEEK & Seek::Above) {
         // Execute in parents up to root, if requested                 
         if (mOwner and mOwner->template
         RunIn<Seek::HereAndAbove>(verb).IsDone())
            return verb;
      }

      if constexpr (SEEK & Seek::Below) {
         // Execute in children, if requested                           
         for (auto& child : mChildren) {
            V local = verb;
            local.ShortCircuit(false);
            verb << Abandon(child->template
               RunIn<Seek::HereAndBelow>(local).GetOutput());
         }
      }

      return verb;
   }

   /// Execute verb in this thing only, scanning units for required verbs     
   ///   @param verb the verb to execute                                      
   ///   @return verb output                                                  
   template<CT::Executable V>
   V& Thing::Run(V& verb) {
      try {
         // Dispatch to entity first, using reflected and default verbs,
         // but disallowing custom dispatch, because we're currently in 
         // it and there's a potential for infinite regress             
         Resolvable::Run<false>(verb);
      }
      catch (...) {}

      if (verb.IsDone())
         return verb;

      // If verb is still not satisfied, dispatch to ALL units          
      for (auto& unit : mUnitsList) {
         try {
            V local = verb;
            local.ShortCircuit(false);
            unit->Run(local);

            if (local.IsDone()) {
               // The local verb may or may not have an output, but a   
               // success should always be carried over                 
               verb.Done();
               verb << Abandon(local.GetOutput());
            }
         }
         catch (...) {}
      }

      return verb;
   }
   
   /// Register part by all its bases in mPartsAmbiguous                      
   ///   @param part the part instance to register                            
   ///   @param type type to register the part as                             
   inline void Thing::AddPartBases(Part* part, DMeta type) {
      const auto found = mPartsAmbiguous.Find(type);
      if (found)
         found.GetVal() << part;
      else
         mPartsAmbiguous.Merge(type, part);

      for (auto& base : type->mBases) {
         if (base.mType->IsExact<Part>())
            break;

         AddPartBases(part, base.mType);
      }
   }

   /// Unregister part by all its bases in mPartsAmbiguous                    
   ///   @param part the part instance to unregister                          
   ///   @param type the type to unregister the part as                       
   inline void Thing::RemovePartBases(Part* part, DMeta type) {
      const auto found = mPartsAmbiguous.Find(type);
      if (found) {
         auto& set = found.GetVal();
         if (set.Erase(part) and not set)
            mPartsAmbiguous.Erase(found);
      }

      for (auto& base : type->mBases) {
         if (base.mType->IsExact<Part>())
            break;

         RemoveUnitBases(part, base.mType);
      }
   }

   /// Add a new part to the thing.                                           
   /// Adding parts coupled with different runtimes is not allowed.           
   /// You can only duplicate parts, if they're unique instances memorywise.  
   ///   @tparam TWOSIDED if true, will both add part to thing, and add       
   ///      the thing as an owner for the part; used mainly internally to     
   ///      avoid endless loops                                               
   ///   @param part part to add                                              
   ///   @return 1 if part has been added                                     
   template<bool TWOSIDED>
   size_t Thing::AddPart(Part* part) {
      // Check if the unit instance is already registered here          
      const auto meta = part->GetType();
      const auto found = mPartsAmbiguous.Find(meta);
      if (found and found.GetVal().Find(part))
         return 0;

      // We must guarantee, that no unit is coupled to entities with    
      // different runtimes!                                            
      for (const auto& owners : part->mOwners) {
         LglsAssert(owners->GetRuntime() == GetRuntime(),
            "Coupling a part to multiple runtimes is not allowed"
         );
      }

      // Log self before unit being added, it might change name         
      THINGS_VERBOSE_SELF("");

      if constexpr (TWOSIDED) {
         #if LANGULUS(SAFE)
            part->mOwners <<= this;
         #else
            part->mOwners << this;
         #endif
      }

      mPartsList << part;
      AddPartBases(part, meta);
      mRefreshRequired = true;

      THINGS_VERBOSE(
         part, " added as part (now at ", GetReferences(), " references)");
      return 1;
   }

   /// Remove part instance from the thing                                    
   ///   @tparam TWOSIDED if true, will both remove part from thing, and      
   ///      then remove the thing from part's owners; used mainly internally  
   ///      to avoid endless loops                                            
   ///   @param part part to remove from the entity                           
   ///   @return 1 if part has been removed                                   
   template<bool TWOSIDED>
   size_t Thing::RemovePart(Part* part) {
      const auto meta = part->GetType();
      const auto foundType = mPartsAmbiguous.Find(meta);
      if (not foundType)
         return 0;

      const auto partIndex = foundType.GetVal().Find(part);
      if (partIndex) {
         // Decouple before part is destroyed                           
         if constexpr (TWOSIDED)
            part->mOwners.Remove(this);

         // Notify all other parts about the environment change         
         mRefreshRequired = true;
         THINGS_VERBOSE_SELF(part, " removed from parts");

         // Dereference (and eventually destroy) part                   
         RemovePartBases(part, meta);
         mPartsList.Remove(part);
         return 1;
      }

      return 0;
   }

   /// Remove all parts that are derived from the provided type               
   ///   @tparam T the type of parts to remove, use Part for all              
   ///   @tparam TWOSIDED if true, will both remove part from thing, and      
   ///      then remove the thing from part's owners; used mainly internally  
   ///      to avoid endless loops                                            
   ///   @return the number of removed parts                                  
   template<CT::Part T, bool TWOSIDED>
   size_t Thing::RemoveParts() {
      if constexpr (CT::Same<T, Part>) {
         // Remove all parts                                            
         const auto removed = mPartsList.GetCount();
         if constexpr (TWOSIDED) {
            // Decouple before parts are (potentially) destroyed        
            for (auto& part : mPartsList)
               part->mOwners.Remove(this);
         }

         mPartsList.Reset();
         mPartsAmbiguous.Reset();
         mRefreshRequired = true;
         THINGS_VERBOSE_SELF("All ", removed, " parts were removed");
         return removed;
      }
      else {
         // Remove units of a specific type                             
         const auto meta = MetaOf<Decay<T>>();
         const auto found = mPartsAmbiguous.Find(meta);
         if (not found)
            return 0;

         // List intentionally shallow-copied, its memory will diverge  
         // upon RemovePart                                             
         auto list = found.GetVal();
         size_t removed {};
         for (auto& part : list)
            removed += RemovePart(part);
         return removed;
      }
   }

   /// Count the number of matching parts in this entity                      
   ///   @tparam T the type of parts to seach for, use Part for all           
   ///   @return the number of matching parts                                 
   template<CT::Part T>
   size_t Thing::HasParts() const {
      return HasParts(MetaOf<Decay<T>>());
   }

   /// Get the list of parts, in order of addition                            
   ///   @return a reference to the list of parts                             
   inline auto Thing::GetParts() const noexcept -> PartList const& {
      return mPartsList;
   }

   /// Get the ambiguous map of parts                                         
   ///   @return a reference to the map of parts                              
   inlineauto Thing::GetPartsMap() const noexcept -> PartMap const& {
      return mPartsAmbiguous;
   }

   /// Create a part by static type and arguments, relying on producers       
   /// in the hierarchy                                                       
   ///   @tparam T the part to create                                         
   ///   @tparam A... arguments for the part's creation                       
   ///   @param arguments... the arguments to provide for construct           
   ///   @return the created part(s)                                          
   template<CT::Part T, class...A>
   Many Thing::CreatePart(A&&...arguments) {
      return CreateData(Recipe::From<Decay<T>>(LglsFwd(arguments)...));
   }

   /// Create default-initialized instances of each part type                 
   ///   @tparam T... the part types to instantiate                           
   ///   @return the created part(s)                                          
   template<CT::Part...T>
   Many Thing::CreateParts() {
      Many result;
      (result.Compose(CreatePart<T>()), ...);
      return result;
   }

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   /// Create a part by type token and arguments, relying on producers        
   /// in the hierarchy                                                       
   ///   @tparam A... arguments for the part's creation                       
   ///   @param token the part type token                                     
   ///   @param arguments... the arguments to provide for construct           
   ///   @return the created part(s)                                          
   template<class... A>
   Many Thing::CreatePartToken(Token const& token, A&&...arguments) {
      return CreateData(Recipe::FromToken(token, LglsFwd(arguments)...));
   }
#endif

   /// Get a part by a static type and an optional offset                     
   ///   @tparam T the type of part we're searching for                       
   ///   @param offset optional offset (Nth match)                            
   ///   @return the part if found, or nullptr if not                         
   template<CT::Part T, class SELF>
   auto Thing::GetPart(this SELF&& self, Index offset) -> Tmut<SELF, Decay<T>*, Decay<T> const*> {
      if constexpr (not Same<T, Part>) {
         return static_cast<Decay<T>*>(
            GetPartMeta(MetaOf<Decay<T>>(), offset)
         );
      }
      else return static_cast<Decay<T>*>(GetPartMeta(DMeta {}, offset));
   }

   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      /// Get a part by a token, and an optional offset (const)               
      /// This is available only if managed reflection feature is enabled     
      ///   @param token part type token                                      
      ///   @param offset optional offset (Nth match)                         
      ///   @return the part if found, or nullptr if not                      
      Part const* Thing::GetPartMeta(Token const& token, Index offset) const {
         return const_cast<Thing*>(this)->GetPartMeta(token, offset);
      }

      /// Get a part by a token, and an optional offset, then dynamic_cast it 
      /// This is available only if managed reflection feature is enabled     
      ///   @tparam T the type of part we're casting to                       
      ///   @param token part type token                                      
      ///   @param offset optional offset (Nth match)                         
      ///   @return the part if found, or nullptr if not                      
      template<CT::Part T>
      Decay<T>* Thing::GetPartAs(Token const& token, Index offset) {
         return dynamic_cast<Decay<T>*>(GetPartMeta(token, offset));
      }
   #endif

   template<CT::DefineTag T>
   Tag Thing::GetTag(Index offset) {
      return GetTag(MetaTagOf<T>(), offset);
   }

   /// Get local tag by static type and offset                                
   ///   @tparam T the tag to search for                                      
   ///   @param offset the offset of the tag to return (optional)             
   ///   @return the tag or nullptr if none found                             
   template<CT::DefineTag T>
   Tag* Thing::GetLocalTag(this auto&& self, Index offset) {
      return self.GetLocalTag(MetaTagOf<T>(), offset);
   }

   /// Produce constructs (including parts) from the hierarchy                
   ///   @attention assumes construct has a valid type                        
   ///   @tparam SEEK what part of the hierarchy to use for the creation      
   ///   @param construct instructions for the creation of the data           
   ///   @return created data                                                 
   template<Seek SEEK>
   Many Thing::CreateData(Recipe const& recipe) {
      LglsAssumeUser(recipe.GetTarget(), "Invalid construct type");
      const auto type = recipe.GetTarget();
      const auto producer = type and type->mProducerRetriever
         ? type->mProducerRetriever() : nullptr;
      Recipe descriptor = recipe;

      THINGS_VERBOSE_SELF(
         "Acting as producer context for making `", 
         type, "` (at ", GetReferences(), " references)"
      );

      // Implicitly add a parent tag to descriptor, if one isn't        
      // already added - it will be stripped later, when normalizing    
      // the descriptor when producing the item from a factory          
      Tags::Parent parent;
      if (not descriptor->ExtractTrait<Tags::Parent>(parent)) {
         parent = this;
         parent.MakeMissing();
         descriptor << parent;
         THINGS_VERBOSE_SELF(
            "Referenced as Tags::Parent (now at ",
            GetReferences(), " references)"
         );
      }
      else if (parent.IsMissing())
         parent << this;

      if (producer) {
         // Data has a specific producer, we can narrow the required    
         // contexts for creation a lot                                 
         if (producer->template CastsTo<Part>()) {
            // Data is producible from a unit                           
            auto producers = GatherParts<SEEK>(producer);
            LANGULUS_ASSERT(producers, Construct, 
               "No producers", " (of unit type `", producer, "`) available "
               "in hierarchy for construct: ", construct
            );

            // Potential unit producers found, attempt creation         
            producers.MakeOr();
            Verbs::Create creator {descriptor};
            if (Flow::DispatchFlat(producers, creator))
               return Abandon(creator.GetOutput());

            Logger::Error(
               "Failed to create `", Logger::PushDarkYellow, type, Logger::Pop,
               "` in unit(s) ", Logger::PushDarkYellow, producers, Logger::Pop,
               ": ", descriptor
            );
         }
         else if (producer->template CastsTo<Things::Module>()) {
            // Data is producible from a module                         
            auto producers = GetRuntime()->GetModules(producer);
            LANGULUS_ASSERT(producers, Construct,
               "No producers", " (of module type `", producer, "`) available "
               "in hierarchy for construct: ", construct
            );

            // Potential module producers found, attempt creation       
            producers.MakeOr();
            Verbs::Create creator {descriptor};
            if (Flow::DispatchFlat(producers, creator))
               return Abandon(creator.GetOutput());

            Logger::Error(
               "Failed to create `", Logger::PushDarkYellow, type, Logger::Pop,
               "` in module(s) ", Logger::PushDarkYellow, producers, Logger::Pop,
               ": ", descriptor
            );
         }
         else if (producer->template CastsTo<Thing>()) {
            // Data is producible from a thing                          
            TODO();
         }
      }
      else if (type->mIsAbstract and not type->mConcreteRetriever) {
         // Data doesn't have a specific producer, but it is abstract   
         // so we know that only a module/unit can concretize it        
         // Gather all units in the desired part of the hierarchy       
         auto producers = GatherParts<Part, SEEK>();
         if (producers) {
            // Potential unit producers found, attempt creation there   
            producers.MakeOr();
            Verbs::Create creator {descriptor};
            if (Flow::DispatchFlat(producers, creator))
               return Abandon(creator.GetOutput());

            Logger::Error(
               "Failed to create abstract `", Logger::PushDarkYellow, type, Logger::Pop,
               "` in unit(s) ", Logger::PushDarkYellow, producers, Logger::Pop,
               ": ", descriptor
            );
         }

         //TODO try modules too?
         TODO();
      }
      else {
         // Data is not abstract, nor has a producer, so just make it   
         // right here if possible, passing the descriptor over, if     
         // such constructor is reflected. If it's a unit, its          
         // descriptor is resposible for registering it with the parent 
         // via the Traits::Parent trait                                
         Verbs::Create creator {descriptor};
         if (Verbs::Create::ExecuteStateless(creator))
            return Abandon(creator.GetOutput());
      }

      LANGULUS_THROW(Construct, "Unable to create data");
      return {};
   }

   /// Execute verb in all owners, seeking valid context in the specified     
   /// seek direction                                                         
   ///   @tparam SEEK - direction to seek valid execution context in          
   ///   @param verb - the verb to execute                                    
   ///   @return the verb output                                              
   template<Seek SEEK, CT::VerbBased V>
   V& Part::RunIn(V& verb) {
      if (not mOwners) {
         Logger::Warning(Self(), "No owners available for executing: ", verb);
         return verb;
      }

      // Dispatch to all owner, accumulate outputs                      
      for (auto& owner : mOwners) {
         V local = verb;
         local.ShortCircuit(false);
         verb << Abandon(owner->template RunIn<SEEK>(local).GetOutput());
      }

      return verb;
   }
}