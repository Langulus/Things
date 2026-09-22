///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Asset.hpp"
#include <Langulus/Mapping.hpp>
#include <Langulus/Vectors/TVector.hpp>


namespace Langulus::Things
{
   ///                                                                        
   ///   Vertex/index buffer view                                             
   ///                                                                        
   struct MeshView {
      using MapModeType = Math::MapModeType;

      // Number of primitives                                           
      uint32_t mPrimitiveCount = 0;
      // Starting primitive                                             
      uint32_t mPrimitiveStart = 0;
      // Number of indices                                              
      uint32_t mIndexCount = 0;
      // Starting index                                                 
      uint32_t mIndexStart = 0;
      // Data topology                                                  
      DMeta mTopology;
      // Double-sidedness                                               
      bool mBilateral = false;
      // Texture mapping mode                                           
      MapModeType mTextureMapping = MapModeType::Auto;

      bool operator == (const MeshView&) const noexcept;

      auto Decay() const -> MeshView;
      Hash GetHash() const noexcept;
   };

   
   ///                                                                        
   ///   Abstract geometry content                                            
   ///                                                                        
   struct Mesh : Asset {
   protected:
      using MapModeType = Math::MapModeType;
      using Vec2u = Math::Vec2u;
      using Vec3u = Math::Vec3u;
      MeshView mView;

   public:
      using CTTI_Bases = Asset;

      Mesh() : Resolvable {this} {}

      template<CT::Topology, CT::Topology...>
      bool CheckTopology()    const;
      auto GetTopology()      const noexcept -> DMeta;
      auto GetTextureMapper() const noexcept -> MapModeType;

      auto GetView() const noexcept -> MeshView const&;
      auto GetView()       noexcept -> MeshView&;

      virtual auto GetLOD(const Math::LOD&) const -> Ref<Mesh> = 0;

      // Point utilities                                                
      bool MadeOfPoints() const noexcept;
      auto GetPointCount() const -> size_t;
      template<CT::DefineTag>
      Many GetPointTrait(size_t) const;

      // Line utilities                                                 
      bool MadeOfLines() const noexcept;
      auto GetLineCount() const -> size_t;
      auto GetLineIndices(size_t) const -> Vec2u;
      template<CT::DefineTag>
      Many GetLineTrait(size_t) const;

      // Triangle utilities                                             
      bool MadeOfTriangles() const noexcept;
      auto GetTriangleCount() const -> size_t;
      auto GetTriangleIndices(size_t) const -> Vec3u;
      template<CT::DefineTag>
      Many GetTriangleTrait(size_t) const;

      ///                                                                     
      ///   Iteration                                                         
      ///                                                                     
      size_t ForEachVertex(auto&&) const;

   protected:
      Vec2u InnerGetIndices(const Data*, const Vec2u&) const;
      Vec3u InnerGetIndices(const Data*, const Vec3u&) const;

      template<bool INDEXED, class...T>
      size_t ForEachVertexInner(Types<T...>, auto&& call) const;

      template<CT::DefineTag T>
      T ForEachVertex_PrepareStream() const;

      template<CT::DefineTag T>
      T ForEachVertex_PrepareIndexStream() const;

      template<CT::Topology>
      auto PickVertex(size_t i, const CT::DefineTag auto& data, const CT::DefineTag auto& indices) const;

      template<CT::Topology, size_t...STREAM_ID>
      auto GenerateVertex(size_t i, const auto& data, const auto& indices, std::index_sequence<STREAM_ID...>&&) const;
   };
}

namespace Langulus::CT
{
   /// A concept for any kind of geometric content unit                       
   template<class T>
   concept Mesh = DerivedFrom<T, A::Mesh>;
}

#include "Mesh.inl"