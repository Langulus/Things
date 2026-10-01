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

   /// Find a unit by type and optional offset                                
   ///   @tparam SEEK - the direction to seek in                              
   ///   @param type - the type of unit to search for                         
   ///   @param offset - the match to return                                  
   ///   @return a pointer to the found unit, or nullptr if not found         
   TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekPart(DMeta type, Index offset) const -> Part const* {
      return const_cast<THIS*>(static_cast<const THIS*>(this))
         ->template SeekPart<SEEK>(type, offset);
   }

   TEMPLATE() template<CT::NotVoid T, Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekPart(Index offset) -> Decay<T>* {
      return dynamic_cast<Decay<T>*>(static_cast<THIS*>(this)
         ->template SeekPart<SEEK>(MetaDataOf<Decay<T>>(), offset));
   }

   TEMPLATE() template<CT::NotVoid T, Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekPart(Index offset) const -> const Decay<T>* {
      return const_cast<TME()*>(this)
         ->template SeekPart<T, SEEK>(offset);
   }

   /// Find a unit by type and optional offset, but search first in an        
   /// auxiliary descriptor                                                   
   ///   @tparam SEEK - the direction to seek in                              
   ///   @param aux - the auxiliary descriptor to scan first                  
   ///   @param type - the type of unit to search for                         
   ///   @param offset - the match to return                                  
   ///   @return a pointer to the found unit, or nullptr if not found         
   TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekPartAux(Many const& aux, DMeta type, Index offset) const -> Part const* {
      return const_cast<THIS*>(static_cast<const THIS*>(this))
         ->template SeekPartAux<SEEK>(aux, type, offset);
   }

   TEMPLATE() template<CT::NotVoid T, Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekPartAux(Many const& aux, Index offset) -> Decay<T>* {
      return dynamic_cast<Decay<T>*>(static_cast<THIS*>(this)
         ->template SeekPartAux<SEEK>(aux, MetaDataOf<Decay<T>>(), offset));
   }

   TEMPLATE() template<CT::NotVoid T, Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekPartAux(Many const& aux, Index offset) const -> const Decay<T>* {
      return const_cast<TME()*>(this)
         ->template SeekPartAux<T, SEEK>(aux, offset);
   }
      
   /// Find a unit by type and specific properties and optional offset        
   ///   @tparam SEEK - the direction to seek in                              
   ///   @param type - the type of unit to search for                         
   ///   @param ext - the properties of the unit to search for                
   ///   @param offset - the match to return                                  
   ///   @return a pointer to the found unit, or nullptr if not found         
   TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekPartExt(DMeta type, Many const& ext, Index offset) const -> Part const* {
      return const_cast<THIS*>(static_cast<const THIS*>(this))
         ->template SeekPartExt<SEEK>(type, ext, offset);
   }

   TEMPLATE() template<CT::NotVoid T, Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekPartExt(Many const& ext, Index offset) -> Decay<T>* {
      return dynamic_cast<Decay<T>*>(static_cast<THIS*>(this)
         ->template SeekPartExt<SEEK>(MetaDataOf<Decay<T>>(), ext, offset));
   }

   TEMPLATE() template<CT::NotVoid T, Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekPartExt(Many const& ext, Index offset) const -> const Decay<T>* {
      return const_cast<TME()*>(this)
         ->template SeekPartExt<T, SEEK>(ext, offset);
   }

   /// Find a unit by type, specific properties, and optional offset, but     
   /// search first in an auxiliary descriptor                                
   ///   @tparam SEEK - the direction to seek in                              
   ///   @param aux - the auxiliary descriptor to scan first                  
   ///   @param type - the type of unit to search for                         
   ///   @param ext - the properties of the unit to search for                
   ///   @param offset - the match to return                                  
   ///   @return a pointer to the found unit, or nullptr if not found         
   TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekPartAuxExt(DMeta type, Many const& aux, Many const& ext, Index offset) const -> Part const* {
      return const_cast<THIS*>(static_cast<const THIS*>(this))
         ->template SeekPartAuxExt<SEEK>(type, aux, ext, offset);
   }

   TEMPLATE() template<CT::NotVoid T, Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekPartAuxExt(Many const& aux, Many const& ext, Index offset) -> Decay<T>* {
      return dynamic_cast<Decay<T>*>(static_cast<THIS*>(this)
         ->template SeekPartAuxExt<SEEK>(MetaDataOf<Decay<T>>(), aux, ext, offset));
   }

   TEMPLATE() template<CT::NotVoid T, Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekPartAuxExt(Many const& aux, Many const& ext, Index offset) const -> const Decay<T>* {
      return const_cast<TME()*>(this)
         ->template SeekPartAuxExt<T, SEEK>(aux, ext, offset);
   }
      
   /// Find a trait by type and optional offset                               
   ///   @tparam SEEK - the direction to seek in                              
   ///   @param type - the type of unit to search for                         
   ///   @param offset - the match to return                                  
   ///   @return a pointer to the found unit, or nullptr if not found         
   TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekTag(TMeta type, Index offset) const -> Tag {
      return const_cast<THIS*>(static_cast<const THIS*>(this))
         ->template SeekTag<SEEK>(type, offset);
   }

   TEMPLATE() template<CT::Tag T, Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekTag(Index offset) -> Tag {
      return static_cast<THIS*>(this)
         ->template SeekTag<SEEK>(MetaTraitOf<T>(), offset);
   }

   TEMPLATE() template<CT::Tag T, Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekTag(Index offset) const -> Tag {
      return const_cast<TME()*>(this)
         ->template SeekTag<T, SEEK>(offset);
   }

   /// Find a trait by type and specific properties and optional offset       
   ///   @tparam SEEK - the direction to seek in                              
   ///   @param ext - the properties of the unit to search for                
   ///   @param type - the type of trait to search for                        
   ///   @param offset - the match to return                                  
   ///   @return a pointer to the found unit, or nullptr if not found         
   TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekTagAux(Many const& aux, TMeta type, Index offset) const -> Tag {
      return const_cast<THIS*>(static_cast<const THIS*>(this))
         ->template SeekTagAux<SEEK>(aux, type, offset);
   }

   TEMPLATE() template<CT::Tag T, Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekTagAux(Many const& aux, Index offset) -> Tag {
      return static_cast<THIS*>(this)
         ->template SeekTagAux<SEEK>(aux, MetaTraitOf<T>(), offset);
   }

   TEMPLATE() template<CT::Tag T, Seek SEEK> LANGULUS(INLINED)
   auto TME()::SeekTagAux(Many const& aux, Index offset) const -> Tag {
      return const_cast<TME()*>(this)
         ->template SeekTagAux<T, SEEK>(aux, offset);
   }



   TEMPLATE() template<CT::Tag T, Seek SEEK> LANGULUS(INLINED)
   bool TME()::SeekValue(CT::NotTagged auto& output, Index offset) const {
      return static_cast<const THIS*>(this)
         ->template SeekValue<SEEK>(MetaTraitOf<T>(), output, offset);
   }

   TEMPLATE() template<CT::Tag T, Seek SEEK> LANGULUS(INLINED)
   bool TME()::SeekValueAux(Many const& aux, CT::NotTagged auto& output, Index offset) const {
      return static_cast<const THIS*>(this)
         ->template SeekValueAux<SEEK>(MetaTraitOf<T>(), aux, output, offset);
   }
   


   TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
   bool TME()::SeekValue(CT::Tagged auto& output, Index offset) const {
      using T = Deref<decltype(output)>;
      auto lambda = [&]<class T>() {
         return static_cast<const THIS*>(this)->template
            SeekValue<SEEK>(MetaTraitOf<T>(), output.mData, offset);
      };
      return T::Tags::ForEachOr(lambda);
   }

   TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
   bool TME()::SeekValueAux(Many const& aux, CT::Tagged auto& output, Index offset) const {
      using T = Deref<decltype(output)>;
      auto lambda = [&]<class T>() {
         return static_cast<const THIS*>(this)->template
            SeekValueAux<SEEK>(MetaTraitOf<T>(), aux, output.mData, offset);
      };
      return T::Tags::ForEachOr(lambda);
   }
   


   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      ///                                                                     
      /// Token based interface                                               
      /// Available only when managed reflection is enabled                   
      ///                                                                     
      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      auto TME()::SeekPart(Token const& dataToken, Index offset) -> Part* {
         return static_cast<THIS*>(this)
            ->template SeekPart<SEEK>(RTTI::GetMetaData(dataToken), offset);
      }

      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      auto TME()::SeekPart(Token const& dataToken, Index offset) const -> Part const* {
         return static_cast<THIS*>(this)
            ->template SeekPart<SEEK>(RTTI::GetMetaData(dataToken), offset);
      }

      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      auto TME()::SeekPartAux(Many const& aux, Token const& dataToken, Index offset) -> Part* {
         return static_cast<THIS*>(this)
            ->template SeekPartAux<SEEK>(aux, RTTI::GetMetaData(dataToken), offset);
      }

      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      auto TME()::SeekPartAux(Many const& aux, Token const& dataToken, Index offset) const -> Part const* {
         return static_cast<THIS*>(this)
            ->template SeekPartAux<SEEK>(aux, RTTI::GetMetaData(dataToken), offset);
      }
      
      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      auto TME()::SeekTag(Token const& traitToken, Index offset) -> Tag {
         return static_cast<THIS*>(this)
            ->template SeekTag<SEEK>(RTTI::GetMetaTrait(traitToken), offset);
      }

      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      auto TME()::SeekTag(Token const& traitToken, Index offset) const -> Tag {
         return static_cast<THIS*>(this)
            ->template SeekTag<SEEK>(RTTI::GetMetaTrait(traitToken), offset);
      }

      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      auto TME()::SeekTagAux(Many const& aux, Token const& traitToken, Index offset) const -> Tag {
         return static_cast<THIS*>(this)
            ->template SeekTagAux<SEEK>(aux, RTTI::GetMetaTrait(traitToken), offset);
      }

      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      bool TME()::SeekValue(Token const& traitToken, CT::NotVoid auto& output, Index offset) const {
         return static_cast<THIS*>(this)
            ->template SeekValue<SEEK>(RTTI::GetMetaTrait(traitToken), output, offset);
      }
     
      TEMPLATE() template<Seek SEEK> LANGULUS(INLINED)
      bool TME()::SeekValueAux(Token const& traitToken, Many const& aux, CT::NotVoid auto& output, Index offset) const {
         return static_cast<THIS*>(this)
            ->template SeekValueAux<SEEK>(RTTI::GetMetaTrait(traitToken), aux, output, offset);
      }
   #endif

} // namespace Langulus::Things

