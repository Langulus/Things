///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "../Thing.hpp"
#include "../Part.hpp"
#include "../Event.hpp"
#include <Langulus/TMap.hpp>
//#include <Langulus/MetaOf.hpp>
#include <utility>


namespace Langulus::Things
{
   ///                                                                        
   ///   Abstract input module                                                
   ///                                                                        
   struct InputModule : virtual Module {
      using CTTI_Bases = Module;
      InputModule() : Downcastable {this}, Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract input part                                                  
   ///                                                                        
   struct Input : virtual Part {
      using CTTI_Bases = Part;
      Input() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract input gatherer                                              
   ///                                                                        
   struct InputGatherer : virtual Input {
      using CTTI_Bases = Input;
      using CTTI_Producer = InputModule;

   protected:
      // Accumulated events, dispatched on each Update() to listeners   
      TMapUnsorted<::std::pair<RTTI::DMeta, EventState>, Event> mEventQueue;

   public:
      InputGatherer() : Downcastable {this} {}

      /// Push an event to the queue                                          
      void PushEvent(Event const& e) {
         const auto pair  = ::std::pair {e.GetEvent(), e.GetEventState()};
         const auto found = mEventQueue.Find(pair);
         if (found) {
            // Event-state pair already exists, merge payload           
            (*found.GetVal()).Concat(e.GetPayload());
         }
         else {
            // Event-state pair doesn't exist yet                       
            mEventQueue.Merge(pair, e);
         }
      }
   };

   ///                                                                        
   ///   Abstract input listener                                              
   ///                                                                        
   struct InputListener : virtual Input {
      using CTTI_Bases = Input;
      using CTTI_Producer = InputGatherer;
      
      InputListener() : Downcastable {this} {}
   };
}
