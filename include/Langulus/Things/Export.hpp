///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/TTag.hpp>
#include <Langulus/Verbs/Select.hpp>

#if defined(LANGULUS_EXPORT_ALL) or defined(LANGULUS_EXPORT_THINGS)
   #define LANGULUS_API_THINGS() LANGULUS_EXPORT()
#else
   #define LANGULUS_API_THINGS() LANGULUS_IMPORT()
#endif

/// Make the rest of the code aware, that Langulus::Things has been included  
#define LANGULUS_LIBRARY_THINGS() 1

#if 0
   #define THINGS_VERBOSE_ENABLED()             1
   #define THINGS_VERBOSE_SELF(...)             Logger::Info(this, ": ", __VA_ARGS__)
   #define THINGS_VERBOSE_SELF_TAB(...)         const auto scoped = Logger::InfoTab(this, ": ", __VA_ARGS__)
   #define THINGS_VERBOSE(...)                  Logger::Append(__VA_ARGS__)
   #define THINGS_CREATION_VERBOSE_SELF(...)    Logger::Verbose(Self(), __VA_ARGS__)
   #define THINGS_SELECTION_VERBOSE_SELF(...)   Logger::Verbose(Self(), __VA_ARGS__) //TODO use the new include
#else
   #define THINGS_VERBOSE_ENABLED()             0
   #define THINGS_VERBOSE_SELF(...)             LANGULUS(NOOP)
   #define THINGS_VERBOSE_SELF_TAB(...)         LANGULUS(NOOP)
   #define THINGS_VERBOSE(...)                  LANGULUS(NOOP)
   #define THINGS_CREATION_VERBOSE_SELF(...)    LANGULUS(NOOP)
   #define THINGS_SELECTION_VERBOSE_SELF(...)   LANGULUS(NOOP)
#endif

LANGULUS_DEFINE_TAG(Logger,
   "Logger instances");
LANGULUS_DEFINE_TAG(Count,
   "Container size, or other similar properties");
LANGULUS_DEFINE_TAG(Name,
   "Names, or other similar properties");
LANGULUS_DEFINE_TAG(Path,
   "Files, folders, or other file-system related things");
LANGULUS_DEFINE_TAG(Data,
   "Raw container data, or other similar properties");
LANGULUS_DEFINE_TAG(Index,
   "Index of an element in a container, or other similar properties");
LANGULUS_DEFINE_TAG(Context,
   "Verb source, code environment, or other similar properties");
LANGULUS_DEFINE_TAG(Tag,
   "Variables and properties");
LANGULUS_DEFINE_TAG(State,
   "Boolean states and other similar properties");
LANGULUS_DEFINE_TAG(Child,
   "Children in a hierarchy");
LANGULUS_DEFINE_TAG(Parent,
   "Parents in a hierarchy");
LANGULUS_DEFINE_TAG(Clipboard,
   "Operating system clipboard");
LANGULUS_DEFINE_TAG(Color,
   "Color properties");
LANGULUS_DEFINE_TAG(Input,
   "Verb arguments, or general inputs of an operation");
LANGULUS_DEFINE_TAG(Output,
   "Verb outputs, or general output of an operation");
LANGULUS_DEFINE_TAG(Mass,
   "Magnitude, amplitude, or just physical mass");
LANGULUS_DEFINE_TAG(Rate,
   "Rate, frequency, or other similar properties");
LANGULUS_DEFINE_TAG(Time,
   "Temporal properties");
LANGULUS_DEFINE_TAG(Precedence,
   "Precedence, priority, or other similar properties");

namespace Langulus::CTTI
{
   LglsImplementAbilitiesFor(void) {
      using Can = Verbs::Select;

      /// Stateless selection, for selecting some global things, like the     
      /// logger, for example.                                                
      ///   @param verb selection verb                                        
      ///   @return true if verb has been satisfied                           
      bool Stateless(Verb& verb) {
         verb.ForEachDeep([&](RTTI::TMeta t) {
            if (t == MetaTagOf<Tags::Logger>())
               verb << Logger::Context {};
         });
         return verb.IsDone();
      }
   }
}