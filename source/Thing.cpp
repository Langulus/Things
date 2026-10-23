///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/Thing.hpp>
#include <Langulus/Temporal.hpp>
#include <Langulus/Describe.hpp>
#include <Langulus/Profiler.hpp>
#include "Thing.inl"


namespace Langulus::Things
{
   /// Default-constructor, always creates a parentless root thing            
   Thing::Thing() /*: Downcastable {this}*/ {
      THINGS_VERBOSE_SELF("Created (root, ", GetReferences(), " references)");
   }
   
   /// Descriptor-constructor                                                 
   ///   @param describe instructions for creating the thing                  
   Thing::Thing(Describe&& describe) /*: Downcastable {this}*/ {
      if (describe) {
         THINGS_VERBOSE_SELF_TAB("Creating from descriptor: ", describe.what);
         Many unused_sideeffects;
         CreateInner(unused_sideeffects, describe.what);
      }

      if (mOwner) {
         THINGS_VERBOSE_SELF(
            "Created as child to ", mOwner,
            " (", GetReferences(), " references; parent now has ",
            mOwner->GetReferences(), " references)"
         );
      }
      else {
         THINGS_VERBOSE_SELF(
            "Created (root, ", GetReferences(), " references)"
         );
      }
   }
   
   /// Construct as a child of another thing                                  
   ///   @param parent the thing that owns this thing                         
   ///   @param descriptor instructions for creating the thing                
   Thing::Thing(Thing* parent, Many const& descriptor)
      : /*Downcastable {this}
      ,*/ mOwner       {parent}
   {
      THINGS_VERBOSE_SELF_TAB("Created manually");

      if (parent) {
         parent->AddChild<false>(this);
         mRuntime = parent->GetRuntime();
         mFlow = parent->GetFlow();
      }

      if (descriptor) {
         // Create any subthings/tags/part in this thing                
         Many unused_sideeffects;
         CreateInner(unused_sideeffects, descriptor);
      }

      if (mOwner) {
         THINGS_VERBOSE_SELF(
            "Created as child to ", mOwner,
            " (", GetReferences(), " references; parent now has ",
            mOwner->GetReferences(), " references)"
         );
      }
      else {
         THINGS_VERBOSE_SELF(
            "Created (root, ", GetReferences(), " references)"
         );
      }
   }

   /// Move constructor                                                       
   ///   @attention owner is never moved, you're moving only the hierarchy    
   ///      below the parent, however other's parent is notified of the move, 
   ///      because 'other' is removed from its children                      
   ///   @param other thing to move                                           
   Thing::Thing(Thing&& other) noexcept
      : /*Downcastable    {this}
      ,*/ mRuntime        {Move(other.mRuntime)}
      , mFlow           {Move(other.mFlow)}
      , mChildren       {Move(other.mChildren)}
      , mPartsList      {Move(other.mPartsList)}
      , mPartsAmbiguous {Move(other.mPartsAmbiguous)}
      , mTags           {Move(other.mTags)}
      , mRefreshRequired{true}
   {
      // Remap children                                                 
      for (auto& child : mChildren)
         child->mOwner = this;

      // Remap parts                                                    
      for (auto& part : mPartsList)
         part->ReplaceOwner(&other, this);

      // Make sure the losing parent is notified of the change          
      if (other.mOwner)
         other.mOwner->RemoveChild(&other);

      THINGS_VERBOSE_SELF("Moved from ", other);
   }
   
   /// Abandon constructor                                                    
   ///   @param other thing to abandon                                        
   Thing::Thing(Abandon<Thing>&& other)
      : /*Downcastable    {this}
      ,*/ mRuntime        {Abandon(other->mRuntime)}
      , mFlow           {Abandon(other->mFlow)}
      , mChildren       {Abandon(other->mChildren)}
      , mPartsList      {Abandon(other->mPartsList)}
      , mPartsAmbiguous {Abandon(other->mPartsAmbiguous)}
      , mTags         {Abandon(other->mTags)}
      , mRefreshRequired{true}
   {
      // Remap children                                                 
      for (auto& child : mChildren)
         child->mOwner = this;

      // Remap parts                                                    
      for (auto& part : mPartsList)
         part->ReplaceOwner(&other.what, this);

      // Make sure the losing parent is notified of the change          
      if (other->mOwner)
         other->mOwner->RemoveChild(&other.what);

      THINGS_VERBOSE_SELF("Abandoned from ", *other);
   }

   /// Clone constructor                                                      
   ///   @param other thing to clone                                          
   Thing::Thing(Clone<Thing>&& other)
      : /*Downcastable    {this}
      ,*/ mChildren       {Clone(other->mChildren)}
      , mRefreshRequired{true}
   {
      TODO();
      //TODO clone flow and runtime if pinned, recreate modules if new runtime, 
      // recreate parts and tags, then recreate children
      THINGS_VERBOSE_SELF("Cloned from ", *other);
   }

