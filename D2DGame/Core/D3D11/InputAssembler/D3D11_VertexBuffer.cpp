#include "stdafx.h"
#include "D3D11_VertexBuffer.h"

D3D11_VertexBuffer::D3D11_VertexBuffer(Context * context)
{
    device = context->GetSubsystem<Graphics>()->GetDevice();
}

D3D11_VertexBuffer::~D3D11_VertexBuffer()
{
    Clear();
}

void D3D11_VertexBuffer::Clear()
{
    SAFE_RELEASE(buffer);
    stride  = 0;
    offset  = 0;
    count   = 0;
}