#undef TEMPLATE
#undef TME


namespace Langulus::Things
{

   /// Find a specific unit, searching into the hierarchy                     
   ///   @tparam SEEK - where in the hierarchy are we seeking in?             
   ///   @param meta - the unit to seek for                                   
   ///   @param offset - which of the matches to return                       
   ///   @return the found unit, or nullptr if no such unit was found         
   template<Seek SEEK> LANGULUS(INLINED)
   auto Hierarchy::SeekPart(DMeta meta, Index offset) -> Part* {
      for (auto owner : *this) {
         Part* result = owner->template SeekPart<SEEK>(meta, offset);
         if (result)
            return result;
      }

      return nullptr;
   }

   /// Find a unit by type and index from the hierarchy                       
   /// Scan a locally provided descriptor first                               
   ///   @tparam SEEK - where to seek for the unit                            
   ///   @param aux - descriptor to search through                            
   ///   @param meta - the type of the unit to seek for                       
   ///   @param offset - the index of the unit to return                      
   ///   @return the unit if found, or nullptr otherwise                      
   template<Seek SEEK> LANGULUS(INLINED)
   auto Hierarchy::SeekPartAux(Many const& aux, DMeta meta, Index offset) -> Part* {
      Part const* result = nullptr;

      // Scan descriptor even if hierarchy is empty                     
      if constexpr (SEEK & Seek::Here) {
         aux.ForEachDeep([&](Part const* unit) {
            if (unit->CastsTo(meta)) {
               // Found match                                           
               if (offset == 0) {
                  // We're done                                         
                  result = unit;
                  return Loop::Break;
               }
               else --offset;
            }

            return Loop::Continue;
         });

         if (result)
            return const_cast<Part*>(result);
      }
      
      // If reached, then no unit was found in the descriptor           
      // Let's delve into the hierarchy, by scanning for Traits::Parent 
      // and Traits::Child inside the 'aux'                             
      if constexpr (SEEK & Seek::Above) {
         aux.ForEachDeep([&](const Traits::Parent& trait) -> LoopControl {
            trait.ForEach([&](const Thing& parent) {
               if (nullptr != (result = parent.SeekPart<Seek::HereAndAbove>(meta, offset)))
                  // Value was found                                    
                  return Loop::Break;
               return Loop::Continue;
            });
            return result == nullptr;
         });
      }

      if constexpr (SEEK & Seek::Below) {
         aux.ForEachDeep([&](const Traits::Child& trait) -> LoopControl {
            trait.ForEach([&](const Thing& child) {
               if (nullptr != (result = child.SeekPart<Seek::HereAndBelow>(meta, offset)))
                  // Value was found                                    
                  return Loop::Break;
               return Loop::Continue;
            });
            return result == nullptr;
         });
      }

      return const_cast<Part*>(result);
   }

