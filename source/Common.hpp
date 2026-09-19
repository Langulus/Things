///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/TSet.hpp>
#include <Langulus/TMap.hpp>
#include <Langulus/Many.hpp>
#include <Langulus/TRef.hpp>
#include <Langulus/Tag.hpp>
#include <Langulus/Path.hpp>
#include <Langulus/Flow/Verb.hpp>
#include <Langulus/Flow/Resolvable.hpp>

#if defined(LANGULUS_EXPORT_ALL) or defined(LANGULUS_EXPORT_THINGS)
   #define LANGULUS_API_THINGS() LANGULUS_EXPORT()
#else
   #define LANGULUS_API_THINGS() LANGULUS_IMPORT()
#endif


namespace Langulus
{
   using namespace Annies;
   using namespace Flow;
}


/// Make the rest of the code aware, that Langulus::Things has been included  
#define LANGULUS_LIBRARY_THINGS() 1

#if 0
   #define THINGS_VERBOSE_ENABLED()             1
   #define THINGS_VERBOSE_SELF(...)             Logger::Info(this, ": ", __VA_ARGS__)
   #define THINGS_VERBOSE_SELF_TAB(...)         const auto scoped = Logger::InfoTab(this, ": ", __VA_ARGS__)
   #define THINGS_VERBOSE(...)                  Logger::Append(__VA_ARGS__)
   #define THINGS_CREATION_VERBOSE_SELF(...)    Logger::Verbose(Self(), __VA_ARGS__)
   #define THINGS_SELECTION_VERBOSE_SELF(...)   Logger::Verbose(Self(), __VA_ARGS__)
#else
   #define THINGS_VERBOSE_ENABLED()             0
   #define THINGS_VERBOSE_SELF(...)             LANGULUS(NOOP)
   #define THINGS_VERBOSE_SELF_TAB(...)         LANGULUS(NOOP)
   #define THINGS_VERBOSE(...)                  LANGULUS(NOOP)
   #define THINGS_CREATION_VERBOSE_SELF(...)    LANGULUS(NOOP)
   #define THINGS_SELECTION_VERBOSE_SELF(...)   LANGULUS(NOOP)
#endif

/// Define a tag, both in a type-erased and templated form                    
#define LANGULUS_DEFINE_TAG(NAME, INFO)                                       \
   namespace Langulus::Tags {                                                 \
      struct NAME : ::Langulus::Annies::Tag {                                 \
         using CTTI_DefineTag = ::Langulus::Yes<#NAME>;                       \
         using CTTI_Info      = ::Langulus::Yes<INFO>;                        \
         using ::Langulus::Annies::Tag::Tag;                                  \
      };                                                                      \
      template<CT::NotVoid T>                                                 \
      struct T##NAME : ::Langulus::Annies::TTag<::Langulus::Tags::NAME, T> {  \
         using ::Langulus::Annies::TTag<T>::TTag;                             \
      };                                                                      \
   }

LANGULUS_DEFINE_TAG(Logger,
   "Logger trait, used to access the logger instance");
LANGULUS_DEFINE_TAG(Count,
   "Count trait, used to access container size, or other similar properties");
LANGULUS_DEFINE_TAG(Name,
   "Name trait, used to access names, or other similar properties");
LANGULUS_DEFINE_TAG(Path,
   "Path trait, used to access files and folders, or other file-system related stuff");
LANGULUS_DEFINE_TAG(Data,
   "Raw data trait, used to access raw container data, or other similar properties");
LANGULUS_DEFINE_TAG(Index,
   "Index trait, used to access the index of elements, or other similar properties");
LANGULUS_DEFINE_TAG(Context,
   "Context trait, used to access verb source, the current environment, or other similar properties");
LANGULUS_DEFINE_TAG(Trait, 
   "Accesses traits (static or dynamic variables) of an instantiated object of any kind");
LANGULUS_DEFINE_TAG(State, 
   "State trait, used to access the state of an object");
LANGULUS_DEFINE_TAG(Child,
   "Accesses children in any kind of hierarchy");
LANGULUS_DEFINE_TAG(Parent,
   "Accesses parents in any kind of hierarchy");
LANGULUS_DEFINE_TAG(Clipboard,
   "Accesses the system clipboard");
LANGULUS_DEFINE_TAG(Color,
   "Accesses associated color properties");
LANGULUS_DEFINE_TAG(Min,
   "Accesses smallest element in a container, or in other similar contexts");
LANGULUS_DEFINE_TAG(Max,
   "Accesses biggest element in a container, or in other similar contexts");
LANGULUS_DEFINE_TAG(Input,
   "For accessing verb arguments, or general inputs of some operation");
LANGULUS_DEFINE_TAG(Output,
   "For accessing the outputs of a verb, or general output of some operation");
LANGULUS_DEFINE_TAG(Mass,
   "Mass of anything with charge, amplitude, or literally physical mass");
LANGULUS_DEFINE_TAG(Rate,
   "Rate of anything with charge, or with physical frequency");
LANGULUS_DEFINE_TAG(Time,
   "Time of anything with charge, or with a temporal component");
LANGULUS_DEFINE_TAG(Priority,
   "Priority of anything with charge, or some kind of priority");
