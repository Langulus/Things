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
   /// Find a part by type and optional offset                                
   ///   @tparam SEEK the direction to seek in                                
   ///   @param meta the type of part to search for                           
   ///   @param offset the Nth match to return                                
   ///   @return a pointer to the found part, or nullptr if not found         
   template<Seek SEEK, class SELF>
   auto SeekInterface::SeekPart(this SELF&& self, DMeta meta, CT::Index auto&& offset)
   -> Tmut<SELF, Part*, Part const*> {
      Tmut<SELF, Part*, Part const*> result;

      if constexpr (CT::Part<SELF>) {
         for (auto owner : self.mOwners) {
            result = owner->template SeekPart<SEEK>(meta, offset);
            if (result)
               return result;
         }
      }
      else {
         result = nullptr;
         if constexpr (SEEK & Seek::Here) {
            // Seek here if requested                                   
            result = self.GetPartMeta(meta, offset);
            if (result)
               return result;
         }

         if constexpr (SEEK & Seek::Above) {
            // Seek in parents up to root, if requested                 
            if (self.mOwner) {
               result = self.mOwner->template SeekPart<Seek::HereAndAbove>(meta, offset);
               if (result)
                  return result;
            }
         }

         if constexpr (SEEK & Seek::Below) {
            // Seek children, if requested                              
            for (auto child : self.mChildren) {
               result = child->template SeekPart<Seek::HereAndBelow>(meta, offset);
               if (result)
                  return result;
            }
         }
      }
      return nullptr;
   }

   template<CT::Part T, Seek SEEK, class SELF>
   auto SeekInterface::SeekPart(this SELF&& self, CT::Index auto&& offset)
   -> Tmut<SELF, Decay<T>*, Decay<T> const*> {
      return self.template SeekPart<SEEK>(MetaDataOf<Decay<T>>(), LglsFwd(offset));
   }

   /// Find part by type and optional offset, but search first in an          
   /// auxiliary descriptor. Cease search if found there.                     
   ///   @tparam SEEK the direction to seek in                                
   ///   @param aux the auxiliary descriptor to scan first                    
   ///   @param meta the type of part to search for                           
   ///   @param offset the Nth match to return                                
   ///   @return a pointer to the found part, or nullptr if not found         
   template<Seek SEEK, class SELF>
   auto SeekInterface::SeekPartAux(this SELF&& self, Many const& aux, DMeta meta, CT::Index auto&& offset) 
   -> Tmut<SELF, Part*, Part const*> {
      Tmut<SELF, Part*, Part const*> result = nullptr;

      // Scan descriptor even if hierarchy is empty                     
      if constexpr (SEEK & Seek::Here) {
         aux.ForEachDeep([&](Part const* part) {
            if (part->GetResolved().CastsTo(meta)) {
               // Found match                                           
               if (offset == 0) {
                  // We're done                                         
                  result = part;
                  return Loop::Break;
               }
               else --offset;
            }

            return Loop::Continue;
         });

         if (result)
            return result;
      }
      
      // If reached, then no part was found in the descriptor.          
      // Let's delve into the aux hierarchy, by scanning for            
      // Tags::Parent and Tags::Child inside the 'aux'.                 
      // 'aux' acts like a mini world for us.                           
      if constexpr (SEEK & Seek::Above) {
         aux.ForEachDeep([&](const Tags::Parent& tag) -> LoopControl {
            tag.ForEach([&](const Thing& parent) {
               if (nullptr != (result = parent.SeekPart<Seek::HereAndAbove>(meta, offset)))
                  // Value was found                                    
                  return Loop::Break;
               return Loop::Continue;
            });
            return result == nullptr;
         });
      }

      if constexpr (SEEK & Seek::Below) {
         aux.ForEachDeep([&](const Tags::Child& tag) -> LoopControl {
            tag.ForEach([&](const Thing& child) {
               if (nullptr != (result = child.SeekPart<Seek::HereAndBelow>(meta, offset)))
                  // Value was found                                    
                  return Loop::Break;
               return Loop::Continue;
            });
            return result == nullptr;
         });
      }

      if (result)
         return result;

      // If reached, then no part was found in the descriptor.          
      // Let's delve into the reak hierarchy.                           
      return self.template SeekPart<SEEK>(meta, offset);
   }

   template<CT::Part T, Seek SEEK, class SELF>
   auto SeekInterface::SeekPartAux(this SELF&& self, Many const& aux, CT::Index auto&& offset)
   -> Tmut<SELF, Decay<T>*, Decay<T> const*> {
      return self.template SeekPartAux<SEEK>(aux, MetaDataOf<Decay<T>>(), LglsFwd(offset));
   }
      
   /// Find part by type and specific properties and optional offset          
   ///   @tparam SEEK the direction to seek in                                
   ///   @param meta the type of part to search for                           
   ///   @param ext the properties of the part to search for                  
   ///   @param offset the Nth match to return                                
   ///   @return a pointer to the found part, or nullptr if not found         
   template<Seek SEEK, class SELF>
   auto SeekInterface::SeekPartExt(this SELF&& self, DMeta meta, Many const& ext, CT::Index auto&& offset)
   -> Tmut<SELF, Part*, Part const*> {
      Tmut<SELF, Part*, Part const*> result;

      if constexpr (CT::Part<SELF>) {
         for (auto owner : self.mOwners) {
            result = owner->template SeekPartExt<SEEK>(meta, ext, offset);
            if (result)
               return result;
         }
      }
      else {
         result = nullptr;
         if constexpr (SEEK & Seek::Here) {
            // Seek here if requested                                   
            result = self.GetPartExt(meta, ext, offset);
            if (result)
               return result;
         }

         if constexpr (SEEK & Seek::Above) {
            // Seek in parents up to root, if requested                 
            if (self.mOwner) {
               result = self.mOwner->template SeekPartExt<Seek::HereAndAbove>(meta, ext, offset);
               if (result)
                  return result;
            }
         }

         if constexpr (SEEK & Seek::Below) {
            // Seek children, if requested                              
            for (auto child : self.mChildren) {
               result = child->template SeekPartExt<Seek::HereAndBelow>(meta, ext, offset);
               if (result)
                  return result;
            }
         }
      }

      return nullptr;
   }

   template<CT::Part T, Seek SEEK, class SELF>
   auto SeekInterface::SeekPartExt(this SELF&& self, Many const& ext, CT::Index auto&& offset)
   -> Tmut<SELF, Decay<T>*, Decay<T> const*> {
      return self.template SeekPartExt<SEEK>(MetaDataOf<Decay<T>>(), ext, LglsFwd(offset));
   }

   /// Find part by type, specific properties, and optional offset, but       
   /// search first in an auxiliary descriptor                                
   ///   @tparam SEEK the direction to seek in                                
   ///   @param aux the auxiliary descriptor to scan first                    
   ///   @param meta the type of part to search for                           
   ///   @param ext the properties of the part to search for                  
   ///   @param offset the Nth match to return                                
   ///   @return a pointer to the found part, or nullptr if not found         
   template<Seek SEEK, class SELF>
   auto SeekInterface::SeekPartAuxExt(this SELF&& self, DMeta meta, Many const& aux, Many const& ext, CT::Index auto&& offset)
   -> Tmut<SELF, Part*, Part const*> {
      Tmut<SELF, Part*, Part const*> result = nullptr;

      // Scan descriptor even if hierarchy is empty                     
      aux.ForEachDeep([&](Part const* u) {
         if (u->GetResolved().CastsTo(meta)) {
            //TODO check construct arguments
            // Found match                                              
            if (offset == 0) {
               // We're done                                            
               result = u;
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
      return self.template SeekPartExt<SEEK>(meta, ext, offset);
   }

   template<CT::Part T, Seek SEEK, class SELF>
   auto SeekInterface::SeekPartAuxExt(this SELF&& self, Many const& aux, Many const& ext, CT::Index auto&& offset)
   -> Tmut<SELF, Decay<T>*, Decay<T> const*> {
      return self.template SeekPartAuxExt<SEEK>(MetaDataOf<Decay<T>>(), aux, ext, LglsFwd(offset));
   }
      
   /// Find a trait by type and optional offset                               
   ///   @tparam SEEK - the direction to seek in                              
   ///   @param type - the type of unit to search for                         
   ///   @param offset - the match to return                                  
   ///   @return a pointer to the found unit, or nullptr if not found         
   template<Seek SEEK>
   auto SeekInterface::SeekTag(TMeta type, CT::Index auto&& offset) const -> Tag {
      return const_cast<THIS*>(static_cast<const THIS*>(this))->template SeekTag<SEEK>(type, offset);
   }

   template<CT::Tag T, Seek SEEK>
   auto SeekInterface::SeekTag(CT::Index auto&& offset) -> Tag {
      return static_cast<THIS*>(this)->template SeekTag<SEEK>(MetaTraitOf<T>(), offset);
   }

   /// Find a trait by type and specific properties and optional offset       
   ///   @tparam SEEK - the direction to seek in                              
   ///   @param ext - the properties of the unit to search for                
   ///   @param type - the type of trait to search for                        
   ///   @param offset - the match to return                                  
   ///   @return a pointer to the found unit, or nullptr if not found         
   template<Seek SEEK>
   auto SeekInterface::SeekTagAux(Many const& aux, TMeta type, CT::Index auto&& offset) const -> Tag {
      return const_cast<THIS*>(static_cast<const THIS*>(this))
         ->template SeekTagAux<SEEK>(aux, type, offset);
   }

   template<CT::Tag T, Seek SEEK>
   auto SeekInterface::SeekTagAux(Many const& aux, CT::Index auto&& offset) -> Tag {
      return static_cast<THIS*>(this)
         ->template SeekTagAux<SEEK>(aux, MetaTraitOf<T>(), offset);
   }



   template<CT::Tag T, Seek SEEK>
   bool SeekInterface::SeekValue(CT::NotTagged auto& output, CT::Index auto&& offset) const {
      return static_cast<const THIS*>(this)
         ->template SeekValue<SEEK>(MetaTraitOf<T>(), output, offset);
   }

   template<CT::Tag T, Seek SEEK>
   bool SeekInterface::SeekValueAux(Many const& aux, CT::NotTagged auto& output, CT::Index auto&& offset) const {
      return static_cast<const THIS*>(this)
         ->template SeekValueAux<SEEK>(MetaTraitOf<T>(), aux, output, offset);
   }
   


   template<Seek SEEK>
   bool SeekInterface::SeekValue(CT::Tagged auto& output, CT::Index auto&& offset) const {
      using T = Deref<decltype(output)>;
      auto lambda = [&]<class T>() {
         return static_cast<const THIS*>(this)->template
            SeekValue<SEEK>(MetaTraitOf<T>(), output.mData, offset);
      };
      return T::Tags::ForEachOr(lambda);
   }

   template<Seek SEEK>
   bool SeekInterface::SeekValueAux(Many const& aux, CT::Tagged auto& output, CT::Index auto&& offset) const {
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
      template<Seek SEEK>
      auto SeekInterface::SeekPart(Token const& dataToken, CT::Index auto&& offset) -> Part* {
         return static_cast<THIS*>(this)
            ->template SeekPart<SEEK>(RTTI::GetMetaData(dataToken), offset);
      }

      template<Seek SEEK>
      auto SeekInterface::SeekPartAux(Many const& aux, Token const& dataToken, CT::Index auto&& offset) -> Part* {
         return static_cast<THIS*>(this)
            ->template SeekPartAux<SEEK>(aux, RTTI::GetMetaData(dataToken), offset);
      }
      
      template<Seek SEEK>
      auto SeekInterface::SeekTag(Token const& traitToken, CT::Index auto&& offset) -> Tag {
         return static_cast<THIS*>(this)
            ->template SeekTag<SEEK>(RTTI::GetMetaTrait(traitToken), offset);
      }

      template<Seek SEEK>
      auto SeekInterface::SeekTagAux(Many const& aux, Token const& traitToken, CT::Index auto&& offset) const -> Tag {
         return static_cast<THIS*>(this)
            ->template SeekTagAux<SEEK>(aux, RTTI::GetMetaTrait(traitToken), offset);
      }

      template<Seek SEEK>
      bool SeekInterface::SeekValue(Token const& traitToken, CT::NotVoid auto& output, CT::Index auto&& offset) const {
         return static_cast<THIS*>(this)
            ->template SeekValue<SEEK>(RTTI::GetMetaTrait(traitToken), output, offset);
      }
     
      template<Seek SEEK>
      bool SeekInterface::SeekValueAux(Token const& traitToken, Many const& aux, CT::NotVoid auto& output, CT::Index auto&& offset) const {
         return static_cast<THIS*>(this)
            ->template SeekValueAux<SEEK>(RTTI::GetMetaTrait(traitToken), aux, output, offset);
      }
   #endif

   
   /// Find a trait by type (and index), searching into the hierarchy         
   ///   @tparam SEEK - direction to search at                                
   ///   @param meta - the trait to search for                                
   ///   @param offset - the offset to apply                                  
   ///   @return the trait, which is not empty, if trait was found            
   template<Seek SEEK>
   auto Hierarchy::SeekTag(TMeta meta, CT::Index auto&& offset) -> Tag {
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
   template<Seek SEEK>
   auto Hierarchy::SeekTagAux(Many const& aux, TMeta meta, CT::Index auto&& offset) -> Tag {
      // Scan descriptor                                                
      Tag result;
      aux.ForEachDeep([&](Tag const& trait) {
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
   template<Seek SEEK>
   bool Hierarchy::SeekValue(TMeta meta, CT::NotVoid auto& output, CT::Index auto&& offset) const {
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
   template<Seek SEEK>
   bool Hierarchy::SeekValueAux(TMeta meta, Many const& aux, CT::NotVoid auto& output, CT::Index auto&& offset) const {
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
            aux.ForEachDeep([&](Tag const& trait) -> LoopControl {
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
}