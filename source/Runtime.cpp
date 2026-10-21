///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/Thing.hpp>
#include <Langulus/TTable.hpp>
#include <Langulus/MetaOf.hpp>

#include <Langulus/CppAPI/Art.hpp>
#include <Langulus/CppAPI/Graphics.hpp>
#include <Langulus/CppAPI/Image.hpp>
#include <Langulus/CppAPI/IO.hpp>
#include <Langulus/CppAPI/Material.hpp>
#include <Langulus/CppAPI/Mesh.hpp>
#include <Langulus/CppAPI/Physical.hpp>
#include <Langulus/CppAPI/Platform.hpp>
#include <Langulus/CppAPI/UI.hpp>
#include <Langulus/CppAPI/AI.hpp>
#include <Langulus/CppAPI/Input.hpp>
#include <Langulus/CppAPI/Life.hpp>
#include <Langulus/CppAPI/Network.hpp>
#include <Langulus/CppAPI/User.hpp>
#include <Langulus/CppAPI/Economy.hpp>

#if LANGULUS_OS(WINDOWS)
   #include <Windows.h>
#endif

#if LANGULUS_OS(LINUX) or LANGULUS_COMPILER(WASM)
   #include <dlfcn.h>
#endif

#if LANGULUS_OS(ANDROID)
   //TODO
#endif

#if LANGULUS_OS(MACOS)
   //TODO
#endif

#if LANGULUS_OS(UNIX)
   //TODO
#endif

#if LANGULUS_OS(FREEBSD)
   //TODO
#endif

#if 0
   #define VERBOSE(...) Logger::Info(__VA_ARGS__)
#else
   #define VERBOSE(...) LANGULUS(NOOP)
#endif

namespace
{
   using namespace Langulus;
   using Langulus::RTTI::DMeta;
   using Langulus::Things::ModList;
   using ModMap = Runtime::ModMap;


   /// Close a shared library handle, unloading it                            
   ///   @param library the library handle                                    
   void UnloadSharedLibraryInner(uintptr_t library) {
      #if LANGULUS_OS(WINDOWS)
         FreeLibrary(reinterpret_cast<HMODULE>(library));
      #else
         dlclose(reinterpret_cast<void*>(library));
      #endif
   }

   /// Loaded shared libraries, indexed by filename.                          
   /// This is a static registry - all Runtimes use the same shared           
   /// library objects, but manage their own module instantiations.           
   TMapUnsorted<Path, Runtime::SharedLibrary> gLibraries;

   /// Register by all bases in mModulesByType                                
   ///   @param map [in/out] the map to fill                                  
   ///   @param module the module instance to push                            
   ///   @param type the type to unregister the module as                     
   void RegisterAllBases(ModMap& map, Module* module, DMeta type) {
      VERBOSE("Registering `", type, '`');
      auto found = map.Find(type);
      if (found)
         (*found.GetVal()).Insert(module);
      else
         map.Merge(type, module);

      for (auto& base : type.GetBases()) {
         //if (base.type.IsExact<Resolvable>())//TODO explain why is this exception here?
         //   break;
         RegisterAllBases(map, module, base.type);
      }
   }

   /// Unregister by all bases in mModulesByType (in reverse order)           
   ///   @param map [in/out] the map to unregister from                       
   ///   @param module the module instance to push                            
   ///   @param type the type to register the module as                       
   void UnregisterAllBases(ModMap& map, Module* module, DMeta type) {
      for (auto& base : type.GetBases()) {
         //if (base.mType->IsExact<Resolvable>())//TODO explain why is this exception here?
         //   break;
         UnregisterAllBases(map, module, base.type);
      }

      const auto found = map.Find(type);
      if (found) {
         auto& list = *found.GetVal();
         if (list.Erase(module) and not list) {
            VERBOSE("Unregistering `", type, '`');
            (void) map.Erase(found);
            if (not map)
               map.Reset();
         }
      }
   }
}

