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
#include <Langulus/Annies/Functional/Describe.hpp>


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

   template<CT::Part P, Seek SEEK, CT::Index IDX, class SELF> requires CT::Decayed<P>
   auto SeekInterface::SeekPart(this SELF&& self, IDX&& offset)
   -> LglsMutIf(SELF, P*) {
      return dynamic_cast<LglsMutIf(SELF, P*)>(
         self.template SeekPart<SEEK>(MetaDataOf<P>(), LglsFwd(offset)
      ));
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
      using R = LglsMutIf(SELF, Part*);
      R result = nullptr;

      // Scan descriptor even if hierarchy is empty                     
      if constexpr (SEEK & Seek::Here) {
         aux.ForEachDeep([&](Part const* part) {
            if (part->Downcast().type.CastsTo(meta)) {
               // Found match                                           
               if (offset == 0) {
                  // We're done                                         
                  result = const_cast<R>(part);
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
         aux.ForEachDeep([&](Tags::Parent const& tag) -> LoopControl {
            tag.ForEach([&](Thing const& parent) {
               result = const_cast<R>(parent.SeekPart<Seek::HereAndAbove>(meta, offset));
               return result ? Loop::Break : Loop::Continue;
            });
            return result == nullptr;
         });
      }

      if constexpr (SEEK & Seek::Below) {
         aux.ForEachDeep([&](Tags::Child const& tag) -> LoopControl {
            tag.ForEach([&](Thing const& child) {
               result = const_cast<R>(child.SeekPart<Seek::HereAndBelow>(meta, offset));
               return result ? Loop::Break : Loop::Continue;
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

   template<CT::Part P, Seek SEEK, CT::Index IDX, class SELF> requires CT::Decayed<P>
   auto SeekInterface::SeekPartAux(this SELF&& self, Many const& aux, IDX&& offset)
   -> LglsMutIf(SELF, P*) {
      return dynamic_cast<LglsMutIf(SELF, P*)>(
         self.template SeekPartAux<SEEK>(aux, MetaDataOf<P>(), LglsFwd(offset)
      ));
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

   template<CT::Part P, Seek SEEK, CT::Index IDX, class SELF> requires CT::Decayed<P>
   auto SeekInterface::SeekPartExt(this SELF&& self, Many const& ext, IDX&& offset)
   -> LglsMutIf(SELF, P*) {
      return dynamic_cast<LglsMutIf(SELF, P*)>(
         self.template SeekPartExt<SEEK>(MetaDataOf<P>(), ext, LglsFwd(offset))
      );
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
      using R = LglsMutIf(SELF, Part*);
      R result = nullptr;

      // Scan descriptor even if hierarchy is empty                     
      aux.ForEachDeep([&](Part const* u) {
         if (u->Downcast().type.CastsTo(meta)) {
            //TODO check recipe arguments
            // Found match                                              
            if (offset == 0) {
               // We're done                                            
               result = const_cast<R>(u);
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

   template<CT::Part P, Seek SEEK, CT::Index IDX, class SELF> requires CT::Decayed<P>
   auto SeekInterface::SeekPartAuxExt(this SELF&& self, Many const& aux, Many const& ext, IDX&& offset)
   -> LglsMutIf(SELF, P*) {
      return dynamic_cast<LglsMutIf(SELF, P*)>(
         self.template SeekPartAuxExt<SEEK>(MetaDataOf<P>(), aux, ext, LglsFwd(offset))
      );
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
   ///   @param offset the Nth matching tag if 'meta' is available.           
   ///      otherwise it's the Nth output-matching type.                      
   ///   @return true if output was rewritten                                 
   template<Seek SEEK, CT::Index IDX, class SELF>
   bool SeekInterface::SeekValue(this SELF const& self, TMeta meta, auto& output, IDX&& offset) {
      using D = Deref<decltype(output)>;
      if constexpr (CT::Pinnable<D>) {
         // Never touch pinned values                                   
         if (output.IsPinned())
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
            Tag tag = self.GetTag(meta, offset);
            if (tag) {
               if constexpr (CT::Pinnable<D>) {
                  using DT = TypeOf<D>;
                  output = tag.As<DT>();
               }
               else output = tag.As<D>();
               return true;
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
   ///   @param offset the Nth matching tag if 'meta' is available.           
   ///      otherwise it's the Nth output-matching type.                      
   ///   @return shallow copy of the tag if found                             
   template<Seek SEEK, CT::Index IDX, class SELF>
   bool SeekInterface::SeekValueAux(this SELF const& self, TMeta meta, Neat const& aux, auto& output, IDX&& offset) {
      using D = Deref<decltype(output)>;
      if constexpr (CT::Pinnable<D>) {
         // Never touch pinned values                                   
         if (output.IsPinned())
            return false;
      }

      // Scan descriptor                                                
      bool done = false;
      if constexpr (SEEK & Seek::Here) {
         if (meta) {
            const auto found = aux.GetTag(meta, offset);
            if (found) {
               if constexpr (CT::Pinnable<D>) {
                  using DT = TypeOf<D>;
                  output = found->template As<DT>();
               }
               else output = found->template As<D>();
               done = true;
            }
         }
         else if constexpr (CT::Pinnable<D>) {
            using DT = TypeOf<D>;
            const auto found = aux.GetData<DT>();
            if (found) {
               size_t local_offset = offset;
               for (Many const& group : *found) {
                  if (local_offset < group.GetCount()) {
                     output = group.AsAt<DT>(local_offset);
                     done = true;
                     break;
                  }
                  else local_offset -= group.GetCount();
               }
            }
         }
         else {
            const auto found = aux.GetData<D>();
            if (found) {
               size_t local_offset = offset;
               for (Many const& group : *found) {
                  if (local_offset < group.GetCount()) {
                     output = group.AsAt<D>(local_offset);
                     done = true;
                     break;
                  }
                  else local_offset -= group.GetCount();
               }
            }
         }
      }

      // If reached, then no tag was found in the descriptor            
      // Let's delve into the auxiliary-provided hierarchy, by scanning 
      // for Tags::Parent and Tags::Child inside the 'aux'.             
      if (not done) {
         if constexpr (SEEK & Seek::Above) {
            const auto parents = aux.GetTag<Tags::Parent>();
            if (parents) {
               parents->ForEach([&](Thing const& parent) {
                  if (parent.template SeekValue<Seek::HereAndAbove>(meta, output, offset)) {
                     // Value was found                                 
                     done = true;
                     return Loop::Break;
                  }
                  return Loop::Continue;
               });
            }
         }

         if constexpr (SEEK & Seek::Below) {
            const auto children = aux.GetTag<Tags::Child>();
            if (children) {
               children->ForEach([&](Thing const& child) {
                  if (child.template SeekValue<Seek::HereAndBelow>(meta, output, offset)) {
                     // Value was found                                 
                     done = true;
                     return Loop::Break;
                  }
                  return Loop::Continue;
               });
            }
         }
      }

      if (done) {
         // Tag was found in the descriptor, which means our            
         // intent is to set it to a custom value and pin it, so it     
         // doesn't get overwritten on next update/refresh.             
         if constexpr (CT::Pinnable<D>)
            output.Pin();
         return true;
      }

      // If reached, then no data was found in the descriptor.          
      // Let's delve into the local hierarchy.                          
      return self.template SeekValue<SEEK>(meta, output, offset);
   }
   
   /// Same as above, but derives the tag we search from the output           
   template<Seek SEEK, CT::Index IDX, class SELF>
   bool SeekInterface::SeekValueAux(this SELF const& self, Neat const& aux, CT::Tagged auto& output, IDX&& offset) {
      using TAGS = TagsOf<decltype(output)>;
      return ForEachOr(TAGS{}, [&]<class T>() {
         return self.template SeekValueAux<SEEK>(MetaTagOf<T>(), aux, output, offset);
      });
   }

   /// Same as above, but you can specify tag manually at compile-time        
   template<CT::DefineTag T, Seek SEEK, CT::Index IDX, class SELF>
   bool SeekInterface::SeekValueAux(this SELF const& self, Neat const& aux, CT::NotTagged auto& output, IDX&& offset) {
      return self.template SeekValueAux<SEEK>(MetaTagOf<T>(), aux, output, offset);
   }

   #if LANGULUS_FEATURE(MANAGED_REFLECTION)
      ///                                                                     
      /// Token based interface.                                              
      /// Available only when managed reflection is enabled.                  
      template<Seek SEEK, CT::Index IDX, class SELF>
      auto SeekInterface::SeekPart(this SELF&& self, Token const& token, IDX&& offset)
      -> LglsMutIf(SELF, Part*) {
         return self.template SeekPart<SEEK>(
            RTTI::Registry::GetMetaDataByToken(token),
            offset
         );
      }

      template<Seek SEEK, CT::Index IDX, class SELF>
      auto SeekInterface::SeekPartAux(this SELF&& self, Many const& aux, Token const& dataToken, IDX&& offset)
      -> LglsMutIf(SELF, Part*) {
         return self.template SeekPartAux<SEEK>(
            aux, RTTI::Registry::GetMetaDataByToken(dataToken),
            offset
         );
      }
      
      template<Seek SEEK, CT::Index IDX, class SELF>
      auto SeekInterface::SeekTag(this SELF&& self, Token const& tag, IDX&& offset) -> Tag {
         return self.template SeekTag<SEEK>(
            RTTI::Registry::GetMetaTagByToken(tag),
            offset
         );
      }

      template<Seek SEEK, CT::Index IDX, class SELF>
      auto SeekInterface::SeekTagAux(this SELF&& self, Many const& aux, Token const& tag, IDX&& offset) -> Tag {
         return self.template SeekTagAux<SEEK>(
            aux, RTTI::Registry::GetMetaTagByToken(tag),
            offset
         );
      }

      template<Seek SEEK, CT::Index IDX, class SELF>
      bool SeekInterface::SeekValue(this SELF const& self, Token const& tag, CT::NotVoid auto& output, IDX&& offset) {
         return self.template SeekValue<SEEK>(
            RTTI::Registry::GetMetaTagByToken(tag),
            output, offset
         );
      }
     
      template<Seek SEEK, CT::Index IDX, class SELF>
      bool SeekInterface::SeekValueAux(this SELF const& self, Token const& tag, Neat const& aux, CT::NotVoid auto& output, IDX&& offset) {
         return self.template SeekValueAux<SEEK>(
            RTTI::Registry::GetMetaTagByToken(tag),
            aux, output, offset
         );
      }
   #endif
}