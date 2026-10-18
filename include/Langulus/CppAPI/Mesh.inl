///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Mesh.hpp"
#include <Langulus/Primitives/Primitive.hpp>
#include <Langulus/Primitives/TLine.hpp>
#include <Langulus/Primitives/TTriangle.hpp>


namespace Langulus::Things
{
   /// Compare geometry views                                                 
   ///   @param rhs the geometry view to compare against                      
   ///   @return true if both views are the same                              
   inline bool MeshView::operator == (const MeshView& rhs) const noexcept {
      return mPrimitiveCount == rhs.mPrimitiveCount
         and mPrimitiveStart == rhs.mPrimitiveStart
         and mIndexCount == rhs.mIndexCount
         and mIndexStart == rhs.mIndexStart
         and (mTopology == rhs.mTopology or (mTopology and mTopology == rhs.mTopology))
         and mBilateral == rhs.mBilateral;
   }

   /// Decay the geometry view to a list of points                            
   ///   @return the decayed view                                             
   inline auto MeshView::Decay() const -> MeshView {
      LglsAssert(mPrimitiveCount and mTopology, "Bad vertex view");
      const auto abstract_point = MetaDataOf<Math::Point>();
      if (mTopology.Is(abstract_point))
         return *this;

      MeshView result {*this};
      result.mTopology = abstract_point;

      // mPrimitiveCount corresponds to the number of points in these   
      // cases:                                                         
      if (  mTopology.Is(MetaDataOf<Math::TriangleStrip>())
         or mTopology.Is(MetaDataOf<Math::TriangleFan>())
         or mTopology.Is(MetaDataOf<Math::LineStrip>())
         or mTopology.Is(MetaDataOf<Math::LineLoop>()))
         return result;

      if (mTopology.Is(MetaDataOf<Math::Triangle>())) {
         // Decay triangles to points                                   
         result.mPrimitiveCount *= 3;
      }
      if (mTopology.Is(MetaDataOf<Math::Line>())) {
         // Decay lines to points                                       
         result.mPrimitiveCount *= 2;
      }
      else throw Exception("Bad primitive type");
      return result;
   }
   
   /// Get the hash of a normalized descriptor (cached)                       
   ///   @return the hash                                                     
   inline Hash MeshView::GetHash() const noexcept {
      return HashOf(
         mPrimitiveCount, mPrimitiveStart, mIndexCount, mIndexStart,
         mTopology, mBilateral, mTextureMapping
      );
   }
   
   /// Get the topology of the geometry                                       
   ///   @return the topology type                                            
   inline auto Mesh::GetTopology() const noexcept -> DMeta {
      return mView.mTopology;
   }
   
   /// Get the texture mapping mode                                           
   ///   @return the texture mapping mode                                     
   inline auto Mesh::GetTextureMapper() const noexcept -> MapModeType {
      return mView.mTextureMapping;
   }

   /// Check if topology matches one of the specified types                   
   ///   @tparam T1, TN types to check                                        
   ///   @return true if T matches one of the topology types                  
   template<CT::Topology T1, CT::Topology...TN>
   bool Mesh::CheckTopology() const {
      return   mView.mTopology == MetaDataOf<T1>()
          or ((mView.mTopology == MetaDataOf<TN>()) or ...);
   }

   /// Helper that indirects in case there is an index buffer                 
   ///   @param indices index buffer                                          
   ///   @param where line indices                                            
   ///   @return the (eventually indirected) line indices                     
   inline auto Mesh::InnerGetIndices(const Data* indices, const Vec2u& where) const -> Vec2u {
      if (not indices or not *indices)
         return where;

      if (indices->IsExact<uint8_t>()) {
         auto idx = indices->As<uint8_t*>();
         return {idx[where[0]], idx[where[1]]};
      }
      
      if (indices->IsExact<uint16_t>()) {
         auto idx = indices->As<uint16_t*>();
         return {idx[where[0]], idx[where[1]]};
      }

      if (indices->IsExact<uint32_t>()) {
         auto idx = indices->As<uint32_t*>();
         return {idx[where[0]], idx[where[1]]};
      }

      if (indices->IsExact<uint64_t>()) {
         auto idx = indices->As<uint64_t*>();
         return {
            static_cast<uint32_t>(idx[where[0]]),
            static_cast<uint32_t>(idx[where[1]])
         };
      }

      throw Exception("Trying to get index from incompatible index buffer");
   }

