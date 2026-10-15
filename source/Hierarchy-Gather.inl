///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Hierarchy.hpp>


namespace Langulus::Things
{
   /// Collects all parts of the given type inside the hierarchy              
   ///   @tparam SEEK where in the hierarchy are we seeking in?               
   ///   @param meta the part type to seek for                                
   ///   @return the gathered parts that match the type                       
   template<Seek SEEK, class SELF>
   auto SeekInterface::GatherParts(this SELF&& self, DMeta meta) -> TMany<LglsMutIf(SELF, Part*)> {
      TMany<LglsMutIf(SELF, Part*)> result;

      if constexpr (CT::Part<SELF>) {
         // Start seeking from each part owner                          
         for (auto owner : self.mOwners)
            result += owner->template GatherParts<SEEK>(meta);
      }
      else {
         if constexpr (SEEK & Seek::Here) {
            // Seek here if requested                                   
            auto found = self.mPartsAmbiguous.Find(meta);
            if (found)
               result += found.GetVal();
         }

         if constexpr (SEEK & Seek::Above) {
            // Seek in parents up to root, if requested                 
            if (self.mOwner)
               result += self.mOwner->template GatherParts<Seek::HereAndAbove>(meta);
         }

         if constexpr (SEEK & Seek::Below) {
            // Seek children, if requested                              
            for (auto& child : self.mChildren)
               result += child->template GatherParts<Seek::HereAndBelow>(meta);
         }
      }

      return result;
   }

   template<CT::Part P, Seek SEEK, class SELF> requires CT::Decayed<P>
   auto SeekInterface::GatherParts(this SELF&& self) -> TMany<LglsMutIf(SELF, P*)> {
      return self.template GatherParts<SEEK>(MetaDataOf<P>());
   }

   /// Collects all tags of the given type inside the hierarchy               
   ///   @tparam SEEK where in the hierarchy are we seeking in?               
   ///   @param tag the tag to seek for                                       
   ///   @return the gathered tags that match the tag                         
   ///   @note tags are always returned by value, because they can also       
   ///      interface with member variables, statics, etc.                    
   template<Seek SEEK, class SELF>
   auto SeekInterface::GatherTags(this SELF&& self, TMeta tag) -> TagList {
      TagList result;

      if constexpr (CT::Part<SELF>) {
         // Start seeking from each part owner                          
         for (auto owner : self.mOwners)
            result += owner->template GatherTags<SEEK>(tag);
      }
      else {
         if constexpr (SEEK & Seek::Here) {
            // Handle some predefined trags here                        
            if (tag == MetaTagOf<Tags::Part>()) {
               // Gather all parts                                      
               for (auto& part : self.mPartsList)
                  result << Tags::Part {part};
            }
            else if (tag == MetaTagOf<Tags::Child>()) {
               // Gather all children                                   
               for (auto& child : self.mChildren)
                  result << Tags::Part {child};
            }
            else if (tag == MetaTagOf<Tags::Runtime>()) {
               // Get the nearest runtime                               
               result << Tags::Runtime {*self.mRuntime};
            }
            else if (tag == MetaTagOf<Tags::Parent>()) {
               // Get the parent                                        
               result << Tags::Parent {*self.mOwner};
            }

            // Check dynamic tags in the entity                       
            const auto found = self.mTags.Find(tag);
            if (found)
               result += found.GetVal();

            // Then check each part's static tags                     
            for (auto& part : self.mPartsList) {
               size_t index = 0;
               auto t = part->GetMember(tag, index);
               while (t) {
                  result <<= Tag::From(tag, t);
                  t = part->GetMember(tag, ++index);
               }
            }
         }

         if constexpr (SEEK & Seek::Above) {
            // Seek in parents up to root, if requested                 
            if (self.mOwner)
               result += self.mOwner->template GatherTags<Seek::HereAndAbove>(tag);
         }

         if constexpr (SEEK & Seek::Below) {
            // Seek children, if requested                              
            for (auto& child : self.mChildren)
               result += child->template GatherTags<Seek::HereAndBelow>(tag);
         }
      }
      return result;
   }

   template<CT::DefineTag T, Seek SEEK, class SELF> requires CT::Decayed<T>
   auto SeekInterface::GatherTags(this SELF&& self) -> TagList {
      return self.template GatherTags<SEEK>(MetaTagOf<T>());
   }

   /// Gather all values convertible to a type                                
   ///   @tparam D type to convert to                                         
   ///   @tparam SEEK where in the hierarchy are we seeking in?               
   ///   @return the gathered values                                          
   template<class D, Seek SEEK, class SELF>
   auto SeekInterface::GatherValues(this SELF const& self) -> TMany<D> {
      TMany<D> result;

      if constexpr (CT::Part<SELF>) {
         // Start seeking from each part owner                          
         for (auto owner : self.mOwners)
            result += owner->template GatherValues<D, SEEK>();
      }
      else {
         if constexpr (SEEK & Seek::Here) {
            // Check dynamic tags in the entity                       
            for (auto tagGroup : self.mTags) {
               for (auto tag : tagGroup.GetValue()) {
                  try { result << tag.template AsCast<D>(); }
                  catch (...) {}
               }
            }

            // Then check each part's static tags                     
            for (auto& part : self.mPartsList) {
               size_t index = 0;
               auto t = part->GetMember(TMeta {}, index);
               while (t) {
                  try { result << t.template AsCast<D>(); }
                  catch (...) {}

                  t = part->GetMember(TMeta {}, ++index);
               }
            }

            // Then check the Thing's members                           
            size_t index = 0;
            auto t = self.GetMember(TMeta {}, index);
            while (t) {
               try { result << t.template AsCast<D>(); }
               catch (...) {}

               t = self.GetMember(TMeta {}, ++index);
            }
         }

         if constexpr (SEEK & Seek::Above) {
            // Seek in parents up to root, if requested                 
            if (self.mOwner)
               result += self.mOwner->template GatherValues<Seek::HereAndAbove, D>();
         }

         if constexpr (SEEK & Seek::Below) {
            // Seek children, if requested                              
            for (auto& child : self.mChildren)
               result += child->template GatherValues<Seek::HereAndBelow, D>();
         }
      }

      return result;
   }
   
   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      template<Seek SEEK, class SELF>
      auto SeekInterface::GatherParts(this SELF&& self, Token const& token)
      -> TMany<LglsMutIf(SELF, Part*)> {
         return self.template GatherParts<SEEK>(
            RTTI::Registry::GetMetaDataByToken(token)
         );
      }
      
      template<Seek SEEK>
      auto SeekInterface::GatherTags(this auto&& self, Token const& token)
      -> TagList {
         return self.template GatherTags<SEEK>(
            RTTI::Registry::GetMetaTagByToken(token)
         );
      }
   #endif
}