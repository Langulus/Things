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
#include <Langulus/TPin.hpp>
#include <Langulus/TRef.hpp>
#include <Langulus/Recipe.hpp>
#include <Langulus/Code.hpp>
#include <Langulus/Temporal.hpp>

#include <Langulus/Verbs/Do.hpp>
#include <Langulus/Verbs/Create.hpp>
#include <Langulus/Verbs/Select.hpp>


namespace Langulus::Things
{
   using PartList = TMany<Part*>;
   using PartMap  = TMapUnsorted<RTTI::DMeta, TMany<Part*>>;
   using TagMap   = TMapUnsorted<RTTI::TMeta, TagList>;


   ///                                                                        
   ///   Thing                                                                
   ///                                                                        
   /// The primary composable type. Its functionality comes from its parts    
   /// and children/owner's parts. The Thing is an aggregate of tags,         
   /// parts, and subthings.                                                  
   ///                                                                        
   struct Thing final : Resolvable, Referenced, SeekInterface {
      using CTTI_Named     = Yes<"Thing">;
      using CTTI_Abstract  = No;
      using CTTI_Producer  = Thing;
      using CTTI_Pooled    = PooledByType<>;
      using CTTI_Ability   = Types<Verbs::Do, Verbs::Create, Verbs::Select>;
      //using CTTI_Bases     = Resolvable; //auto-detected?

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
      // Parts indexed by concrete type, in order of addition           
      PartList mPartsList;
      // Parts indexed by all their relevant reflected bases            
      PartMap mPartsAmbiguous;
      // Tags                                                           
      TagMap mTags;
      // Hierarchy requires an update                                   
      bool mRefreshRequired = false;
      // The entity's parent                                            
      Ref<Thing> mOwner;

      template<Seek = Seek::HereAndAbove>
      Many CreateData(Recipe const&);

      template<class T>
      void CreateInner(Many&, T const&);

   public:
      using Code = Flow::Code;

      LANGULUS_API(THINGS) Thing();
      LANGULUS_API(THINGS) Thing(Describe&&);
      LANGULUS_API(THINGS) Thing(Thing*, Many const& = {});
                           Thing(Thing const&) = delete;
      LANGULUS_API(THINGS) Thing(Thing&&) noexcept;
      LANGULUS_API(THINGS) Thing(Move<Thing>&&);
      LANGULUS_API(THINGS) Thing(Clone<Thing>&&);
      LANGULUS_API(THINGS) Thing(Abandon<Thing>&&);
      LANGULUS_API(THINGS)~Thing();

      // Assignment is disabled                                         
      auto operator = (auto) = delete;

      template<bool CREATE_FLOW = true>
      static Thing Root(CT::Text auto&&...);

      LANGULUS_API(THINGS)
      bool RequiresRefresh() const noexcept;

      LANGULUS_API(THINGS)
      auto GetRuntime() const noexcept -> Pin<Ref<Runtime>> const&;

      LANGULUS_API(THINGS)
      auto GetFlow() const noexcept -> Pin<Ref<Temporal>> const&;

      LANGULUS_API(THINGS) void Do(Verb&);
      LANGULUS_API(THINGS) void Select(Verb&);
      LANGULUS_API(THINGS) void Create(Verb&);

      template<Seek = Seek::HereAndAbove>
      void DoIn(Verb&);

      /*template<CT::Executable V>
      V& Run(V&);*/

      LANGULUS_API(THINGS) Many Say(Text const&);
      LANGULUS_API(THINGS) Many Run(Code const&);

      LANGULUS_API(THINGS) bool Update(Time);
      LANGULUS_API(THINGS) void Refresh(bool force = false);
      LANGULUS_API(THINGS) void Reset();

      LANGULUS_API(THINGS)
      bool operator == (const Thing&) const;

      //LANGULUS_API(THINGS)
      //explicit operator Text() const;


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
      auto LoadMod(Token const&, Many const& = {}) -> Module*;
      LANGULUS_API(THINGS)
      auto LoadModPath(Path const&, Many const& = {}) -> Module*;

      LANGULUS_API(THINGS)
      auto GetOwner() const noexcept -> const Ref<Thing>&;

      LANGULUS_API(THINGS)
      auto GetChildren() const noexcept -> const Hierarchy&;

      LANGULUS_API(THINGS)
      auto GetChild(size_t = 0) -> Thing*;
      LANGULUS_API(THINGS)
      auto GetChild(size_t = 0) const -> const Thing*;

      LANGULUS_API(THINGS)
      auto GetNamedChild(Token const&, size_t = 0) -> Thing*;
      LANGULUS_API(THINGS)
      auto GetNamedChild(Token const&, size_t = 0) const -> const Thing*;

      LANGULUS_API(THINGS)
      void DumpHierarchy() const;


      ///                                                                     
      ///   Part management                                                   
      ///                                                                     
      template<bool TWOSIDED = true>
      size_t AddPart(Part*);
      template<bool TWOSIDED = true>
      size_t RemovePart(Part*);

      template<CT::Part, class...A>
      Many CreatePart(A&&...);
      template<CT::Part...>
      Many CreateParts();

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         template<class...A>
         Many CreatePartToken(Token const&, A&&...);
      #endif

      template<CT::Part = Part, bool TWOSIDED = true>
      size_t RemoveParts();

