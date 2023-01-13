#include "stdafx.h"
#include "Texture2D.h"
#include "Core/D3D11/D3D11_Helper.h"

bool Texture2D::CreateTexture(ID3D11Device * const device, ID3D11Texture2D ** resource, const uint & width, const uint & height, const uint & channels, const uint & bpc, const uint & array_size, const DXGI_FORMAT & format, const uint & flags, std::vector<std::vector<std::byte>>& data)
{
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
    desc.Width              = width;
    desc.Height             = height;
    desc.MipLevels          = data.empty() ? 1 : static_cast<uint>(data.size());
    desc.ArraySize          = array_size;
    desc.Format             = format;
    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage              = (flags & D3D11_BIND_RENDER_TARGET) ? D3D11_USAGE_DEFAULT : D3D11_USAGE_IMMUTABLE;
    desc.BindFlags          = flags;
    desc.MiscFlags          = 0;
    desc.CPUAccessFlags     = 0;

    std::vector<D3D11_SUBRESOURCE_DATA> subresource_datas;
    for (uint mip_level = 0; mip_level < data.size(); mip_level++)
    {
        if (data[mip_level].empty())
        {
            LOG_ERROR_F("Mipmap %d has invalid data", mip_level);
            return false;
        }

        auto& subresource_data              = subresource_datas.emplace_back(D3D11_SUBRESOURCE_DATA{});
        subresource_data.pSysMem            = data[mip_level].data();
        subresource_data.SysMemPitch        = (width >> mip_level) * channels * (bpc / 8);
        subresource_data.SysMemSlicePitch   = 0;
    }

    const auto result = device->CreateTexture2D(&desc, subresource_datas.data(), resource);
    if (FAILED(result))
    {
        LOG_ERROR_F("Invalid parameter, failed to create ID3D11Texture2D, %s", D3D11_Helper::DXGI_ERROR_TO_STRING(result));
        return false;
    }

    return true;
}

bool Texture2D::CreateRenderTargetView(ID3D11Device * const device, ID3D11Resource * const resource, std::vector<ID3D11RenderTargetView*>& render_target_views, const DXGI_FORMAT & format, const uint & array_size)
{
    D3D11_RENDER_TARGET_VIEW_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    desc.Format                         = format;
    desc.ViewDimension                  = (array_size == 1) ? D3D11_RTV_DIMENSION_TEXTURE2D : D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    desc.Texture2DArray.ArraySize       = array_size;
    desc.Texture2DArray.FirstArraySlice = 0;
    desc.Texture2DArray.MipSlice        = 0;

    for (uint i = 0; i < array_size; i++)
    {
        desc.Texture2DArray.FirstArraySlice = i;
        const auto result = device->CreateRenderTargetView(resource, &desc, &render_target_views.emplace_back(nullptr));
        if (FAILED(result))
        {
            LOG_ERROR_F("Invalid parameter, failed to create ID3D11RenderTargetView, %s", D3D11_Helper::DXGI_ERROR_TO_STRING(result));
            return false;
        }
    }

    return true;
}

bool Texture2D::CreateShaderResourceView(ID3D11Device * device, ID3D11Resource * resource, ID3D11ShaderResourceView ** shader_resource_view, const DXGI_FORMAT & format, const uint & array_size, std::vector<std::vector<std::byte>>& data)
{
    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    desc.Format                         = format;
    desc.ViewDimension                  = (array_size == 1) ? D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    desc.Texture2DArray.ArraySize       = array_size;
    desc.Texture2DArray.FirstArraySlice = 0;
    desc.Texture2DArray.MipLevels       = data.empty() ? 1 : static_cast<uint>(data.size());
    desc.Texture2DArray.MostDetailedMip = 0;

    const auto result = device->CreateShaderResourceView(resource, &desc, shader_resource_view);
    if (FAILED(result))
    {
        LOG_ERROR_F("Invalid parameter, failed to create ID3D11ShaderResourceView, %s", D3D11_Helper::DXGI_ERROR_TO_STRING(result));
        return false;
    }

    return true;
}

Texture2D::Texture2D(Context * const context)
    : D3D11_Texture(context, ResourceType::Texture2D)
{
    this->resource_view_flags = ResourceView_SRV;
}

Texture2D::Texture2D(Context * const context, const DXGI_FORMAT & format, const uint & width, const uint & height, const uint & array_size, const uint & flags)
    : D3D11_Texture(context, ResourceType::Texture2D)
{
    this->width                 = width;
    this->height                = height;
    this->format                = format;
    this->channels              = GetChannelsFromFormat(format);
    this->viewport              = D3D11_VIEWPORT{ 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f };
    this->array_size            = array_size;
    this->resource_view_flags   = flags;

    CreateGPUResource();
}

bool Texture2D::CreateGPUResource()
{
    auto d3d11_bind_flags   = GetTextureFlags(resource_view_flags);
    bool is_buffer_result   = true;
    bool is_srv_result      = true;
    bool is_rtv_result      = true;

    ID3D11Texture2D* resource = nullptr;
    is_buffer_result = CreateTexture
    (
        device,
        &resource,
        width,
        height,
        channels,
        bpc,
        array_size,
        format,
        d3d11_bind_flags,
        mip_chain
    );

    if (IsSRV())
    {
        is_srv_result = CreateShaderResourceView
        (
            device,
            resource,
            &shader_resource_view,
            format,
            array_size,
            mip_chain
        );
    }

    if (IsRTV())
    {
        is_rtv_result = CreateRenderTargetView
        (
            device,
            resource,
            render_target_views,
            format,
            array_size
        );
    }

    SAFE_RELEASE(resource);
    return is_buffer_result && is_srv_result && is_rtv_result;
}
