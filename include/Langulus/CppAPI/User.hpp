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
   ///   Abstract user module                                                 
   ///                                                                        
   struct UserModule : virtual Module {
      using CTTI_Bases = Module;
      UserModule() : Downcastable {this}, Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract user part                                                   
   ///                                                                        
   struct User : virtual Part {
      using CTTI_Bases = Part;
      using CTTI_Producer = UserModule;
      User() : Downcastable {this} {}
   };
}