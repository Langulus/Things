///                                                                           
/// Langulus::Flow                                                            
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Verbs/Do.hpp>
#include <Langulus/TTag.hpp>
#include <Langulus/TMany.hpp>

#if 0
   #define VERBOSE(...)      Logger::Verbose(__VA_ARGS__)
   #define VERBOSE_TAB(...)  const auto tab = Logger::VerboseTab(__VA_ARGS__)
#else
   #define VERBOSE(...)      LANGULUS(NOOP)
   #define VERBOSE_TAB(...)  LANGULUS(NOOP)
#endif

#define FLOW_ERRORS(...)  Logger::Error(__VA_ARGS__)


namespace Langulus::CTTI
{
   LglsImplementAbilitiesFor(Annies::Many) {
      using Can = Verbs::Do;

      /// Perform any flow in a deep context by dispatching the argument to   
      /// each subcontainer, preserving hierarchy in the outputs. Example:    
      ///    {1 or 2 or 3} add 1 --> {{1 add 1} or {2 add 1} or {3 add 1}}    
      ///                         resulting in:                               
      ///                         {2 or 3 or 4}                               
      static bool Default(Annies::Many const& lhs, Annies::Verb& verb) {
         auto& output  = verb.GetOutput();
         auto dispatch = Verbs::Do::Like(verb).In(lhs);
         if (not dispatch.Run())
            return false;
      
         output.Compose(Move(dispatch.GetOutput()));
         return true;
      }
   };
   
   LglsImplementAbilitiesFor(Annies::Tag) {
      using Can = Verbs::Do;

      /// Perform any verb in every tag inside the context, preserving        
      /// hierarchy in the outputs. Example:                                  
      ///         tag(1 or 2) add 1 --> tag({1 add 1} or {2 add 1})           
      ///                         resulting in:                               
      ///                          tag(2 or 3)                                
      static bool Default(Annies::Tag const& tag, Annies::Verb& verb) {
         auto& output  = verb.GetOutput();
         auto dispatch = Verbs::Do::Like(verb).In(tag.GetData());
         if (not dispatch.Run())
            return false;
      
         output.Compose(Annies::Tag::From(tag, Move(dispatch.GetOutput())));
         return true;
      }
   };
}

namespace Langulus::Flow
{
   /// Invoke a single verb on a single context                               
   ///   @tparam DISPATCH - whether or not to use context's dispatcher, if    
   ///      any is statically available or reflected. This is mainly used for 
   ///      infinite regress protection when doing a verb from a dispatcher   
   ///   @tparam DEFAULT - whether or not to attempt default verb execution   
   ///      if such is statically available or reflected this is done only    
   ///      if direct or dispatched execution fails                           
   ///   @tparam FALLBACK - for internal use by the function - used to nest   
   ///      with default functionality, if DEFAULT is enabled                 
   ///   @param context - the context in which to execute in                  
   ///   @param verb - the verb to execute                                    
   ///   @return the number of successful executions                          
   template<bool DISPATCH, bool DEFAULT, bool FALLBACK, CT::NotVoid T>
   size_t Execute(T& context, CT::Executable auto& verb) {
      // Always reset verb progress prior to execution                  
      verb.Undo();

      if constexpr (not FALLBACK and DISPATCH and requires { context.Do(verb); }) {
         // Custom reflected dispatcher is available                    
         // It's your responsibility to implement it adequately         
         // Keep in mind, that once you declare a custom Do for your    
         // type, you no longer rely on reflected bases' verbs or       
         // default verbs. You must invoke those by yourself in your    
         // dispatcher - the custom dispatcher provides full control!   
         context.Do(verb);
      }
      else {
         if constexpr (FALLBACK) {
            // Execute the default verb                                 
            if (context.IsValid())
               GenericExecuteDefault(context, verb);
            else
               GenericExecuteStateless(verb);
         }
         else if constexpr (DISPATCH) {
            // Context might have a dispatcher                          
            // If that is the case, then it is the context's            
            // responsibility to dispatch the verb!                     
            if constexpr (CT::Deep<T>) {
               if (context) {
                  auto meta = context.GetType();
                  if constexpr (CT::Constant<T>) {
                     if (meta->mDispatcherConstant)
                        meta->mDispatcherConstant(context.GetRaw(), verb);
                     else
                        GenericExecuteIn(context, verb);
                  }
                  else if (meta->mDispatcherConstant)
                     meta->mDispatcherConstant(context.GetRaw(), verb);
                  else if (meta->mDispatcherMutable)
                     meta->mDispatcherMutable(context.GetRaw(), verb);
                  else
                     GenericExecuteIn(context, verb);
               }
            }
            else GenericExecuteIn(context, verb);
         }
         else GenericExecuteIn(context, verb);

         if (verb.IsDone())
            return verb.GetSuccesses();

         // If that fails, attempt executing the default verb           
         if constexpr (DEFAULT and not FALLBACK) {
            if (not verb.IsDone()) {
               // Verb wasn't executed neither in current element,      
               // nor in any of its bases, so we resort to the          
               // default abilities                                     
               Execute<false, false, true>(context, verb);
            }
         }
      }

      return verb.GetSuccesses();
   }

}

#undef VERBOSE
#undef VERBOSE_TAB
#undef FLOW_ERRORS
