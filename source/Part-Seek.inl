///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Part.hpp>
#include "Hierarchy-Seek.inl"


namespace Langulus
{

   /// Find a trait by type (and index), searching into the hierarchy         
   ///   @tparam SEEK - direction to search at                                
   ///   @param meta - the trait to search for                                
   ///   @param offset - the offset to apply                                  
   ///   @return the trait, which is not empty, if trait was found            
   template<Seek SEEK> LANGULUS(INLINED)
   auto Part::SeekTag(TMeta meta, Index offset) -> Langulus::Tag {
      return mOwners.template SeekTag<SEEK>(meta, offset);
   }
   
   /// Find a trait, searching into the hierarchy (const)                     
   ///   @tparam SEEK - direction to search at                                
   ///   @param aux - descriptor to search through                            
   ///   @param meta - the trait type to search for                           
   ///   @param offset - the number of the matching trait to use              
   ///   @return the trait, which is not empty, if trait was found            
   template<Seek SEEK> LANGULUS(INLINED)
   auto Part::SeekTagAux(Many const& aux, TMeta meta, Index offset) -> Langulus::Tag {
      return mOwners.template SeekTagAux<SEEK>(aux, meta, offset);
   }
    
   /// Find a trait by type (and index) from the hierarchy, and attempt       
   /// converting it to a desired output type                                 
   /// Supports pinnable outputs, and won't change anything if D is pinned    
   ///   @tparam SEEK - direction to search at                                
   ///   @param meta - the trait type to search for                           
   ///   @param output - [out] the output                                     
   ///   @param offset - the number of the matching trait to use              
   ///   @return true if output was rewritten                                 
   template<Seek SEEK> LANGULUS(INLINED)
   bool Part::SeekValue(TMeta meta, CT::NotVoid auto& output, Index offset) const {
      return mOwners.template SeekValue<SEEK>(meta, output, offset);
   }
  
   /// Find a trait by type (and index) from the hierarchy, and attempt       
   /// converting it to a desired output type. Scan the aux container first   
   /// Supports pinnable outputs, and pinnables will be pinned if trait was   
   /// found in the aux container.                                            
   ///   @tparam SEEK - direction to search at                                
   ///   @param aux - descriptor to search through                            
   ///   @param meta - the trait type to search for                           
   ///   @param output - [out] the output                                     
   ///   @param offset - the number of the matching trait to use              
   ///   @return the trait, which is not empty, if trait was found            
   template<Seek SEEK> LANGULUS(INLINED)
   bool Part::SeekValueAux(TMeta meta, Many const& aux, CT::NotVoid auto& output, Index offset) const {
      return mOwners.template SeekValueAux<SEEK>(meta, aux, output, offset);
   }
}