///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Thing.hpp>


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
   struct Graphics : virtual Part {
      using CTTI_Bases = Part;
      Graphics() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics renderer                                           
   ///                                                                        
   struct Renderer : virtual Graphics {
      using CTTI_Producer = GraphicsModule;
      LANGULUS_BASES(Graphics);
      Renderer() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics layer                                              
   ///                                                                        
   struct Layer : virtual Graphics {
      using CTTI_Producer = Renderer;
      LANGULUS_BASES(Graphics);
      Layer() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics camera                                             
   ///                                                                        
   struct Camera : virtual Graphics {
      using CTTI_Producer = Layer;
      LANGULUS_BASES(Graphics);
      Camera() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics renderable                                         
   ///                                                                        
   struct Renderable : virtual Graphics {
      using CTTI_Producer = Layer;
      LANGULUS_BASES(Graphics);
      Renderable() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics light                                              
   ///                                                                        
   struct Light : virtual Graphics {
      using CTTI_Producer = Layer;
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
