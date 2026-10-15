///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Material.hpp"


namespace Langulus::A
{
   
   /// Get input tags for a given rate (const)                              
   ///   @param rate - the rate                                               
   ///   @return the input tag list                                         
   LANGULUS(INLINED)
   auto Material::GetInputs(RefreshRate rate) const -> const TagList& {
      return GetInputs(rate.GetInputIndex());
   }

   /// Get input tags for a given input offset (const)                      
   ///   @attention offset must be in the range [0; Rate::InputCount)         
   ///   @param rate - the input offset                                       
   ///   @return the input tag list                                         
   LANGULUS(INLINED)
   auto Material::GetInputs(size_t rate) const -> const TagList& {
      LglsAssumeDev(rate < RefreshRate::InputCount,
         "Input offset out of range");
      return mInputs[rate];
   }

   /// Get output tags for a given rate (const)                             
   ///   @param rate - the rate                                               
   ///   @return the output tag list                                        
   LANGULUS(INLINED)
   auto Material::GetOutputs(RefreshRate rate) const -> const TagList& {
      return GetOutputs(rate.GetInputIndex());
   }

   /// Get output tags for a given input offset (const)                     
   ///   @attention offset must be in the range [0; Rate::InputCount)         
   ///   @param rate - the input offset                                       
   ///   @return the output tag list                                        
   LANGULUS(INLINED)
   auto Material::GetOutputs(size_t rate) const -> const TagList& {
      LglsAssumeDev(rate < RefreshRate::InputCount,
         "Input offset out of range");
      return mOutputs[rate];
   }

} // namespace Langulus::A