   /// Destroy the thing                                                      
   Thing::~Thing() {
      // First stage destruction that severs all connections            
      // If Thing was on the stack, this will be the first time it's    
      // called. If it was on the managed heap, it would've been called 
      // from the Reference routine                                     
      Teardown();

      LglsAssumeDev(GetReferences() <= 1,
         "Can't teardown while this Thing is still used", " in ",
         GetReferences(), " places"
      );

      // Destroy the entire herarchy under this Thing                   
      THINGS_VERBOSE_SELF_TAB("Destroying...");

      #if THINGS_VERBOSE_ENABLED()
         for (auto& child : mChildren) {
            if (child->GetReferences() != 1) {
               Logger::Warning(
                  child, " can't be destroyed yet - has ",
                  child->GetReferences(), " uses instead of 1"
               );
            }
         }

         for (auto& part : mPartsList) {
            if (part->GetReferences() > 3) {
               Logger::Warning(
                  part, " can't be destroyed yet - has ",
                  part->GetReferences(), " uses instead of 3 (or less)" 
               );
            }
         }
      #endif
   }

   /// This is used for first-stage destruction, severing ties to avoid       
   /// circular dependencies. After this runs, there should be only one       
   /// reference remaining for this Thing                                     
   void Thing::Teardown() {
      THINGS_VERBOSE_SELF_TAB(
         "Teardown initiated at ", GetReferences(), " uses...");

      // Reset owner, so that only one reference to this Thing remains  
      // in the hierarchy: the owner's mChildren                        
      mOwner.Reset();

      // Tags might be exposing members in parts. Make sure we          
      // dereference those first, so that parts have as small number of 
      // references as possible                                         
      THINGS_VERBOSE_SELF("Tearing off tags (name might change)");
      mTags.Reset();

      // Decouple all parts from this owner because parts might get     
      // destroyed upon destroying mPartsList and mPartsAmbiguous, if   
      // the Parts were created on the stack.                           
      // If they still have owners, they will attempt to Decouple in    
      // Part::~Part from already destroyed mPartsList/Ambiguous        
      mPartsAmbiguous.Reset();
      for (auto& part : mPartsList) {
         THINGS_VERBOSE_SELF(
            "Tearing off part ", part, " at ", part->GetReferences(), " uses...");
         part->mOwners.Erase(this);

         if (part->mOwners.IsEmpty())
            part->mOwners.Reset();
      }

      // Propagate Teardown through the hierarchy of Things             
      for (auto& child : mChildren)
         child->Teardown();

      if (not mFlow.IsPinned())
         (*mFlow).Reset();

      if (not mRuntime.IsPinned())
         (*mRuntime).Reset();

      THINGS_VERBOSE_SELF("Teardown complete: ", GetReferences(), " uses remain");
   }

   /// Compare two entities                                                   
   ///   @param other thing to compare with                                   
   ///   @return true if entities are functionally same, that is, they have   
   ///      the exact same behavior (actual state is ignored)                 
   bool Thing::operator == (const Thing& other) const {
      return mChildren == other.mChildren
         and mPartsList == other.mPartsList
         and mTags == other.mTags;
   }

   /// Dump the entity's hierarchy in log                                     
   void Thing::DumpHierarchy() const {
      const auto tab = Logger::VerboseScoped(
         Logger::White, Logger::Underline, *this
      );

      if (mTags) {
         const auto tab2 = Logger::Section(Logger::White, Logger::Underline,
            "Tags (", mTags.GetCount(), "):");
         for (auto tagpair : mTags) {
            for (auto& tag : tagpair.GetVal())
               Logger::Verbose(tag);
         }
      }

      if (mPartsList) {
         const auto tab2 = Logger::Section(Logger::White, Logger::Underline,
            "Parts (", mPartsList.GetCount(), "):");
         for (auto& part : mPartsList)
            Logger::Verbose(*part);
      }

      if (mChildren) {
         const auto tab2 = Logger::Section(Logger::White, Logger::Underline,
            "Children (", mChildren.GetCount(), "):");
         for (auto& child : mChildren)
            child->DumpHierarchy();
      }
   }

