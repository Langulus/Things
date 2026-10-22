///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/Part.hpp>
#include "Thing.inl"

using namespace Langulus::Things;


/// Part destructor decouples part from all owners                            
Part::~Part() {
   for (auto entity : mOwners)
      Decouple(*entity);
}

/// Default part selection simply relays to the owner                         
///   @param verb the selection verb                                          
void Part::Select(Verb& verb) {
   for (auto owner : mOwners)
      owner->Select(verb);
}
   
/// Check if this part has a given set of properties                          
///   @param descriptor descriptor with required properties                   
///   @return true if the part has the given properties                       
bool Part::CompareDescriptor(Many const& descriptor) const {
   // First we compare tags only, all of them must be present           
   bool mismatch = false;
   size_t memberOffset = 0;
   const auto concrete = Downcast();
   descriptor.ForEachDeep([&concrete,&memberOffset,&mismatch](Tag const& tag) {
      const auto member = concrete.GetMember(tag.GetTag(), {}, memberOffset);
      if (not member or Many {Annies::Absorb, member}.CompareEqual(tag.GetData())) {
         mismatch = true;
         return Loop::Break;
      }

      ++memberOffset;
      return Loop::Continue;
   });

   // Then we run another check based on data types, again - all        
   // of them must be present, either as tags or in other form          
   memberOffset = 0;
   descriptor.ForEachDeep([&concrete,&memberOffset,&mismatch](Many const& anythingElse) {
      const auto member = concrete.GetMember({}, anythingElse.GetType(), memberOffset);
      if (not member or Many {Annies::Absorb, member}.CompareEqual(anythingElse)) {
         mismatch = true;
         return Loop::Break;
      }

      ++memberOffset;
      return Loop::Continue;
   });

   return not mismatch;
}
   
/// Get the list of part owners                                               
///   @return the owners                                                      
auto Part::GetOwners() const noexcept -> Hierarchy const& {
   return mOwners;
}
     
/// Refresh part on environment change                                        
void Part::Refresh() {}

/// Get the runtime                                                           
///   @attention assumes parts are correctly coupled and coupling to          
///      different runtimes should be explicitly disallowed.                  
///   @return a pointer to the runtime, if available                          
auto Part::GetRuntime() const noexcept -> Runtime* {
   if (not mOwners)
      return nullptr;
   return mOwners->GetRuntime();
}

/// Get the temporal flow                                                     
///   @attention assumes parts are correctly coupled and coupling to          
///      different flows should be explicitly disallowed.                     
///   @return a pointer to the flow, if available                             
auto Part::GetFlow() const noexcept -> Temporal* {
   if (not mOwners)
      return nullptr;
   return mOwners->GetFlow();
}

/// Couple the component with a Thing, extracted from a descriptor's          
/// Tags::Parent, if any was defined (always two-sided).                      
/// This will call refresh to all parts in that Thing on next tick            
///   @param desc the descriptor to scan for parents                          
///   @param fallback a fallback Thing to couple to (optional)                
///      This usually comes from the producer's context. For example, if you  
///      don't provide a parent for the renderer, it will be instantiated as  
///      a child to the graphics module owner (i.e. the runtime owner)        
void Part::Couple(Many const& desc, const Thing* fallback) {
   const Thing* owner = nullptr;
   if (not desc.ExtractTag<Tags::Parent>(owner))
      desc.ExtractData(owner);

   if (owner and mOwners.Merge(const_cast<Thing*>(owner)))
      const_cast<Thing*>(owner)->AddPart<false>(this);
   else if (fallback)
      const_cast<Thing*>(fallback)->AddPart<false>(this);
}

/// Decouple the component from a Thing (always two-sided)                    
/// This will call refresh to all parts in that entity on next frame          
///   @param entity the entity to decouple with                               
void Part::Decouple(const Thing* entity) {
   if (not entity)
      return;
      
   if (mOwners.Erase(entity))
      const_cast<Thing*>(entity)->RemovePart<false>(this);
}

/// Replace one owner instance with another (used when moving things)         
///   @attention assumes both pointers are different, and not nullptr         
///   @param replaceThis owner to replace                                     
///   @param withThis entity to replace it with                               
void Part::ReplaceOwner(const Thing* replaceThis, const Thing* withThis) {
   LglsAssumeDev(replaceThis !=  withThis, "Pointers are the same");
   LglsAssumeDev(replaceThis and withThis, "Nullptr not allowed");

   auto found = mOwners.Find(replaceThis);
   if (found)
      found.Assign(const_cast<Thing*>(withThis));
}