   /// Helper that indirects in case there is an index buffer                 
   ///   @param indices index buffer                                          
   ///   @param where triangle indices                                        
   ///   @return the (eventually indirected) triangle indices                 
   inline auto Mesh::InnerGetIndices(const Data* indices, const Vec3u& where) const -> Vec3u {
      if (not indices or not *indices)
         return where;

      if (indices->IsExact<uint8_t>()) {
         auto idx = indices->As<uint8_t*>();
         return {idx[where[0]], idx[where[1]], idx[where[2]]};
      }

      if (indices->IsExact<uint16_t>()) {
         auto idx = indices->As<uint16_t*>();
         return {idx[where[0]], idx[where[1]], idx[where[2]]};
      }

      if (indices->IsExact<uint32_t>()) {
         auto idx = indices->As<uint32_t*>();
         return {idx[where[0]], idx[where[1]], idx[where[2]]};
      }

      if (indices->IsExact<uint64_t>()) {
         auto idx = indices->As<uint64_t*>();
         return {
            static_cast<uint32_t>(idx[where[0]]),
            static_cast<uint32_t>(idx[where[1]]),
            static_cast<uint32_t>(idx[where[2]])
         };
      }

      throw Exception("Trying to get index from incompatible index buffer");
   }

   /// Is topology a point list?                                              
   ///   @return true if geometry is made of points                           
   inline bool Mesh::MadeOfPoints() const noexcept {
      return CheckTopology<Math::Point>();
   }

   /// Get the number of points inside the geometry                           
   ///   @return the number of points                                         
   inline size_t Mesh::GetPointCount() const {
      TODO();
      return {};
   }

   template<CT::DefineTag T>
   Many Mesh::GetPointTag(size_t) const {
      TODO();
      return {};
   }

   /// Is topology line list/strip/loop?                                      
   ///   @return true if geometry is made of lines                            
   inline bool Mesh::MadeOfLines() const noexcept {
      return CheckTopology<Math::Line, Math::LineStrip, Math::LineLoop>();
   }

   /// Get the number of lines inside the geometry                            
   ///   @return the number of points                                         
   inline size_t Mesh::GetLineCount() const {
      if (MadeOfPoints())
         return 0;
      
      if (CheckTopology<Math::Line>()) {
         if (mView.mIndexCount > 1)
            return mView.mIndexCount / 2;
         else {
            auto decayed = mView.Decay();
            if (decayed.mPrimitiveCount > 1)
               return decayed.mPrimitiveCount / 2;
         }
         return 0;
      }
      else if (CheckTopology<Math::LineStrip>()) {
         if (mView.mIndexCount > 1)
            return mView.mIndexCount - 1;
         else if (mView.mPrimitiveCount > 1)
            return mView.mPrimitiveCount - 1;
         return 0;
      }
      else if (CheckTopology<Math::LineLoop>()) {
         if (mView.mIndexCount > 1)
            return mView.mIndexCount;
         else if (mView.mPrimitiveCount > 1)
            return mView.mPrimitiveCount;
         return 0;
      }
      else if (CheckTopology<Math::Triangle>()) {
         if (mView.mIndexCount > 2)
            return mView.mIndexCount / 3;
         else {
            auto decayed = mView.Decay();
            if (decayed.mPrimitiveCount > 2)
               return decayed.mPrimitiveCount;
         }
         return 0;
      }
      else if (CheckTopology<Math::TriangleStrip, Math::TriangleFan>()) {
         if (mView.mIndexCount > 2)
            return 1 + (mView.mIndexCount - 2) * 2;
         else if (mView.mPrimitiveCount > 2)
            return 1 + (mView.mPrimitiveCount - 2) * 2;
         return 0;
      }

      throw Exception("Trying to count lines for unknown topology");
   }