namespace Langulus::Things
{
   /// Runtime construction                                                   
   ///   @param owner the owner of the runtime                                
   Runtime::Runtime(Thing* owner) noexcept
      : mOwner {owner} {
      VERBOSE(this, ": Initializing...");

      // Reflect fundamentals                                           
      /*(void)MetaDataOf<A::Number>();
      (void)MetaDataOf<A::Integer>();
      (void)MetaDataOf<A::Signed>();
      (void)MetaDataOf<A::Unsigned>();
      (void)MetaDataOf<A::UnsignedInteger>();
      (void)MetaDataOf<A::Real>();
      (void)MetaDataOf<A::SignedInteger>();
      (void)MetaDataOf<A::Char>();
      (void)MetaDataOf<A::Bool>();

      Math::RegisterTraits();
      Math::RegisterVerbs();

      // We always prefer 'type' definitions that are in the main       
      // boundary, to avoid segfaults when unloading libraries from     
      // mModulesByType - it is indexed by a DMeta                      
      // So we make sure that the base modules are defined here         
      (void)MetaDataOf<Thing>();

      (void)MetaDataOf<Things::PlatformModule>();
      (void)MetaDataOf<Things::Platform>();
      (void)MetaDataOf<Things::Window>();
      (void)MetaDataOf<Things::Cursor>();

      (void)MetaDataOf<Things::PhysicalModule>();
      (void)MetaDataOf<Things::Physical>();
      (void)MetaDataOf<Things::World>();
      (void)MetaDataOf<Things::Instance>();

      (void)MetaDataOf<Things::UIModule>();
      (void)MetaDataOf<Things::UIUnit>();
      (void)MetaDataOf<Things::UISystem>();
      (void)MetaDataOf<Things::UIButton>();
      (void)MetaDataOf<Things::UILabel>();
      (void)MetaDataOf<Things::UIInput>();

      (void)MetaDataOf<Things::FileSystem>();
      (void)MetaDataOf<Things::File>();
      (void)MetaDataOf<Things::Folder>();

      (void)MetaDataOf<Things::GraphicsModule>();
      (void)MetaDataOf<Things::Graphics>();
      (void)MetaDataOf<Things::Renderer>();
      (void)MetaDataOf<Things::Layer>();
      (void)MetaDataOf<Things::Camera>();
      (void)MetaDataOf<Things::Renderable>();
      (void)MetaDataOf<Things::Light>();

      (void)MetaDataOf<Things::AssetModule>();
      (void)MetaDataOf<Things::Asset>();
      (void)MetaDataOf<Things::Image>();
      (void)MetaDataOf<Things::Font>();
      (void)MetaDataOf<Things::Material>();
      (void)MetaDataOf<Things::Mesh>();

      (void)MetaDataOf<Things::AIModule>();
      (void)MetaDataOf<Things::AIPart>();
      (void)MetaDataOf<Things::Mind>();

      (void)MetaDataOf<Things::InputModule>();
      (void)MetaDataOf<Things::Input>();
      (void)MetaDataOf<Things::InputGatherer>();
      (void)MetaDataOf<Things::InputListener>();

      (void)MetaDataOf<Things::Life>();
      (void)MetaDataOf<Things::LifeUnit>();
      (void)MetaDataOf<Things::Ecosystem>();
      (void)MetaDataOf<Things::Organism>();

      (void)MetaDataOf<Things::Economy>();
      (void)MetaDataOf<Things::EconomyPart>();
      (void)MetaDataOf<Things::Resource>();
      (void)MetaDataOf<Things::ResourceInstance>();
      (void)MetaDataOf<Things::Converter>();
      (void)MetaDataOf<Things::ConverterInstance>();
      (void)MetaDataOf<Things::Trader>();

      (void)MetaDataOf<Things::Network>();
      (void)MetaDataOf<Things::NetworkPart>();
      (void)MetaDataOf<Things::Client>();
      (void)MetaDataOf<Things::Server>();
      (void)MetaDataOf<Things::Shared>();

      (void)MetaDataOf<Things::UserModule>();
      (void)MetaDataOf<Things::User>();*/

      VERBOSE(this, ": Initialized");
   }

