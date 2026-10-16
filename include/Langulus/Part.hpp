///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Hierarchy.hpp"
#include <Langulus/Text.hpp>


namespace Langulus::Things
{
   struct Temporal;

   ///                                                                        
   ///   An abstract part                                                     
   ///                                                                        
   ///   Part is a shorter name for a component, or extension. It's used for  
   /// composing Things' behavior. Parts are usually produced from factories  
   /// inside external, dynamically loaded modules. There are parts for       
   /// graphics, input, AI, content, and whatever extensions you make.        
   ///                                                                        
   struct LANGULUS_API(THINGS) Part
      : virtual Resolvable
      , virtual Referenced
      , Things::SeekInterface
   {
      using CTTI_Bases = Resolvable;

   protected:
      friend struct Thing;

      // Things that are coupled with this part.                        
      // Owners act as an environment for the part's context, providing 
      // additional tags and other parts for interoperability.          
      Hierarchy mOwners;

   public:
      Part() noexcept /*: Resolvable {this}*/ {}
      Part(Part const&) = delete;
      Part(Part&&) noexcept = delete;
      virtual ~Part();

      Part& operator = (Part const&) = delete;
      Part& operator = (Part&&) noexcept = delete;

      void Select(Verb&);

      virtual void Refresh();

      auto GetRuntime() const noexcept -> Runtime*;
      auto GetFlow() const noexcept -> Temporal*;
      auto GetOwners() const noexcept -> Hierarchy const&;
      bool CompareDescriptor(Many const&) const;
      
      ///                                                                     
      ///   Flow                                                              
      ///                                                                     
      template<Seek = Seek::HereAndAbove, CT::Executable V>
      V& RunIn(V&);

   protected:
      void Couple(Many const&, const Thing* = nullptr);
      void Decouple(const Thing*);
      void ReplaceOwner(const Thing*, const Thing*);
   };
}

/// Convert to text, by writing a short name and address                      
LANGULUS_MORPHISM_CUSTOM(Things::Part, {
   return Annies::Text::Mention(from);
}, Annies::Text);

/// ...even if encountered by pointer                                         
LANGULUS_MORPHISM_CUSTOM(Things::Part*, {
   return Annies::Text::Mention(from);
}, Annies::Text);
