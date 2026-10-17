///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Thing.hpp>
#include <Langulus/Event.hpp>


namespace Langulus::Things
{
   ///                                                                        
   ///   Abstract input module                                                
   ///                                                                        
   struct InputModule : virtual Module {
      using CTTI_Bases = Module;
      InputModule() : /*Resolvable {this},*/ Module {nullptr} {}
   };

   ///                                                                        
   ///   Abstract input part                                                  
   ///                                                                        
   struct Input : virtual Part {
      using CTTI_Bases = Part;
      Input() /*: Resolvable {this}*/ {}
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
      InputGatherer() /*: Resolvable {this}*/ {}

      void PushEvent(const Event& e) {
         const auto foundEvent = mEventQueue.Find(e.GetEvent());
         if (foundEvent) {
            const auto foundState = foundEvent.GetValue().Find(e.GetEventState());
            if (foundState) {
               // Event already exists, merge payload                   
               foundState.GetValue().mPayload += e.GetPayload();
               return;
            }
            foundEvent.GetValue().Insert(e.GetEventState(), e);
         }
         else {
            mEventQueue.Insert(e.GetEvent());
            auto& newGroup = mEventQueue[e.GetEvent()];
            newGroup.Insert(e.GetEventState(), e);
         }
      }
   };

   ///                                                                        
   ///   Abstract input listener                                              
   ///                                                                        
   struct InputListener : virtual Input {
      using CTTI_Bases = Input;
      using CTTI_Producer = InputGatherer;
      
      InputListener() /*: Resolvable {this}*/ {}
   };
}
