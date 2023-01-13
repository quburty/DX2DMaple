#include "stdafx.h"
#include "D3D11_ConstantBuffer.h"

D3D11_ConstantBuffer::D3D11_ConstantBuffer(Context * const context)
{
    device          = context->GetSubsystem<Graphics>()->GetDevice();
    device_context  = context->GetSubsystem<Graphics>()->GetDeviceContext();
}

D3D11_ConstantBuffer::~D3D11_ConstantBuffer()
{
    Clear();
}

void D3D11_ConstantBuffer::Clear()
{
    SAFE_RELEASE(buffer);
}

bool D3D11_ConstantBuffer::Unmap()
{
    if (!buffer)
        return false;

    device_context->Unmap(buffer, 0);
    return true;
}