   /// Simulate the hierarchy of things for a given period of time, by        
   /// updating all runtimes and flows at and under this Thing                
   /// Also synchronizes changes between all parts, if mRefreshRequired       
   ///   @param deltaTime how much time passes for the simulation             
   ///   @return true if no exit was requested by any of the runtimes/flows   
   bool Thing::Update(Time deltaTime) {
      LglsProfile();

      // Refresh the hierarchy on any changes before updating anything  
      Refresh();

      if (mFlow.IsPinned()) {
         // This thing owns its flow, so we need to update it here      
         // This will execute any temporally based verbs and scripts    
         // Game logic basically happens in this flow                   
         Many unsusedSideeffects;
         if (not (*mFlow)->Update(deltaTime, unsusedSideeffects))
            return false;
      }

      if (mRuntime.IsPinned()) {
         // This thing owns its runtime, so we need to update it here   
         // This is where modules are updated in parallel, physical     
         // simulations happen, images get rendered, etc.               
         if (not mRuntime->Update(deltaTime))
            return false;
      }

      // Cascade the update down the hierarchy                          
      for (auto& child : mChildren) {
         if (not child->Update(deltaTime))
            return false;
      }

      return true;
   }

   /// Refresh all parts and children down the hierarchy                      
   ///   @param force true to force refresh, otherwise refresh will happen    
   ///      only if Thing's configuration changed.                            
   void Thing::Refresh(bool force) {
      LglsProfile();
      if (not force and not mRefreshRequired)
         return;

      mRefreshRequired = false;

      // Refresh all parts                                              
      for (auto& part : mPartsList)
         part->Refresh();

      // And cascade down the hierarchy                                 
      for (auto& child : mChildren)
         child->Refresh(true);
   }

   /// Reset the entity, clearing all children, parts, tags                   
   void Thing::Reset() {
      // Decouple all children from this parent                         
      for (auto& child : mChildren)
         child->mOwner.Reset();

      // Decouple all parts from this owner                             
      for (auto& part : mPartsList)
         part->mOwners.Erase(this);

      mChildren.Reset();
      mPartsList.Reset();
      mPartsAmbiguous.Reset();
      mTags.Reset();
   }

   /// Get a part by type and offset                                          
   /// If type is nullptr searches only by offset                             
   /// If type is not nullptr, gets the Nth matching part, if any             
   ///   @param id the type of the part                                       
   ///   @param index the part index to seek                                  
   ///   @return the part if found, or nullptr if not                         
   auto Thing::GetPartMeta(DMeta id, size_t index) -> Part* {
      if (id) {
         // Search a typed tag                                          
         const auto found = mPartsAmbiguous.Find(id);
         return found
            ? static_cast<Part*>((*found.GetVal())[index])
            : static_cast<Part*>(nullptr);
      }

      // Get part by index only                                         
      return mPartsList[index];
   }

   auto Thing::GetPartMeta(DMeta type, size_t offset) const -> Part const* {
      return const_cast<Thing*>(this)->GetPartMeta(type, offset);
   }
   
   /// Get a part by type, properties, and offset.                            
   /// If target type is nullptr, searches only by offset and properties.     
   /// Otherwise gets the Nth matching part, if any.                          
   ///   @param meta the type of the part                                     
   ///   @param what the desired properties of the part                       
   ///   @param index the Nth match to return                                 
   ///   @return the part if found, nullptr otherwise                         
   auto Thing::GetPartExt(DMeta meta, Many const& what, size_t index) -> Part* {
      if (meta) {
         // Search a typed part                                         
         const auto found = mPartsAmbiguous.Find(meta);
         if (found) {
            auto& bucket = *found.GetVal();
            if (not what)
               return bucket[index];

            // Check all parts in that bucket for required properties   
            for (auto& part : bucket) {
               if (part->CompareDescriptor(what)) {
                  // Match found                                        
                  if (index == 0) return *part;
                  else --index;
               }
            }
         }
      }
      else {
         // Search part by index and properties only                    
         for (auto& part : mPartsList) {
            if (part->CompareDescriptor(what)) {
               // Match found                                           
               if (index == 0) return *part;
               else --index;
            }
         }
      }

      return nullptr;
   }

   auto Thing::GetPartExt(DMeta meta, Many const& what, size_t index) const -> Part const* {
      return const_cast<Thing*>(this)->GetPartExt(meta, what, index);
   }

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   /// Get a part by token and offset                                         
   ///   @param token the type name of the part                               
   ///   @param offset the part index                                         
   ///   @return the part if found, or nullptr if not                         
   auto Thing::GetPartMeta(Token const& token, size_t offset) -> Part* {
      return GetPartMeta(RTTI::Registry::DisambiguateMeta(token), offset);
   }
#endif

   /// Get the owner                                                          
   ///   @return the owner                                                    
   auto Thing::GetOwner() const noexcept -> Ref<Thing> const& {
      return mOwner;
   }

   /// Get children hierarchy                                                 
   ///   @return the hierarchy                                                
   auto Thing::GetChildren() const noexcept -> Hierarchy const& {
      return mChildren;
   }

   /// Get a child by index                                                   
   ///   @param id the index to pick                                          
   ///   @return the child entity, or nullptr of none was found               
   auto Thing::GetChild(size_t offset) -> Thing* {
      return mChildren[offset];
   }