   /// Runtime destruction                                                    
   Runtime::~Runtime() {
      VERBOSE(this, ": Shutting down...");

      // First-stage destruction: tear down any potential circular      
      // references. This minimizes references and destroys no longer   
      // referenced data.                                               
      for (auto& modlist : InValuesOf(mModules)) {
         for (auto& mod : modlist)
            (*mod)->Teardown();
      }

      // Second-stage destruction: cycle through N libraries N^2        
      // times, unloading anything that is no longer in use each time   
      auto attempts = gLibraries.GetCount();
      while (attempts) {
         for (auto library : gLibraries) {
            if (UnloadSharedLibrary(*library.GetVal()))
               library = gLibraries.Erase(library);
         }

         --attempts;
      }

      // If after all attempts there's still active libraries, then     
      // something is definitely not right. This will always result in  
      // a crash, since we can't really throw inside a destructor.      
      if (gLibraries) {
         Logger::Error(this, ": Can't unload last module(s): ");
         for (auto library : gLibraries)
            Logger::Append(library.GetKey(), " ");

         Logger::Error(this, ": This likely involves a memory leak "
            "that withholds managed data reflected by the given modules");

         IF_LANGULUS_MANAGED_MEMORY(Allocator::CollectGarbage());
         IF_LANGULUS_MEMORY_STATISTICS(Allocator::DumpPools());
         LglsError("Can't unload last module(s)");
      }
   }

   /// Create a module instance or return an already instantiated one         
   ///   @param path explicit relative path                                   
   ///   @param d module initialization descriptor                            
   ///   @return the new module instance                                      
   auto Runtime::InstantiateModulePath(Path const& path, Many const& d) -> Module* {
      // Load the library if not loaded yet                             
      const auto library = LoadSharedLibraryPath(path);

      // Check if module is already instantiated                        
      auto& foundModules = GetModules(library.mModuleType);
      if (foundModules)
         return const_cast<Module*>(foundModules[0]);

      // A module instance doesn't exist yet, so instantiate it         
      VERBOSE(this, ": Module `", path, "` is not instantiated yet"
         ", so attempting to create it...");
      const auto instance = InstantiateModule(library, d);
      if (not instance)
         (void) UnloadSharedLibrary(library);
      return instance;
   }
   
   /// Create a module instance or return an already instantiated one         
   ///   @param name module name                                              
   ///   @param d module initialization descriptor                            
   ///   @return the new module instance                                      
   auto Runtime::InstantiateModule(Token const& name, Many const& d) -> Module* {
      // Load the library if not loaded yet                             
      const auto library = LoadSharedLibrary(name);

      // Check if module is already instantiated                        
      auto& foundModules = GetModules(library.mModuleType);
      if (foundModules)
         return const_cast<Module*>(foundModules[0]);

      // A module instance doesn't exist yet, so instantiate it         
      VERBOSE(this, ": Module `", name, "` is not instantiated yet"
         ", so attempting to create it...");
      const auto instance = InstantiateModule(library, d);
      if (not instance)
         (void) UnloadSharedLibrary(library);
      return instance;
   }
   
   /// Create a module instance or return an already instantiated one         
   ///   @param library the library handle                                    
   ///   @param d module initialization descriptor                            
   ///   @return the new module instance                                      
   auto Runtime::InstantiateModule(const SharedLibrary& library, Many const& d) -> Module* {
      if (not library.IsValid())
         return nullptr;

      // Use the creation point of the library to instantiate module    
      const auto info = library.mInfo();
      Things::Module* module {};

      try { module = library.mCreator(this, d); }
      catch (...) {
         Logger::Error("Module `", info->mName,
            "` creator has thrown an exception");
         return nullptr;
      }

      if (not module) {
         Logger::Error("Module `", info->mName,
            "` creator didn't provide a module");
         return nullptr;
      }

      // Register the module in the various maps, for fast retrieval    
      try {
         auto found = mModules.Find(info->mPriority);
         if (found)
            (*found.GetVal()).Insert(module);
         else
            mModules.Merge(info->mPriority, ModList {module});

         RegisterAllBases(mModulesByType, module, module->GetType());
      }
      catch (...) {
         Logger::Error("Registering module `", info->mName, "` failed");

         // Make sure we end up in an invariant state                   
         const auto found = mModules.Find(info->mPriority);
         if (found) {
            (*found.GetVal()).Erase(module);
            if (not *found.GetVal())
               (void) mModules.Erase(found);
         }

         if (not mModules)
            mModules.Reset();

         UnregisterAllBases(mModulesByType, module, module->GetType());
         delete module;
         return nullptr;
      }

      // Done, if reached                                               
      VERBOSE(this, ": Module `", info->mName,
         "` registered with priority ", info->mPriority);
      return module;
   }

