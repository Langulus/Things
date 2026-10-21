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


namespace Langulus::Things
{
   ///                                                                        
   ///   Abstract graphics module                                             
   ///                                                                        
   struct GraphicsModule : virtual Module {
      using CTTI_Bases = Module;
      GraphicsModule() : Downcastable {this}, Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract graphics parts                                              
   ///                                                                        
   struct Graphics : virtual Part {
      using CTTI_Bases = Part;
      Graphics() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics renderer                                           
   ///                                                                        
   struct Renderer : virtual Graphics {
      using CTTI_Producer = GraphicsModule;
      using CTTI_Bases = Graphics;
      Renderer() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics layer                                              
   ///                                                                        
   struct Layer : virtual Graphics {
      using CTTI_Producer = Renderer;
      using CTTI_Bases = Graphics;
      Layer() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics camera                                             
   ///                                                                        
   struct Camera : virtual Graphics {
      using CTTI_Producer = Layer;
      using CTTI_Bases = Graphics;
      Camera() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics renderable                                         
   ///                                                                        
   struct Renderable : virtual Graphics {
      using CTTI_Producer = Layer;
      using CTTI_Bases = Graphics;
      Renderable() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract graphics light                                              
   ///                                                                        
   struct Light : virtual Graphics {
      using CTTI_Producer = Layer;
      using CTTI_Bases = Graphics;
      Light() : Downcastable {this} {}

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
   /// A concept for any kind of a graphics part                              
   template<class T>
   concept Graphics = DerivedFrom<T, Things::Graphics>;
}
