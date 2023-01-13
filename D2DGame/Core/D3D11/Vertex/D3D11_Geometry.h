#pragma once
#include "stdafx.h"

template <class V, class I>
class D3D11_Geometry
{
public:
    D3D11_Geometry() = default;
    virtual ~D3D11_Geometry();   

    /*
        Vertex
    */
    auto GetVertexCount()       const { return static_cast<uint>(vertices.size()); }
    auto GetVertexByteWidth()   const { return static_cast<uint>(sizeof(V) * GetVertexCount()); }
    auto GetVertexPointer()     const { return vertices.data(); }
    auto GetVertices()          const -> const std::vector<V>& { return vertices; }
    auto GetVertices(const uint& offset, const uint& count) -> const std::vector<V>;
    void AddVertex(const V& vertex);
    void AddVertices(const std::vector<V>& vertices);
    void SetVertices(const std::vector<V>& vertices);

    /*
        Index
    */
    auto GetIndexCount()        const { return static_cast<uint>(indices.size()); }
    auto GetIndexByteWidth()    const { return static_cast<uint>(sizeof(I) * GetIndexCount()); }
    auto GetIndexPointer()      const { return indices.data(); }
    auto GetIndices()           const -> const std::vector<I>& { return indices; }
    auto GetIndices(const uint& offset, const uint& count) -> const std::vector<I>;
    auto AddIndex(const I& index);
    void AddIndices(const std::vector<I>& indices);
    void SetIndices(const std::vector<I>& indices);

    /*
        MISC - Miscellaneous
    */
    void Clear();

protected:
    std::vector<V> vertices;
    std::vector<I> indices;
};
#include "D3D11_Geometry.inl"