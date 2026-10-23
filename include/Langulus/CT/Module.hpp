
///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Typenav.hpp>
#include <Langulus/CT/Derived.hpp>

namespace Langulus::Things
{
   struct Module;
}

namespace Langulus::CTTI
{
   /// Affects CT::Module<T>                                                  
   template<class T>
   struct Module;

   /// Anything inheriting the Things::Module interface is considered         
   /// CT::Module                                                             
   ///   @attention ignores first level of indirection                        
   template<class T> requires CT::DerivedFrom<Deptr<T>, Things::Module>
   struct Module<T> {};
}

LANGULUS_CTTI_CONCEPT_DECVQ(Module);