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
   ///   Abstract user module                                                 
   ///                                                                        
   struct UserModule : virtual Module {
      using CTTI_Bases = Module;
      UserModule() : Resolvable {this}, Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract user unit                                                   
   ///                                                                        
   struct User : virtual Unit {
      using CTTI_Bases = Unit;
      using CTTI_Producer = UserModule;
      User() : Resolvable {this} {}
   };
}