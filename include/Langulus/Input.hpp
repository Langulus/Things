///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Thing.hpp"
#include "Event.hpp"


namespace Langulus::Things
{
   ///                                                                        
   ///   Abstract input module                                                
   ///                                                                        
   struct InputModule : virtual Module {
      using CTTI_Bases = Module;
      InputModule() : Resolvable {this}, Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract input unit                                                  
   ///                                                                        
   struct Input : virtual Unit {
      using CTTI_Bases = Unit;
      Input() : Resolvable {this} {}
   };

   ///                                                                        
   ///   Abstract input gatherer                                              
   ///                                                                        
   struct InputGatherer : virtual Input {
      using CTTI_Bases = Input;
      using CTTI_Producer = InputModule;

   protected:
      // Accumulated events, dispatched on each Update() to listeners   
      EventList mEventQueue;

   public:
      InputGatherer() : Resolvable {this} {}

      void PushEvent(const Event& e) {
         const auto foundEvent = mEventQueue.FindIt(e.mType);
         if (foundEvent) {
            const auto foundState = foundEvent.GetValue().FindIt(e.mState);
            if (foundState) {
               // Event already exists, merge payload                   
               foundState.GetValue().mPayload += e.mPayload;
               return;
            }
            foundEvent.GetValue().Insert(e.mState, e);
         }
         else {
            mEventQueue.Insert(e.mType);
            auto& newGroup = mEventQueue[e.mType];
            newGroup.Insert(e.mState, e);
         }
      }
   };

   ///                                                                        
   ///   Abstract input listener                                              
   ///                                                                        
   struct InputListener : virtual Input {
      using CTTI_Bases = Input;
      using CTTI_Producer = InputGatherer;
      
      InputListener() : Resolvable {this} {}
   };
}
