///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Thing.hpp>
#include "Hierarchy-Gather.inl"


namespace Langulus::Things
{
   
   /// Collects all units of the given type inside the hierarchy              
   ///   @tparam SEEK - where in the hierarchy are we seeking in?             
   ///   @param meta - the units to seek for                                  
   ///   @return the gathered units that match the type                       
   template<Seek SEEK>
   TMany<Part*> Thing::GatherParts(DMeta meta) {
      TMany<Part*> result;

      if constexpr (SEEK & Seek::Here) {
         // Seek here if requested                                      
         const auto found = mUnitsAmbiguous.Find(meta);
         if (found) {
            auto& list = mUnitsAmbiguous.GetValue(found);
            for (auto& unit : list)
               result << unit;
         }
      }

      if constexpr (SEEK & Seek::Above) {
         // Seek in parents up to root, if requested                    
         if (mOwner) {
            auto inParents = mOwner->template
               GatherParts<Seek::HereAndAbove>(meta);
            result += Abandon(inParents);
         }
      }

      if constexpr (SEEK & Seek::Below) {
         // Seek children, if requested                                 
         for (auto& child : mChildren) {
            auto inChildren = child->template 
               GatherParts<Seek::HereAndBelow>(meta);
            result += Abandon(inChildren);
         }
      }

      return result;
   }
      
   /// Collects all traits of the given type inside the hierarchy             
   ///   @tparam SEEK - where in the hierarchy are we seeking in?             
   ///   @param trait - the trait to seek for                                 
   ///   @return the gathered traits that match the type                      
   template<Seek SEEK>
   TMany<Tag> Thing::GatherTags(TMeta trait) {
      TMany<Tag> results;

      if constexpr (SEEK & Seek::Here) {
         // Handle some predefined traits here                          
         if (trait->template Is<Traits::Part>()) {
            // Gather all units                                         
            for (auto& unit : mUnitsList)
               results << Traits::Part {unit};
         }
         else if (trait->template Is<Traits::Child>()) {
            // Gather all children                                      
            for (auto& child : mChildren)
               results << Traits::Part {child};
         }
         else if (trait->template Is<Traits::Runtime>()) {
            // Get the nearest runtime                                  
            results << Traits::Runtime {*mRuntime};
         }
         else if (trait->template Is<Traits::Parent>()) {
            // Get the parent                                           
            results << Traits::Parent {*mOwner};
         }

         // Check dynamic traits in the entity                          
         const auto found = mTraits.Find(trait);
         if (found)
            results += mTraits.GetValue(found);

         // Then check each unit's static traits                        
         for (auto& unit : mUnitsList) {
            size_t index {};
            auto t = unit->GetMember(trait, index);
            while (t) {
               results <<= Tag::From(trait, t);
               t = unit->GetMember(trait, ++index);
            }
         }
      }

      if constexpr (SEEK & Seek::Above) {
         // Seek in parents up to root, if requested                    
         if (mOwner) {
            results += mOwner->template
               GatherTags<Seek::HereAndAbove>(trait);
         }
      }

      if constexpr (SEEK & Seek::Below) {
         // Seek children, if requested                                 
         for (auto& child : mChildren) {
            results += child->template
               GatherTags<Seek::HereAndBelow>(trait);
         }
      }

      return Abandon(results);
   }

   /// Gather all traits/members convertible to a type                        
   ///   @tparam D - type to convert to                                       
   ///   @tparam SEEK - where in the hierarchy are we seeking in?             
   ///   @return the gathered values                                          
   template<CT::NotVoid D, Seek SEEK>
   TMany<D> Thing::GatherValues() const {
      TMany<D> results;

      if constexpr (SEEK & Seek::Here) {
         // Check dynamic traits in the entity                          
         for (auto traitGroup : mTraits) {
            for (auto trait : traitGroup.GetValue()) {
               try { results << trait.template AsCast<D>(); }
               catch (...) {}
            }
         }

         // Then check each unit's static traits                        
         for (auto& unit : mUnitsList) {
            size_t index {};
            auto t = unit->GetMember(TMeta {}, index);
            while (t) {
               try { results << t.template AsCast<D>(); }
               catch (...) {}

               t = unit->GetMember(TMeta {}, ++index);
            }
         }

         // Then check the Thing's members                              
         size_t index {};
         auto t = GetMember(TMeta {}, index);
         while (t) {
            try { results << t.template AsCast<D>(); }
            catch (...) {}

            t = GetMember(TMeta {}, ++index);
         }
      }

      if constexpr (SEEK & Seek::Above) {
         // Seek in parents up to root, if requested                    
         if (mOwner) {
            results += mOwner->template
               GatherValues<Seek::HereAndAbove, D>();
         }
      }

      if constexpr (SEEK & Seek::Below) {
         // Seek children, if requested                                 
         for (auto& child : mChildren) {
            results += child->template
               GatherValues<Seek::HereAndBelow, D>();
         }
      }

      return Abandon(results);
   }

} // namespace Langulus::Things
