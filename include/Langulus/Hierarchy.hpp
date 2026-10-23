///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Module.hpp"
#include <Langulus/Seek.hpp>
#include <Langulus/CT/DefineTag.hpp>
#include <Langulus/CT/Index.hpp>
#include <Langulus/CT/Tagged.hpp>
#include <Langulus/CT/Part.hpp>


namespace Langulus::Things
{
   using Flow::Seek;


   ///                                                                        
   /// MARK: Seek interface                                                   
   ///                                                                        
   /// Contains all kinds of variations of seeking/gathering functions, that  
   /// can be used to collect data from hierarchical systems. You can filter  
   /// based on type, contents, seek direction, etc.                          
   struct SeekInterface {
      using DMeta = RTTI::DMeta;
      using TMeta = RTTI::TMeta;

      ///                                                                     
      /// MARK: Seek parts                                                    
      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      auto SeekPart(this SELF&&, DMeta, IDX&& = 0)
         -> LglsMutIf(SELF, Part*);

      template<CT::Part T = Part, Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      auto SeekPart(this SELF&&, IDX&& = 0)
         -> LglsMutIf(SELF, Decay<T>*);

      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      auto SeekPartAux(this SELF&&, Many const&, DMeta, IDX&& = 0)
         -> LglsMutIf(SELF, Part*);

      template<CT::Part T = Part, Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      auto SeekPartAux(this SELF&&, Many const&, IDX&& = 0)
         -> LglsMutIf(SELF, Decay<T>*);

      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      auto SeekPartExt(this SELF&&, DMeta, Many const&, IDX&& = 0)
         -> LglsMutIf(SELF, Part);

      template<CT::Part T = Part, Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      auto SeekPartExt(this SELF&&, Many const&, IDX&& = 0)
         -> LglsMutIf(SELF, Decay<T>*);

      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      auto SeekPartAuxExt(this SELF&&, DMeta, Many const&, Many const&, IDX&& = 0)
         -> LglsMutIf(SELF, Part*);

      template<CT::Part T = Part, Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      auto SeekPartAuxExt(this SELF&&, Many const&, Many const&, IDX&& = 0)
         -> LglsMutIf(SELF, Decay<T>*);

      ///                                                                     
      /// MARK: Seek tags                                                     
      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      auto SeekTag(this SELF&&, TMeta = {}, IDX&& = 0) -> Tag;
      template<CT::DefineTag, Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      auto SeekTag(this SELF&&, IDX&& = 0) -> Tag;

      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      auto SeekTagAux(this SELF&&, Many const&, TMeta = {}, IDX&& = 0) -> Tag;
      template<CT::DefineTag, Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      auto SeekTagAux(this SELF&&, Many const&, IDX&& = 0) -> Tag;

      ///                                                                     
      /// MARK: Seek data                                                     
      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      bool SeekValue(this SELF const&, TMeta, auto&, IDX&& = 0);
      template<CT::DefineTag, Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      bool SeekValue(this SELF const&, CT::NotTagged auto&, IDX&& = 0);
      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      bool SeekValue(this SELF const&, CT::Tagged auto&, IDX&& = 0);

      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      bool SeekValueAux(this SELF const&, TMeta, Many const&, auto&, IDX&& = 0);
      template<CT::DefineTag, Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      bool SeekValueAux(this SELF const&, Many const&, CT::NotTagged auto&, IDX&& = 0);
      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t, class SELF>
      bool SeekValueAux(this SELF const&, Many const&, CT::Tagged auto&, IDX&& = 0);

      ///                                                                     
      /// MARK: Gather parts                                                  
      template<Seek = Seek::HereAndAbove, class SELF>
      auto GatherParts(this SELF&&, DMeta)
         -> TMany<LglsMutIf(SELF, Part*)>;

      template<CT::Part P, Seek = Seek::HereAndAbove, class SELF> requires CT::Decayed<P>
      auto GatherParts(this SELF&&)
         -> TMany<LglsMutIf(SELF, P*)>;

      template<Seek = Seek::HereAndAbove>
      auto GatherPartsExt(this auto&&, DMeta, Many const&) -> TMany<Part*>;
      template<CT::NotVoid T = Part, Seek = Seek::HereAndAbove>
      auto GatherPartsExt(this auto&&, Many const&) -> TMany<T*>;

      ///                                                                     
      /// MARK: Gather tags                                                   
      template<Seek = Seek::HereAndAbove>
      auto GatherTags(this auto&&, TMeta = {}) -> TagList;
      template<CT::DefineTag T, Seek = Seek::HereAndAbove> requires CT::Decayed<T>
      auto GatherTags(this auto&&) -> TagList;

      template<class D, Seek = Seek::HereAndAbove, class SELF>
      auto GatherValues(this SELF const&) -> TMany<D>;
      
     
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      ///                                                                     
      /// MARK: Token based interface                                         
      /// Available only when managed reflection is enabled                   
      ///                                                                     
      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t>
      auto SeekPart(this auto&&, Token const&, IDX&& = 0) -> Part*;
      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t>
      auto SeekPartAux(this auto&&, Many const&, Token const&, IDX&& = 0) -> Part*;
      
      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t>
      auto SeekTag(this auto&&, Token const&, IDX&& = 0) -> Tag;
      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t>
      auto SeekTagAux(this auto&&, Many const&, Token const&, IDX&& = 0) -> Tag;

      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t>
      bool SeekValue(this auto const&, Token const&, CT::NotVoid auto&, IDX&& = 0);
      template<Seek = Seek::HereAndAbove, CT::Index IDX = size_t>
      bool SeekValueAux(this auto const&, Token const&, Many const&, CT::NotVoid auto&, IDX&& = 0);

      template<Seek = Seek::HereAndAbove, class SELF>
      auto GatherParts(this SELF&&, Token const&) -> TMany<LglsMutIf(SELF, Part*)>;
      template<Seek = Seek::HereAndAbove>
      auto GatherTags(this auto&&, Token const&) -> TagList;
   #endif
   };


   ///                                                                        
   /// MARK: Hierarchy                                                        
   ///                                                                        
   /// Container of Things, with various quality-of-life functions related to 
   /// hierarchical retrieval of sub-things, parts and tags.                  
   ///                                                                        
   using Hierarchy = TMany<Thing*>;
   /*struct Hierarchy : TMany<Thing*>, SeekInterface {
      using Base        = TMany<Thing*>;
      using CTTI_Bases  = Base;

      using Base::TMany;
      using Base::operator =;
      using Base::operator ==;
   };*/
}
