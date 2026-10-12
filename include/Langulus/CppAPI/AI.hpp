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
   ///   Abstract artificial intelligence module                              
   ///                                                                        
   struct AIModule : virtual Module {
      using CTTI_Bases = Module;
      AIModule() : /*Resolvable {this},*/ Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract artificial intelligence part                                
   ///                                                                        
   struct AIPart : virtual Part {
      using CTTI_Bases     = Part;
      using CTTI_Producer  = AIModule;
      AIPart() /*: Resolvable {this}*/ {}
   };

   ///                                                                        
   ///   Abstract mind                                                        
   ///                                                                        
   struct Mind : virtual AIPart {
      using CTTI_Bases = AIPart;
      Mind() /*: Resolvable {this}*/ {}

      virtual Many Interpret(Text const&) = 0;
   };
}