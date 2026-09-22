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
   ///   Abstract network module                                              
   ///                                                                        
   struct Network : virtual Module {
      using CTTI_Bases = Module;
      Network() : Resolvable {this}, Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract network unit                                                
   ///                                                                        
   struct NetworkUnit : virtual Unit {
      using CTTI_Bases = Unit;
      using CTTI_Producer = Network;
      NetworkUnit() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract network client                                              
   ///                                                                        
   struct Client : virtual NetworkUnit {
      using CTTI_Bases = NetworkUnit;
      Client() : Resolvable {this} {}
   };
   
   ///                                                                        
   ///   Abstract network server                                              
   ///                                                                        
   struct Server : virtual NetworkUnit {
      using CTTI_Bases = NetworkUnit;
      Server() : Resolvable {this} {}
   };
   
   ///                                                                        
   ///   Abstract network shared object                                       
   ///                                                                        
   struct Shared : virtual NetworkUnit {
      using CTTI_Bases = NetworkUnit;
      using CTTI_Producer = NetworkUnit;
      Shared() : Resolvable {this} {}
   };
}