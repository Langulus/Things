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
   ///   Abstract UI module                                                   
   ///                                                                        
   struct UIModule : virtual Module {
      using CTTI_Bases = Module;
      UIModule() : Downcastable {this}, Things::Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract UI part                                                     
   ///                                                                        
   struct UIUnit : virtual Part {
      using CTTI_Bases = Part;
      using CTTI_Producer = UIModule;
      UIUnit() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract UI system                                                   
   ///                                                                        
   struct UISystem : virtual UIUnit {
      using CTTI_Bases = UIUnit;
      UISystem() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract UI button                                                   
   ///                                                                        
   struct UIButton : virtual UIUnit {
      using CTTI_Bases = UIUnit;
      using CTTI_Producer = UISystem;
      UIButton() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract UI text field                                               
   ///                                                                        
   struct UILabel : virtual UIUnit {
      using CTTI_Bases = UIUnit;
      using CTTI_Producer = UISystem;
      UILabel() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract UI input field                                              
   ///                                                                        
   struct UIInput : virtual UIUnit {
      using CTTI_Bases = UIUnit;
      using CTTI_Producer = UISystem;
      UIInput() : Downcastable {this} {}
   };
}