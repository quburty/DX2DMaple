#include "stdafx.h"
#include "Geometry_Generator.h"

void Geometry_Generator::CreateQuad(D3D11_Geometry<struct D3D11_VertexTexture, uint>& geometry)
{
    //vertices
    geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXVECTOR2(0.0f, 1.0f)));
    geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(-0.5f, +0.5f, 0.0f), D3DXVECTOR2(0.0f, 0.0f)));
    geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(+0.5f, -0.5f, 0.0f), D3DXVECTOR2(1.0f, 1.0f)));
    geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(+0.5f, +0.5f, 0.0f), D3DXVECTOR2(1.0f, 0.0f)));
    
    //indices
    geometry.AddIndex(0); geometry.AddIndex(1); geometry.AddIndex(2);
    geometry.AddIndex(2); geometry.AddIndex(1); geometry.AddIndex(3);
}

void Geometry_Generator::CreateScreenQuad(D3D11_Geometry<struct D3D11_VertexTexture, uint>& geometry, const uint & width, const uint & height)
{
    const auto half_width   = width * 0.5f;
    const auto half_height  = height * 0.5f;

    //vertices
    geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(-half_width, -half_height, 0.0f), D3DXVECTOR2(0.0f, 1.0f)));
    geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(-half_width, +half_height, 0.0f), D3DXVECTOR2(0.0f, 0.0f)));
    geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(+half_width, -half_height, 0.0f), D3DXVECTOR2(1.0f, 1.0f)));
    geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(+half_width, +half_height, 0.0f), D3DXVECTOR2(1.0f, 0.0f)));

    //indices
    geometry.AddIndex(0); geometry.AddIndex(1); geometry.AddIndex(2);
    geometry.AddIndex(2); geometry.AddIndex(1); geometry.AddIndex(3);
}
