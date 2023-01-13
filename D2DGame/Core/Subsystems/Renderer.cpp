#include "stdafx.h"
#include "Renderer.h"
#include "Scene/stdafx_Scene.h"

Renderer::Renderer(Context * const context)
	:ISubsystem(context)
{
	actor_render_order.clear();

	actor_render_order.emplace_back(typeid(BackgroundActor).name());
	actor_render_order.emplace_back(typeid(PlatformActor).name());
	actor_render_order.emplace_back(typeid(MobActor).name());
	actor_render_order.emplace_back(typeid(PortalActor).name());
	actor_render_order.emplace_back(typeid(SkillActor).name());
	actor_render_order.emplace_back(typeid(EffectActor).name());
	
	actor_render_order.emplace_back(typeid(PlayerActor).name());
	actor_render_order.emplace_back(typeid(ItemDropActor).name());
	actor_render_order.emplace_back(typeid(DamageFontActor).name());


	//UI
	actor_render_order.emplace_back(typeid(BarActor).name());
	actor_render_order.emplace_back(typeid(StatusUIActor).name());
	actor_render_order.emplace_back(typeid(QuickSlotUIActor).name());
	actor_render_order.emplace_back(typeid(Actor).name());

}

void Renderer::Update(const float & delta_time_sec)
{
	is_rendered.clear();
	auto actors = context->GetSubsystem<Scene>()->GetActors();
	UpdateCPUCameraBuffer();

	for (const auto tag : actor_render_order)
	{
		for (const auto actor : actors[tag])
		{
			RenderActor(actor);
		}
		is_rendered[tag] = true;
	}

	for (const auto& actor_tags : actors)
	{
		if (is_rendered[actor_tags.first]==true)
			continue;

		for (const auto actor : actor_tags.second)
		{
			RenderActor(actor);
		}
	}
}

bool Renderer::Initialize()
{
	pipeline = std::make_shared<D3D11_Pipeline>(context);

	//Geometry
	D3D11_Geometry<D3D11_VertexTexture, uint> geometry;
	Geometry_Generator::CreateQuad(geometry);
	
	vertex_buffer = std::make_shared<D3D11_VertexBuffer>(context);
	vertex_buffer->Create(geometry.GetVertices());

	index_buffer = std::make_shared<D3D11_IndexBuffer>(context);
	index_buffer->Create(geometry.GetIndices());

	//Sampler State
	{
		sampler_state = std::make_shared<D3D11_SamplerState>(context);
		sampler_state->Create
		(
			D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_CLAMP
		);
	}

	//Blend State
	{
		blend_state = std::make_shared<D3D11_BlendState>(context);
		blend_state->Create(true);
	}

	//Rasterizer State
	{
		rasterizer_state = std::make_shared<D3D11_RasterizerState>(context);
		rasterizer_state->Create(D3D11_CULL_NONE, D3D11_FILL_SOLID);
	}

	//Shader
	pipeline_state = std::make_shared<D3D11_PipelineState>();

	vertex_shaders[ShaderType::Texture] = std::make_shared<D3D11_Shader>(context);
	vertex_shaders[ShaderType::Texture]->Create(ShaderScope_VS, "../_assets/Shaders/Texture.hlsl");

	vertex_shaders[ShaderType::Default] = std::make_shared<D3D11_Shader>(context);
	vertex_shaders[ShaderType::Default]->Create(ShaderScope_VS, "../_assets/Shaders/Animation.hlsl");

	vertex_shaders[ShaderType::Fade] = std::make_shared<D3D11_Shader>(context);
	vertex_shaders[ShaderType::Fade]->Create(ShaderScope_VS, "../_assets/Shaders/Fading.hlsl");

	pixel_shaders[ShaderType::Texture] = std::make_shared<D3D11_Shader>(context);
	pixel_shaders[ShaderType::Texture]->Create(ShaderScope_PS, "../_assets/Shaders/Texture.hlsl");

	pixel_shaders[ShaderType::Default] = std::make_shared<D3D11_Shader>(context);
	pixel_shaders[ShaderType::Default]->Create(ShaderScope_PS, "../_assets/Shaders/Animation.hlsl");

	pixel_shaders[ShaderType::Fade] = std::make_shared<D3D11_Shader>(context);
	pixel_shaders[ShaderType::Fade]->Create(ShaderScope_PS, "../_assets/Shaders/Fading.hlsl");

	pipeline_state->vertex_shader = vertex_shaders[ShaderType::Texture].get();
	pipeline_state->pixel_shader = pixel_shaders[ShaderType::Texture].get();

	//Pipeline State
	{
		pipeline_state->blend_state = blend_state.get();
		pipeline_state->primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pipeline_state->rasterizer_state = rasterizer_state.get();
	}

	//Constant Buffer
	{
		D3DXMatrixIdentity(&cpu_transform_buffer.world);
		D3DXMatrixIdentity(&cpu_camera_buffer.view);
		D3DXMatrixIdentity(&cpu_camera_buffer.projection);

		D3DXMatrixOrthoLH
		(
			&cpu_camera_buffer.projection,
			Settings::Get().GetWidth(),
			Settings::Get().GetHeight(),
			0.0f,
			1.0f
		);

		D3DXMatrixLookAtLH
		(
			&cpu_camera_buffer.view,
			&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			&D3DXVECTOR3(0.0f, 0.0f, 1.0f),
			&D3DXVECTOR3(0.0f, 1.0f, 0.0f)
		);

		identity_view_matrix = cpu_camera_buffer.view;
		cpu_static_camera_buffer = cpu_camera_buffer;
		D3DXMatrixTranspose(&cpu_camera_buffer.view, &cpu_camera_buffer.view);
		D3DXMatrixTranspose(&cpu_camera_buffer.projection, &cpu_camera_buffer.projection);

		gpu_transform_buffer = std::make_shared<D3D11_ConstantBuffer>(context);
		gpu_transform_buffer->Create<TransformBuffer>();

		gpu_camera_buffer = std::make_shared<D3D11_ConstantBuffer>(context);
		gpu_camera_buffer->Create<CameraBuffer>();
		auto map_ptr = gpu_camera_buffer->Map<CameraBuffer>();
		*map_ptr = cpu_camera_buffer;
		gpu_camera_buffer->Unmap();

		gpu_animation_buffer = std::make_shared<D3D11_ConstantBuffer>(context);
		gpu_animation_buffer->Create<AnimationBuffer>();
	}

	//Default Values
	pipeline->SetVertexBuffer(vertex_buffer.get());
	pipeline->SetIndexBuffer(index_buffer.get());
	pipeline->SetSamplerState(0, ShaderScope_PS, sampler_state.get());
	pipeline->SetConstantBuffer(0, ShaderScope_VS, gpu_transform_buffer.get());
	pipeline->SetConstantBuffer(1, ShaderScope_VS, gpu_camera_buffer.get());
	pipeline->SetConstantBuffer(2, ShaderScope_VS | ShaderScope_PS, gpu_animation_buffer.get());


	return true;
}

