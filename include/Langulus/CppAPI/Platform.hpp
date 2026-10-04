///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Math/Scale.hpp>
#include <Langulus/Thing.hpp>


namespace Langulus::Things
{
   struct Image; // If undefined, include <Langulus/CppAPI/Image.hpp>   


   ///                                                                        
   ///   Abstract platform module                                             
   ///                                                                        
   struct PlatformModule : virtual Module {
      using CTTI_Bases = Module;
      PlatformModule() : Resolvable {this}, Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract platform unit                                               
   ///                                                                        
   struct Platform : virtual Part {
      using CTTI_Bases = Part;
      Platform() : Resolvable {this} {}
   };
   
   ///                                                                        
   ///   Abstract platform window                                             
   ///                                                                        
   struct Window : virtual Platform {
      using CTTI_Bases = Platform;
      using CTTI_Producer = PlatformModule;
      using Platform::Platform;

      using Scale2 = Math::Scale2;

      virtual void* GetNativeHandle() const noexcept = 0;
      virtual auto GetSize() const noexcept -> Scale2 = 0;
      virtual bool IsMinimized() const noexcept = 0;
      virtual bool Draw(const Ref<Image>&) const { return false; }
   };
   
   ///                                                                        
   ///   Abstract platform cursor                                             
   ///                                                                        
   struct Cursor : virtual Platform {
      using CTTI_Producer = PlatformModule;
      using CTTI_Bases = Platform;
      using Platform::Platform;
   };
}
