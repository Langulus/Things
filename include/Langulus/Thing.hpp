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
   ///   MARK: Thing                                                          
   ///                                                                        
   ///   The primary composable type. Its functionality comes from its parts  
   /// and children/owner's parts. The Thing is an aggregate of tags,         
   /// parts, and subthings.                                                  
   struct Thing final : Referenced, SeekInterface {
      using CTTI_Named     = Yes<"Thing">;
      using CTTI_Abstract  = No;
      using CTTI_Producer  = Thing;
      using CTTI_Pooled    = PooledByType<>;
      using CTTI_Ability   = Types<Verbs::Do, Verbs::Create, Verbs::Select>;

   protected:
      friend struct SeekInterface;

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

      auto GetRuntime() const noexcept -> Runtime* {
         return const_cast<Runtime*>(**mRuntime);
      }

      auto GetFlow() const noexcept -> Temporal* {
         return const_cast<Temporal*>(**mFlow);
      }

      LANGULUS_API(THINGS) void Do(Verb&);
      LANGULUS_API(THINGS) void Select(Verb&);
      LANGULUS_API(THINGS) void Create(Verb&);

      LANGULUS_API(THINGS) Many Say(Text const&);
      LANGULUS_API(THINGS) Many Run(Code const&);

      LANGULUS_API(THINGS) bool Update(Time);
      LANGULUS_API(THINGS) void Refresh(bool force = false);
      LANGULUS_API(THINGS) void Reset();

      LANGULUS_API(THINGS)
      bool operator == (Thing const&) const;

      //LANGULUS_API(THINGS)
      //explicit operator Text() const;


      ///                                                                     
      /// MARK: Hierarchy management                                          
      LANGULUS_API(THINGS)
      auto CreateRuntime() -> Runtime*;

      LANGULUS_API(THINGS)
      auto CreateFlow() -> Temporal*;

      auto CreateChild(auto&&...arguments) -> Ref<Thing> {
         THINGS_VERBOSE_SELF_TAB("Producing child (at ", GetReferences(), " references): ");
         Ref<Thing> newThing;
         newThing.New(this, Many {LglsFwd(arguments)...});
         return newThing;
      }

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
      auto GetChild(size_t = 0) const -> Thing const*;

      LANGULUS_API(THINGS)
      auto GetNamedChild(Token const&, size_t = 0) -> Thing*;
      LANGULUS_API(THINGS)
      auto GetNamedChild(Token const&, size_t = 0) const -> Thing const*;

      LANGULUS_API(THINGS)
      void DumpHierarchy() const;


      ///                                                                     
      /// MARK: Part management                                               
      template<bool TWOSIDED = true>
      size_t AddPart(Part*);
      template<bool TWOSIDED = true>
      size_t RemovePart(Part*);

      template<CT::Part, class...A>
      Many CreatePart(A&&...);
      template<CT::Part...>
      Many CreateParts();

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         Many CreatePartToken(Token const& token, auto&&...arguments) {
            return CreateData(Recipe::Of(token, LglsFwd(arguments)...));
         }
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
      /// MARK: Tag management                                                
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
      auto GetLocalTag(this SELF&&, size_t = 0) -> LglsMutIf(SELF, Tag*);

      LANGULUS_API(THINGS)
      void SetName(Text const&);

      LANGULUS_API(THINGS)
      Text GetName() const;
   };
}


/// MARK: Morphisms                                                           
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


