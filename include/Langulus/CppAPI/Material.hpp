///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Art.hpp"
#include <Langulus/Rate.hpp>


namespace Langulus::Things
{
   ///                                                                        
   ///   Shader stages                                                        
   ///                                                                        
   namespace ShaderStage
   {
      enum Enum : uint32_t {
         Vertex = 0,          // Vertex shader stage                    
         Geometry,            // Geometry shader stage                  
         TessCtrl,            // Tesselation control stage              
         TessEval,            // Tesselation evaluation stage           
         Pixel,               // Pixel shader stage                     
         Compute,             // Compute shader stage                   

         Counter              // Shader stage counter (keep at end)     
      };

      /// Shader names                                                        
      constexpr Token Names[Enum::Counter] = {
         "vertex",
         "geometry",
         "tesselation control",
         "tesselation evaluation",
         "fragment",
         "compute"
      };
   }

   ///                                                                        
   ///   Shader layout tokens                                                 
   ///                                                                        
   namespace ShaderToken
   {
      constexpr Token Version       = "//#VERSION\n";
      constexpr Token Defines       = "//#DEFINES\n";
      constexpr Token Input         = "//#INPUT\n";
      constexpr Token Output        = "//#OUTPUT\n";
      constexpr Token Dependencies  = "//#DEPENDENCY\n";
      constexpr Token Colorize      = "//#COLORIZE\n";
      constexpr Token Transform     = "//#TRANSFORM\n";
      constexpr Token Position      = "//#POSITION\n";
      constexpr Token Uniform       = "//#UNIFORM\n";
      constexpr Token Texturize     = "//#TEXTURIZE\n";
      constexpr Token Functions     = "//#FUNCTIONS\n";
   }

   ///                                                                        
   ///   Abstract material content                                            
   ///                                                                        
   struct Material : Art {
      using CTTI_Bases = Art;
      using Art::Art;

      virtual auto GetLOD(Math::LOD const&) const -> Ref<Material> = 0;

      /// Get input tags for a given rate                                     
      ///   @param rate the rate                                              
      ///   @return the input tag list                                        
      auto GetInputs(RefreshRate rate) const -> TagList const& {
         return GetInputs(rate.GetInputIndex());
      }

      auto GetInputs(size_t rate) const -> TagList const& {
         LglsAssumeDev(rate < RefreshRate::InputCount,
            "Input offset out of range");
         return mInputs[rate];
      }

      /// Get output tags for a given rate (const)                            
      ///   @param rate the rate                                              
      ///   @return the output tag list                                       
      auto GetOutputs(RefreshRate rate) const -> TagList const& {
         return GetOutputs(rate.GetInputIndex());
      }

      auto GetOutputs(size_t rate) const -> TagList const& {
         LglsAssumeDev(rate < RefreshRate::InputCount,
            "Input offset out of range");
         return mOutputs[rate];
      }

   protected:
      mutable TagList mInputs[RefreshRate::InputCount];
      mutable TagList mOutputs[RefreshRate::InputCount];
   };
}