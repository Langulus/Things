///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Hierarchy.hpp"
#include <Langulus/Text.hpp>
#include <Langulus/Temporal.hpp>


namespace Langulus::Things
{
   ///                                                                        
   ///   An abstract part                                                     
   ///                                                                        
   ///   Part is a shorter name for a component, or extension. It's used for  
   /// composing Things' behavior. Parts are usually produced from factories  
   /// inside external, dynamically loaded modules. There are parts for       
   /// graphics, input, AI, content, and whatever extensions you make.        
   struct LANGULUS_API(THINGS) Part
      : virtual Downcastable
      , virtual Referenced
      , Things::SeekInterface
   {
      using CTTI_Bases = Downcastable;

   protected:
      friend struct Thing;
      friend struct SeekInterface;

      // Things that are coupled with this part.                        
      // Owners act as an environment for the part's context, providing 
      // additional tags and other parts for interoperability.          
      Hierarchy mOwners;

   public:
      Part() noexcept : Downcastable {this} {}
      Part(Part const&) = delete;
      Part(Part&&) noexcept = delete;
      virtual ~Part();

      Part& operator = (Part const&) = delete;
      Part& operator = (Part&&) noexcept = delete;

      void Select(Verb&);

      virtual void Refresh();

      auto GetRuntime() const noexcept -> Runtime*;
      auto GetFlow() const noexcept -> Temporal*;
      auto GetOwners() const noexcept -> Hierarchy const&;
      bool CompareDescriptor(Many const&) const;
      
      ///                                                                     
      ///   Flow                                                              
      ///                                                                     
      template<Seek = Seek::HereAndAbove, CT::Executable V>
      V& RunIn(V&);

   protected:
      void Couple(Neat const&, Thing const* = nullptr);
      void Decouple(Thing const*);
      void ReplaceOwner(Thing const*, Thing const*);
   };
}

/// Convert to text, by writing a short name and address                      
LANGULUS_MORPHISM_CONCEPT_CUSTOM(CT::Part, {
   auto& dense = DenseCast(from);
   return Annies::Text::Mention(dense.Downcast().type.GetName(), dense);
}, Annies::Text);

/// ...even if encountered by pointer                                         
/*LANGULUS_MORPHISM_CUSTOM(Things::Part*, {
   return Annies::Text::Mention(from);
}, Annies::Text);*/