   /// Get the point indices of a given line                                  
   ///   @param index line index                                              
   ///   @return the point indices as a 32bit unsigned 2D vector              
   inline auto Mesh::GetLineIndices(size_t index) const -> Vec2u {
      const auto indices = GetData<Tags::Index>();

      if (CheckTopology<Math::Line>())
         return InnerGetIndices(indices, Vec2u(index * 2, index * 2 + 1));
      else if (CheckTopology<Math::LineStrip>())
         return InnerGetIndices(indices, Vec2u(index > 0 ? index - 1 : 0, index > 0 ? index : 1));
      else if (CheckTopology<Math::LineLoop>()) {
         if (index == GetLineCount() - 1)
            return InnerGetIndices(indices, Vec2u(index, 0));
         else
            return InnerGetIndices(indices, Vec2u(index > 0 ? index - 1 : 0, index > 0 ? index : 1));
      }
      else if (CheckTopology<Math::Triangle>()) {
         switch (index % 3) {
         case 0: case 1:
            return InnerGetIndices(indices, Vec2u(index, index + 1));
         case 2:
            return InnerGetIndices(indices, Vec2u(index, index - 2));
         }
      }
      else if (CheckTopology<Math::TriangleStrip>()) {
         switch (index % 3) {
         case 0:
            return InnerGetIndices(indices, Vec2u(index == 0 ? 0 : index - 2, index == 0 ? 1 : index - 1));
         case 1:
            return InnerGetIndices(indices, Vec2u(index - 1, index));
         case 2:
            return InnerGetIndices(indices, Vec2u(index, index - 2));
         }
      }
      else if (CheckTopology<Math::TriangleFan>()) {
         switch (index % 3) {
         case 0:
            return InnerGetIndices(indices, Vec2u(0, index == 0 ? 1 : index - 1));
         case 1:
            return InnerGetIndices(indices, Vec2u(index - 1, index));
         case 2:
            return InnerGetIndices(indices, Vec2u(index, 0));
         }
      }

      throw Exception("Trying to count lines for unknown topology");
   }

   /// Get a specific property of a specific line                             
   ///   @tparam T the tag to retrieve                                        
   ///   @param lineIndex the line index                                      
   ///   @return data for the specific line                                   
   template<CT::DefineTag T>
   Many Mesh::GetLineTag(size_t lineIndex) const {
      const auto indices = GetLineIndices(lineIndex);
      const auto soughtt = GetData<T>(0);
      if (not soughtt or not *soughtt)
         return {};

      Many soughtDecayed;
      if (soughtt->template CastsTo<Math::Triangle3>())
         soughtDecayed = soughtt->template ReinterpretAs<Math::Vec3>();
      else if (soughtt->template CastsTo<Math::Triangle2>())
         soughtDecayed = soughtt->template ReinterpretAs<Math::Vec2>();
      else
         soughtDecayed = *soughtt;

      Many result;
      result.Concat(soughtDecayed.GetHandle() + indices[0]);
      result.Concat(soughtDecayed.GetHandle() + indices[1]);
      return result;
   }
   
   /// Is geometry made up of triangles list/strip/fan?                       
   ///   @return true if geometry is made of triangles                        
   inline bool Mesh::MadeOfTriangles() const noexcept {
      return CheckTopology<Math::Triangle, Math::TriangleStrip, Math::TriangleFan>();
   }
   
   /// Get the number of triangles inside the geometry, after indexing and    
   /// topology considerations                                                
   ///   @return the number of triangles                                      
   inline size_t Mesh::GetTriangleCount() const {
      if (MadeOfPoints() or MadeOfLines())
         return 0;

      if (CheckTopology<Math::Triangle>()) {
         if (mView.mIndexCount > 2)
            return mView.mIndexCount / 3;

         auto decayed = mView.Decay();
         LglsAssert(decayed.mPrimitiveCount % 3, "Bad topology");
         return decayed.mPrimitiveCount / 3;
      }
      else if (CheckTopology<Math::TriangleStrip, Math::TriangleFan>()) {
         if (mView.mIndexCount > 2)
            return mView.mIndexCount - 2;

         LglsAssert(mView.mPrimitiveCount < 3, "Bad topology");
         return mView.mPrimitiveCount - 2;
      }

      throw Exception("Trying to count triangles for unknown topology");
   }

   /// Get the indices of a given triangle                                    
   ///   @attention not optimized for iteration                               
   ///   @param index triangle index                                          
   ///   @return the indices as a 32bit unsigned 3D vector                    
   inline auto Mesh::GetTriangleIndices(size_t index) const -> Vec3u {
      const auto indices = GetData<Tags::Index>(0);

      if (CheckTopology<Math::Triangle>()) {
         return InnerGetIndices(indices, Vec3u(
            index * 3,
            index * 3 + 1,
            index * 3 + 2)
         );
      }
      else if (CheckTopology<Math::TriangleStrip>()) {
         return InnerGetIndices(indices, Vec3u(
            index == 0 ? 0 : index - 1,
            index == 0 ? 1 : index,
            index == 0 ? 2 : index + 1)
         );
      }
      else if (CheckTopology<Math::TriangleFan>()) {
         return InnerGetIndices(indices, Vec3u(
            0,
            index == 0 ? 1 : index,
            index == 0 ? 2 : index + 1)
         );
      }

      throw Exception("Trying to count triangles for unknown topology");
   }

