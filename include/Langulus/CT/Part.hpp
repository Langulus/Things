
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
   struct Part;
}

namespace Langulus::CTTI
{
   /// Affects CT::Part<T>                                                    
   template<class T>
   struct Part;

   /// Anything inheriting the Things::Part interface is considered CT::Part  
   template<class T> requires CT::DerivedFrom<T, Things::Part>
   struct Part<T> {};
}

LANGULUS_CTTI_CONCEPT_DECVQ(Part);