///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Asset.hpp"


namespace Langulus::A
{
   
   /// Retrieve the asset module's repository folder                          
   ///   @return a reference to the folder interface                          
   LANGULUS(INLINED)
   auto AssetModule::GetFolder() const noexcept -> const Ref<Things::Folder>& {
      return mFolder;
   }

   /// Get the entire content data map                                        
   ///   @attention this doesn't generate any data                            
   ///   @return a reference to the contents                                  
   LANGULUS(INLINED)
   auto Asset::GetDataListMap() const noexcept -> DataListMap const& {
      return mDataListMap;
   }

   LANGULUS(INLINED)
   auto Asset::GetDataListMap() noexcept -> DataListMap& {
      return mDataListMap;
   }

   /// Get a single data entry from the contents                              
   ///   @tparam T - the tag to search for                                  
   ///   @param index - the Nth data associated to the tag                  
   ///   @return a pointer to the data entry, or nullptr if none exists       
   template<CT::DefineTag T> LANGULUS(INLINED)
   auto Asset::GetData(size_t index) noexcept -> Data* {
      TMeta tag;
      if constexpr (CT::Tag<T>)
         tag = MetaTraitOf<T>();
      return GetData(tag, index);
   }

   template<CT::DefineTag T> LANGULUS(INLINED)
   auto Asset::GetData(size_t index) const noexcept -> Data const* {
      return const_cast<Asset*>(this)->GetData<T>(index);
   }

   /// Get a single data entry from the contents                              
   ///   @param tag - the tag to search for                               
   ///   @param index - the Nth data associated to the tag                  
   ///   @return a pointer to the data entry, or nullptr if none exists       
   LANGULUS(INLINED)
   auto Asset::GetData(TMeta tag, size_t index) noexcept -> Data* {
      if (not Generate(tag, index))
         return nullptr;

      const auto datalist = GetDataList(tag);
      return datalist and datalist->GetCount() > index
         ? &(*datalist)[index]
         : nullptr;
   }

   LANGULUS(INLINED)
   auto Asset::GetData(TMeta tag, size_t index) const noexcept -> Data const* {
      return const_cast<Asset*>(this)->GetData(tag, index);
   }

   /// Get a data list from the contents                                      
   ///   @tparam T - the tag to search for                                  
   ///   @return a pointer to the data list, or nullptr if none exists        
   template<CT::DefineTag T> LANGULUS(INLINED)
   auto Asset::GetDataList() noexcept -> DataList* {
      TMeta tag;
      if constexpr (CT::Tag<T>)
         tag = MetaTraitOf<T>();
      return GetDataList(tag);
   }

   template<CT::DefineTag T> LANGULUS(INLINED)
   auto Asset::GetDataList() const noexcept -> DataList const* {
      return const_cast<Asset*>(this)->GetDataList<T>();
   }

   /// Get a data list from the contents                                      
   ///   @param tag - the tag to search for                               
   ///   @return a pointer to the data list, or nullptr if none exists        
   LANGULUS(INLINED)
   auto Asset::GetDataList(TMeta tag) noexcept -> DataList* {
      if (not const_cast<Asset*>(this)->Generate(tag))
         return nullptr;

      const auto found = mDataListMap.FindIt(tag);
      return found ? &found.GetValue() : nullptr;
   }

   LANGULUS(INLINED)
   auto Asset::GetDataList(TMeta tag) const noexcept -> const DataList* {
      return const_cast<Asset*>(this)->GetDataList(tag);
   }

   /// Commit data to an asset                                                
   ///   @attention data is always comitted, even if tag already exists     
   ///   @tparam T - an optional tag type                                   
   ///      @attention default value leads all data to the no-tag bucket    
   ///   @param content - block of data to insert (with intents or not)       
   ///      @attention you can use references to external data, but beware    
   ///         that asset data is intrinsically paired with the asset         
   ///         descriptor, and any silent change in contents might cause      
   ///         disparity and confusion in asset libraries. Still useful when  
   ///         you don't use factories, and just want some local content      
   ///         representation.                                                
   template<CT::DefineTag T> LANGULUS(INLINED)
   void Asset::Commit(auto&& content) {
      TMeta tag;
      if constexpr (CT::Tag<T>)
         tag = MetaTraitOf<T>();

      using S = IntentOf<decltype(content)>;
      const auto found = mDataListMap.FindIt(tag);
      if (found)
         found.GetValue() << S::Nest(content);
      else
         mDataListMap.Insert(tag, S::Nest(content));
   }

} // namespace Langulus::A