      LANGULUS_API(THINGS)
      auto HasParts(DMeta) const -> size_t;
      template<CT::Part>
      auto HasParts() const -> size_t;

      LANGULUS_API(THINGS)
      auto GetParts() const noexcept -> PartList const&;
      LANGULUS_API(THINGS)
      auto GetPartsMap() const noexcept -> PartMap const&;

      LANGULUS_API(THINGS)
      auto GetPartMeta(DMeta, size_t = 0)       -> Part*;
      LANGULUS_API(THINGS)
      auto GetPartMeta(DMeta, size_t = 0) const -> Part const*;

      LANGULUS_API(THINGS)
      auto GetPartExt(DMeta, Many const&, size_t = 0)       -> Part*;
      LANGULUS_API(THINGS)
      auto GetPartExt(DMeta, Many const&, size_t = 0) const -> Part const*;

      template<CT::Part T = Part, class SELF> requires CT::Decayed<T>
      auto GetPart(this SELF&&, CT::Index auto&& = 0) -> LglsMutIf(SELF, T*);

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         LANGULUS_API(THINGS)
         auto GetPartMeta(Token const&, size_t = 0) const -> Part const*;
         LANGULUS_API(THINGS)
         auto GetPartMeta(Token const&, size_t = 0)       -> Part*;

         template<CT::Part T, class SELF> requires CT::Decayed<T>
         auto GetPartAs(this SELF&&, Token const&, CT::Index auto&& = 0) -> LglsMutIf(SELF, T*);
      #endif

   private:
      LANGULUS_API(THINGS) void AddPartBases(Part*, DMeta);
      LANGULUS_API(THINGS) void RemovePartBases(Part*, DMeta);

   public:
      ///                                                                     
      ///   Tag management                                                    
      ///                                                                     
      LANGULUS_API(THINGS) auto AddTag(Tag) -> Tag*;

      LANGULUS_API(THINGS) size_t RemoveTag(TMeta);
      LANGULUS_API(THINGS) size_t RemoveTag(Tag);

      LANGULUS_API(THINGS)
      size_t HasTags(TMeta) const;
      LANGULUS_API(THINGS)
      size_t HasTags(Tag const&) const;

      /// Get all tags associated with this thing                             
      auto GetTags() const noexcept -> TagMap const& { //TODO should we allow mutation from outside? what's the worse that can happen? :P
         return mTags;
      }

      /// Get a tag by type                                                   
      ///   @param tag tag id                                                 
      ///   @param offset Nth result to use                                   
      ///   @return a filled tag if found, empty if not                       
      auto GetTag(this auto&& self, TMeta meta, size_t offset = 0) -> Tag {
         return self.GetTag(Tag::From(meta), offset);
      }

      LANGULUS_API(THINGS)
      auto GetTag(Tag const&, size_t = 0) const -> Tag;
      LANGULUS_API(THINGS)
      auto GetTag(Tag const&, size_t = 0)       -> Tag;

      template<CT::DefineTag>
      auto GetTag(this auto&&, CT::Index auto&& = 0) -> Tag;

      LANGULUS_API(THINGS)
      auto GetLocalTag(TMeta, size_t = 0) const -> Tag const*;
      LANGULUS_API(THINGS)
      auto GetLocalTag(TMeta, size_t = 0)       -> Tag*;
      
      template<CT::DefineTag, class SELF>
      auto GetLocalTag(this SELF&&, CT::Index auto&& = 0) -> LglsMutIf(SELF, Tag*);

      LANGULUS_API(THINGS)
      void SetName(Text const&);

      LANGULUS_API(THINGS)
      Text GetName() const;
   };
}

/// Convert to text, by writing a short name or address                       
LANGULUS_MORPHISM_CUSTOM(Things::Thing, {
   const auto name = from.GetName();
   if (name)
      return '#' + name;
   return Annies::Text::Mention(from);
}, Annies::Text);

/// ...even if encountered by pointer                                         
LANGULUS_MORPHISM_CUSTOM(Things::Thing*, {
   const auto name = from->GetName();
   if (name)
      return '#' + name;
   return Annies::Text::Mention(from);
}, Annies::Text);

static_assert(::Langulus::CT::Convertible<::Langulus::Things::Thing, ::Langulus::Annies::Text>);//TODO move these to test
static_assert(::Langulus::CT::Convertible<::Langulus::Things::Thing*, ::Langulus::Annies::Text>);
static_assert(::Langulus::CT::Convertible<::Langulus::Things::Thing const*, ::Langulus::Annies::Text>);
static_assert(not ::Langulus::CT::Container<::Langulus::Things::Thing>);
static_assert(not ::Langulus::CT::Container<::Langulus::Things::Thing*>);
static_assert(not ::Langulus::CT::Container<::Langulus::Things::Thing const*>);
static_assert(not ::std::is_fundamental_v<::Langulus::Things::Thing>);
static_assert(not ::std::is_fundamental_v<::Langulus::Things::Thing*>);
static_assert(not ::std::is_fundamental_v<::Langulus::Things::Thing const*>);
static_assert(::fmt::is_formattable<::Langulus::Things::Thing>::value);
static_assert(not ::fmt::is_formattable<::Langulus::Things::Thing*>::value);        // libfmt never formats pointers
static_assert(not ::fmt::is_formattable<::Langulus::Things::Thing const*>::value);  // libfmt never formats pointers