///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           

/// INTENTIONALLY NOT GUARDED                                                 
/// Include this file once in each cpp file, after all other headers          
#include <Langulus/Thing.hpp>
#include <Langulus/Testing.hpp>


/// A part implementation for testing                                         
class TestUnit1 final : public Part {
public:
   using CTTI_Abstract = No;
   LANGULUS_BASES(Part);
   LANGULUS(POOL_TACTIC) RTTI::PoolTactic::Type;

   TestUnit1() : Downcastable {this} {}

   TestUnit1(Describe&& describe)
      : Downcastable {this} {
      Couple(*describe);
   }

   ~TestUnit1() {
      Logger::Verbose(this, ": destroying...");
   }

   void Refresh() {}
};

/// A part implementation for testing                                         
class TestUnit2 final : public Part {
public:
   using CTTI_Abstract = No;
   LANGULUS_BASES(Part);
   LANGULUS(POOL_TACTIC) RTTI::PoolTactic::Type;

   TestUnit2() : Downcastable {this} {}

   TestUnit2(Describe&& describe)
      : Downcastable {this} {
      Couple(*describe);
   }

   ~TestUnit2() {
      Logger::Verbose(this, ": destroying...");
   }

   void Refresh() {}
};