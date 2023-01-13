#include "stdafx.h"
#include "D3D11_Texture.h"
#include "Resource/Importer/TextureImporter.h"

uint D3D11_Texture::GetTextureFlags(const uint & resource_view_flags)
{
	uint final_d3d11_flags = 0U;

	final_d3d11_flags |= (resource_view_flags & ResourceView_RTV) ? D3D11_BIND_RENDER_TARGET : 0U;
	//final_d3d11_flags |= (resource_view_flags & ResourceView_DSV) ? D3D11_BIND_DEPTH_STENCIL : 0U;
	final_d3d11_flags |= (resource_view_flags & ResourceView_SRV) ? D3D11_BIND_SHADER_RESOURCE : 0U;
	//final_d3d11_flags |= (resource_view_flags & ResourceView_UAV) ? D3D11_BIND_UNORDERED_ACCESS : 0U;

	return final_d3d11_flags;
}

D3D11_Texture::D3D11_Texture(Context* const context, const ResourceType& resource_type)
	:IResource(context, resource_type)
{
	device = context->GetSubsystem<Graphics>()->GetDevice();
}

D3D11_Texture::~D3D11_Texture()
{
	SAFE_RELEASE(shader_resource_view);

	for (auto render_target_view : render_target_views)
		SAFE_RELEASE(render_target_view);
}

bool D3D11_Texture::SaveToFile(const std::string & path)
{
	uint byte_count = 0;

	if (FileSystem::IsExists(path))
	{
		auto stream = std::make_unique<FileStream>(path, FILE_STREAM_READ);
		if (stream->IsOpen())
			stream->Read(byte_count);
	}

	auto stream = std::make_unique<FileStream>(path, FILE_STREAM_WRITE | FILE_STREAM_APPEND);
	if (!stream->IsOpen())
		return false;

	if (byte_count != 0 && mip_chain.empty())
		stream->Skip(sizeof(uint) + sizeof(uint) + byte_count);
	else
	{
		byte_count = GetByteCountFromMipChain();

		stream->Write(byte_count);
		stream->Write(static_cast<uint>(mip_chain.size()));

		for (const auto& mip : mip_chain)
			stream->Write(mip);

		ClearMipChain();
	}

	stream->Write(bpp);
	stream->Write(bpc);
	stream->Write(width);
	stream->Write(height);
	stream->Write(channels);
	stream->Write(is_gray_scale);
	stream->Write(is_transparent);
	stream->Write(resource_name);
	stream->Write(resource_path_foreign);
	stream->Write(resource_path_native);

	return true;
}

bool D3D11_Texture::LoadFromFile(const std::string & path)
{
	auto relative_path = FileSystem::GetRelativeFromPath(path);
	if (!FileSystem::IsExists(relative_path))
	{
		LOG_ERROR_F("Path \"%s\" is inavlid", relative_path.c_str());
		return false;
	}

	ClearMipChain();

	bool is_loading = false;
	if (FileSystem::IsNativeFile(relative_path))
		is_loading = Load_Native(relative_path);
	else if (FileSystem::IsSupportedTextureFile(relative_path))
		is_loading = Load_Foreign(relative_path, is_generate_mip_map);

	if (!is_loading)
	{
		LOG_ERROR_F("Failed to load \"%s\"", relative_path.c_str());
		return false;
	}

	if (width == 0 || height == 0 || channels == 0||mip_chain.empty()||mip_chain.front().empty())
	{
		LOG_ERROR_F("Invalid parameter, \"%s\"", relative_path.c_str());
		return false;
	}

	if (!CreateGPUResource())
	{
		LOG_ERROR_F("Failed to create gpu resource for \"%s\"", relative_path.c_str());
		return false;
	}

	if (FileSystem::IsNativeFile(relative_path))
		ClearMipChain();

	return true;
}

void D3D11_Texture::GetMipChain(std::vector<mip_level>* mip_chain)
{
	if (!this->mip_chain.empty())
	{
		mip_chain = &this->mip_chain;
		return;
	}

	auto stream = std::make_unique<FileStream>(resource_path_native, FILE_STREAM_READ);
	if (!stream->IsOpen())
		return;

	stream->Skip(sizeof(uint));

	auto mip_count = stream->Read<uint>();
	mip_chain->reserve(mip_count);
	mip_chain->resize(mip_count);

	for (auto& mip : *mip_chain)
		stream->Read(mip);
}

void D3D11_Texture::ClearMipChain()
{
	for (auto& mip : mip_chain)
	{
		mip.clear();
		mip.shrink_to_fit();
	}

	mip_chain.clear();
	mip_chain.shrink_to_fit();
}

auto D3D11_Texture::GetMipLevel(const uint & level) -> const mip_level
{
	mip_level data;

	if (level < static_cast<uint>(mip_chain.size()))
		data = mip_chain[level];
	else
	{
		auto stream = std::make_unique<FileStream>(resource_path_native, FILE_STREAM_READ);
		if (stream->IsOpen())
		{
			stream->Skip(sizeof(uint));

			uint mip_count = stream->Read<uint>();
			if (level < mip_count)
			{
				for (uint i = 0; i <= level; i++)
					stream->Read(data);
			}
			else
				LOG_ERROR("Invalid level, out of range");

			stream->Close();
		}
		else
			LOG_ERROR("Unable to retreive data");
	}

	return data;
}

auto D3D11_Texture::GetMipLevel_ptr(const uint & level) -> mip_level * const
{
	if (level >= mip_chain.size())
	{
		LOG_ERROR("Invalid level, out of range");
		return nullptr;
	}

	return &mip_chain[level];
}

bool D3D11_Texture::Load_Foreign(const std::string & path, const bool & is_generate_mip_map)
{
	//외부 이미지 파일

	auto importer = context->GetSubsystem<ResourceManager>()->GetTextureImporter();
	if (!importer->Load(path, this))
	{
		LOG_ERROR("Failed loading texture");
		return false;
	}

	SetResourcePath(path);

	return true;
}

bool D3D11_Texture::Load_Native(const std::string & path)
{
	//이진 텍스쳐 파일

	auto stream = std::make_unique<FileStream>(path, FILE_STREAM_READ);
	if (!stream->IsOpen())
		return false;

	ClearMipChain();

	stream->Skip(sizeof(uint));
	
	auto mip_count = stream->Read<uint>();
	
	mip_chain.reserve(mip_count);
	mip_chain.resize(mip_count);

	for (auto& mip : mip_chain)
		stream->Read(mip);

	stream->Read(bpp);
	stream->Read(bpc);
	stream->Read(width);
	stream->Read(height);
	stream->Read(channels);
	stream->Read(is_gray_scale);
	stream->Read(is_transparent);
	stream->Read(resource_name);
	stream->Read(resource_path_foreign);
	stream->Read(resource_path_native);

	return true;
}

uint D3D11_Texture::GetByteCountFromMipChain()
{
	uint byte_count = 0;

	for (const auto& mip : mip_chain)
		byte_count += static_cast<uint>(mip.size());

	return byte_count;
}

auto D3D11_Texture::GetChannelsFromFormat(const DXGI_FORMAT & format) -> const uint
{
	return 0;
}
