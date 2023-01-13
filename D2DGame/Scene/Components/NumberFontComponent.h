#pragma once
#include "IComponent.h"

class NumberFontComponent final : public IComponent
{
public:
	NumberFontComponent
	(
		class Context* const context,
		class Actor* const actor
	);
	~NumberFontComponent() = default;

	auto DeepCopy(Actor* const target) const -> const std::shared_ptr<IComponent> override;

	void OnStart() override;
	void OnUpdate(const float& delta_time_sec) override;
	void OnEnd() override;

	auto GetFontTextureDir() const -> const std::string& { return font_texture_dir; }
	void SetFontTextureDir(const std::string& dir) { font_texture_dir = dir; }

	auto GetFontShaderType() const -> const ShaderType& { return shader_type; }
	void SetFontShaderType(const ShaderType& type) { shader_type = type; }

	auto GetValue() const -> const uint& { return value; }
	void SetValue(const uint& value) { this->value = value; }

private:
	std::string font_texture_dir = "../_assets/Textures/Font/font_default.png";
	ShaderType shader_type = ShaderType::Default;
	uint value = 0;
};