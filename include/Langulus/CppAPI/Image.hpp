///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Art.hpp"
#include <Langulus/Math/Color.hpp>
#include <Langulus/Math/Scale.hpp>


namespace Langulus::Things
{
   ///                                                                        
   ///   Universal pixel buffer view                                          
   struct ImageView {
      uint32_t mWidth  = 1;
      uint32_t mHeight = 1;
      uint32_t mDepth  = 1;
      uint32_t mFrames = 1;
      RTTI::DMeta mFormat {};
      // Reverse RGBA to BGRA                                           
      //TODO This is not a scalable solution and would eventually fail      
      bool mReverseFormat {};

      bool operator == (ImageView const&) const noexcept = default;
      bool DimensionsMatch(ImageView const& other) const noexcept;

      constexpr uint32_t GetPixelCount() const noexcept;
      constexpr uint32_t GetDimensionCount() const noexcept;
      auto GetPixelBytesize() const noexcept -> size_t;
      auto GetBytesize() const noexcept -> size_t;
      auto GetChannelCount() const noexcept -> size_t;
      Hash GetHash() const noexcept;
      auto GetScale() const noexcept -> Math::Scale3;
   };


   ///                                                                        
   ///   Abstract texture content                                             
   struct Image : Art {
   protected:
      ImageView mView;

   public:
      using CTTI_Bases = Art;

      Image() : Downcastable {this} {}

      virtual auto GetLOD(Math::LOD const&) const -> Ref<Image> { return {}; }
      virtual auto GetGPUHandle() const noexcept -> void* { return nullptr; }

      auto GetFormat() const noexcept -> DMeta;
      auto GetView() const noexcept -> ImageView const&;
      auto GetView() noexcept -> ImageView&;

      template<class F>
      auto ForEachPixel(F&&) const;

      void Upload(CT::Container auto&&);

      /// MARK: Iteration                                                     
      decltype(auto) begin(this auto&& self) noexcept {
         return self.template GetData<Tags::Color>()->GetHandle();
      }

      decltype(auto) end(this auto&& self) noexcept {
         auto color_data = self.template GetData<Tags::Color>();
         return color_data->GetHandle() + color_data->GetCount();
      }
   };


   ///                                                                        
   ///   Abstract font content                                                
   struct Font : Image {
      using CTTI_Bases = Image;

      Font() : Downcastable {this} {}
   };
}

namespace Langulus::CT
{
   /// A concept for any kind of image content part                           
   template<class T>
   concept Image = DerivedFrom<T, Things::Image>;
}

#include "Image.inl"