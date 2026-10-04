///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Thing.hpp>
#include <Langulus/Part.hpp>

#define TEMPLATE()   template<class THIS>
#define TME()        SeekInterface<THIS>


namespace Langulus::Things
{

   /// Collects all units of the given type inside the hierarchy              
   ///   @tparam SEEK - where in the hierarchy are we seeking in?             
   ///   @param meta - the units to seek for                                  
   ///   @return the gathered units that match the type                       
   TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
   TMany<Part const*> TME()::GatherParts(DMeta meta) const {
      return const_cast<THIS*>(static_cast<const THIS*>(this))
         ->template GatherParts<SEEK>(meta);
   }

   TEMPLATE() template<CT::NotVoid T, Seek SEEK> LANGULUS(INLINED)
   TMany<T*> TME()::GatherParts() {
      return static_cast<THIS*>(this)
         ->template GatherParts<SEEK>(MetaDataOf<Decay<T>>());
   }

   TEMPLATE() template<CT::NotVoid T, Seek SEEK> LANGULUS(INLINED)
   TMany<const T*> TME()::GatherParts() const {
      return const_cast<TME()*>(this)
         ->template GatherParts<T, SEEK>();
   }

   /// Collects all traits of the given type inside the hierarchy             
   ///   @tparam SEEK - where in the hierarchy are we seeking in?             
   ///   @param trait - the trait to seek for                                 
   ///   @return the gathered traits that match the type                      
   TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
   TagList TME()::GatherTags(TMeta trait) const {
      return const_cast<THIS*>(static_cast<const THIS*>(this))
         ->template GatherTags<SEEK>(trait);
   }

   TEMPLATE() template<CT::Tag T, Seek SEEK> LANGULUS(INLINED)
   TagList TME()::GatherTags() {
      return static_cast<THIS*>(this)
         ->template GatherTags<SEEK>(T::GetTag());
   }

   TEMPLATE() template<CT::Tag T, Seek SEEK> LANGULUS(INLINED)
   TagList TME()::GatherTags() const {
      return const_cast<TME()*>(this)
         ->template GatherTags<T, SEEK>();
   }

   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      TMany<Part*> TME()::GatherParts(Token const& token) {
         return static_cast<THIS*>(this)
            ->template GatherParts<SEEK>(RTTI::GetMetaData(token));
      }

      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      TMany<Part const*> TME()::GatherParts(Token const& token) const {
         return static_cast<THIS*>(this)
            ->template GatherParts<SEEK>(RTTI::GetMetaData(token));
      }
      
      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      TagList TME()::GatherTags(Token const& token) {
         return static_cast<THIS*>(this)
            ->template GatherTags<SEEK>(RTTI::GetMetaTrait(token));
      }

      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      TagList TME()::GatherTags(Token const& token) const {
         return static_cast<THIS*>(this)
            ->template GatherTags<SEEK>(RTTI::GetMetaTrait(token));
      }
   #endif

} // namespace Langulus::Things

#undef TEMPLATE
#undef TME

namespace Langulus::Things
{

   /// Collects all units of the given type inside the hierarchy              
   ///   @tparam SEEK - where in the hierarchy are we seeking in?             
   ///   @param meta - the units to seek for                                  
   ///   @return the gathered units that match the type                       
   template<Seek SEEK> LANGULUS(INLINED)
   TMany<Part*> Hierarchy::GatherParts(DMeta meta) {
      TMany<Part*> result;
      for (auto owner : *this)
         result += owner->template GatherParts<SEEK>(meta);
      return Abandon(result);
   }
      
   /// Collects all traits of the given type inside the hierarchy             
   ///   @tparam SEEK - where in the hierarchy are we seeking in?             
   ///   @param trait - the trait to seek for                                 
   ///   @return the gathered traits that match the type                      
   template<Seek SEEK> LANGULUS(INLINED)
   TMany<Tag> Hierarchy::GatherTags(TMeta trait) {
      TMany<Tag> result;
      for (auto owner : *this)
         result += owner->template GatherTags<SEEK>(trait);
      return Abandon(result);
   }

   /// Gather all values convertible to a type                                
   ///   @tparam D - type to convert to                                       
   ///   @tparam SEEK - where in the hierarchy are we seeking in?             
   ///   @return the gathered values                                          
   template<CT::NotVoid D, Seek SEEK> LANGULUS(INLINED)
   TMany<D> Hierarchy::GatherValues() const {
      TMany<D> result;
      for (auto owner : *this)
         result += owner->template GatherValues<D, SEEK>();
      return Abandon(result);
   }

} // namespace Langulus::Things