///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Hierarchy.hpp"


namespace Langulus::Things
{
   struct Temporal;

   ///                                                                        
   ///   An abstract part                                                     
   ///                                                                        
   ///   Part is a shorter name for a component, or extension. It's used for  
   /// composing Things' behavior. Parts are usually produced from factories  
   /// inside external, dynamically loaded modules. There are parts for       
   /// graphics, input, AI, content, and whatever extensions you make.        
   ///                                                                        
   struct LANGULUS_API(THINGS) Part
      : virtual Resolvable
      , virtual Referenced
      , Things::SeekInterface
   {
      using CTTI_Bases = Resolvable;

   protected:
      friend struct Thing;

      // Things that are coupled with this unit                         
      // Owners act as an environment for the part's context, providing 
      // additional tags and other parts for interoperability           
      Hierarchy mOwners;

   public:
      Part() noexcept /*: Resolvable {this}*/ {}
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

      ///                                                                     
      ///   Seek                                                              
      ///                                                                     
      /*using SeekInterface::SeekPart;
      using SeekInterface::SeekPartAux;
      using SeekInterface::SeekPartExt;
      using SeekInterface::SeekPartAuxExt;
      using SeekInterface::SeekTag;
      using SeekInterface::SeekTagAux;
      using SeekInterface::SeekValue;
      using SeekInterface::SeekValueAux;

      template<Seek = Seek::HereAndAbove>
      auto SeekPart(DMeta, Index = 0) -> Part*;
      template<Seek = Seek::HereAndAbove>
      auto SeekPartAux(Many const&, DMeta, Index = 0) -> Part*;
      template<Seek = Seek::HereAndAbove>
      auto SeekPartExt(DMeta, Many const&, Index = 0) -> Part*;
      template<Seek = Seek::HereAndAbove>
      auto SeekPartAuxExt(DMeta, Many const&, Many const&, Index = 0) -> Part*;

      template<Seek = Seek::HereAndAbove>
      auto SeekTag(TMeta, Index = 0) -> Langulus::Tag;
      template<Seek = Seek::HereAndAbove>
      auto SeekTagAux(Many const&, TMeta, Index = 0) -> Langulus::Tag;

      template<Seek = Seek::HereAndAbove>
      bool SeekValue(TMeta, CT::NotVoid auto&, Index = 0) const;
      template<Seek = Seek::HereAndAbove>
      bool SeekValueAux(TMeta, Many const&, CT::NotVoid auto&, Index = 0) const;*/

      ///                                                                     
      ///   Gather                                                            
      ///                                                                     
      /*using SeekInterface::GatherParts;
      using SeekInterface::GatherPartsExt;
      using SeekInterface::GatherTags;

      template<Seek = Seek::HereAndAbove>
      auto GatherParts(DMeta) -> TMany<Part*>;
      template<Seek = Seek::HereAndAbove>
      auto GatherPartsExt(DMeta, Many const&) -> TMany<Part*>;

      template<Seek = Seek::HereAndAbove>
      auto GatherTags(TMeta) -> TagList;

      template<CT::NotVoid D, Seek = Seek::HereAndAbove>
      auto GatherValues() const -> TMany<D>;*/

   protected:
      void Couple(Many const&, const Thing* = nullptr);
      void Decouple(const Thing*);
      void ReplaceOwner(const Thing*, const Thing*);
   };
}

namespace Langulus::CT
{
   /// Any type that inherits Part is considered a CT::Part                   
   template<class T>
   concept Part = DerivedFrom<T, Things::Part>;
}