   /// Load a shared library for a module using explicit path                 
   ///   @param path the relative path to the library                         
   ///   @return the shared library handle                                    
   LANGULUS(NOINLINE)
   auto Runtime::LoadSharedLibraryPath(Path path) -> SharedLibrary {
      // Check if this library is already loaded                        
      const auto preloaded = gLibraries.Find(path);
      if (preloaded)
         return *preloaded.GetVal();

      // Load the library                                               
      #if LANGULUS_OS(WINDOWS)
         const auto dll = LoadLibraryA(path.GetRaw());
      #else
         const auto dll = dlopen(path.GetRaw(), RTLD_NOW);
      #endif

      if (not dll) {
         Logger::Error("Failed to load module `", path, "` - "
            "file is missing or corrupted; Error code: ");
         #if LANGULUS_OS(WINDOWS)
            Logger::Append(::GetLastError());
         #else
            Logger::Append(dlerror());
         #endif
         return {};
      }

      SharedLibrary library;
      static_assert(sizeof(library.mHandle) == sizeof(dll), "Size mismatch");
      library.mHandle = reinterpret_cast<decltype(library.mHandle)>(dll);

      // Get entry, creator, info and exit points from the library      
      #if LANGULUS_OS(WINDOWS)
         library.mEntry = reinterpret_cast<Things::Module::EntryFunction>(
            GetProcAddress(dll, LANGULUS_MODULE_ENTRY_TOKEN()));
         library.mCreator = reinterpret_cast<Things::Module::CreateFunction>(
            GetProcAddress(dll, LANGULUS_MODULE_CREATE_TOKEN()));
         library.mInfo = reinterpret_cast<Things::Module::InfoFunction>(
            GetProcAddress(dll, LANGULUS_MODULE_INFO_TOKEN()));
      #else
         library.mEntry = reinterpret_cast<Things::Module::EntryFunction>(
            dlsym(dll, LANGULUS_MODULE_ENTRY_TOKEN()));
         library.mCreator = reinterpret_cast<Things::Module::CreateFunction>(
            dlsym(dll, LANGULUS_MODULE_CREATE_TOKEN()));
         library.mInfo = reinterpret_cast<Things::Module::InfoFunction>(
            dlsym(dll, LANGULUS_MODULE_INFO_TOKEN()));
      #endif

      if (not library.mEntry) {
         Logger::Error("Module `", path, "` has no valid entry point - ",
            "the function " LANGULUS_MODULE_ENTRY_TOKEN() " is missing");
         (void) UnloadSharedLibrary(library);
         return {};
      }

      if (not library.mCreator) {
         Logger::Error("Module `", path, "` has no valid instantiation point - ",
            "the function " LANGULUS_MODULE_CREATE_TOKEN() " is missing");
         (void) UnloadSharedLibrary(library);
         return {};
      }

      if (not library.mInfo) {
         Logger::Error("Module `", path, "` has no valid information point - ",
            "the function " LANGULUS_MODULE_INFO_TOKEN() " is missing");
         (void) UnloadSharedLibrary(library);
         return {};
      }

      LglsDisableWarningPush
      LglsDisableWarning_MicrosoftCast
         library.mBoundary = RTTI::GetBoundaryOf(library.mEntry);
      LglsDisableWarningPop

      LglsAssert(library.mBoundary != RTTI::GetMainBoundary(),
         "Shared library reports main boundary");

      // Link the module - this shall merge RTTI definitions.           
      // It might throw if out of memory or on meta collision, while    
      // registering new types on the other side.                       
      try {
         // Invoke the entry point, it should reflect external data     
         MetaList types;
         library.mEntry(library.mModuleType, types);
         if (types.IsEmpty()) {
            Logger::Error("A module must register at least one type"
               " - the module instantiation type");
            (void) UnloadSharedLibrary(library);
            return {};
         }

         // Make sure all registered types have the proper boundary     
         /*bool firstType = true;
         for (auto externalType : types) {
            if (firstType)
               library.mBoundary = externalType->mLibraryName;
            else if (library.mBoundary == RTTI::MainBoundary
                 or  library.mBoundary != externalType->mLibraryName) {
               Logger::Error(
                  "The external type `", externalType->mToken,
                  "` registered by module `", path,
                  "` has a mismatching boundary `", externalType->mLibraryName, '`');
               (void)UnloadSharedLibrary(library);
               return {};
            }
            firstType = false;
         }*/

         #if LANGULUS(SAFE)
         // Test if the boundary conflicts with any of the previously   
         // loaded libraries                                            
         for (auto lib : gLibraries) {
            if (lib.GetVal()->mBoundary == library.mBoundary) {
               Logger::Error(
                  "The library `", path, "` boundary `", library.mBoundary,
                  "` conflicts with already loaded library `", *lib.GetKey(),
                  "` boundary `", lib.GetVal()->mBoundary, '`'
               );
               (void)UnloadSharedLibrary(library);
               return {};
            }
         }
         #endif

         // Library is properly registered                              
         gLibraries.Merge(path, library);

         // Do some info logging                                        
         Logger::Info("Module `", library.mInfo()->mName, 
            "` exposed the following types: ", Logger::DarkGreen);
         bool first = true;
         for (auto& t : types) {
            if (not first) Logger::Append(", ");
            Logger::Append(DMeta(t));
            first = false;
         }
      }
      catch (...) {
         // Make sure we end up in an invariant state                   
         Logger::Error("Could not enter `", path, "` due to an exception");
         if (UnloadSharedLibrary(library))
            gLibraries.Erase(path);
         if (not gLibraries)
            gLibraries.Reset();
         return {};
      }

      // Great success!                                                 
      Logger::Info("Module `", library.mInfo()->mName, "` loaded (", path, ')');
      return library;
   }
   
