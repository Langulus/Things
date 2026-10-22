///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/TTable.hpp>
#include <Langulus/Thing.hpp>
#include "Thing.inl"


namespace Langulus::Things
{
   /// Get a tag from this entity's tag list                                  
   ///   @param id the tag to search for                                      
   ///   @param index the index of the tag we seek                            
   ///   @return a pointer to the tag, or nullptr if not found                
   auto Thing::GetLocalTag(TMeta id, size_t index) -> Tag* {
      if (id) {
         // Search a typed tag                                          
         const auto found = mTags.Find(id);
         if (found)
            return &((*found.GetVal())[index]);
         return nullptr;
      }

      // Search tag by index                                            
      Tag* found = nullptr;
      for (TMany<Tag>& taglist : InValuesOf(mTags)) {
         if (index < taglist.GetCount()) {
            found = &taglist[index];
            break;
         }

         index -= taglist.GetCount();
      }
      return found;
   }

   auto Thing::GetLocalTag(TMeta id, size_t index) const -> Tag const* {
      return const_cast<Thing*>(this)->GetLocalTag(id, index);
   }

   /// Get a tag by type and contents                                         
   ///   @param id tag to match                                               
   ///   @param index offset of result to use                                 
   ///   @return a non-empty tag, if found                                    
   auto Thing::GetTag(Tag const& id, size_t index) -> Tag {
      if (id.GetTag()) {
         // Handle some predefined tags here                            
         if (id.template IsTag<Tags::Part>()) {
            // Get a component                                          
            auto part = GetPartMeta(DMeta {}, index);
            if (part)
               return Tags::Part {part};
            return {};
         }
         else if (id.template IsTag<Tags::Child>()) {
            // Get a child entity                                       
            auto child = GetChild(index);
            if (child)
               return Tags::Child {child};
            return {};
         }
         else if (id.template IsTag<Tags::Runtime>()) {
            // Get the nearest runtime                                  
            return Tags::Runtime {**mRuntime};
         }
         else if (id.template IsTag<Tags::Parent>()) {
            // Get the parent                                           
            return Tags::Parent {**mOwner};
         }
      }

      // Check dynamic tags in the entity                               
      auto found = GetLocalTag(id.GetTag(), index);
      if (found)
         return *found;

      // Nothing was found                                              
      return {};
   }

   auto Thing::GetTag(Tag const& id, size_t index) const -> Tag {
      return const_cast<Thing*>(this)->GetTag(id, index);
   }

   /// Add a new tag to the thing                                             
   ///   @param tag tag to shallow copy                                       
   ///   @return the new tag instance                                         
   auto Thing::AddTag(Tag tag) -> Tag* {
      const auto tmeta = tag.GetTag();
      auto found = mTags.Find(tmeta);
      if (found)
         return &found.GetVal()->Emplace(tag);

      found = mTags.Merge(tmeta, tag);
      mRefreshRequired = true;
      THINGS_VERBOSE_SELF(tag, " added");
      return &*found.GetVal();
   }

   /// Remove a tag from the universal entity                                 
   ///   @param tag type of tag to remove                                     
   ///   @return the number of removed tags                                   
   auto Thing::RemoveTag(TMeta tag) -> size_t {
      const auto found = mTags.Find(tag);
      if (found) {
         const size_t removed = found.GetVal().GetCount();
         mTags.Erase(found);
         THINGS_VERBOSE_SELF(tag, " removed");
         mRefreshRequired = true;
         return removed;
      }

      return 0;
   }

   /// Remove an exact-matching tag from this entity                          
   ///   @param tag type and value to remove                                  
   ///   @return the number of removed tags                                   
   auto Thing::RemoveTag(Tag tag) -> size_t {
      const auto found = mTags.Find(tag.GetTag());
      if (found) {
         const auto removed = found.GetVal().Erase(tag);
         if (removed) {
            THINGS_VERBOSE_SELF(tag, " removed");
            mRefreshRequired = true;
            return removed;
         }
      }

      return 0;
   }

   /// A fast check whether tags of the given type are inside this entity     
   ///   @param tag type of tag to check                                      
   ///   @return the number of matching tags                                  
   auto Thing::HasTags(TMeta tag) const -> size_t {
      const auto found = mTags.Find(tag);
      return found ? found.GetVal().GetCount() : 0;
   }

   /// A fast check whether tags of the given type and value are inside       
   ///   @param tag tag to search for                                         
   ///   @return the number of matching tags                                  
   auto Thing::HasTags(Tag const& tag) const -> size_t {
      const auto found = mTags.Find(tag.GetTag());
      if (not found)
         return 0;

      size_t counter = 0;
      for (auto& t : found.GetVal()) {
         if (t == tag)
            ++counter;
      }
      return counter;
   }
   
   /// Add/overwrite entity's name tag                                        
   ///   @param name the name to set                                          
   void Thing::SetName(Text const& name) {
      auto found = GetLocalTag<Tags::Name>();
      if (not found)
         AddTag(Tags::Name {name});
      else
         *found = name;
   }

   /// Get entity's name tag, if any. It may reside in one of the parts.      
   ///   @return the name, or empty string if no such tag was found here      
   Text Thing::GetName() const {
      Text name;
      SeekValue<Tags::Name, Seek::Here>(name);
      return name;
   }
}
