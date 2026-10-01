///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Runtime.hpp"
#include "Hierarchy.hpp"
//#include "Part.hpp"
#include <Langulus/Verbs/Create.hpp>
#include <Langulus/Verbs/Select.hpp>
#include <Langulus/TPin.hpp>

namespace Langulus::Things
{
   struct Part;
   using UnitList = TMany<Part*>;
   using UnitMap  = TMapUnsorted<RTTI::DMeta, TMany<Part*>>;
   using TagMap   = TMapUnsorted<RTTI::TMeta, TagList>;


   ///                                                                        
   ///   Thing                                                                
   ///                                                                        
   /// The primary composable type. Its functionality comes from its units    
   /// and children/owner's units. The Thing is an aggregate of traits,       
   /// units, and subthings.                                                  
   ///                                                                        
   class Thing final
      : public Resolvable
      , public Referenced
      , public SeekInterface<Thing>
   {
      LANGULUS(NAME) "Thing";
      LANGULUS(ABSTRACT) false;
      LANGULUS(PRODUCER) Thing;
      LANGULUS(POOL_TACTIC) RTTI::PoolTactic::Type;
      LANGULUS_BASES(Resolvable);
      LANGULUS_VERBS(Verbs::Create, Verbs::Select);

   protected:
      LANGULUS_API(THINGS) void ResetRuntime(Runtime*);
      LANGULUS_API(THINGS) void ResetFlow(Temporal*);
      LANGULUS_API(THINGS) void Teardown();

      // The order of members is critical!                              
      // Runtime should be destroyed last, hence it is the first member 
      Pin<Ref<Runtime>> mRuntime;
      // Temporal flow                                                  
      Pin<Ref<Temporal>> mFlow;
      // Hierarchy                                                      
      Hierarchy mChildren;
      // Units indexed by concrete type, in order of addition           
      UnitList mUnitsList;
      // Units indexed by all their relevant reflected bases            
      UnitMap mUnitsAmbiguous;
      // Traits                                                         
      TagMap mTags;
      // Hierarchy requires an update                                   
      bool mRefreshRequired {};
      // The entity's parent                                            
      Ref<Thing> mOwner;

      template<Seek = Seek::HereAndAbove>
      Many CreateData(const Construct&);

      template<class T>
      void CreateInner(Verb&, const T&);

   public:
      LANGULUS_API(THINGS) Thing();
      LANGULUS_API(THINGS) Thing(Describe&&);
      LANGULUS_API(THINGS) Thing(Thing*, Many const& = {});
      LANGULUS_API(THINGS) Thing(Thing&&) noexcept;
      LANGULUS_API(THINGS) Thing(Cloned<Thing>&&);
      LANGULUS_API(THINGS) Thing(Abandoned<Thing>&&);
      LANGULUS_API(THINGS)~Thing();

      template<bool CREATE_FLOW = true>
      static Thing Root(CT::String auto&&...);

      // Shallow copy is disabled, you should be able only to clone,    
      // move, or abandon                                               
      Thing(const Thing&) = delete;
      auto operator = (auto) = delete;

      LANGULUS_API(THINGS)
      bool RequiresRefresh() const noexcept;

      LANGULUS_API(THINGS)
      auto GetRuntime() const noexcept -> const Pin<Ref<Runtime>>&;

      LANGULUS_API(THINGS)
      auto GetFlow() const noexcept -> const Pin<Ref<Temporal>>&;

      LANGULUS_API(THINGS) void Do(Verb&);
      LANGULUS_API(THINGS) void Select(Verb&);
      LANGULUS_API(THINGS) void Create(Verb&);

      template<Seek = Seek::HereAndAbove, CT::VerbBased V>
      V& RunIn(V&);
      template<CT::VerbBased V>
      V& Run(V&);

      LANGULUS_API(THINGS) Many Say(const Text&);
      LANGULUS_API(THINGS) Many Run(const Code&);

      LANGULUS_API(THINGS) bool Update(Time);
      LANGULUS_API(THINGS) void Refresh(bool force = false);
      LANGULUS_API(THINGS) void Reset();

      LANGULUS_API(THINGS)
      bool operator == (const Thing&) const;

      LANGULUS_API(THINGS)
      explicit operator Text() const;

   public:
      ///                                                                     
      ///   Hierarchy management                                              
      ///                                                                     
      LANGULUS_API(THINGS)
      auto CreateRuntime() -> Runtime*;

      LANGULUS_API(THINGS)
      auto CreateFlow() -> Temporal*;

      template<class...T>
      auto CreateChild(T&&...) -> Ref<Thing>;

      template<bool TWOSIDED = true>
      size_t AddChild(Thing*);
      template<bool TWOSIDED = true>
      size_t RemoveChild(Thing*);

      LANGULUS_API(THINGS)
      auto LoadMod(Token const&, Many const& = {}) -> A::Module*;
      LANGULUS_API(THINGS)
      auto LoadModPath(const Path&, Many const& = {}) -> A::Module*;

      LANGULUS_API(THINGS)
      auto GetOwner() const noexcept -> const Ref<Thing>&;

      LANGULUS_API(THINGS)
      auto GetChildren() const noexcept -> const Hierarchy&;

      LANGULUS_API(THINGS)
      auto GetChild(Index = 0) -> Thing*;

      LANGULUS_API(THINGS)
      auto GetChild(Index = 0) const -> const Thing*;

      LANGULUS_API(THINGS)
      auto GetNamedChild(Token const&, Index = 0) -> Thing*;

      LANGULUS_API(THINGS)
      auto GetNamedChild(Token const&, Index = 0) const -> const Thing*;

      LANGULUS_API(THINGS)
      void DumpHierarchy() const;

   public:
      ///                                                                     
      ///   Part management                                                   
      ///                                                                     
      template<bool TWOSIDED = true>
      size_t AddUnit(Part*);
      template<bool TWOSIDED = true>
      size_t RemoveUnit(Part*);

      template<CT::Part, class...A>
      Many CreateUnit(A&&...);
      template<CT::Part...>
      Many CreateUnits();

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         template<class...A>
         Many CreateUnitToken(Token const&, A&&...);
      #endif

      template<CT::Part = Part, bool TWOSIDED = true>
      size_t RemoveUnits();

      LANGULUS_API(THINGS)
      auto HasUnits(DMeta) const -> size_t;
      template<CT::Part>
      auto HasUnits() const -> size_t;

      LANGULUS_API(THINGS)
      auto GetUnits() const noexcept -> const UnitList&;
      LANGULUS_API(THINGS)
      auto GetUnitsMap() const noexcept -> const UnitMap&;

      LANGULUS_API(THINGS)
      auto GetUnitMeta(DMeta, Index = 0)       -> Part*;
      LANGULUS_API(THINGS)
      auto GetUnitMeta(DMeta, Index = 0) const -> Part const*;

      LANGULUS_API(THINGS)
      auto GetUnitExt(DMeta, Many const&, Index = 0)       -> Part*;
      LANGULUS_API(THINGS)
      auto GetUnitExt(DMeta, Many const&, Index = 0) const -> Part const*;

      template<CT::Part T = Part>
      auto GetUnit(Index = 0)       -> Decay<T>*;
      template<CT::Part T = Part>
      auto GetUnit(Index = 0) const -> Decay<T> const*;

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         LANGULUS_API(THINGS)
         auto GetUnitMeta(Token const&, Index = 0) const -> Part const*;
         LANGULUS_API(THINGS)
         auto GetUnitMeta(Token const&, Index = 0)       -> Part*;

         template<CT::Part T>
         auto GetUnitAs(Token const&, Index = 0) -> Decay<T>*;
      #endif

   private:
      LANGULUS_API(THINGS) void AddUnitBases(Part*, DMeta);
      LANGULUS_API(THINGS) void RemoveUnitBases(Part*, DMeta);

   public:
      ///                                                                     
      ///   Tag management                                                  
      ///                                                                     
      LANGULUS_API(THINGS) auto AddTrait(Tag) -> Tag*;

      LANGULUS_API(THINGS) size_t RemoveTrait(TMeta);
      LANGULUS_API(THINGS) size_t RemoveTrait(Tag);

      LANGULUS_API(THINGS)
      size_t HasTraits(TMeta) const;
      LANGULUS_API(THINGS)
      size_t HasTraits(const Tag&) const;

      LANGULUS_API(THINGS)
      auto GetTraits() const noexcept -> const TraitMap&;
      LANGULUS_API(THINGS)
      auto GetTrait(TMeta, Index = 0) const -> Tag;
      LANGULUS_API(THINGS)
      auto GetTrait(TMeta, Index = 0)       -> Tag;
      LANGULUS_API(THINGS)
      auto GetTrait(const Tag&, Index = 0) const -> Tag;
      LANGULUS_API(THINGS)
      auto GetTrait(const Tag&, Index = 0)       -> Tag;
      template<CT::TraitBased = Tag>
      auto GetTrait(Index = 0) -> Tag;

      LANGULUS_API(THINGS)
      auto GetLocalTrait(TMeta, Index = 0) const -> Tag const*;
      LANGULUS_API(THINGS)
      auto GetLocalTrait(TMeta, Index = 0)       -> Tag*;
      template<CT::TraitBased = Tag>
      auto GetLocalTrait(Index = 0)       -> Tag*;
      template<CT::TraitBased = Tag>
      auto GetLocalTrait(Index = 0) const -> Tag const*;

      LANGULUS_API(THINGS)
      void SetName(const Text&);

      LANGULUS_API(THINGS)
      Text GetName() const;

      ///                                                                     
      ///   Seek                                                              
      ///                                                                     
      using SeekInterface::SeekPart;
      using SeekInterface::SeekPartAux;
      using SeekInterface::SeekPartExt;
      using SeekInterface::SeekPartAuxExt;
      using SeekInterface::SeekTag;
      using SeekInterface::SeekTagAux;
      using SeekInterface::SeekValue;
      using SeekInterface::SeekValueAux;

      template<Seek = Seek::HereAndAbove>
      auto SeekPart(DMeta, Index = 0) -> Part*;
      template<Seek = Seek::HereAndAbove>
      auto SeekPartAux(Many const&, DMeta, Index = 0) -> Part*;
      template<Seek = Seek::HereAndAbove>
      auto SeekPartExt(DMeta, Many const&, Index = 0) -> Part*;
      template<Seek = Seek::HereAndAbove>
      auto SeekPartAuxExt(DMeta, Many const&, Many const&, Index = 0) -> Part*;

      template<Seek = Seek::HereAndAbove>
      auto SeekTag(TMeta, Index = 0) -> Tag;
      template<Seek = Seek::HereAndAbove>
      auto SeekTagAux(Many const&, TMeta, Index = 0) -> Tag;

      template<Seek = Seek::HereAndAbove>
      bool SeekValue(TMeta, CT::NotVoid auto&, Index = 0) const;
      template<Seek = Seek::HereAndAbove>
      bool SeekValueAux(TMeta, Many const&, CT::NotVoid auto&, Index = 0) const;

      ///                                                                     
      ///   Gather                                                            
      ///                                                                     
      using SeekInterface::GatherParts;
      using SeekInterface::GatherPartsExt;
      using SeekInterface::GatherTags;

      template<Seek = Seek::HereAndAbove>
      auto GatherParts(DMeta) -> TMany<Part*>;
      template<Seek = Seek::HereAndAbove>
      auto GatherPartsExt(DMeta, Many const&) -> TMany<Part*>;

      template<Seek = Seek::HereAndAbove>
      auto GatherTags(TMeta) -> TagList;

      template<CT::NotVoid D, Seek = Seek::HereAndAbove>
      auto GatherValues() const -> TMany<D>;
   };
}