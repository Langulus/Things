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


namespace Langulus::Things
{
   using Flow::Seek;
   struct Part;
   struct Thing;


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
      template<Seek = Seek::HereAndAbove>
      auto SeekPart(this auto&&, DMeta, CT::Index auto&& = 0) -> Part*;
      template<CT::NotVoid T = Part, Seek = Seek::HereAndAbove>
      auto SeekPart(this auto&&, CT::Index auto&& = 0) -> Decay<T>*;

      template<Seek = Seek::HereAndAbove>
      auto SeekPartAux(this auto&&, Many const&, DMeta, CT::Index auto&& = 0) -> Part*;
      template<CT::NotVoid T = Part, Seek = Seek::HereAndAbove>
      auto SeekPartAux(this auto&&, Many const&, CT::Index auto&& = 0) -> Decay<T>*;

      template<Seek = Seek::HereAndAbove>
      auto SeekPartExt(this auto&&, DMeta, Many const&, CT::Index auto&& = 0) -> Part*;
      template<CT::NotVoid T = Part, Seek = Seek::HereAndAbove>
      auto SeekPartExt(this auto&&, Many const&, CT::Index auto&& = 0) -> Decay<T>*;

      template<Seek = Seek::HereAndAbove>
      auto SeekPartAuxExt(this auto&&, DMeta, Many const&, Many const&, CT::Index auto&& = 0) -> Part*;
      template<CT::NotVoid T = Part, Seek = Seek::HereAndAbove>
      auto SeekPartAuxExt(this auto&&, Many const&, Many const&, CT::Index auto&& = 0) -> Decay<T>*;

      ///                                                                     
      /// MARK: Seek tags                                                     
      template<Seek = Seek::HereAndAbove>
      auto SeekTag(this auto&&, TMeta = {}, CT::Index auto&& = 0) -> Tag;
      template<CT::DefineTag, Seek = Seek::HereAndAbove>
      auto SeekTag(this auto&&, CT::Index auto&& = 0) -> Tag;

      template<Seek = Seek::HereAndAbove>
      auto SeekTagAux(this auto&&, Many const&, TMeta = {}, CT::Index auto&& = 0) -> Tag;
      template<CT::DefineTag, Seek = Seek::HereAndAbove>
      auto SeekTagAux(this auto&&, Many const&, CT::Index auto&& = 0) -> Tag;

      ///                                                                     
      /// MARK: Seek data                                                     
      template<Seek = Seek::HereAndAbove>
      bool SeekValue(this auto const&, TMeta, auto&, CT::Index auto&& = 0);
      template<CT::DefineTag, Seek = Seek::HereAndAbove>
      bool SeekValue(this auto const&, CT::NotTagged auto&, CT::Index auto&& = 0);
      template<Seek = Seek::HereAndAbove>
      bool SeekValue(this auto const&, CT::Tagged auto&, CT::Index auto&& = 0);

      template<Seek = Seek::HereAndAbove>
      bool SeekValueAux(this auto const&, TMeta, Many const&, auto&, CT::Index auto&& = 0);
      template<CT::DefineTag, Seek = Seek::HereAndAbove>
      bool SeekValueAux(this auto const&, Many const&, CT::NotTagged auto&, CT::Index auto&& = 0);
      template<Seek = Seek::HereAndAbove>
      bool SeekValueAux(this auto const&, Many const&, CT::Tagged auto&, CT::Index auto&& = 0);

      ///                                                                     
      /// MARK: Gather parts                                                  
      template<Seek = Seek::HereAndAbove>
      auto GatherParts(this auto&&, DMeta) -> TMany<Part*>;
      template<CT::NotVoid T = Part, Seek = Seek::HereAndAbove>
      auto GatherParts(this auto&&) -> TMany<T*>;

      template<Seek = Seek::HereAndAbove>
      auto GatherPartsExt(this auto&&, DMeta, Many const&) -> TMany<Part*>;
      template<CT::NotVoid T = Part, Seek = Seek::HereAndAbove>
      auto GatherPartsExt(this auto&&, Many const&) -> TMany<T*>;

      ///                                                                     
      /// MARK: Gather tags                                                   
      template<Seek = Seek::HereAndAbove>
      auto GatherTags(this auto&&, TMeta = {}) -> TagList;
      template<CT::DefineTag, Seek = Seek::HereAndAbove>
      auto GatherTags(this auto&&) -> TagList;

      template<class D, Seek = Seek::HereAndAbove>
      auto GatherValues(this auto const&) -> TMany<D>;
      
     
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      ///                                                                     
      /// MARK: Token based interface                                         
      /// Available only when managed reflection is enabled                   
      ///                                                                     
      template<Seek = Seek::HereAndAbove>
      auto SeekPart(this auto&&, Token const&, CT::Index auto&& = 0) -> Part*;
      template<Seek = Seek::HereAndAbove>
      auto SeekPartAux(this auto&&, Many const&, Token const&, CT::Index auto&& = 0) -> Part*;
      
      template<Seek = Seek::HereAndAbove>
      auto SeekTag(this auto&&, Token const&, CT::Index auto&& = 0) -> Tag;
      template<Seek = Seek::HereAndAbove>
      auto SeekTagAux(this auto&&, Many const&, Token const&, CT::Index auto&& = 0) -> Tag;

      template<Seek = Seek::HereAndAbove>
      bool SeekValue(this auto const&, Token const&, CT::NotVoid auto&, CT::Index auto&& = 0);
      template<Seek = Seek::HereAndAbove>
      bool SeekValueAux(this auto const&, Token const&, Many const&, CT::NotVoid auto&, CT::Index auto&& = 0);

      template<Seek = Seek::HereAndAbove>
      auto GatherParts(this auto&&, Token const&) -> TMany<Part*>;
      template<Seek = Seek::HereAndAbove>
      auto GatherTags(this auto&&, Token const&) -> TagList;
   #endif
   };


   ///                                                                        
   /// MARK: Hierarchy                                                        
   ///                                                                        
   /// Simply a container of Things, with various quality-of-life             
   /// functions related to hierarchical retrieval of things, parts and tags. 
   ///                                                                        
   struct Hierarchy : TMany<Thing*>, SeekInterface {
      using Base        = TMany<Thing*>;
      using CTTI_Bases  = Base;

      using Base::TMany;
      using Base::operator =;
      using Base::operator ==;
   };
}
