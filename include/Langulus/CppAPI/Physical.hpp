///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Part.hpp>
#include <Langulus/Math/LOD.hpp>
#include <Langulus/Math/Level.hpp>
#include <Langulus/Math/Matrix.hpp>
#include <Langulus/Math/Color.hpp>


namespace Langulus::Things
{
   ///                                                                        
   ///   Abstract physical module                                             
   struct PhysicalModule : virtual Module {
      using CTTI_Bases = Module;
      PhysicalModule() : Downcastable {this}, Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract physical part                                               
   struct Physical : virtual Part {
      using CTTI_Bases = Part;
      Physical() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract physical world part                                         
   struct World : virtual Physical {
      using CTTI_Bases = Physical;
      using CTTI_Producer = PhysicalModule;
      World() : Downcastable {this} {}
   };
   
   ///                                                                        
   ///   Abstract physical instance part                                      
   struct Instance : virtual Physical {
      using CTTI_Bases = Physical;
      using CTTI_Producer = World;
      Instance() : Downcastable {this} {}

      using LOD   = Math::LOD;
      using Level = Math::Level;
      using Mat4  = Math::Mat4;
      using RGBA  = Math::RGBA;

      virtual bool Cull(LOD const&) const noexcept = 0;
      virtual auto GetLevel() const noexcept -> Level = 0;
      virtual auto GetModelTransform(LOD const&) const noexcept -> Mat4 = 0;
      virtual auto GetModelTransform(Level const& = {}) const noexcept -> Mat4 = 0;
      virtual auto GetViewTransform(LOD const&) const noexcept -> Mat4 = 0;
      virtual auto GetViewTransform(Level const& = {}) const noexcept -> Mat4 = 0;
      virtual auto GetColor() const noexcept -> RGBA = 0;
   };

   ///                                                                        
   ///   Abstract physical bond part                                          
   struct Bond : virtual Physical {
      using CTTI_Bases = Physical;
      using CTTI_Producer = World;
      Bond() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract physical particles part                                     
   struct Particles : virtual Physical {
      using CTTI_Bases = Physical;
      using CTTI_Producer = World;
      Particles() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract physical field part                                         
   struct Field : virtual Physical {
      using CTTI_Bases = Physical;
      using CTTI_Producer = World;
      Field() : Downcastable {this} {}
   };
}

namespace Langulus::CT
{
   /// A concept for any kind of a physical part                              
   template<class T>
   concept Physical = DerivedFrom<T, Things::Physical>;
}
