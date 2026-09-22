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
      AssetModule() : Resolvable {this}, Module {nullptr} {}

   protected:
      // Data folder, where assets will be saved or loaded from         
      Ref<Folder> mFolder;

   public:
      // @attention never delete stuff from this call! Do it on         
      // Update() instead                                               
      virtual void RequestGarbageCollection() = 0;
      auto GetFolder() const noexcept -> const Ref<A::Folder>&;
   };


   ///                                                                        
   ///   Abstract asset unit                                                  
   ///                                                                        
   struct Asset : virtual Unit, virtual ProducedFrom<AssetModule> {
      using CTTI_Bases     = Unit;
      using CTTI_Producer  = AssetModule;      
      using Data           = Many;
      using DataList       = TMany<Data>;
      using DataListMap    = TUnorderedMap<TMeta, DataList>;

   protected:
      // Map of lists of generated data                                 
      DataListMap mDataListMap;

   public:
      Asset() : Resolvable {this} {}

      virtual bool Generate(TMeta, size_t = 0) { return true; }

      template<CT::TraitBased = Langulus::Trait>
      void Commit(auto&&);

      template<CT::TraitBased = Langulus::Trait>
      auto GetData(size_t = 0)       noexcept -> Data*;
      template<CT::TraitBased = Langulus::Trait>
      auto GetData(size_t = 0) const noexcept -> Data const*;

      auto GetData(TMeta, size_t = 0)       noexcept -> Data*;
      auto GetData(TMeta, size_t = 0) const noexcept -> Data const*;

      template<CT::TraitBased = Langulus::Trait>
      auto GetDataList()       noexcept -> DataList*;
      template<CT::TraitBased = Langulus::Trait>
      auto GetDataList() const noexcept -> DataList const*;

      auto GetDataList(TMeta)       noexcept -> DataList*;
      auto GetDataList(TMeta) const noexcept -> DataList const*;

      auto GetDataListMap()       noexcept -> DataListMap&;
      auto GetDataListMap() const noexcept -> DataListMap const&;
   };
}

namespace Langulus::CT
{
   /// A concept for any kind of asset                                        
   template<class T>
   concept Asset = DerivedFrom<T, A::Asset>;
}

#include "Asset.inl"
