///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Art.hpp"


namespace Langulus::Things
{
   /// Retrieve the art module's repository folder                            
   ///   @return a reference to the folder interface                          
   inline auto ArtModule::GetFolder() const noexcept
   -> Ref<Things::Folder> const& {
      return mFolder;
   }

   /// Get the entire content data map                                        
   ///   @attention this doesn't generate any data                            
   ///   @return a reference to the contents                                  
   template<class C>
   auto Art::GetDataListMap(this C&& self) noexcept
   -> LglsMutIf(C, DataListMap&) {
      return self.mDataListMap;
   }

   /// Get a single data entry from the contents                              
   ///   @tparam T the tag to search for                                      
   ///   @param index the Nth data associated to the tag                      
   ///   @return a pointer to the data entry, or nullptr if none exists       
   template<CT::DefineTag T, class C>
   auto Art::GetData(this C&& self, size_t index) noexcept
   -> LglsMutIf(C, Data*) {
      return self.GetData(MetaTagOf<T>(), index);
   }

   /// Get a single data entry from the contents                              
   ///   @param tag the tag to search for                                     
   ///   @param index the Nth data associated to the tag                      
   ///   @return a pointer to the data entry, or nullptr if none exists       
   template<class C>
   auto Art::GetData(this C&& self, TMeta tag, size_t index) noexcept
   -> LglsMutIf(C, Data*) {
      if (not DecvqAllCast(self).Generate(tag, index))
         return nullptr;

      const auto datalist = self.GetDataList(tag);
      return datalist and datalist->GetCount() > index
         ? &(*datalist)[index]
         : nullptr;
   }

   /// Get a data list from the contents                                      
   ///   @tparam T the tag to search for                                      
   ///   @return a pointer to the data list, or nullptr if none exists        
   template<CT::DefineTag T, class C>
   auto Art::GetDataList(this C&& self) noexcept
   -> LglsMutIf(C, DataList*) {
      return self.GetDataList(MetaTagOf<T>());
   }

   /// Get a data list from the contents                                      
   ///   @param tag the tag to search for                                     
   ///   @return a pointer to the data list, or nullptr if none exists        
   template<class C>
   auto Art::GetDataList(this C&& self, TMeta tag) noexcept
   -> LglsMutIf(C, DataList*) {
      if (not DecvqAllCast(self).Generate(tag))
         return nullptr;

      const auto found = self.mDataListMap.Find(tag);
      return found ? &found.GetVal() : nullptr;
   }

   /// Commit data to an art                                                  
   ///   @attention data is always comitted, even if tag already exists       
   ///   @tparam T an optional tag type                                       
   ///      @attention default value leads all data to the no-tag bucket      
   ///   @param content block of data to insert (with intents or not)         
   ///      @attention you can use references to external data, but beware    
   ///         that art data is intrinsically paired with the art             
   ///         descriptor, and any silent change in contents might cause      
   ///         disparity and confusion in art libraries. Still useful when    
   ///         you don't use factories, and just want some local content      
   ///         representation.                                                
   template<CT::DefineTag T> LANGULUS(INLINED)
   void Art::Commit(auto&& content) {
      const auto tag = MetaTagOf<T>();
      const auto found = mDataListMap.Find(tag);
      if (found)
         *found.GetVal() << LglsFwd(content);
      else
         mDataListMap.Merge(tag, LglsFwd(content));
   }
}