   /// Get a specific property of a specific triangle                         
   ///   @attention not optimized for iteration                               
   ///   @tparam T the tag to retrieve                                        
   ///   @param triangleIndex the triangle index                              
   ///   @return data for the specific triangle                               
   template<CT::DefineTag T>
   Many Mesh::GetTriangleTag(size_t triangleIndex) const {
      const auto indices = GetTriangleIndices(triangleIndex);
      const auto soughtt = GetData<T>(0);
      if (not soughtt or not *soughtt)
         return {};

      Many soughtDecayed;
      if (soughtt->template CastsTo<Math::Triangle3>())
         soughtDecayed = soughtt->template ReinterpretAs<Math::Vec3>();
      else if (soughtt->template CastsTo<Math::Triangle2>())
         soughtDecayed = soughtt->template ReinterpretAs<Math::Vec2>();
      else
         soughtDecayed = *soughtt;

      Many result;
      result.Concat(soughtDecayed.GetHandle() + indices[0]);
      result.Concat(soughtDecayed.GetHandle() + indices[1]);
      result.Concat(soughtDecayed.GetHandle() + indices[2]);
      return result;
   }
   
   /// Optimized for cycling through the data of all triangles                
   /// Will generate triangles based on indices and topology                  
   ///   @param call lambda with the desired triangle tags as arguments       
   ///   @return the number of iterated triangles                             
   size_t Mesh::ForEachVertex(auto&& call) const {
      using F = Deref<decltype(call)>;
      using A = ArgumentsOf<F>;

      const auto indices = GetDataList<Tags::Index>();
      if (not indices or not *indices)
         return ForEachVertexInner<false>(A {}, LglsFwd(call));
      else
         return ForEachVertexInner<true> (A {}, LglsFwd(call));
   }

   /// Inner vertex iteration                                                 
   ///   @tparam ...T the decomposed list of call arguments                   
   ///   @param call the call to execute on each iteration                    
   ///   @return the number of executions of 'call'                           
   template<bool INDEXED, CT::DefineTag...T>
   size_t Mesh::ForEachVertexInner(Types<T...>, auto&& call) const {
      // Represent iterator arguments as a tuple of disowned Blocks     
      using Tuple = std::tuple<T...>;
      const Tuple data_streams {ForEachVertex_PrepareStream<T>()...};
      size_t counter = 0;

      if constexpr (not INDEXED) {
         // Unindexed triangles                                         
         // Apply the tuple to the call on each iteration               
         TODO();
         std::apply(call, data_streams);
      }
      else {
         // Indexed triangles                                           
         // There could be separate index streams for each data stream  
         // If that's not the case, then each data stream will reuse    
         // the same index stream                                       
         const Tuple index_streams {ForEachVertex_PrepareIndexStream<T>()...};
         using Sequence = std::make_index_sequence<sizeof...(T)>;

         if (CheckTopology<Math::Triangle>()) {
            // Triangle list                                            
            for (uint32_t i = 0; i < mView.mIndexCount; ++i) {
               std::apply(call, GenerateVertex<Math::Triangle>(
                  i, data_streams, index_streams, Sequence {}));
            }
            counter = mView.mIndexCount;
         }
         else if (CheckTopology<Math::TriangleStrip>()) {
            // Triangle strip                                           
            // (two of the vertices are reused for each triangle)       
            uint32_t marker = 0;
            Tuple reuse[3] {
               GenerateVertex<Math::TriangleStrip>(
                  0, data_streams, index_streams, Sequence {}),
               GenerateVertex<Math::TriangleStrip>(
                  1, data_streams, index_streams, Sequence {}),
               GenerateVertex<Math::TriangleStrip>(
                  2, data_streams, index_streams, Sequence {})
            };

            std::apply(call, reuse[0]);
            std::apply(call, reuse[1]);
            std::apply(call, reuse[2]);

            for (uint32_t i = 3; i < mView.mIndexCount; ++i) {
               // Apply the tuple to the call on each triangle vertex   
               reuse[marker] = GenerateVertex<Math::TriangleStrip>(
                  i, data_streams, index_streams, Sequence {});

               switch (marker) {
               case 0:
                  std::apply(call, reuse[1]);
                  std::apply(call, reuse[2]);
                  std::apply(call, reuse[0]);
                  marker = 1;
                  break;
               case 1:
                  std::apply(call, reuse[2]);
                  std::apply(call, reuse[0]);
                  std::apply(call, reuse[1]);
                  marker = 2;
                  break;
               case 2:
                  std::apply(call, reuse[0]);
                  std::apply(call, reuse[1]);
                  std::apply(call, reuse[2]);
                  marker = 0;
                  break;
               }
            }
            counter = (mView.mIndexCount - 3) * 3;
         }
         else if (CheckTopology<Math::TriangleFan>()) {
            // Triangle fan                                             
            // (first vertex in a fan is constantly reused)             
            uint32_t marker = 1;
            Tuple reuse[3] {
               GenerateVertex<Math::TriangleFan>(
                  0, data_streams, index_streams, Sequence {}),
               GenerateVertex<Math::TriangleFan>(
                  1, data_streams, index_streams, Sequence {}),
               GenerateVertex<Math::TriangleFan>(
                  2, data_streams, index_streams, Sequence {})
            };

            std::apply(call, reuse[0]);
            std::apply(call, reuse[1]);
            std::apply(call, reuse[2]);

            for (uint32_t i = 3; i < mView.mIndexCount; ++i) {
               // Apply the tuple to the call on each triangle vertex   
               reuse[marker] = GenerateVertex<Math::TriangleFan>(
                  i, data_streams, index_streams, Sequence {});

               switch (marker) {
               case 2:
                  std::apply(call, reuse[0]);
                  std::apply(call, reuse[1]);
                  std::apply(call, reuse[2]);
                  marker = 1;
                  break;
               case 1:
                  std::apply(call, reuse[0]);
                  std::apply(call, reuse[2]);
                  std::apply(call, reuse[1]);
                  marker = 2;
                  break;
               }
            }
            counter = (mView.mIndexCount - 3) * 3;
         }
         else LglsError("Unsupported topology");
      }

      return counter;
   }

