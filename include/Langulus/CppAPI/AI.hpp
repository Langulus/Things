///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Thing.hpp>


namespace Langulus::Things
{
   ///                                                                        
   ///   Abstract artificial intelligence module                              
   ///                                                                        
   struct AIModule : virtual Module {
      using CTTI_Bases = Module;
      AIModule() : Resolvable {this}, Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract artificial intelligence unit                                
   ///                                                                        
   struct AIUnit : virtual Part {
      using CTTI_Bases     = Part;
      using CTTI_Producer  = AIModule;
      AIUnit() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract mind                                                        
   ///                                                                        
   struct Mind : virtual AIUnit {
      using CTTI_Bases = AIUnit;
      Mind() : Resolvable {this} {}

      virtual Many Interpret(Text const&) = 0;
   };
}