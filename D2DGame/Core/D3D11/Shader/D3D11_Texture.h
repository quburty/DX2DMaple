#pragma once
#include "Resource/IResource.h"

enum ResourceViewFlags : uint
{
	ResourceView_SRV = 1U << 0,
	ResourceView_RTV = 1U << 1,
};

class D3D11_Texture: public Object, public IResource
{
public:
	static uint GetTextureFlags(const uint& resource_view_flags);

public:
	using mip_level = std::vector<std::byte>;

public:
    D3D11_Texture(class Context* const context, const ResourceType& resource_type);
    virtual ~D3D11_Texture();

	bool SaveToFile(const std::string& path) override;
	bool LoadFromFile(const std::string& path) override;

    auto GetResource_SRV() const -> ID3D11ShaderResourceView* const { return shader_resource_view; }
	auto GetResource_RTV(const uint& idx = 0) const -> ID3D11RenderTargetView* const { return idx < render_target_views.size() ? render_target_views[idx] : nullptr; }

	auto GetViewport() const -> const D3D11_VIEWPORT& { return viewport; }
	auto GetArraySize() const -> const uint& { return array_size; }

	auto GetFormat() const -> const DXGI_FORMAT& { return format; }
	void SetFormat(const DXGI_FORMAT& format) { this->format = format; }

	auto GetBPP() const -> const uint& { return bpp; }
	void SetBPP(const uint& bpp) { this->bpp = bpp; }

	auto GetBPC() const -> const uint& { return bpc; }
	void SetBPC(const uint& bpc) { this->bpc = bpc; }

	auto GetWidth() const -> const uint& { return width; }
	void SetWidth(const uint& width) { this->width = width; }

	auto GetHeight() const -> const uint& { return height; }
	void SetHeight(const uint& height) { this->height = height; }

	auto GetChannels() const -> const uint& { return channels; }
	void SetChannels(const uint& channels) { this->channels = channels; }

	auto IsGrayscale() const -> const bool { return is_gray_scale; }
	void SetGrayscale(const bool& is_gray_scale) { this->is_gray_scale = is_gray_scale; }

	auto IsTransparent() const -> const bool { return is_transparent; }
	void SetTransparent(const bool& is_transparent) { this->is_transparent = is_transparent; }

	auto IsGenerateMipChain() const -> const bool { return is_generate_mip_map; }
	void SetGenerateMipChain(const bool& is_generate_mip_map) { this->is_generate_mip_map = is_generate_mip_map; }
	
	/*Mipmap*/
	void GetMipChain(std::vector<mip_level>* mip_chain);
	void SetMipChain(const std::vector<mip_level>& mip_chain) { this->mip_chain = mip_chain; }
	void ClearMipChain();

	auto GetMipLevel(const uint& level) -> const mip_level;
	auto GetMipLevel_ptr(const uint& level)->mip_level* const;
	auto AddMipLevel() -> mip_level* { return &mip_chain.emplace_back(mip_level()); }

	auto HasMipChain() -> const bool { return !mip_chain.empty(); }

	/*MISC*/
	bool IsSRV() const { return resource_view_flags & ResourceView_SRV; }
	bool IsRTV() const { return resource_view_flags & ResourceView_RTV; }

protected:
	virtual bool CreateGPUResource() = 0;

protected:
	bool Load_Foreign(const std::string& path, const bool& is_generate_mip_map);
	bool Load_Native(const std::string& path);

	uint GetByteCountFromMipChain();
	auto GetChannelsFromFormat(const DXGI_FORMAT& format) -> const uint;

protected:
    ID3D11Device* device                = nullptr;
    ID3D11ShaderResourceView* shader_resource_view  = nullptr;
	std::vector<ID3D11RenderTargetView*> render_target_views;

	D3D11_VIEWPORT viewport;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uint resource_view_flags = 0;
	uint bpp = 32;
	uint bpc = 8;
	uint channels = 4;
	uint width = 0;
	uint height = 0;
	uint array_size = 1;
	uint mip_levels = 1;
	bool is_gray_scale = false;
	bool is_transparent = false;
	bool is_generate_mip_map = false;

	std::vector<mip_level> mip_chain;

	D3DXVECTOR2 uv_start = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 uv_end = D3DXVECTOR2(1.0f, 1.0f);
};