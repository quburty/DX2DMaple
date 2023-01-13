#include "D3D11_Geometry.h"

template<class V, class I>
inline D3D11_Geometry<V, I>::~D3D11_Geometry()
{
}

template<class V, class I>
inline auto D3D11_Geometry<V, I>::GetVertices(const uint & offset, const uint & count) -> const std::vector<V>
{
    std::vector<V> sub_vertices;

    auto first  = vertices.begin() + offset;
    auto last   = first + count;

    sub_vertices.assign(first, last);

    return sub_vertices;
}

template<class V, class I>
inline void D3D11_Geometry<V, I>::AddVertex(const V & vertex)
{
    vertices.emplace_back(vertex);
}

template<class V, class I>
inline void D3D11_Geometry<V, I>::AddVertices(const std::vector<V>& vertices)
{
    this->vertices.insert
    (
        this->vertices.end(),
        vertices.begin(),
        vertices.end()
    );
}

template<class V, class I>
inline void D3D11_Geometry<V, I>::SetVertices(const std::vector<V>& vertices)
{
    this->vertices.clear();
    this->vertices.shrink_to_fit();
    this->vertices = vertices;
}

template<class V, class I>
inline auto D3D11_Geometry<V, I>::GetIndices(const uint & offset, const uint & count) -> const std::vector<I>
{
    std::vector<I> sub_indices;

    auto first  = indices.begin() + offset;
    auto last   = first + count;

    sub_indices.assign(first, last);

    return sub_indices;
}

template<class V, class I>
inline auto D3D11_Geometry<V, I>::AddIndex(const I & index)
{
    indices.emplace_back(index);
}

template<class V, class I>
inline void D3D11_Geometry<V, I>::AddIndices(const std::vector<I>& indices)
{
    this->indices.insert
    (
        this->indices.end(),
        indices.begin(),
        indices.end()
    );
}

template<class V, class I>
inline void D3D11_Geometry<V, I>::SetIndices(const std::vector<I>& indices)
{
    this->indices.clear();
    this->indices.shrink_to_fit();
    this->indices = indices;
}

template<class V, class I>
inline void D3D11_Geometry<V, I>::Clear()
{
    vertices.clear();
    vertices.shrink_to_fit();

    indices.clear();
    indices.shrink_to_fit();
}