   /// Seek a unit with specific properties                                   
   ///   @tparam SEEK - where to seek for the unit                            
   ///   @param type - type of unit to search for                             
   ///   @param ext - the properties of the unit to seek for                  
   ///   @param offset - the index of the unit to return                      
   ///   @return the unit if found, or nullptr otherwise                      
   template<Seek SEEK> LANGULUS(INLINED)
   auto Hierarchy::SeekPartExt(DMeta type, Many const& ext, Index offset) -> Part* {
      for (auto owner : *this) {
         Part* result = owner->template SeekPartExt<SEEK>(type, ext, offset);
         if (result)
            return result;
      }

      return nullptr;
   }

   /// Seek a unit with specific properties                                   
   /// Scan a locally provided descriptor first                               
   ///   @tparam SEEK - the direction in which we're scanning the hierarchy   
   ///   @param type - type of unit to search for                             
   ///   @param aux - local descriptor to seek through first                  
   ///   @param ext - the unit properties to seek for                         
   ///   @param offset - the index of the unit to return                      
   ///   @return a pointer to the found unit, or nullptr if not found         
   template<Seek SEEK> LANGULUS(INLINED)
   auto Hierarchy::SeekPartAuxExt(DMeta type, Many const& aux, Many const& ext, Index offset) -> Part* {
      // Scan descriptor even if hierarchy is empty                     
      Part* result = nullptr;
      aux.ForEachDeep([&](Part const* u) {
         if (u->CastsTo(type)) {
            //TODO check construct arguments
            // Found match                                              
            if (offset == 0) {
               // We're done                                            
               result = const_cast<Part*>(u);
               return Loop::Break;
            }
            else --offset;
         }

         return Loop::Continue;
      });

      if (result)
         return result;

      // If reached, then no unit was found in the descriptor           
      // Let's delve into the hierarchy                                 
      return SeekPartExt<SEEK>(type, ext, offset);
   }
   
