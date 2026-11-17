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
   #define THINGS_CREATION_VERBOSE_SELF(...)    Logger::Verbose(this, __VA_ARGS__)
   #define THINGS_SELECTION_VERBOSE_SELF(...)   Logger::Verbose(this, __VA_ARGS__) //TODO use the new include
#else
   #define THINGS_VERBOSE_ENABLED()             0
   #define THINGS_VERBOSE_SELF(...)             LANGULUS(NOOP)
   #define THINGS_VERBOSE_SELF_TAB(...)         LANGULUS(NOOP)
   #define THINGS_VERBOSE(...)                  LANGULUS(NOOP)
   #define THINGS_CREATION_VERBOSE_SELF(...)    LANGULUS(NOOP)
   #define THINGS_SELECTION_VERBOSE_SELF(...)   LANGULUS(NOOP)
#endif

namespace Langulus
{   
   LANGULUS_API(THINGS) extern void RegisterEvents();
}

namespace Langulus::Things
{
   struct Runtime;
   struct Module;
   struct Thing;
   struct Part;
   struct File;
   struct Folder;
}