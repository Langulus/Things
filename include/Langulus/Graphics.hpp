///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Thing.hpp"


namespace Langulus
{
   ///                                                                        
   ///   Abstract graphics module                                             
   ///                                                                        
   struct GraphicsModule : virtual Module {
      using CTTI_Bases = Module;
      GraphicsModule() : Resolvable {this}, Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract graphics units                                              
   ///                                                                        
   struct Graphics : virtual Unit {
      using CTTI_Bases = Unit;
      Graphics() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics renderer                                           
   ///                                                                        
   struct Renderer : virtual Graphics {
      LANGULUS(PRODUCER) GraphicsModule;
      LANGULUS_BASES(Graphics);
      Renderer() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics layer                                              
   ///                                                                        
   struct Layer : virtual Graphics {
      LANGULUS(PRODUCER) Renderer;
      LANGULUS_BASES(Graphics);
      Layer() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics camera                                             
   ///                                                                        
   struct Camera : virtual Graphics {
      LANGULUS(PRODUCER) Layer;
      LANGULUS_BASES(Graphics);
      Camera() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics renderable                                         
   ///                                                                        
   struct Renderable : virtual Graphics {
      LANGULUS(PRODUCER) Layer;
      LANGULUS_BASES(Graphics);
      Renderable() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics light                                              
   ///                                                                        
   struct Light : virtual Graphics {
      LANGULUS(PRODUCER) Layer;
      LANGULUS_BASES(Graphics);
      Light() : Resolvable {this} {}

      enum Type {
         Directional = 0,
         Point,
         Spot,
         Domain
      };

      Type mType = Directional;
      bool mCastShadows = true;
   };
}

namespace Langulus::CT
{
   /// A concept for any kind of a graphics unit                              
   template<class T>
   concept Graphics = DerivedFrom<T, A::Graphics>;
}