   /// Prepare a tag block, that will be offsetted while iterating            
   ///   @tparam T the tag to interface                                       
   ///   @return a disowned tag block                                         
   template<CT::DefineTag T>
   T Mesh::ForEachVertex_PrepareStream() const {
      const auto found = GetData<T>(0);
      if (not found or not *found) {
         // No such data was found, no iteration will be performed      
         // on that tag                                                 
         return {};
      }

      // Data was found, always decay it down to a vector type to       
      // simplify things a bit on the other end                         
      //TODO this can be made more elegant by CastsTo<float/double, true>(count)
      if (found->template CastsTo<Math::Triangle3>(1, true))
         return Disown(found->template ReinterpretAs<Math::Vec3>());
      else if (found->template CastsTo<Math::Triangle2>(1, true))
         return Disown(found->template ReinterpretAs<Math::Vec2>());
      else if (found->template CastsTo<Math::Vec3>(1, true))
         return Disown(found->template ReinterpretAs<Math::Vec3>());
      else if (found->template CastsTo<Math::Vec2>(1, true))
         return Disown(found->template ReinterpretAs<Math::Vec2>());

      LglsError("Unsupported tag format");
      return {};
   }

   /// Prepare an index block, that will be offsetted while iterating         
   ///   @tparam T the tag to interface                                       
   ///   @return a disowned tag block                                         
   template<CT::DefineTag T>
   T Mesh::ForEachVertex_PrepareIndexStream() const {
      const auto indices = GetDataList<Tags::Index>();
      if (not indices or not *indices)
         return {};

      if (indices->GetCount() == 1) {
         // Single index source will be used for all vertex tags        
         return Disown((*indices)[0]);
      }

      // Multiple index sequences for different streams                 
      // Each sequence should be kept in a corresponding tag            
      T result;
      indices->ForEachDeep([&result](T const& tag) noexcept {
         result = tag;
         return Loop::Break;
      });
      return result;
   }

   /// Invoke 'call' with the required arguments                              
   ///   @tparam T the topology                                               
   ///   @param i the vertex index to dispatch                                
   ///   @param data the data stream tuple                                    
   ///   @param indices the index stream tuple                                
   ///   @return the tuple of arguments for a vertex                          
   template<CT::Topology T, size_t...STREAM_ID>
   auto Mesh::GenerateVertex(size_t i,
      auto const& data, auto const& indices,
      std::index_sequence<STREAM_ID...>&&
   ) const {
      return ::std::make_tuple(PickVertex<T>(i,
         ::std::get<STREAM_ID>(data),
         ::std::get<STREAM_ID>(indices)
      )...);
   }

