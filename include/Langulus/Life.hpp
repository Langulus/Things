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
   ///   Abstract life module                                                 
   ///                                                                        
   struct Life : virtual Module {
      using CTTI_Bases = Module;
      Life() : Resolvable {this}, Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract life unit                                                   
   ///                                                                        
   struct LifeUnit : virtual Unit {
      using CTTI_Bases = Unit;
      using CTTI_Producer = Life;
      LifeUnit() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract ecosystem                                                   
   ///                                                                        
   struct Ecosystem : virtual LifeUnit {
      using CTTI_Bases = LifeUnit;
      Ecosystem() : Resolvable {this} {}
   };
   
   ///                                                                        
   ///   Abstract organism                                                    
   ///                                                                        
   struct Organism : virtual LifeUnit {
      using CTTI_Bases = LifeUnit;
      using CTTI_Producer = Ecosystem;
      Organism() : Resolvable {this} {}
   };
}