///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Hierarchy.hpp>
#include <Langulus/CT/Pinnable.hpp>


namespace Langulus::Things
{
   /// Find a part by type and optional offset                                
   ///   @tparam SEEK the direction to seek in                                
   ///   @param meta the type of part to search for                           
   ///   @param offset the Nth match to return                                
   ///   @return a pointer to the found part, or nullptr if not found         
   template<Seek SEEK, CT::Index IDX, class SELF>
   auto SeekInterface::SeekPart(this SELF&& self, DMeta meta, IDX&& offset)
   -> LglsMutIf(SELF, Part*) {
      LglsMutIf(SELF, Part*) result;

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

   template<CT::Part T, Seek SEEK, CT::Index IDX, class SELF>
   auto SeekInterface::SeekPart(this SELF&& self, IDX&& offset)
   -> LglsMutIf(SELF, Decay<T>*) {
      return self.template SeekPart<SEEK>(
         MetaDataOf<Decay<T>>(), LglsFwd(offset)
      );
   }

   /// Find part by type and optional offset, but search first in an          
   /// auxiliary descriptor. Cease search if found there.                     
   ///   @tparam SEEK the direction to seek in                                
   ///   @param aux the auxiliary descriptor to scan first                    
   ///   @param meta the type of part to search for                           
   ///   @param offset the Nth match to return                                
   ///   @return a pointer to the found part, or nullptr if not found         
   template<Seek SEEK, CT::Index IDX, class SELF>
   auto SeekInterface::SeekPartAux(this SELF&& self, Many const& aux, DMeta meta, IDX&& offset) 
   -> LglsMutIf(SELF, Part*) {
      LglsMutIf(SELF, Part*) result = nullptr;

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
      // Let's delve into the real hierarchy.                           
      return self.template SeekPart<SEEK>(meta, offset);
   }

   template<CT::Part T, Seek SEEK, CT::Index IDX, class SELF>
   auto SeekInterface::SeekPartAux(this SELF&& self, Many const& aux, IDX&& offset)
   -> LglsMutIf(SELF, Decay<T>*) {
      return self.template SeekPartAux<SEEK>(
         aux, MetaDataOf<Decay<T>>(), LglsFwd(offset)
      );
   }
      
   /// Find part by type and specific properties and optional offset          
   ///   @tparam SEEK the direction to seek in                                
   ///   @param meta the type of part to search for                           
   ///   @param ext the properties of the part to search for                  
   ///   @param offset the Nth match to return                                
   ///   @return a pointer to the found part, or nullptr if not found         
   template<Seek SEEK, CT::Index IDX, class SELF>
   auto SeekInterface::SeekPartExt(this SELF&& self, DMeta meta, Many const& ext, IDX&& offset)
   -> LglsMutIf(SELF, Part*) {
      LglsMutIf(SELF, Part*) result;

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

   template<CT::Part T, Seek SEEK, CT::Index IDX, class SELF>
   auto SeekInterface::SeekPartExt(this SELF&& self, Many const& ext, IDX&& offset)
   -> LglsMutIf(SELF, Decay<T>*) {
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
   template<Seek SEEK, CT::Index IDX, class SELF>
   auto SeekInterface::SeekPartAuxExt(this SELF&& self, DMeta meta, Many const& aux, Many const& ext, IDX&& offset)
   -> LglsMutIf(SELF, Part*) {
      LglsMutIf(SELF, Part*) result = nullptr;

      // Scan descriptor even if hierarchy is empty                     
      aux.ForEachDeep([&](Part const* u) {
         if (u->GetResolved().CastsTo(meta)) {
            //TODO check recipe arguments
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

      // If reached, then no part was found in the descriptor           
      // Let's delve into the hierarchy                                 
      return self.template SeekPartExt<SEEK>(meta, ext, offset);
   }

   template<CT::Part T, Seek SEEK, CT::Index IDX, class SELF>
   auto SeekInterface::SeekPartAuxExt(this SELF&& self, Many const& aux, Many const& ext, IDX&& offset)
   -> LglsMutIf(SELF, Decay<T>*) {
      return self.template SeekPartAuxExt<SEEK>(MetaDataOf<Decay<T>>(), aux, ext, LglsFwd(offset));
   }
      
   /// Find a tag by type and optional offset                                 
   ///   @tparam SEEK the direction to seek in                                
   ///   @param meta the type of tag to search for                            
   ///   @param offset the Nth match to return                                
   ///   @return a shallow copy of the found tag                              
   template<Seek SEEK, CT::Index IDX, class SELF>
   auto SeekInterface::SeekTag(this SELF&& self, TMeta meta, IDX&& offset) -> Tag {
      if constexpr (CT::Part<SELF>) {
         for (auto owner : self.mOwners) {
            auto result = owner->template SeekTag<SEEK>(meta, offset);
            if (result)
               return result;
         }
      }
      else {
         if constexpr (SEEK & Seek::Here) {
            // Seek here if requested                                   
            auto output = self.GetTag(meta, offset);
            if (output)
               return Abandon(output);
         }

         if constexpr (SEEK & Seek::Above) {
            // Seek in parents up to root, if requested                 
            if (self.mOwner) {
               auto output = self.mOwner->template SeekTag<Seek::HereAndAbove>(meta, offset);
               if (output)
                  return Abandon(output);
            }
         }

         if constexpr (SEEK & Seek::Below) {
            // Seek children, if requested                              
            for (auto child : self.mChildren) {
               auto output = child->template SeekTag<Seek::HereAndBelow>(meta, offset);
               if (output)
                  return Abandon(output);
            }
         }
      }

      return {};
   }

   template<CT::DefineTag T, Seek SEEK, CT::Index IDX, class SELF>
   auto SeekInterface::SeekTag(this SELF&& self, IDX&& offset) -> Tag {
      return self.template SeekTag<SEEK>(MetaTagOf<T>(), LglsFwd(offset));
   }

   /// Find a tag by type and optional offset, starting from an auxiliary     
   /// descriptor.                                                            
   ///   @tparam SEEK the direction to seek in                                
   ///   @param aux auxiliary descriptor to check first                       
   ///   @param meta the type of tag to search for                            
   ///   @param offset the Nth match to return                                
   ///   @return a shallow copy of the found tag                              
   template<Seek SEEK, CT::Index IDX, class SELF>
   auto SeekInterface::SeekTagAux(this SELF&& self, Many const& aux, TMeta meta, IDX&& offset) -> Tag {
      // Scan descriptor                                                
      Tag result;
      aux.ForEachDeep([&](Tag const& tag) {
         if (tag.IsTag(meta)) {
            // Found match                                              
            result = tag;
            return Loop::Break;
         }

         // Just keep searching...                                      
         return Loop::Continue;
      });

      if (result)
         return Abandon(result);

      // If reached, then no tag was found in the descriptor          
      // Let's delve into the hierarchy                                 
      return self.template SeekTag<SEEK>(meta, offset);
   }

   template<CT::DefineTag T, Seek SEEK, CT::Index IDX, class SELF>
   auto SeekInterface::SeekTagAux(this SELF&& self, Many const& aux, IDX&& offset) -> Tag {
      return self.template SeekTagAux<SEEK>(aux, MetaTagOf<T>(), offset);
   }

   /// Find a tag by type (and index) from the hierarchy, and attempt         
   /// converting it to a desired output type. Supports pinnable outputs.     
   ///   @tparam SEEK direction to search at                                  
   ///   @param meta the tag type to search for                               
   ///   @param output [out] the output                                       
   ///   @param offset the Nth matching tag to use                            
   ///   @return true if output was rewritten                                 
   template<Seek SEEK, CT::Index IDX, class SELF>
   bool SeekInterface::SeekValue(this SELF const& self, TMeta meta, auto& output, IDX&& offset) {
      using D = Deref<decltype(output)>;
      if constexpr (CT::Pinnable<D>) {
         // Never touch pinned values                                   
         if (output.mLocked)
            return false;
      }

      if constexpr (CT::Part<SELF>) {
         // Let's delve into the hierarchy                              
         for (auto owner : self.mOwners) {
            if (owner->template SeekValue<SEEK>(meta, output, offset)) {
               // Value was found                                       
               return true;
            }
         }
      }
      else {
         if constexpr (SEEK & Seek::Here) {
            // Seek here if requested                                   
            auto temp = self.GetTag(meta, offset);
            if (temp) {
               try {
                  if (CT::Pinnable<D> and temp.template Is<TypeOf<D>>())
                     output = temp.template As<TypeOf<D>>();
                  else if (not CT::Pinnable<D> and temp.template Is<D>())
                     output = temp.template As<D>();
                  else if constexpr (CT::DescribeConstructible<D>)
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
            if (self.mOwner) {
               if (self.mOwner->template SeekValue<Seek::HereAndAbove>(meta, output, offset))
                  return true;
            }
         }

         if constexpr (SEEK & Seek::Below) {
            // Seek children, if requested                              
            for (auto child : self.mChildren) {
               if (child->template SeekValue<Seek::HereAndBelow>(meta, output, offset))
                  return true;
            }
         }
      }

      // If reached, nothing was found                                  
      return false;
   }

   /// Same as above, but derives the tag we search from the output           
   template<Seek SEEK, CT::Index IDX, class SELF>
   bool SeekInterface::SeekValue(this SELF const& self, CT::Tagged auto& output, IDX&& offset) {
      using TAGS = TagsOf<decltype(output)>;
      return ForEachOr(TAGS{}, [&]<class T>() {
         return self.template SeekValue<SEEK>(MetaTagOf<T>(), output, offset);
      });
   }

   /// Same as above, but you can specify tag manually at compile-time        
   template<CT::DefineTag T, Seek SEEK, CT::Index IDX, class SELF>
   bool SeekInterface::SeekValue(this SELF const& self, CT::NotTagged auto& output, IDX&& offset) {
      return self.template SeekValue<SEEK>(MetaTagOf<T>(), output, offset);
   }

   /// Find a tag by type (and index) from the hierarchy, and attempt         
   /// converting it to a desired output type. Scan the aux container first.  
   /// Supports pinnable outputs, and pinnables will be pinned only if tag was
   /// found in the aux container.                                            
   ///   @tparam SEEK direction to search at                                  
   ///   @param aux descriptor to search through                              
   ///   @param meta the tag type to search for                               
   ///   @param output [out] the output                                       
   ///   @param offset the Nth matching tag to use                            
   ///   @return shallow copy of the tag if found                             
   template<Seek SEEK, CT::Index IDX, class SELF>
   bool SeekInterface::SeekValueAux(this SELF const& self, TMeta meta, Many const& aux, auto& output, IDX&& offset) {
      using D = Deref<decltype(output)>;
      if constexpr (CT::Pinnable<D>) {
         // Never touch pinned values                                   
         if (output.mLocked)
            return false;
      }

      // Scan descriptor                                                
      bool done = false;
      if constexpr (SEEK & Seek::Here) {
         if (meta) {
            aux.ForEachDeep([&](Tag const& tag) -> LoopControl {
               if (tag.IsTag(meta)) {
                  // Found match                                     
                  try {
                     if (CT::Pinnable<D> and tag.Is<TypeOf<D>>())
                        output = tag.As<TypeOf<D>>();
                     else if (not CT::Pinnable<D> and tag.Is<D>())
                        output = tag.As<D>();
                     else if constexpr (CT::DescribeConstructible<D>)
                        output = D {Describe {tag.GetData()}};
                     else if constexpr (CT::Pinnable<D>)
                        output = tag.template AsCast<TypeOf<D>>();
                     else
                        output = tag.template AsCast<D>();

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
                  else if constexpr (CT::DescribeConstructible<D>)
                     output = D {Describe {group}};
                  else if constexpr (CT::Pinnable<D>)
                     output = group.template AsCast<TypeOf<D>>();
                  else
                     output = group.template AsCast<D>();

                  // Didn't throw, but we're done only if offset     
                  // matches                                         
                  done = offset == 0;
                  --offset;
                  return not done;
               }
               catch (...) {}

               return Loop::Continue;
            });
         }
      }

      // If reached, then no tag was found in the descriptor            
      // Let's delve into the hierarchy, by scanning for Tags::Parent   
      // and Tags::Child inside the 'aux'                               
      if (not done) {
         if constexpr (SEEK & Seek::Above) {
            aux.ForEachDeep([&](const Tags::Parent& tag) -> LoopControl {
               tag.ForEach([&](const Thing& parent) {
                  if (parent.template SeekValue<Seek::HereAndAbove>(meta, output, offset)) {
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
            aux.ForEachDeep([&](const Tags::Child& tag) -> LoopControl {
               tag.ForEach([&](const Thing& child) {
                  if (child.template SeekValue<Seek::HereAndBelow>(meta, output, offset)) {
                     // Value was found                                    
                     done = true;
                     return Loop::Break;
                  }
                  return Loop::Continue;
               });

               return not done;
            });
         }
      }

      if (done) {
         // Tag was found in the descriptor, which means our            
         // intent is to set it to a custom value - pin it, so it       
         // doesn't get overwritten on update/refresh.                  
         if constexpr (CT::Pinnable<D>)
            output.mLocked = true;
         return true;
      }

      // If reached, then no data was found in the descriptor           
      // Let's delve into the hierarchy                                 
      return self.template SeekValue<SEEK>(meta, output, offset);
   }
   
   /// Same as above, but derives the tag we search from the output           
   template<Seek SEEK, CT::Index IDX, class SELF>
   bool SeekInterface::SeekValueAux(this SELF const& self, Many const& aux, CT::Tagged auto& output, IDX&& offset) {
      using TAGS = TagsOf<decltype(output)>;
      return ForEachOr(TAGS{}, [&]<class T>() {
         return self.template SeekValueAux<SEEK>(MetaTagOf<T>(), aux, output, offset);
      });
   }

   /// Same as above, but you can specify tag manually at compile-time        
   template<CT::DefineTag T, Seek SEEK, CT::Index IDX, class SELF>
   bool SeekInterface::SeekValueAux(this SELF const& self, Many const& aux, CT::NotTagged auto& output, IDX&& offset) {
      return self.template SeekValueAux<SEEK>(MetaTagOf<T>(), aux, output, offset);
   }

   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      ///                                                                     
      /// Token based interface                                               
      /// Available only when managed reflection is enabled                   
      ///                                                                     
      template<Seek SEEK, CT::Index IDX>
      auto SeekInterface::SeekPart(Token const& dataToken, IDX&& offset) -> Part* {
         return static_cast<THIS*>(this)
            ->template SeekPart<SEEK>(RTTI::GetMetaData(dataToken), offset);
      }

      template<Seek SEEK, CT::Index IDX>
      auto SeekInterface::SeekPartAux(Many const& aux, Token const& dataToken, IDX&& offset) -> Part* {
         return static_cast<THIS*>(this)
            ->template SeekPartAux<SEEK>(aux, RTTI::GetMetaData(dataToken), offset);
      }
      
      template<Seek SEEK, CT::Index IDX>
      auto SeekInterface::SeekTag(Token const& traitToken, IDX&& offset) -> Tag {
         return static_cast<THIS*>(this)
            ->template SeekTag<SEEK>(RTTI::GetMetaTrait(traitToken), offset);
      }

      template<Seek SEEK, CT::Index IDX>
      auto SeekInterface::SeekTagAux(Many const& aux, Token const& traitToken, IDX&& offset) const -> Tag {
         return static_cast<THIS*>(this)
            ->template SeekTagAux<SEEK>(aux, RTTI::GetMetaTrait(traitToken), offset);
      }

      template<Seek SEEK, CT::Index IDX>
      bool SeekInterface::SeekValue(Token const& traitToken, CT::NotVoid auto& output, IDX&& offset) const {
         return static_cast<THIS*>(this)
            ->template SeekValue<SEEK>(RTTI::GetMetaTrait(traitToken), output, offset);
      }
     
      template<Seek SEEK, CT::Index IDX>
      bool SeekInterface::SeekValueAux(Token const& traitToken, Many const& aux, CT::NotVoid auto& output, IDX&& offset) const {
         return static_cast<THIS*>(this)
            ->template SeekValueAux<SEEK>(RTTI::GetMetaTrait(traitToken), aux, output, offset);
      }
   #endif
}