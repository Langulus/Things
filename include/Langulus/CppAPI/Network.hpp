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
   ///   Abstract network module                                              
   ///                                                                        
   struct Network : virtual Module {
      using CTTI_Bases = Module;
      Network() : /*Resolvable {this},*/ Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract network part                                                
   ///                                                                        
   struct NetworkPart : virtual Part {
      using CTTI_Bases = Part;
      using CTTI_Producer = Network;
      NetworkPart() /*: Resolvable {this}*/ {}
   };

   ///                                                                        
   ///   Abstract network client                                              
   ///                                                                        
   struct Client : virtual NetworkPart {
      using CTTI_Bases = NetworkPart;
      Client() /*: Resolvable {this}*/ {}
   };
   
   ///                                                                        
   ///   Abstract network server                                              
   ///                                                                        
   struct Server : virtual NetworkPart {
      using CTTI_Bases = NetworkPart;
      Server() /*: Resolvable {this}*/ {}
   };
   
   ///                                                                        
   ///   Abstract network shared object                                       
   ///                                                                        
   struct Shared : virtual NetworkPart {
      using CTTI_Bases = NetworkPart;
      using CTTI_Producer = NetworkPart;
      Shared() /*: Resolvable {this}*/ {}
   };
}