   /// Find a trait by type (and index), searching into the hierarchy         
   ///   @tparam SEEK - direction to search at                                
   ///   @param meta - the trait to search for                                
   ///   @param offset - the offset to apply                                  
   ///   @return the trait, which is not empty, if trait was found            
   template<Seek SEEK> LANGULUS(INLINED)
   auto Hierarchy::SeekTag(TMeta meta, Index offset) -> Tag {
      for (auto owner : *this) {
         auto result = owner->template SeekTag<SEEK>(meta, offset);
         if (result)
            return result;
      }

      return {};
   }
   
   /// Find a trait, searching into the hierarchy (const)                     
   ///   @tparam SEEK - direction to search at                                
   ///   @param aux - descriptor to search through                            
   ///   @param meta - the trait type to search for                           
   ///   @param offset - the number of the matching trait to use              
   ///   @return the trait, which is not empty, if trait was found            
   template<Seek SEEK> LANGULUS(INLINED)
   auto Hierarchy::SeekTagAux(Many const& aux, TMeta meta, Index offset) -> Tag {
      // Scan descriptor                                                
      Tag result;
      aux.ForEachDeep([&](const Tag& trait) {
         if (trait.IsTrait(meta)) {
            if (offset == 0) {
               // Match found                                           
               result = trait;
               return Loop::Break;
            }
            
            --offset;
         }

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
   template<Seek SEEK> LANGULUS(INLINED)
   bool Hierarchy::SeekValue(TMeta meta, CT::NotVoid auto& output, Index offset) const {
      using D = Deref<decltype(output)>;

      if constexpr (CT::Pinnable<D>) {
         // Never touch pinned values                                   
         if (output.mLocked)
            return false;
      }

      // Let's delve into the hierarchy                                 
      for (auto owner : *this) {
         if (owner->template SeekValue<SEEK>(meta, output, offset)) {
            // Value was found                                          
            return true;
         }
      }

      // If reached, nothing was found                                  
      return false;
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
   bool Hierarchy::SeekValueAux(TMeta meta, Many const& aux, CT::NotVoid auto& output, Index offset) const {
      using D = Deref<decltype(output)>;

      if constexpr (CT::Pinnable<D>) {
         // Never touch pinned values                                   
         if (output.mLocked)
            return false;
      }

      // Scan descriptor                                                
      if constexpr (SEEK & Seek::Here) {
         bool done = false;
         if (meta) {
            aux.ForEachDeep([&](const Tag& trait) -> LoopControl {
               if (trait.IsTrait(meta)) {
                  // Found match                                        
                  try {
                     if (CT::Pinnable<D> and trait.Is<TypeOf<D>>())
                        output = trait.As<TypeOf<D>>();
                     else if (not CT::Pinnable<D> and trait.Is<D>())
                        output = trait.As<D>();
                     else if constexpr (CT::DescriptorMakable<D>)
                        output = D {Describe(static_cast<Many const&>(trait))};
                     else if constexpr (CT::Pinnable<D>)
                        output = trait.template AsCast<TypeOf<D>>();
                     else
                        output = trait.template AsCast<D>();

                     // Didn't throw, but we're done only if offset     
                     // matches                                         
                     done = offset == 0;
                     --offset;
                     return not done;
                  }
                  catch (...) {}
               }

               return Loop::Continue;
            });
         }
         else {
            aux.ForEachDeep([&](Many const& group) -> LoopControl {
               try {
                  // Found match if these don't throw                   
                  if (CT::Pinnable<D> and group.Is<TypeOf<D>>())
                     output = group.As<TypeOf<D>>();
                  else if (not CT::Pinnable<D> and group.Is<D>())
                     output = group.As<D>();
                  else if constexpr (CT::DescriptorMakable<D>)
                     output = D {Describe(group)};
                  else if constexpr (CT::Pinnable<D>)
                     output = group.template AsCast<TypeOf<D>>();
                  else
                     output = group.template AsCast<D>();

                  // Didn't throw, but we're done only if offset matches
                  done = offset == 0;
                  --offset;
                  return not done;
               }
               catch (...) {}

               return Loop::Continue;
            });
         }

         if (done) {
            // Tag was found in the descriptor, which means our intent
            // is to set it to a custom value - pin it, so it doesn't   
            // get overwritten on update/refresh                        
            if constexpr (CT::Pinnable<D>)
               output.mLocked = true;
            return true;
         }
      }

      // If reached, then no trait was found in the descriptor          
      // Let's delve into the hierarchy, by scanning for Traits::Parent 
      // and Traits::Child inside the 'aux'                             
      bool done = false;
      if constexpr (SEEK & Seek::Above) {
         aux.ForEachDeep([&](const Traits::Parent& trait) -> LoopControl {
            trait.ForEach([&](const Thing& parent) {
               if (parent.SeekValue<Seek::HereAndAbove>(meta, output, offset)) {
                  // Value was found                                    
                  done = true;
                  return Loop::Break;
               }
               return Loop::Continue;
            });

            return not done;
         });
      }

      if constexpr (SEEK & Seek::Below) {
         aux.ForEachDeep([&](const Traits::Child& trait) -> LoopControl {
            trait.ForEach([&](const Thing& child) {
               if (child.SeekValue<Seek::HereAndBelow>(meta, output, offset)) {
                  // Value was found                                    
                  done = true;
                  return Loop::Break;
               }
               return Loop::Continue;
            });

            return not done;
         });
      }

      return done;
   }

} // namespace Langulus::Things