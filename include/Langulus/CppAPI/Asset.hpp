///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Producible.hpp>
#include <Langulus/LOD.hpp>
#include "IO.hpp"


namespace Langulus::Things
{
   ///                                                                        
   ///   Abstract asset module                                                
   ///                                                                        
   struct AssetModule : virtual Module {
      using CTTI_Bases = Module;
      AssetModule() : /*Resolvable {this},*/ Module {nullptr} {}

   protected:
      // Data folder, where assets will be saved or loaded from         
      Ref<Folder> mFolder;

   public:
      // @attention never delete stuff from this call! Do it on         
      // Update() instead                                               
      virtual void RequestGarbageCollection() = 0;
      auto GetFolder() const noexcept -> Ref<Things::Folder> const&;
   };


   ///                                                                        
   ///   Abstract asset part                                                  
   ///                                                                        
   struct Asset : virtual Part, virtual Flow::ProducedFrom<AssetModule> {
      using CTTI_Bases     = Part;
      using CTTI_Producer  = AssetModule;      
      using Data           = Many;
      using DataList       = TMany<Data>;
      using TMeta          = RTTI::TMeta;
      using DataListMap    = TMapUnsorted<TMeta, DataList>;

   protected:
      // Map of lists of generated data                                 
      DataListMap mDataListMap;

   public:
      Asset() /*: Resolvable {this}*/ {}

      virtual bool Generate(TMeta, size_t = 0) { return true; }

      template<CT::DefineTag>
      void Commit(auto&&);

      template<CT::DefineTag, class C>
      auto GetData(this C&&, size_t = 0) noexcept
         -> LglsMutIf(C, Data*);

      template<class C>
      auto GetData(this C&&, TMeta = {}, size_t = 0) noexcept
         -> LglsMutIf(C, Data*);

      template<CT::DefineTag, class C>
      auto GetDataList(this C&&) noexcept
         -> LglsMutIf(C, DataList*);

      template<class C>
      auto GetDataList(this C&&, TMeta = {}) noexcept
         -> LglsMutIf(C, DataList*);

      template<class C>
      auto GetDataListMap(this C&&) noexcept -> LglsMutIf(C, DataListMap&);
   };
}

namespace Langulus::CT
{
   /// A concept for any kind of asset                                        
   template<class T>
   concept Asset = DerivedFrom<T, Things::Asset>;
}

#include "Asset.inl"