   /// Load a shared library for a module by cross-platform name              
   ///   @param name the name for the dynamic library - the filename will     
   ///      be derived from it, by prefixing with `LangulusMod`, and          
   ///      suffixing with `.so` or `.dll` depending on platform              
   ///   @attention the name should correspond to the RTTI::Boundary it uses  
   ///   @return the shared library handle                                    
   LANGULUS(NOINLINE)
   auto Runtime::LoadSharedLibrary(Token const& name) -> SharedLibrary {
      // File prefix                                                    
      Path path;
      #if LANGULUS_OS(LINUX)
         path += "./lib";
      #endif
      path += "LangulusMod";
      path += name;

      // File postfix                                                   
      #if LANGULUS_OS(WINDOWS)
         path += ".dll";
      #else
         path += ".so";
      #endif
      return LoadSharedLibraryPath(Abandon(path));
   }

   /// Unload a DLL/SO extension module                                       
   ///   @param library the library handle to unload                          
   ///   @return true if shared library was unloaded successfully             
   LANGULUS(NOINLINE)
   bool Runtime::UnloadSharedLibrary(const SharedLibrary& library) {
      if (library.mHandle == 0)
         return true;

      Logger::Info("Unloading module `", library.mInfo()->mName, "`...");

      for (auto list : mModules) {
         for (auto mod : *list.GetVal()) {
            if ((*mod)->Is(library.mModuleType)) {
               // Delete module instance                                
               const auto modType = (*mod)->GetType();
               UnregisterAllBases(mModulesByType, *mod, modType);
               delete *mod;
               mod = (*list.GetVal()).Erase(mod);
            }
         }

         if (not *list.GetVal())
            list = mModules.Erase(list);
      }

      // Make sure memory for the maps is released                      
      if (not mModulesByType)
         mModulesByType.Reset();
      if (not mModules)
         mModules.Reset();

      // Collect garbage, and check if library's boundary is still used 
      const auto wasMarked = library.mMarkedForUnload;
      const auto boundary = library.mBoundary;

      IF_LANGULUS_MANAGED_MEMORY(Allocator::CollectGarbage());

      #if LANGULUS_FEATURE(MANAGED_REFLECTION) and LANGULUS_FEATURE(MANAGED_MEMORY)
         const auto poolsInUse = Allocator::CheckBoundary(boundary);
         if (poolsInUse) {
            // We can't allow the shared object to be unloaded!         
            // It will be attempted on next unload, so that dependent   
            // libraries have a chance of being unloaded first,         
            // releasing required resources.                            
            if (not wasMarked) {
               Logger::Warning(
                  "Module `", boundary, "` can't be unloaded yet, because "
                  "exposed data is still in use in ", poolsInUse, " memory pools. "
                  "Unload has been postponed to the next library unload.");
               const_cast<SharedLibrary&>(library).mMarkedForUnload = true;
            }

            return false;
         }
      #endif

      // If reached, then the library has no known allocations, using   
      // its reflected types - now we can safely unregister these types 
      IF_LANGULUS_MANAGED_REFLECTION(RTTI::UnloadBoundary(boundary));
      Logger::Info(
         "Module `", boundary, "` unloaded ",
         Logger::DarkYellow, (wasMarked ? "(scheduled)" : "")
      );

      // Unload the shared object                                       
      UnloadSharedLibraryInner(library.mHandle);
      return true;
   }