   auto Thing::GetChild(size_t offset) const -> Thing const* {
      return const_cast<Thing*>(this)->GetChild(offset);
   }

   /// Get child by name and offset (searches for Tags::Name in children)     
   ///   @param name name to seek                                             
   ///   @param offset offset to seek                                         
   ///   @return the child entity, or nullptr of none was found               
   auto Thing::GetNamedChild(Token const& name, size_t offset) -> Thing* {
      size_t matches = 0;
      for (auto& child : mChildren) {
         if (child->GetName() == name) {
            if (matches == offset)
               return *child;
            ++matches;
         }
      }

      return nullptr;
   }

   auto Thing::GetNamedChild(Token const& name, size_t offset) const -> Thing const* {
      return const_cast<Thing*>(this)->GetNamedChild(name, offset);
   }

   /// Do a cascading runtime reset - all children that do not have own       
   /// runtime, will incorporate the provided one                             
   ///   @param newrt the new runtime to set                                  
   void Thing::ResetRuntime(Runtime* newrt) {
      if (mRuntime.IsPinned())
         return;

      mRuntime = newrt;
      for (auto& child : mChildren)
         child->ResetRuntime(newrt);
   }

   /// Do a cascading temporal flow reset - all children that do not have own 
   /// flow, will incorporate the provided one                                
   ///   @param newflow the new flow to set                                   
   void Thing::ResetFlow(Temporal* newflow) {
      if (mFlow.IsPinned())
         return;

      mFlow = newflow;
      for (auto& child : mChildren)
         child->ResetFlow(newflow);
   }

   /// Count the number of matching parts in this entity                      
   ///   @param type the type of parts to search for                          
   ///   @return the number of matching parts                                 
   auto Thing::HasParts(DMeta type) const -> size_t {
      const auto found = mPartsAmbiguous.Find(type);
      return found ? found.GetVal().GetCount() : 0;
   }

   /// Check if all parts in the hierarchy require a Refresh() call           
   ///   @return true if the thing is dirty                                   
   bool Thing::RequiresRefresh() const noexcept {
      return mRefreshRequired;
   }

   /// Get the current runtime                                                
   ///   @return the pointer to the runtime                                   
   /*auto Thing::GetRuntime() const noexcept -> Pin<Ref<Runtime>> const& {
      return mRuntime;
   }

   /// Get the current temporal flow                                          
   ///   @return the pointer to the flow                                      
   auto Thing::GetFlow() const noexcept -> Pin<Ref<Temporal>> const& {
      return mFlow;
   }*/

   /// Create a local runtime for this thing                                  
   ///   @return the new runtime instance, or the old one if already created  
   auto Thing::CreateRuntime() -> Runtime* {
      if (mRuntime.IsPinned())
         return **mRuntime;

      mRuntime.Emplace(this);
      mRuntime.Pin();

      // Dispatch the change to all children                            
      for (auto& child : mChildren)
         child->ResetRuntime(**mRuntime);

      THINGS_VERBOSE_SELF("New runtime: ", &*mRuntime);
      return **mRuntime;
   }

   /// Create a local flow for this thing                                     
   ///   @return the new flow instance, or the old one, if already created    
   auto Thing::CreateFlow() -> Temporal* {
      if (mFlow.IsPinned())
         return **mFlow;

      mFlow.Emplace();
      mFlow.Pin();

      // Dispatch the change to all children                            
      for (auto& child : mChildren)
         child->ResetFlow(**mFlow);

      THINGS_VERBOSE_SELF("New flow: ", *mFlow);
      return **mFlow;
   }

   /// Uses the current runtime to load a shared library module, and          
   /// instantiate it for use, if not yet instantiated                        
   ///   @attention assumes a runtime is available in the hierarchy           
   ///   @param module name of the module                                     
   ///   @param descriptor instructions for module setup                      
   ///   @return the instantiated module interface                            
   auto Thing::LoadMod(Token const& module, Many const& descriptor) -> Things::Module* {
      LglsAssumeUser(mRuntime, "No runtime available for loading a module");
      const auto instance = mRuntime->InstantiateModule(module, descriptor);
      LglsAssert(instance, "Missing module");
      return instance;
   }
   
   /// Uses the current runtime to load a shared library module, and          
   /// instantiate it for use, if not yet instantiated                        
   ///   @attention assumes a runtime is available in the hierarchy           
   ///   @param path relative path to the module                              
   ///   @param descriptor instructions for module setup                      
   ///   @return the instantiated module interface                            
   auto Thing::LoadModPath(Path const& path, Many const& descriptor) -> Things::Module* {
      LglsAssumeUser(mRuntime, "No runtime available for loading a module");
      const auto instance = mRuntime->InstantiateModulePath(path, descriptor);
      LglsAssert(instance, "Missing module");
      return instance;
   }
}