void Renderer::RenderTagActors(const std::string & tag)
{
}

void Renderer::RenderActor(const std::shared_ptr<class Actor>& actor)
{
	if (!actor->IsActive())
		return;

	if (actor->IsUIActor() /*|| actor->GetComponent<UIComponent>()*/)
		UpdateGPUCameraBuffer(true);
	else
		UpdateGPUCameraBuffer(false);

	if (actor->GetComponent<AnimatorComponent>())
		DrawTextureSizeAni(actor);
	else if (auto font = actor->GetComponent<NumberFontComponent>())
	{
		DrawNumberFont(actor->GetPosition(), font->GetValue(), font->GetFontTextureDir(), font->GetFontShaderType());
	}
	else
	{
		if (actor->GetTexture())
		{
			if (actor->IsRenderTextureSize())
				DrawTextureSize(actor);
			else
				DrawActorSize(actor);
		}
	}
}

void Renderer::DrawActorSize(const std::shared_ptr<class Actor>& actor)
{
	pipeline_state->vertex_shader = vertex_shaders[ShaderType::Texture].get();
	pipeline_state->pixel_shader = pixel_shaders[ShaderType::Texture].get();
	DrawTexture2D(actor->GetTexture(), actor->GetCenterPosition(), actor->GetScale(), actor->IsDirectionRight());
}

void Renderer::DrawTextureSize(const std::shared_ptr<class Actor>& actor)
{
	pipeline_state->vertex_shader = vertex_shaders[ShaderType::Texture].get();
	pipeline_state->pixel_shader = pixel_shaders[ShaderType::Texture].get();
	DrawTexture2D(actor->GetTexture(),
		actor->GetCenterPosition(),
		D3DXVECTOR2(static_cast<float>(actor->GetTexture()->GetWidth()), static_cast<float>(actor->GetTexture()->GetHeight())), actor->IsDirectionRight());
}

void Renderer::DrawActorSizeAni(const std::shared_ptr<class Actor>& actor)
{
	//pipeline_state->pixel_shader = pixel_shader_animation.get();
	LOG_ERROR("DrawActorSizeAni() doesn't work now");
}

