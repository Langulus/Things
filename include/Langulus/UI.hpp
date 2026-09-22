///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Thing.hpp"
#include "Module.hpp"


namespace Langulus::Things
{
   ///                                                                        
   ///   Abstract UI module                                                   
   ///                                                                        
   struct UIModule : virtual Module {
      using CTTI_Bases = Module;
      UIModule() : Resolvable {this}, A::Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract UI unit                                                     
   ///                                                                        
   struct UIUnit : virtual Unit {
      using CTTI_Bases = Unit;
      using CTTI_Producer = UIModule;
      UIUnit() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract UI system                                                   
   ///                                                                        
   struct UISystem : virtual UIUnit {
      using CTTI_Bases = UIUnit;
      UISystem() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract UI button                                                   
   ///                                                                        
   struct UIButton : virtual UIUnit {
      using CTTI_Bases = UIUnit;
      using CTTI_Producer = UISystem;
      UIButton() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract UI text field                                               
   ///                                                                        
   struct UILabel : virtual UIUnit {
      using CTTI_Bases = UIUnit;
      using CTTI_Producer = UISystem;
      UILabel() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract UI input field                                              
   ///                                                                        
   struct UIInput : virtual UIUnit {
      using CTTI_Bases = UIUnit;
      using CTTI_Producer = UISystem;
      UIInput() : Resolvable {this} {}
   };
}