   /// Each data stream can be indexed differently, for example a box         
   /// can have a position for each vertex, but only one normal for all       
   ///   @param stream the data stream                                        
   ///   @param indices the index stream                                      
   ///   @return the selected element                                         
   template<CT::Topology T>
   auto Mesh::PickVertex(size_t i, const CT::DefineTag auto& stream, const CT::DefineTag auto& indices) const {
      LglsAssumeDev(mView.mIndexCount,
         "PickVertex can be used only on indexed geometry");

      if (stream.GetCount() <= 1) {
         // Stream is invalid, or has only one element                  
         return stream;
      }
      else if (indices.IsEmpty()) {
         // No indices, so forward 'i' directly                         
         return stream.Select(i, 1);
      }
      else if (indices.GetCount() == mView.mIndexCount) {
         // Per-vertex indexing strategy                                
         if (indices.template IsExact<uint32_t>())
            return stream.Select(indices.template GetRaw<uint32_t>()[i], 1);
         else if (indices.template IsExact<uint64_t>())
            return stream.Select(indices.template GetRaw<uint64_t>()[i], 1);
         else if (indices.template IsExact<uint16_t>())
            return stream.Select(indices.template GetRaw<uint16_t>()[i], 1);
         else if (indices.template IsExact<uint8_t>())
            return stream.Select(indices.template GetRaw<uint8_t> ()[i], 1);
         else
            LglsError("Unsupported index format");
      }
      else if (indices.GetCount() < mView.mIndexCount) {
         // Per-primitive indexing strategy                             
         if constexpr (CT::Triangle<T>) {
            const auto p_i = i / 3;
            if (indices.template IsExact<uint32_t>())
               return stream.Select(indices.template GetRaw<uint32_t>()[p_i], 1);
            else if (indices.template IsExact<uint64_t>())
               return stream.Select(indices.template GetRaw<uint64_t>()[p_i], 1);
            else if (indices.template IsExact<uint16_t>())
               return stream.Select(indices.template GetRaw<uint16_t>()[p_i], 1);
            else if (indices.template IsExact<uint8_t>())
               return stream.Select(indices.template GetRaw<uint8_t >()[p_i], 1);
            else
               LglsError("Unsupported index format");
         }
         else if constexpr (CT::TriangleStrip<T> or CT::TriangleFan<T>) {
            const auto p_i = i < 2 ? 0 : i - 2;
            if (indices.template IsExact<uint32_t>())
               return stream.Select(indices.template GetRaw<uint32_t>()[p_i], 1);
            else if (indices.template IsExact<uint64_t>())
               return stream.Select(indices.template GetRaw<uint64_t>()[p_i], 1);
            else if (indices.template IsExact<uint16_t>())
               return stream.Select(indices.template GetRaw<uint16_t>()[p_i], 1);
            else if (indices.template IsExact<uint8_t>())
               return stream.Select(indices.template GetRaw<uint8_t >()[p_i], 1);
            else
               LglsError("Unsupported index format");
         }
         else if constexpr (CT::Line<T>) {
            const auto p_i = i / 2;
            if (indices.template IsExact<uint32_t>())
               return stream.Select(indices.template GetRaw<uint32_t>()[p_i], 1);
            else if (indices.template IsExact<uint64_t>())
               return stream.Select(indices.template GetRaw<uint64_t>()[p_i], 1);
            else if (indices.template IsExact<uint16_t>())
               return stream.Select(indices.template GetRaw<uint16_t>()[p_i], 1);
            else if (indices.template IsExact<uint8_t>())
               return stream.Select(indices.template GetRaw<uint8_t >()[p_i], 1);
            else
               LglsError("Unsupported index format");
         }
         else if constexpr (CT::LineStrip<T> or CT::LineLoop<T>) {
            const auto p_i = i < 1 ? 0 : i - 1;
            if (indices.template IsExact<uint32_t>())
               return stream.Select(indices.template GetRaw<uint32_t>()[p_i], 1);
            else if (indices.template IsExact<uint64_t>())
               return stream.Select(indices.template GetRaw<uint64_t>()[p_i], 1);
            else if (indices.template IsExact<uint16_t>())
               return stream.Select(indices.template GetRaw<uint16_t>()[p_i], 1);
            else if (indices.template IsExact<uint8_t>())
               return stream.Select(indices.template GetRaw<uint8_t >()[p_i], 1);
            else
               LglsError("Unsupported index format");
         }
         else static_assert(false, "Unsupported topology");
      }
      else LglsError("Unsupported indexing strategy");

      return stream;
   }
}
