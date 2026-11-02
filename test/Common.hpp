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
#include <Langulus/Testing.hpp>

using namespace Langulus;
using Things::Part;

/// A part implementation for testing                                         
class TestUnit1 final : public Part {
public:
   using CTTI_Abstract  = No;
   using CTTI_Bases     = Part;
   using CTTI_Pooled    = PooledByType<>;

   TestUnit1() : Downcastable {this} {}

   TestUnit1(Neat const& describe) : Downcastable {this} {
      Couple(describe);
   }

   ~TestUnit1() {
      Logger::Verbose(this, ": destroying...");
   }

   void Refresh() {}
};

/// A part implementation for testing                                         
class TestUnit2 final : public Part {
public:
   using CTTI_Abstract  = No;
   using CTTI_Bases     = Part;
   using CTTI_Pooled    = PooledByType<>;

   TestUnit2() : Downcastable {this} {}

   TestUnit2(Neat const& describe) : Downcastable {this} {
      Couple(describe);
   }

   ~TestUnit2() {
      Logger::Verbose(this, ": destroying...");
   }

   void Refresh() {}
};