void Renderer::DrawTextureSizeAni(const std::shared_ptr<class Actor>& actor)
{
	if (auto animator = actor->GetComponent<AnimatorComponent>())
	{
		pipeline_state->vertex_shader = vertex_shaders[animator->GetShaderType()].get();
		pipeline_state->pixel_shader = pixel_shaders[animator->GetShaderType()].get();

		auto frame = animator->GetCurrentKeyframe();

		cpu_animation_buffer.offset = frame->offset;
		cpu_animation_buffer.size = frame->size;
		cpu_animation_buffer.sprite_texture_size = animator->GetCurrentAnimation()->GetSpriteTextureSize();
		cpu_animation_buffer.time = animator->GetTimeBufferCount();

		auto ptr = gpu_animation_buffer->Map<AnimationBuffer>();
		*ptr = cpu_animation_buffer;
		gpu_animation_buffer->Unmap();

		if (animator->IsUsingActorTexture())
		{
			DrawTexture2D(actor->GetTexture(), actor->GetPosition() + animator->GetCurrentAnimation()->GetDeltaPosition(), D3DXVECTOR2(1.0f, 1.0f), actor->IsDirectionRight());
		}
		else
		{
			auto render_pos = actor->GetPosition();
			render_pos.y += animator->GetCurrentAnimation()->GetDeltaPosition().y;

			if (actor->IsDirectionRight())
				render_pos.x += animator->GetCurrentAnimation()->GetDeltaPosition().x;
			else
				render_pos.x -= animator->GetCurrentAnimation()->GetDeltaPosition().x;

			DrawTexture2D(animator->GetCurrentAnimation()->GetSpriteTexture(), 
				render_pos, 
				D3DXVECTOR2(1.0f, 1.0f), actor->IsDirectionRight());
		}
	}
	else
		LOG_ERROR("actor doesn't have animator component");
}

void Renderer::DrawNumberFont(const D3DXVECTOR2& position, const uint & value, const std::string& texture_dir, const ShaderType& shader_type)
{
	pipeline_state->vertex_shader = vertex_shaders[shader_type].get();
	pipeline_state->pixel_shader = pixel_shaders[shader_type].get();

	auto font_texture = context->GetSubsystem<ResourceManager>()
		->Load<Texture2D>(texture_dir);

	float width = static_cast<float>(font_texture->GetWidth());
	float height = static_cast<float>(font_texture->GetHeight());

	uint length = static_cast<uint>(floorf(log10f(static_cast<float>(value)))) + 1;
	
	for (float i = 0; i < length; i++)
	{
		uint num = static_cast<uint>(value) / static_cast<uint>(pow(10, i)) % 10;

		cpu_animation_buffer.offset = D3DXVECTOR2(width / 10.0f*num, 0.0f);
		cpu_animation_buffer.size = D3DXVECTOR2(width / 10.0f, height);
		cpu_animation_buffer.sprite_texture_size = D3DXVECTOR2(width, height);

		auto ptr = gpu_animation_buffer->Map<AnimationBuffer>();
		*ptr = cpu_animation_buffer;
		gpu_animation_buffer->Unmap();

		DrawTexture2D(font_texture,
			position + D3DXVECTOR2(width / 10.0f*(length - i - 1.0f) - width / 10.0f*length / 2, 0),
			D3DXVECTOR2(1.0f, 1.0f));
	}
}

void Renderer::UpdateCPUCameraBuffer()
{
	auto camera_pos = context->GetSubsystem<Scene>()->GetSceneCameraPos();

	D3DXMatrixLookAtLH
	(
		&cpu_camera_buffer.view,
		&D3DXVECTOR3(0.0f+camera_pos.x, 0.0f+camera_pos.y, 0.0f),
		&D3DXVECTOR3(0.0f+camera_pos.x, 0.0f+camera_pos.y, 1.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f)
	);

	D3DXMatrixTranspose(&cpu_camera_buffer.view, &cpu_camera_buffer.view);
}

void Renderer::UpdateGPUCameraBuffer(const bool & is_static)
{
	if (is_static)
	{
		auto ptr = gpu_camera_buffer->Map<CameraBuffer>();
		*ptr = cpu_static_camera_buffer;
		gpu_camera_buffer->Unmap();
	}
	else
	{
		auto ptr = gpu_camera_buffer->Map<CameraBuffer>();
		*ptr = cpu_camera_buffer;
		gpu_camera_buffer->Unmap();
	}
}

void Renderer::DrawTexture2D(const std::shared_ptr<class D3D11_Texture>& texture2d, const D3DXVECTOR2 & position, const D3DXVECTOR2 & size, const bool& is_dir_right)
{
	D3DXMATRIX world, s, t;

	D3DXMatrixIdentity(&s);
	D3DXMatrixIdentity(&t);

	if(is_dir_right)
		D3DXMatrixScaling(&s, -size.x, size.y, 1.0f);
	else
		D3DXMatrixScaling(&s, +size.x, size.y, 1.0f);

	D3DXMatrixTranslation(&t, position.x, position.y, 0.0f);

	world = s * t;

	D3DXMatrixTranspose(&cpu_transform_buffer.world, &world);

	auto ptr = gpu_transform_buffer->Map<TransformBuffer>();
	*ptr = cpu_transform_buffer;
	gpu_transform_buffer->Unmap();

	pipeline->SetShaderResource(0, ShaderScope_PS, texture2d.get());

	pipeline->Begin(*pipeline_state.get());
	pipeline->DrawIndexed(6);
	pipeline->End();
}


