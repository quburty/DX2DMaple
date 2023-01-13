#pragma once
#include "stdafx.h"
#include "ISubsystem.h"

#pragma region CBUFFER

struct TransformBuffer	//slot 1
{
	D3DXMATRIX world;
};

struct CameraBuffer		//slot 2
{
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

struct AnimationBuffer	//slot 3
{
	D3DXVECTOR2 sprite_texture_size;
	D3DXVECTOR2 offset;
	D3DXVECTOR2 size;
	float time;
	float alpha;
};

#pragma endregion

enum class ShaderType : uint
{
	Texture,
	Default,
	Fade,
};

class Renderer final : public ISubsystem
{
public:
	Renderer(class Context* const context);
	~Renderer() = default;

	void Update(const float& delta_time_sec) override;

	bool Initialize() override;

private:
	void RenderTagActors(const std::string& tag);

	void RenderActor(const std::shared_ptr<class Actor>& actor);

private:
	void DrawActorSize(const std::shared_ptr<class Actor>& actor);
	void DrawTextureSize(const std::shared_ptr<class Actor>& actor);
	
	void DrawActorSizeAni(const std::shared_ptr<class Actor>& actor);
	void DrawTextureSizeAni(const std::shared_ptr<class Actor>& actor);

	void DrawNumberFont(const D3DXVECTOR2& position, const uint & value,
		const std::string& texture_dir = "../_assets/Textures/Font/font_default.png",
		const ShaderType& shader_type = ShaderType::Default);

private:
	void UpdateCPUCameraBuffer();
	void UpdateGPUCameraBuffer(const bool& is_static = false);
	void DrawTexture2D(const std::shared_ptr<class D3D11_Texture>& texture2d, const D3DXVECTOR2& position, const D3DXVECTOR2& size, const bool& is_dir_right = false);

private:
	std::shared_ptr<struct D3D11_PipelineState>		pipeline_state;
	std::shared_ptr<class D3D11_Pipeline>			pipeline;
	std::shared_ptr<class D3D11_VertexBuffer>		vertex_buffer;
	std::shared_ptr<class D3D11_IndexBuffer>		index_buffer;
	std::shared_ptr<class D3D11_RasterizerState>	rasterizer_state;
	std::shared_ptr<class D3D11_SamplerState>		sampler_state;
	std::shared_ptr<class D3D11_BlendState>			blend_state;

	std::map<ShaderType, std::shared_ptr<class D3D11_Shader>> vertex_shaders;
	std::map<ShaderType, std::shared_ptr<class D3D11_Shader>> pixel_shaders;

	D3DXMATRIX identity_view_matrix;
	TransformBuffer	cpu_transform_buffer;
	CameraBuffer	cpu_camera_buffer;
	CameraBuffer	cpu_static_camera_buffer;
	AnimationBuffer	cpu_animation_buffer;

	std::shared_ptr<class D3D11_ConstantBuffer> gpu_transform_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_camera_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_animation_buffer;

	std::vector<std::string> actor_render_order;
	std::map<std::string, bool> is_rendered;
};