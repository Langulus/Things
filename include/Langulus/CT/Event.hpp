
///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Typenav.hpp>


namespace Langulus::CTTI
{
   /// Affects CT::Event<T>                                                   
   template<class T>
   struct Event;
}

LANGULUS_CTTI_CONCEPT_DECVQ(Event);
