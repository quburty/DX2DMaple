#pragma once

class Texture2D final : public D3D11_Texture
{
public:
    static bool CreateTexture
    (
        ID3D11Device* const device,
        ID3D11Texture2D** resource,
        const uint& width,
        const uint& height,
        const uint& channels,
        const uint& bpc,
        const uint& array_size,
        const DXGI_FORMAT& format,
        const uint& flags,
        std::vector<std::vector<std::byte>>& data
    );

    static bool CreateRenderTargetView
    (
        ID3D11Device* const device,
        ID3D11Resource* const resource,
        std::vector<ID3D11RenderTargetView*>& render_target_views,
        const DXGI_FORMAT& format,
        const uint& array_size
    );

    static bool CreateShaderResourceView
    (
        ID3D11Device* device,
        ID3D11Resource* resource,
        ID3D11ShaderResourceView** shader_resource_view,
        const DXGI_FORMAT& format,
        const uint& array_size,
        std::vector<std::vector<std::byte>>& data
    );

public:
    /*SRV*/
    Texture2D(class Context* const context);
    
    /*RTV*/
    Texture2D
    (
        class Context* const context,
        const DXGI_FORMAT& format,
        const uint& width,
        const uint& height,
        const uint& array_size,
        const uint& flags
    );
    ~Texture2D() = default;

    bool CreateGPUResource() override;
};

//bpp - bit per pixel
//bpc - bit per color, channel, color depth