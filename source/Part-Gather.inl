///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Part.hpp>
#include "Hierarchy-Gather.inl"


namespace Langulus
{
   /// Collects all units of the given type inside the hierarchy              
   ///   @tparam SEEK - where in the hierarchy are we seeking in?             
   ///   @param meta - the units to seek for                                  
   ///   @return the gathered units that match the type                       
   template<Seek SEEK> LANGULUS(INLINED)
   TMany<Part*> Part::GatherParts(DMeta meta) {
      return mOwners.template GatherParts<SEEK>(meta);
   }
   
   /// Collects all traits of the given type inside the hierarchy             
   ///   @tparam SEEK - where in the hierarchy are we seeking in?             
   ///   @param trait - the trait to seek for                                 
   ///   @return the gathered traits that match the type                      
   template<Seek SEEK> LANGULUS(INLINED)
   TagList Part::GatherTags(TMeta trait) {
      return mOwners.template GatherTags<SEEK>(trait);
   }

   /// Gather all values convertible to a type                                
   ///   @tparam D - type to convert to                                       
   ///   @tparam SEEK - where in the hierarchy are we seeking in?             
   ///   @return the gathered values                                          
   template<CT::NotVoid D, Seek SEEK> LANGULUS(INLINED)
   TMany<D> Part::GatherValues() const {
      return mOwners.template GatherValues<SEEK, D>();
   }
}