namespace Langulus::Things
{
   extern template auto SeekInterface::SeekPart<Seek::Below,         size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Above,         size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Duplex,        size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Here,          size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Everywhere,    size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndAbove,  size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndBelow,  size_t, Part>(this Part&&, DMeta, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPart<Seek::Below,         size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Above,         size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Duplex,        size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Here,          size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Everywhere,    size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, DMeta, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPart<Part, Seek::Below,         size_t, Part>(this Part&&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Above,         size_t, Part>(this Part&&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Duplex,        size_t, Part>(this Part&&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Here,          size_t, Part>(this Part&&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Everywhere,    size_t, Part>(this Part&&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Part, Seek::HereAndAbove,  size_t, Part>(this Part&&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Part, Seek::HereAndBelow,  size_t, Part>(this Part&&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPart<Part, Seek::Below,         size_t, Part const>(this Part const&&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Above,         size_t, Part const>(this Part const&&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Duplex,        size_t, Part const>(this Part const&&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Here,          size_t, Part const>(this Part const&&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Everywhere,    size_t, Part const>(this Part const&&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Part, Seek::HereAndAbove,  size_t, Part const>(this Part const&&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Part, Seek::HereAndBelow,  size_t, Part const>(this Part const&&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPartAux<Seek::Below,         size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Above,         size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Duplex,        size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Here,          size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, DMeta, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPartAux<Seek::Below,         size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Above,         size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Here,          size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, DMeta, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPartAux<Part, Seek::Below,         size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Above,         size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Duplex,        size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Here,          size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Below,         size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Above,         size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Here,          size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   
   extern template auto SeekInterface::SeekPartExt<Seek::Below,         size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Seek::Above,         size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Seek::Duplex,        size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Seek::Here,          size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Seek::Everywhere,    size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Seek::HereAndAbove,  size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Seek::HereAndBelow,  size_t, Part>(this Part&&, DMeta, Many const&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPartExt<Seek::Below,         size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Seek::Above,         size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Seek::Duplex,        size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Seek::Here,          size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Seek::Everywhere,    size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, DMeta, Many const&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPartExt<Part, Seek::Below,         size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Above,         size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Duplex,        size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Here,          size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPartExt<Part, Seek::Below,         size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Above,         size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Here,          size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPartAuxExt<Seek::Below,         size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Above,         size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Duplex,        size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Here,          size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Everywhere,    size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::HereAndAbove,  size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::HereAndBelow,  size_t, Part>(this Part&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPartAuxExt<Seek::Below,         size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Above,         size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Duplex,        size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Here,          size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Everywhere,    size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Below,         size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Above,         size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Duplex,        size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Here,          size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, Many const&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Below,         size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Above,         size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Here,          size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, Many const&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekTag<Seek::Below,         size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Above,         size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Duplex,        size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Here,          size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Everywhere,    size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndAbove,  size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndBelow,  size_t, Part>(this Part&&, TMeta, size_t&&) -> Tag;

   extern template auto SeekInterface::SeekTag<Seek::Below,         size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Above,         size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Duplex,        size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Here,          size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Everywhere,    size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, TMeta, size_t&&) -> Tag;

   extern template auto SeekInterface::SeekTagAux<Seek::Below,         size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Above,         size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Duplex,        size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Here,          size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, TMeta, size_t&&) -> Tag;

   extern template auto SeekInterface::SeekTagAux<Seek::Below,         size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Above,         size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Here,          size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, TMeta, size_t&&) -> Tag;

   extern template auto SeekInterface::GatherParts<Seek::Below,         Part>(this Part&&, DMeta) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Above,         Part>(this Part&&, DMeta) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Duplex,        Part>(this Part&&, DMeta) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Here,          Part>(this Part&&, DMeta) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Everywhere,    Part>(this Part&&, DMeta) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndAbove,  Part>(this Part&&, DMeta) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndBelow,  Part>(this Part&&, DMeta) -> TMany<Part*>;

   extern template auto SeekInterface::GatherParts<Seek::Below,         Part const>(this Part const&&, DMeta) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Above,         Part const>(this Part const&&, DMeta) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Duplex,        Part const>(this Part const&&, DMeta) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Here,          Part const>(this Part const&&, DMeta) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Everywhere,    Part const>(this Part const&&, DMeta) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndAbove,  Part const>(this Part const&&, DMeta) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndBelow,  Part const>(this Part const&&, DMeta) -> TMany<Part const*>;

   extern template auto SeekInterface::GatherPartsExt<Seek::Below,         Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Above,         Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Duplex,        Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Here,          Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Everywhere,    Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::HereAndAbove,  Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::HereAndBelow,  Part>(this Part&&, DMeta, Many const&) -> TMany<Part*>;

   extern template auto SeekInterface::GatherPartsExt<Seek::Below,         Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Above,         Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Duplex,        Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Here,          Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Everywhere,    Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::HereAndAbove,  Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::HereAndBelow,  Part const>(this Part const&&, DMeta, Many const&) -> TMany<Part const*>;

   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Below,         Part>(this Part&&, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Above,         Part>(this Part&&, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Duplex,        Part>(this Part&&, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Here,          Part>(this Part&&, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Everywhere,    Part>(this Part&&, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::HereAndAbove,  Part>(this Part&&, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::HereAndBelow,  Part>(this Part&&, Many const&) -> TMany<Part*>;

   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Below,         Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Above,         Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Duplex,        Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Here,          Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Everywhere,    Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::HereAndAbove,  Part const>(this Part const&&, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::HereAndBelow,  Part const>(this Part const&&, Many const&) -> TMany<Part const*>;

   extern template auto SeekInterface::GatherTags<Seek::Below,         Part>(this Part&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Above,         Part>(this Part&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Duplex,        Part>(this Part&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Here,          Part>(this Part&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Everywhere,    Part>(this Part&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndAbove,  Part>(this Part&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndBelow,  Part>(this Part&&, TMeta) -> TagList;

   extern template auto SeekInterface::GatherTags<Seek::Below,         Part const>(this Part const&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Above,         Part const>(this Part const&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Duplex,        Part const>(this Part const&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Here,          Part const>(this Part const&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Everywhere,    Part const>(this Part const&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndAbove,  Part const>(this Part const&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndBelow,  Part const>(this Part const&&, TMeta) -> TagList;

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   extern template auto SeekInterface::SeekPart<Seek::Below,         size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Above,         size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Duplex,        size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Here,          size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Everywhere,    size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndAbove,  size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndBelow,  size_t, Part>(this Part&&, Token const&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPart<Seek::Below,         size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Above,         size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Duplex,        size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Here,          size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Everywhere,    size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPartAux<Seek::Below,         size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Above,         size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Duplex,        size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Here,          size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPartAux<Seek::Below,         size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Above,         size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Here,          size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekTag<Seek::Below,         size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Above,         size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Duplex,        size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Here,          size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Everywhere,    size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndAbove,  size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndBelow,  size_t, Part>(this Part&&, Token const&, size_t&&) -> Tag;
   
   extern template auto SeekInterface::SeekTag<Seek::Below,         size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Above,         size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Duplex,        size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Here,          size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Everywhere,    size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Token const&, size_t&&) -> Tag;
   
   extern template auto SeekInterface::SeekTagAux<Seek::Below,         size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Above,         size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Duplex,        size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Here,          size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Everywhere,    size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndAbove,  size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndBelow,  size_t, Part>(this Part&&, Many const&, Token const&, size_t&&) -> Tag;

   extern template auto SeekInterface::SeekTagAux<Seek::Below,         size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Above,         size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Duplex,        size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Here,          size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Everywhere,    size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndAbove,  size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndBelow,  size_t, Part const>(this Part const&&, Many const&, Token const&, size_t&&) -> Tag;

   extern template auto SeekInterface::GatherParts<Seek::Below,         Part>(this Part&&, Token const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Above,         Part>(this Part&&, Token const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Duplex,        Part>(this Part&&, Token const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Here,          Part>(this Part&&, Token const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Everywhere,    Part>(this Part&&, Token const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndAbove,  Part>(this Part&&, Token const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndBelow,  Part>(this Part&&, Token const&) -> TMany<Part*>;
   
   extern template auto SeekInterface::GatherParts<Seek::Below,         Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Above,         Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Duplex,        Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Here,          Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Everywhere,    Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndAbove,  Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndBelow,  Part const>(this Part const&&, Token const&) -> TMany<Part const*>;
   
   extern template auto SeekInterface::GatherTags<Seek::Below,         Part>(this Part&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Above,         Part>(this Part&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Duplex,        Part>(this Part&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Here,          Part>(this Part&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Everywhere,    Part>(this Part&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndAbove,  Part>(this Part&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndBelow,  Part>(this Part&&, Token const&) -> TagList;
   
   extern template auto SeekInterface::GatherTags<Seek::Below,         Part const>(this Part const&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Above,         Part const>(this Part const&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Duplex,        Part const>(this Part const&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Here,          Part const>(this Part const&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Everywhere,    Part const>(this Part const&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndAbove,  Part const>(this Part const&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndBelow,  Part const>(this Part const&&, Token const&) -> TagList;
#endif
}