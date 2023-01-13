#pragma once

template <typename V, typename U> class D3D11_Geometry;

class Geometry_Generator final
{
public:
    static void CreateQuad(D3D11_Geometry<struct D3D11_VertexTexture, uint>& geometry);
    static void CreateScreenQuad(D3D11_Geometry<struct D3D11_VertexTexture, uint>& geometry, const uint& width, const uint& height);
};