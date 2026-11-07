///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/Part.hpp>
#include <Langulus/Temporal.hpp>
#include "Thing.inl"

#include <Langulus/Annies/Functional/Compare.hpp>
#include <Langulus/Annies/Functional/Deepen.hpp>
#include <Langulus/Annies/Functional/Merge.hpp>
#include <Langulus/Annies/Functional/ForEach.hpp>
#include <Langulus/Annies/Functional/Insert.hpp>
#include <Langulus/Annies/Functional/Remove.hpp>
#include <Langulus/Annies/Functional/Describe.hpp>
#include <Langulus/Annies/Functional/Assign.hpp>

using namespace Langulus::Things;


/// Part destructor decouples part from all owners                            
Part::~Part() {
   for (auto entity : mOwners)
      Decouple(entity);
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
/// This will call refresh to all parts in that Thing on next tick.           
///   @param desc the descriptor to scan for parents                          
///   @param fallback a fallback Thing to couple to (optional).               
///      This usually comes from the producer's context. For example, if you  
///      don't provide a parent for the renderer, it will be instantiated as  
///      a child to the graphics module owner (i.e. the runtime owner).       
void Part::Couple(Neat const& desc, Thing const* fallback) {
   Thing const* owner = nullptr;
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
void Part::Decouple(Thing const* entity) {
   if (not entity)
      return;
      
   if (mOwners.Erase(entity))
      const_cast<Thing*>(entity)->RemovePart<false>(this);
}

/// Replace one owner instance with another (used when moving things)         
///   @attention assumes both pointers are different, and not nullptr         
///   @param replaceThis owner to replace                                     
///   @param withThis entity to replace it with                               
void Part::ReplaceOwner(Thing const* replaceThis, Thing const* withThis) {
   LglsAssumeDev(replaceThis !=  withThis, "Pointers are the same");
   LglsAssumeDev(replaceThis and withThis, "Nullptr not allowed");

   auto found = mOwners.Find(replaceThis);
   if (found)
      found.Assign(const_cast<Thing*>(withThis));
}

#include "Hierarchy-Gather.inl"
#include "Hierarchy-Seek.inl"

namespace Langulus::Things
{
   template auto SeekInterface::SeekPart<Seek::Below,         size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Seek::Above,         size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Seek::Duplex,        size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Seek::Here,          size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Seek::Everywhere,    size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Seek::HereAndAbove,  size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Seek::HereAndBelow,  size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;

   template auto SeekInterface::SeekPart<Seek::Below,         size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Seek::Above,         size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Seek::Duplex,        size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Seek::Here,          size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Seek::Everywhere,    size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;

   template auto SeekInterface::SeekPart<Part, Seek::Below,         size_t, Part>(this Part&&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Part, Seek::Above,         size_t, Part>(this Part&&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Part, Seek::Duplex,        size_t, Part>(this Part&&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Part, Seek::Here,          size_t, Part>(this Part&&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Part, Seek::Everywhere,    size_t, Part>(this Part&&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Part, Seek::HereAndAbove,  size_t, Part>(this Part&&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Part, Seek::HereAndBelow,  size_t, Part>(this Part&&, size_t&&) -> Part*;

   template auto SeekInterface::SeekPart<Part, Seek::Below,         size_t, Part const>(this Part const&&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Part, Seek::Above,         size_t, Part const>(this Part const&&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Part, Seek::Duplex,        size_t, Part const>(this Part const&&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Part, Seek::Here,          size_t, Part const>(this Part const&&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Part, Seek::Everywhere,    size_t, Part const>(this Part const&&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Part, Seek::HereAndAbove,  size_t, Part const>(this Part const&&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Part, Seek::HereAndBelow,  size_t, Part const>(this Part const&&, size_t&&) -> Part const*;

   template auto SeekInterface::SeekPartAux<Seek::Below,         size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Seek::Above,         size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Seek::Duplex,        size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Seek::Here,          size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;

   template auto SeekInterface::SeekPartAux<Seek::Below,         size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Seek::Above,         size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Seek::Here,          size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;

   template auto SeekInterface::SeekPartAux<Part, Seek::Below,         size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Part, Seek::Above,         size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Part, Seek::Duplex,        size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Part, Seek::Here,          size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Part, Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Part, Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Part, Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   
   template auto SeekInterface::SeekPartAux<Part, Seek::Below,         size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Part, Seek::Above,         size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Part, Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Part, Seek::Here,          size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Part, Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Part, Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Part, Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   
   template auto SeekInterface::SeekPartExt<Seek::Below,         size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartExt<Seek::Above,         size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartExt<Seek::Duplex,        size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartExt<Seek::Here,          size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartExt<Seek::Everywhere,    size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartExt<Seek::HereAndAbove,  size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartExt<Seek::HereAndBelow,  size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;

   template auto SeekInterface::SeekPartExt<Seek::Below,         size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartExt<Seek::Above,         size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartExt<Seek::Duplex,        size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartExt<Seek::Here,          size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartExt<Seek::Everywhere,    size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartExt<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartExt<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;

   template auto SeekInterface::SeekPartExt<Part, Seek::Below,         size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartExt<Part, Seek::Above,         size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartExt<Part, Seek::Duplex,        size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartExt<Part, Seek::Here,          size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartExt<Part, Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartExt<Part, Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartExt<Part, Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;

   template auto SeekInterface::SeekPartExt<Part, Seek::Below,         size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartExt<Part, Seek::Above,         size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartExt<Part, Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartExt<Part, Seek::Here,          size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartExt<Part, Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartExt<Part, Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartExt<Part, Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;

   template auto SeekInterface::SeekPartAuxExt<Seek::Below,         size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAuxExt<Seek::Above,         size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAuxExt<Seek::Duplex,        size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAuxExt<Seek::Here,          size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAuxExt<Seek::Everywhere,    size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAuxExt<Seek::HereAndAbove,  size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAuxExt<Seek::HereAndBelow,  size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;

   template auto SeekInterface::SeekPartAuxExt<Seek::Below,         size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAuxExt<Seek::Above,         size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAuxExt<Seek::Duplex,        size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAuxExt<Seek::Here,          size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAuxExt<Seek::Everywhere,    size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAuxExt<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAuxExt<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;

   template auto SeekInterface::SeekPartAuxExt<Part, Seek::Below,         size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAuxExt<Part, Seek::Above,         size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAuxExt<Part, Seek::Duplex,        size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAuxExt<Part, Seek::Here,          size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAuxExt<Part, Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAuxExt<Part, Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAuxExt<Part, Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;

   template auto SeekInterface::SeekPartAuxExt<Part, Seek::Below,         size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAuxExt<Part, Seek::Above,         size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAuxExt<Part, Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAuxExt<Part, Seek::Here,          size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAuxExt<Part, Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAuxExt<Part, Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAuxExt<Part, Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;

   template auto SeekInterface::SeekTag<Seek::Below,         size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Above,         size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Duplex,        size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Here,          size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Everywhere,    size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::HereAndAbove,  size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::HereAndBelow,  size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;

   template auto SeekInterface::SeekTag<Seek::Below,         size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Above,         size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Duplex,        size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Here,          size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Everywhere,    size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;

   template auto SeekInterface::SeekTagAux<Seek::Below,         size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Above,         size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Duplex,        size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Here,          size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;

   template auto SeekInterface::SeekTagAux<Seek::Below,         size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Above,         size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Here,          size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;

   template auto SeekInterface::GatherParts<Seek::Below,         Part>(this Part&&, DMeta) -> TMany<Part*>;
   template auto SeekInterface::GatherParts<Seek::Above,         Part>(this Part&&, DMeta) -> TMany<Part*>;
   template auto SeekInterface::GatherParts<Seek::Duplex,        Part>(this Part&&, DMeta) -> TMany<Part*>;
   template auto SeekInterface::GatherParts<Seek::Here,          Part>(this Part&&, DMeta) -> TMany<Part*>;
   template auto SeekInterface::GatherParts<Seek::Everywhere,    Part>(this Part&&, DMeta) -> TMany<Part*>;
   template auto SeekInterface::GatherParts<Seek::HereAndAbove,  Part>(this Part&&, DMeta) -> TMany<Part*>;
   template auto SeekInterface::GatherParts<Seek::HereAndBelow,  Part>(this Part&&, DMeta) -> TMany<Part*>;

   template auto SeekInterface::GatherParts<Seek::Below,         Part const>(this Part const&&, DMeta) -> TMany<Part const*>;
   template auto SeekInterface::GatherParts<Seek::Above,         Part const>(this Part const&&, DMeta) -> TMany<Part const*>;
   template auto SeekInterface::GatherParts<Seek::Duplex,        Part const>(this Part const&&, DMeta) -> TMany<Part const*>;
   template auto SeekInterface::GatherParts<Seek::Here,          Part const>(this Part const&&, DMeta) -> TMany<Part const*>;
   template auto SeekInterface::GatherParts<Seek::Everywhere,    Part const>(this Part const&&, DMeta) -> TMany<Part const*>;
   template auto SeekInterface::GatherParts<Seek::HereAndAbove,  Part const>(this Part const&&, DMeta) -> TMany<Part const*>;
   template auto SeekInterface::GatherParts<Seek::HereAndBelow,  Part const>(this Part const&&, DMeta) -> TMany<Part const*>;

   /*template auto SeekInterface::GatherPartsExt<Seek::Below,         Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;
   template auto SeekInterface::GatherPartsExt<Seek::Above,         Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;
   template auto SeekInterface::GatherPartsExt<Seek::Duplex,        Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;
   template auto SeekInterface::GatherPartsExt<Seek::Here,          Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;
   template auto SeekInterface::GatherPartsExt<Seek::Everywhere,    Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;
   template auto SeekInterface::GatherPartsExt<Seek::HereAndAbove,  Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;
   template auto SeekInterface::GatherPartsExt<Seek::HereAndBelow,  Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;

   template auto SeekInterface::GatherPartsExt<Seek::Below,         Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherPartsExt<Seek::Above,         Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherPartsExt<Seek::Duplex,        Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherPartsExt<Seek::Here,          Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherPartsExt<Seek::Everywhere,    Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherPartsExt<Seek::HereAndAbove,  Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherPartsExt<Seek::HereAndBelow,  Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;

   template auto SeekInterface::GatherPartsExt<Part, Seek::Below,         Part>(this Part&&, Many const&) -> TMany<Part*>;
   template auto SeekInterface::GatherPartsExt<Part, Seek::Above,         Part>(this Part&&, Many const&) -> TMany<Part*>;
   template auto SeekInterface::GatherPartsExt<Part, Seek::Duplex,        Part>(this Part&&, Many const&) -> TMany<Part*>;
   template auto SeekInterface::GatherPartsExt<Part, Seek::Here,          Part>(this Part&&, Many const&) -> TMany<Part*>;
   template auto SeekInterface::GatherPartsExt<Part, Seek::Everywhere,    Part>(this Part&&, Many const&) -> TMany<Part*>;
   template auto SeekInterface::GatherPartsExt<Part, Seek::HereAndAbove,  Part>(this Part&&, Many const&) -> TMany<Part*>;
   template auto SeekInterface::GatherPartsExt<Part, Seek::HereAndBelow,  Part>(this Part&&, Many const&) -> TMany<Part*>;

   template auto SeekInterface::GatherPartsExt<Part, Seek::Below,         Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherPartsExt<Part, Seek::Above,         Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherPartsExt<Part, Seek::Duplex,        Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherPartsExt<Part, Seek::Here,          Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherPartsExt<Part, Seek::Everywhere,    Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherPartsExt<Part, Seek::HereAndAbove,  Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherPartsExt<Part, Seek::HereAndBelow,  Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
*/
   template auto SeekInterface::GatherTags<Seek::Below,         Part>(this Part&&, TMeta) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Above,         Part>(this Part&&, TMeta) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Duplex,        Part>(this Part&&, TMeta) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Here,          Part>(this Part&&, TMeta) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Everywhere,    Part>(this Part&&, TMeta) -> TagList;
   template auto SeekInterface::GatherTags<Seek::HereAndAbove,  Part>(this Part&&, TMeta) -> TagList;
   template auto SeekInterface::GatherTags<Seek::HereAndBelow,  Part>(this Part&&, TMeta) -> TagList;

   template auto SeekInterface::GatherTags<Seek::Below,         Part const>(this Part const&&, TMeta) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Above,         Part const>(this Part const&&, TMeta) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Duplex,        Part const>(this Part const&&, TMeta) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Here,          Part const>(this Part const&&, TMeta) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Everywhere,    Part const>(this Part const&&, TMeta) -> TagList;
   template auto SeekInterface::GatherTags<Seek::HereAndAbove,  Part const>(this Part const&&, TMeta) -> TagList;
   template auto SeekInterface::GatherTags<Seek::HereAndBelow,  Part const>(this Part const&&, TMeta) -> TagList;

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   template auto SeekInterface::SeekPart<Seek::Below,         size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Seek::Above,         size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Seek::Duplex,        size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Seek::Here,          size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Seek::Everywhere,    size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Seek::HereAndAbove,  size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPart<Seek::HereAndBelow,  size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;

   template auto SeekInterface::SeekPart<Seek::Below,         size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Seek::Above,         size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Seek::Duplex,        size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Seek::Here,          size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Seek::Everywhere,    size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPart<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;

   template auto SeekInterface::SeekPartAux<Seek::Below,         size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Seek::Above,         size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Seek::Duplex,        size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Seek::Here,          size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;
   template auto SeekInterface::SeekPartAux<Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;

   template auto SeekInterface::SeekPartAux<Seek::Below,         size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Seek::Above,         size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Seek::Here,          size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;
   template auto SeekInterface::SeekPartAux<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;

   template auto SeekInterface::SeekTag<Seek::Below,         size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Above,         size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Duplex,        size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Here,          size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Everywhere,    size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::HereAndAbove,  size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::HereAndBelow,  size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;

   template auto SeekInterface::SeekTag<Seek::Below,         size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Above,         size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Duplex,        size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Here,          size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::Everywhere,    size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTag<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;

   template auto SeekInterface::SeekTagAux<Seek::Below,         size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Above,         size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Duplex,        size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Here,          size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;

   template auto SeekInterface::SeekTagAux<Seek::Below,         size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Above,         size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Here,          size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;
   template auto SeekInterface::SeekTagAux<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;

   template auto SeekInterface::GatherParts<Seek::Below,         Part>(this Part&&, Token const&) -> TMany<Part*>;
   template auto SeekInterface::GatherParts<Seek::Above,         Part>(this Part&&, Token const&) -> TMany<Part*>;
   template auto SeekInterface::GatherParts<Seek::Duplex,        Part>(this Part&&, Token const&) -> TMany<Part*>;
   template auto SeekInterface::GatherParts<Seek::Here,          Part>(this Part&&, Token const&) -> TMany<Part*>;
   template auto SeekInterface::GatherParts<Seek::Everywhere,    Part>(this Part&&, Token const&) -> TMany<Part*>;
   template auto SeekInterface::GatherParts<Seek::HereAndAbove,  Part>(this Part&&, Token const&) -> TMany<Part*>;
   template auto SeekInterface::GatherParts<Seek::HereAndBelow,  Part>(this Part&&, Token const&) -> TMany<Part*>;
   
   template auto SeekInterface::GatherParts<Seek::Below,         Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherParts<Seek::Above,         Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherParts<Seek::Duplex,        Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherParts<Seek::Here,          Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherParts<Seek::Everywhere,    Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherParts<Seek::HereAndAbove,  Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   template auto SeekInterface::GatherParts<Seek::HereAndBelow,  Part const>(this Part const&&, Token const&) -> TMany<Part const*>;

   template auto SeekInterface::GatherTags<Seek::Below,         Part>(this Part&&, Token const&) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Above,         Part>(this Part&&, Token const&) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Duplex,        Part>(this Part&&, Token const&) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Here,          Part>(this Part&&, Token const&) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Everywhere,    Part>(this Part&&, Token const&) -> TagList;
   template auto SeekInterface::GatherTags<Seek::HereAndAbove,  Part>(this Part&&, Token const&) -> TagList;
   template auto SeekInterface::GatherTags<Seek::HereAndBelow,  Part>(this Part&&, Token const&) -> TagList;

   template auto SeekInterface::GatherTags<Seek::Below,         Part const>(this Part const&&, Token const&) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Above,         Part const>(this Part const&&, Token const&) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Duplex,        Part const>(this Part const&&, Token const&) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Here,          Part const>(this Part const&&, Token const&) -> TagList;
   template auto SeekInterface::GatherTags<Seek::Everywhere,    Part const>(this Part const&&, Token const&) -> TagList;
   template auto SeekInterface::GatherTags<Seek::HereAndAbove,  Part const>(this Part const&&, Token const&) -> TagList;
   template auto SeekInterface::GatherTags<Seek::HereAndBelow,  Part const>(this Part const&&, Token const&) -> TagList;
#endif
}