   /// Get the dependency module of a given type                              
   ///   @param type the type to search for                                   
   ///   @return the shared library handle, you should check if it's valid    
   auto Runtime::GetDependency(DMeta type) const noexcept -> SharedLibrary {
      if (type->mLibraryName == RTTI::MainBoundary)
         return {};

      for (auto library : gLibraries) {
         if (library.GetValue().mBoundary == type->mLibraryName)
            return library.GetValue();
      }
      return {};
   }

   /// Get a module instance by type                                          
   ///   @param type the type to search for                                   
   ///   @return the module instance                                          
   auto Runtime::GetModules(DMeta type) const noexcept -> ModList const& {
      auto found = mModulesByType.Find(type);
      if (found)
         return found.GetValue();

      static const ModList emptyFallback {};
      return emptyFallback;
   }

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   /// Get the dependency module of a given type by token                     
   ///   @param token type token                                              
   ///   @return the shared library handle, you should check if it's valid    
   auto Runtime::GetDependencyToken(Token const& token) const noexcept -> SharedLibrary {
      return GetDependency(RTTI::Registry::GetMetaDataByToken(token));
   }

   /// Get a module instance by type token                                    
   ///   @param token type token                                              
   ///   @return the module instance, or nullptr if not found                 
   auto Runtime::GetModulesToken(Token const& token) const noexcept -> ModList const& {
      return GetModules(RTTI::Registry::GetMetaDataByToken(token));
   }
#endif

   /// Update the runtime, by updating all module instantiations by order of  
   /// their priority                                                         
   ///   @param dt delta time between update calls                            
   ///   @return true if no exit was requested by any of the modules          
   bool Runtime::Update(Time dt) {
      LglsProfile();
      for (auto pair : mModules) {
         for (auto module : pair.GetValue()) {
            if (not module->Update(dt))
               return false;
         }
      }
      return true;
   }
   
   /// Get a file interface, relying on external modules to find it           
   ///   @param path the path for the file                                    
   ///   @return the file interface, or nullptr if file doesn't exist         
   auto Runtime::GetFile(Path const& path) -> Ref<Things::File> {
      auto& fileSystems = GetModules<Things::FileSystem>();
      LglsAssert(fileSystems,
         "Can't retrieve file `", path, "` - no file system module available");
      return fileSystems.template As<Things::FileSystem*>()->GetFile(path);
   }
   
   /// Get a folder interface, relying on external modules to find it         
   ///   @param path the path for the folder                                  
   ///   @return the folder interface, or nullptr if folder doesn't exist     
   auto Runtime::GetFolder(Path const& path) -> Ref<Things::Folder> {
      auto& fileSystems = GetModules<Things::FileSystem>();
      LglsAssert(fileSystems,
         "Can't retrieve folder `", path, "` - no file system module available");
      return fileSystems.template As<Things::FileSystem*>()->GetFolder(path);
   }

   /// Get the current working path (where the main exe was executed)         
   ///   @return the path                                                     
   auto Runtime::GetWorkingPath() const -> Path const& {
      auto& fileSystems = GetModules<Things::FileSystem>();
      LglsAssert(fileSystems,
         "Can't retrieve working path", " - no file system module available");
      return fileSystems.template As<Things::FileSystem*>()->GetWorkingPath();
   }

   /// Get the current data path, like GetWorkingPath() / "data"              
   ///   @return the path                                                     
   auto Runtime::GetDataPath() const -> Path const& {
      auto& fileSystems = GetModules<Things::FileSystem>();
      LglsAssert(fileSystems,
         "Can't retrieve data path", " - no file system module available");
      return fileSystems.template As<Things::FileSystem*>()->GetDataPath();
   }
}