static_assert(    ::Langulus::CT::Convertible<::Langulus::Things::Thing, ::Langulus::Annies::Text>);//TODO move these to test
static_assert(    ::Langulus::CT::Convertible<::Langulus::Things::Thing*, ::Langulus::Annies::Text>);
static_assert(    ::Langulus::CT::Convertible<::Langulus::Things::Thing const*, ::Langulus::Annies::Text>);
static_assert(not ::Langulus::CT::Container<::Langulus::Things::Thing>);
static_assert(not ::Langulus::CT::Container<::Langulus::Things::Thing*>);
static_assert(not ::Langulus::CT::Container<::Langulus::Things::Thing const*>);
static_assert(not ::std::is_fundamental_v<::Langulus::Things::Thing>);
static_assert(not ::std::is_fundamental_v<::Langulus::Things::Thing*>);
static_assert(not ::std::is_fundamental_v<::Langulus::Things::Thing const*>);
static_assert(    ::fmt::is_formattable<::Langulus::Things::Thing>::value);
static_assert(not ::fmt::is_formattable<::Langulus::Things::Thing*>::value);        // libfmt never formats pointers
static_assert(not ::fmt::is_formattable<::Langulus::Things::Thing const*>::value);  // libfmt never formats pointers

namespace Langulus::Things
{
   extern template auto SeekInterface::SeekPart<Seek::Below,         size_t, Thing>(this Thing&&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Above,         size_t, Thing>(this Thing&&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Duplex,        size_t, Thing>(this Thing&&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Here,          size_t, Thing>(this Thing&&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Everywhere,    size_t, Thing>(this Thing&&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndAbove,  size_t, Thing>(this Thing&&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndBelow,  size_t, Thing>(this Thing&&, DMeta, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPart<Seek::Below,         size_t, Thing const>(this Thing const&&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Above,         size_t, Thing const>(this Thing const&&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Duplex,        size_t, Thing const>(this Thing const&&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Here,          size_t, Thing const>(this Thing const&&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Everywhere,    size_t, Thing const>(this Thing const&&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, DMeta, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPart<Part, Seek::Below,         size_t, Thing>(this Thing&&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Above,         size_t, Thing>(this Thing&&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Duplex,        size_t, Thing>(this Thing&&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Here,          size_t, Thing>(this Thing&&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Everywhere,    size_t, Thing>(this Thing&&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Part, Seek::HereAndAbove,  size_t, Thing>(this Thing&&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Part, Seek::HereAndBelow,  size_t, Thing>(this Thing&&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPart<Part, Seek::Below,         size_t, Thing const>(this Thing const&&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Above,         size_t, Thing const>(this Thing const&&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Duplex,        size_t, Thing const>(this Thing const&&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Here,          size_t, Thing const>(this Thing const&&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Part, Seek::Everywhere,    size_t, Thing const>(this Thing const&&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Part, Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Part, Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPartAux<Seek::Below,         size_t, Thing>(this Thing&&, Many const&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Above,         size_t, Thing>(this Thing&&, Many const&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Duplex,        size_t, Thing>(this Thing&&, Many const&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Here,          size_t, Thing>(this Thing&&, Many const&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Everywhere,    size_t, Thing>(this Thing&&, Many const&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndAbove,  size_t, Thing>(this Thing&&, Many const&, DMeta, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndBelow,  size_t, Thing>(this Thing&&, Many const&, DMeta, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPartAux<Seek::Below,         size_t, Thing const>(this Thing const&&, Many const&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Above,         size_t, Thing const>(this Thing const&&, Many const&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Duplex,        size_t, Thing const>(this Thing const&&, Many const&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Here,          size_t, Thing const>(this Thing const&&, Many const&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Everywhere,    size_t, Thing const>(this Thing const&&, Many const&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, Many const&, DMeta, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, Many const&, DMeta, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPartAux<Part, Seek::Below,         size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Above,         size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Duplex,        size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Here,          size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Everywhere,    size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::HereAndAbove,  size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::HereAndBelow,  size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Below,         size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Above,         size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Duplex,        size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Here,          size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::Everywhere,    size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Part, Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   
   extern template auto SeekInterface::SeekPartExt<Seek::Below,         size_t, Thing>(this Thing&&, DMeta, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Seek::Above,         size_t, Thing>(this Thing&&, DMeta, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Seek::Duplex,        size_t, Thing>(this Thing&&, DMeta, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Seek::Here,          size_t, Thing>(this Thing&&, DMeta, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Seek::Everywhere,    size_t, Thing>(this Thing&&, DMeta, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Seek::HereAndAbove,  size_t, Thing>(this Thing&&, DMeta, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Seek::HereAndBelow,  size_t, Thing>(this Thing&&, DMeta, Many const&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPartExt<Seek::Below,         size_t, Thing const>(this Thing const&&, DMeta, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Seek::Above,         size_t, Thing const>(this Thing const&&, DMeta, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Seek::Duplex,        size_t, Thing const>(this Thing const&&, DMeta, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Seek::Here,          size_t, Thing const>(this Thing const&&, DMeta, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Seek::Everywhere,    size_t, Thing const>(this Thing const&&, DMeta, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, DMeta, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, DMeta, Many const&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPartExt<Part, Seek::Below,         size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Above,         size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Duplex,        size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Here,          size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Everywhere,    size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::HereAndAbove,  size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::HereAndBelow,  size_t, Thing>(this Thing&&, Many const&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPartExt<Part, Seek::Below,         size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Above,         size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Duplex,        size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Here,          size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::Everywhere,    size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartExt<Part, Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, Many const&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPartAuxExt<Seek::Below,         size_t, Thing>(this Thing&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Above,         size_t, Thing>(this Thing&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Duplex,        size_t, Thing>(this Thing&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Here,          size_t, Thing>(this Thing&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Everywhere,    size_t, Thing>(this Thing&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::HereAndAbove,  size_t, Thing>(this Thing&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::HereAndBelow,  size_t, Thing>(this Thing&&, DMeta, Many const&, Many const&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPartAuxExt<Seek::Below,         size_t, Thing const>(this Thing const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Above,         size_t, Thing const>(this Thing const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Duplex,        size_t, Thing const>(this Thing const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Here,          size_t, Thing const>(this Thing const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::Everywhere,    size_t, Thing const>(this Thing const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, DMeta, Many const&, Many const&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Below,         size_t, Thing>(this Thing&&, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Above,         size_t, Thing>(this Thing&&, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Duplex,        size_t, Thing>(this Thing&&, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Here,          size_t, Thing>(this Thing&&, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Everywhere,    size_t, Thing>(this Thing&&, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::HereAndAbove,  size_t, Thing>(this Thing&&, Many const&, Many const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::HereAndBelow,  size_t, Thing>(this Thing&&, Many const&, Many const&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Below,         size_t, Thing const>(this Thing const&&, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Above,         size_t, Thing const>(this Thing const&&, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Duplex,        size_t, Thing const>(this Thing const&&, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Here,          size_t, Thing const>(this Thing const&&, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::Everywhere,    size_t, Thing const>(this Thing const&&, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, Many const&, Many const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAuxExt<Part, Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, Many const&, Many const&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekTag<Seek::Below,         size_t, Thing>(this Thing&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Above,         size_t, Thing>(this Thing&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Duplex,        size_t, Thing>(this Thing&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Here,          size_t, Thing>(this Thing&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Everywhere,    size_t, Thing>(this Thing&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndAbove,  size_t, Thing>(this Thing&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndBelow,  size_t, Thing>(this Thing&&, TMeta, size_t&&) -> Tag;

   extern template auto SeekInterface::SeekTag<Seek::Below,         size_t, Thing const>(this Thing const&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Above,         size_t, Thing const>(this Thing const&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Duplex,        size_t, Thing const>(this Thing const&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Here,          size_t, Thing const>(this Thing const&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Everywhere,    size_t, Thing const>(this Thing const&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, TMeta, size_t&&) -> Tag;

   extern template auto SeekInterface::SeekTagAux<Seek::Below,         size_t, Thing>(this Thing&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Above,         size_t, Thing>(this Thing&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Duplex,        size_t, Thing>(this Thing&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Here,          size_t, Thing>(this Thing&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Everywhere,    size_t, Thing>(this Thing&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndAbove,  size_t, Thing>(this Thing&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndBelow,  size_t, Thing>(this Thing&&, Many const&, TMeta, size_t&&) -> Tag;

   extern template auto SeekInterface::SeekTagAux<Seek::Below,         size_t, Thing const>(this Thing const&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Above,         size_t, Thing const>(this Thing const&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Duplex,        size_t, Thing const>(this Thing const&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Here,          size_t, Thing const>(this Thing const&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Everywhere,    size_t, Thing const>(this Thing const&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, Many const&, TMeta, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, Many const&, TMeta, size_t&&) -> Tag;

   extern template auto SeekInterface::GatherParts<Seek::Below,         Thing>(this Thing&&, DMeta) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Above,         Thing>(this Thing&&, DMeta) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Duplex,        Thing>(this Thing&&, DMeta) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Here,          Thing>(this Thing&&, DMeta) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Everywhere,    Thing>(this Thing&&, DMeta) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndAbove,  Thing>(this Thing&&, DMeta) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndBelow,  Thing>(this Thing&&, DMeta) -> TMany<Part*>;

   extern template auto SeekInterface::GatherParts<Seek::Below,         Thing const>(this Thing const&&, DMeta) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Above,         Thing const>(this Thing const&&, DMeta) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Duplex,        Thing const>(this Thing const&&, DMeta) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Here,          Thing const>(this Thing const&&, DMeta) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Everywhere,    Thing const>(this Thing const&&, DMeta) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndAbove,  Thing const>(this Thing const&&, DMeta) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndBelow,  Thing const>(this Thing const&&, DMeta) -> TMany<Part const*>;

   extern template auto SeekInterface::GatherPartsExt<Seek::Below,         Thing>(this Thing&&, DMeta, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Above,         Thing>(this Thing&&, DMeta, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Duplex,        Thing>(this Thing&&, DMeta, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Here,          Thing>(this Thing&&, DMeta, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Everywhere,    Thing>(this Thing&&, DMeta, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::HereAndAbove,  Thing>(this Thing&&, DMeta, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::HereAndBelow,  Thing>(this Thing&&, DMeta, Many const&) -> TMany<Part*>;

   extern template auto SeekInterface::GatherPartsExt<Seek::Below,         Thing const>(this Thing const&&, DMeta, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Above,         Thing const>(this Thing const&&, DMeta, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Duplex,        Thing const>(this Thing const&&, DMeta, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Here,          Thing const>(this Thing const&&, DMeta, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::Everywhere,    Thing const>(this Thing const&&, DMeta, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::HereAndAbove,  Thing const>(this Thing const&&, DMeta, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Seek::HereAndBelow,  Thing const>(this Thing const&&, DMeta, Many const&) -> TMany<Part const*>;

   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Below,         Thing>(this Thing&&, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Above,         Thing>(this Thing&&, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Duplex,        Thing>(this Thing&&, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Here,          Thing>(this Thing&&, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Everywhere,    Thing>(this Thing&&, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::HereAndAbove,  Thing>(this Thing&&, Many const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::HereAndBelow,  Thing>(this Thing&&, Many const&) -> TMany<Part*>;

   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Below,         Thing const>(this Thing const&&, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Above,         Thing const>(this Thing const&&, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Duplex,        Thing const>(this Thing const&&, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Here,          Thing const>(this Thing const&&, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::Everywhere,    Thing const>(this Thing const&&, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::HereAndAbove,  Thing const>(this Thing const&&, Many const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherPartsExt<Part, Seek::HereAndBelow,  Thing const>(this Thing const&&, Many const&) -> TMany<Part const*>;

   extern template auto SeekInterface::GatherTags<Seek::Below,         Thing>(this Thing&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Above,         Thing>(this Thing&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Duplex,        Thing>(this Thing&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Here,          Thing>(this Thing&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Everywhere,    Thing>(this Thing&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndAbove,  Thing>(this Thing&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndBelow,  Thing>(this Thing&&, TMeta) -> TagList;

   extern template auto SeekInterface::GatherTags<Seek::Below,         Thing const>(this Thing const&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Above,         Thing const>(this Thing const&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Duplex,        Thing const>(this Thing const&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Here,          Thing const>(this Thing const&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Everywhere,    Thing const>(this Thing const&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndAbove,  Thing const>(this Thing const&&, TMeta) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndBelow,  Thing const>(this Thing const&&, TMeta) -> TagList;

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   extern template auto SeekInterface::SeekPart<Seek::Below,         size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Above,         size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Duplex,        size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Here,          size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::Everywhere,    size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndAbove,  size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndBelow,  size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPart<Seek::Below,         size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Above,         size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Duplex,        size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Here,          size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::Everywhere,    size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPart<Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekPartAux<Seek::Below,         size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Above,         size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Duplex,        size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Here,          size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::Everywhere,    size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndAbove,  size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Part*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndBelow,  size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Part*;

   extern template auto SeekInterface::SeekPartAux<Seek::Below,         size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Above,         size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Duplex,        size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Here,          size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::Everywhere,    size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Part const*;
   extern template auto SeekInterface::SeekPartAux<Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Part const*;

   extern template auto SeekInterface::SeekTag<Seek::Below,         size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Above,         size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Duplex,        size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Here,          size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Everywhere,    size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndAbove,  size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndBelow,  size_t, Thing>(this Thing&&, Token const&, size_t&&) -> Tag;
   
   extern template auto SeekInterface::SeekTag<Seek::Below,         size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Above,         size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Duplex,        size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Here,          size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::Everywhere,    size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTag<Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, Token const&, size_t&&) -> Tag;
   
   extern template auto SeekInterface::SeekTagAux<Seek::Below,         size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Above,         size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Duplex,        size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Here,          size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Everywhere,    size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndAbove,  size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndBelow,  size_t, Thing>(this Thing&&, Many const&, Token const&, size_t&&) -> Tag;

   extern template auto SeekInterface::SeekTagAux<Seek::Below,         size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Above,         size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Duplex,        size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Here,          size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::Everywhere,    size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndAbove,  size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Tag;
   extern template auto SeekInterface::SeekTagAux<Seek::HereAndBelow,  size_t, Thing const>(this Thing const&&, Many const&, Token const&, size_t&&) -> Tag;

   extern template auto SeekInterface::GatherParts<Seek::Below,         Thing>(this Thing&&, Token const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Above,         Thing>(this Thing&&, Token const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Duplex,        Thing>(this Thing&&, Token const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Here,          Thing>(this Thing&&, Token const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::Everywhere,    Thing>(this Thing&&, Token const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndAbove,  Thing>(this Thing&&, Token const&) -> TMany<Part*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndBelow,  Thing>(this Thing&&, Token const&) -> TMany<Part*>;
   
   extern template auto SeekInterface::GatherParts<Seek::Below,         Thing const>(this Thing const&&, Token const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Above,         Thing const>(this Thing const&&, Token const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Duplex,        Thing const>(this Thing const&&, Token const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Here,          Thing const>(this Thing const&&, Token const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::Everywhere,    Thing const>(this Thing const&&, Token const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndAbove,  Thing const>(this Thing const&&, Token const&) -> TMany<Part const*>;
   extern template auto SeekInterface::GatherParts<Seek::HereAndBelow,  Thing const>(this Thing const&&, Token const&) -> TMany<Part const*>;
   
   extern template auto SeekInterface::GatherTags<Seek::Below,         Thing>(this Thing&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Above,         Thing>(this Thing&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Duplex,        Thing>(this Thing&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Here,          Thing>(this Thing&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Everywhere,    Thing>(this Thing&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndAbove,  Thing>(this Thing&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndBelow,  Thing>(this Thing&&, Token const&) -> TagList;
   
   extern template auto SeekInterface::GatherTags<Seek::Below,         Thing const>(this Thing const&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Above,         Thing const>(this Thing const&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Duplex,        Thing const>(this Thing const&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Here,          Thing const>(this Thing const&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::Everywhere,    Thing const>(this Thing const&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndAbove,  Thing const>(this Thing const&&, Token const&) -> TagList;
   extern template auto SeekInterface::GatherTags<Seek::HereAndBelow,  Thing const>(this Thing const&&, Token const&) -> TagList;
#endif
}