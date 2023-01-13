#include "stdafx.h"
#include "D3D11_IndexBuffer.h"

D3D11_IndexBuffer::D3D11_IndexBuffer(Context * const context)
{
	device = context->GetSubsystem<Graphics>()->GetDevice();
}

D3D11_IndexBuffer::~D3D11_IndexBuffer()
{
    Clear();
}

void D3D11_IndexBuffer::Clear()
{
    SAFE_RELEASE(buffer);
    stride  = 0;
    offset  = 0;
    count   = 0;
}
