///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Thing.hpp>
#include "Hierarchy-Seek.inl"


namespace Langulus::Things
{

   /// Find a trait by type (and index), searching into the hierarchy         
   ///   @tparam SEEK - direction to search at                                
   ///   @param meta - the trait to search for                                
   ///   @param offset - the offset to apply                                  
   ///   @return the trait, which is not empty, if trait was found            
   template<Seek SEEK>
   auto Thing::SeekTag(TMeta meta, Index offset) -> Tag {
      if constexpr (SEEK & Seek::Here) {
         // Seek here if requested                                      
         auto output = GetTag(meta, offset);
         if (output)
            return Abandon(output);
      }

      if constexpr (SEEK & Seek::Above) {
         // Seek in parents up to root, if requested                    
         if (mOwner) {
            auto output = mOwner->template SeekTag<Seek::HereAndAbove>(meta, offset);
            if (output)
               return Abandon(output);
         }
      }

      if constexpr (SEEK & Seek::Below) {
         // Seek children, if requested                                 
         for (auto child : mChildren) {
            auto output = child->template SeekTag<Seek::HereAndBelow>(meta, offset);
            if (output)
               return Abandon(output);
         }
      }

      return {};
   }

   /// Find a trait, searching into the hierarchy (const)                     
   /// Scan an auxiliary descriptor first                                     
   ///   @tparam SEEK - direction to search at                                
   ///   @param aux - descriptor to search through                            
   ///   @param meta - the trait type to search for                           
   ///   @param offset - the number of the matching trait to use              
   ///   @return the trait, which is not empty, if trait was found            
   template<Seek SEEK> LANGULUS(INLINED)
   auto Thing::SeekTagAux(Many const& aux, TMeta meta, Index offset) -> Tag {
      // Scan descriptor                                                
      Tag result;
      aux.ForEachDeep([&](Tag const& trait) {
         if (trait.IsTrait(meta)) {
            // Found match                                              
            result = trait;
            return Loop::Break;
         }

         // Just keep searching...                                      
         return Loop::Continue;
      });

      if (result)
         return Abandon(result);

      // If reached, then no trait was found in the descriptor          
      // Let's delve into the hierarchy                                 
      return SeekTag<SEEK>(meta, offset);
   }
   
   /// Find a trait by type (and index) from the hierarchy, and attempt       
   /// converting it to a desired output type                                 
   /// Supports pinnable outputs                                              
   ///   @tparam SEEK - direction to search at                                
   ///   @param meta - the trait type to search for                           
   ///   @param output - [out] the output                                     
   ///   @param offset - the number of the matching trait to use              
   ///   @return true if output was rewritten                                 
   template<Seek SEEK>
   bool Thing::SeekValue(TMeta meta, CT::NotVoid auto& output, Index offset) const {
      using D = Deref<decltype(output)>;

      if constexpr (CT::Pinnable<D>) {
         // Never touch pinned values                                   
         if (output.mLocked)
            return false;
      }

      if constexpr (SEEK & Seek::Here) {
         // Seek here if requested                                      
         auto temp = GetTag(meta, offset);
         if (temp) {
            try {
               if (CT::Pinnable<D> and temp.Is<TypeOf<D>>())
                  output = temp.As<TypeOf<D>>();
               else if (not CT::Pinnable<D> and temp.Is<D>())
                  output = temp.As<D>();
               else if constexpr (CT::DescriptorMakable<D>)
                  output = D {Describe(static_cast<Many const&>(temp))};
               else if constexpr (CT::Pinnable<D>)
                  output = temp.template AsCast<TypeOf<D>>();
               else
                  output = temp.template AsCast<D>();

               return true;
            }
            catch (...) {}
         }
      }

      if constexpr (SEEK & Seek::Above) {
         // Seek in parents up to root, if requested                    
         if (mOwner) {
            if (mOwner->template SeekValue<Seek::HereAndAbove>(meta, output, offset))
               return true;
         }
      }

      if constexpr (SEEK & Seek::Below) {
         // Seek children, if requested                                 
         for (auto child : mChildren) {
            if (child->template SeekValue<Seek::HereAndBelow>(meta, output, offset))
               return true;
         }
      }

      return false;
   }
   
   /// Find a value (regardless of trait) from the hierarchy, and attempt     
   /// converting it to a desired output type. Scan the aux container first   
   /// Supports pinnable outputs, and pinnables will be pinned if trait was   
   /// found in the aux container.                                            
   ///   @tparam SEEK - the direction we're searching the hierarchy in        
   ///   @param aux - descriptor to search through first                      
   ///   @param output - [out] the found value will be written here           
   ///   @param offset - the number of the matching trait to use              
   ///   @return true if value has been found and rewritten                   
   template<Seek SEEK> LANGULUS(INLINED)
   bool Thing::SeekValueAux(
      TMeta meta, Many const& aux, CT::NotVoid auto& output, Index offset
   ) const {
      using D = Deref<decltype(output)>;

      if constexpr (CT::Pinnable<D>) {
         // Never touch pinned values                                   
         if (output.mLocked)
            return false;
      }

      // Scan descriptor                                                
      bool done = false;
      if (meta) {
         aux.ForEachDeep([&](Tag const& trait) {
            if (trait.IsTrait(meta)) {
               // Found match                                           
               try {
                  if constexpr (CT::Pinnable<D>)
                     output = trait.template AsCast<TypeOf<D>>();
                  else
                     output = trait.template AsCast<D>();

                  // Didn't throw, but we're done only if offset matches
                  done = offset == 0;
                  --offset;
                  return done ? Loop::Break : Loop::Continue;
               }
               catch (...) {}
            }

            return Loop::Continue;
         });
      }
      else {
         aux.ForEachDeep([&](Many const& group) {
            try {
               // Found match if these don't throw                      
               if constexpr (CT::Pinnable<D>)
                  output = group.template AsCast<TypeOf<D>>();
               else
                  output = group.template AsCast<D>();

               // Didn't throw, but we're done only if offset matches   
               done = offset == 0;
               --offset;
               return done ? Loop::Break : Loop::Continue;
            }
            catch(...) {}

            return Loop::Continue;
         });
      }

      if (done) {
         // Data was found in the descriptor                            
         if constexpr (CT::Pinnable<D>) {
            // Make sure to pin the pinnable value                      
            output.mLocked = true;
         }

         return true;
      }

      // If reached, then no data was found in the descriptor           
      // Let's delve into the hierarchy                                 
      return SeekValue<SEEK>(meta, output, offset);
   }

} // namespace Langulus::Things