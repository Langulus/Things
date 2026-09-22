///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Thing.hpp"
#include <Langulus/LOD.hpp>
#include <Langulus/Numbers/Level.hpp>
#include <Langulus/Matrices/TMatrix.hpp>
#include <Langulus/Colors/TColor.hpp>


namespace Langulus::Things
{
   ///                                                                        
   ///   Abstract physical module                                             
   ///                                                                        
   struct PhysicalModule : virtual Module {
      using CTTI_Bases = Module;
      PhysicalModule() : Resolvable {this}, Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract physical unit                                               
   ///                                                                        
   struct Physical : virtual Unit {
      using CTTI_Bases = Unit;
      Physical() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract physical world unit                                         
   ///                                                                        
   struct World : virtual Physical {
      using CTTI_Bases = Physical;
      using CTTI_Producer = PhysicalModule;
      World() : Resolvable {this} {}
   };
   
   ///                                                                        
   ///   Abstract physical instance unit                                      
   ///                                                                        
   struct Instance : virtual Physical {
      using CTTI_Bases = Physical;
      using CTTI_Producer = World;
      Instance() : Resolvable {this} {}

      using LOD   = Math::LOD;
      using Level = Math::Level;
      using Mat4  = Math::Mat4;
      using RGBA  = Math::RGBA;

      virtual bool Cull(const LOD&) const noexcept = 0;
      virtual auto GetLevel() const noexcept -> Level = 0;
      virtual auto GetModelTransform(const LOD&) const noexcept -> Mat4 = 0;
      virtual auto GetModelTransform(const Level& = {}) const noexcept -> Mat4 = 0;
      virtual auto GetViewTransform(const LOD&) const noexcept -> Mat4 = 0;
      virtual auto GetViewTransform(const Level& = {}) const noexcept -> Mat4 = 0;
      virtual auto GetColor() const noexcept -> RGBA = 0;
   };

   ///                                                                        
   ///   Abstract physical bond unit                                          
   ///                                                                        
   struct Bond : virtual Physical {
      using CTTI_Bases = Physical;
      using CTTI_Producer = World;
      Bond() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract physical particles unit                                     
   ///                                                                        
   struct Particles : virtual Physical {
      using CTTI_Bases = Physical;
      using CTTI_Producer = World;
      Particles() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract physical field unit                                         
   ///                                                                        
   struct Field : virtual Physical {
      using CTTI_Bases = Physical;
      using CTTI_Producer = World;
      Field() : Resolvable {this} {}
   };
}

namespace Langulus::CT
{
   /// A concept for any kind of a physical unit                              
   template<class T>
   concept Physical = DerivedFrom<T, Things::Physical>;
}
