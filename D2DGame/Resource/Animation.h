#pragma once
#include "IResource.h"

enum class RepeatType : uint
{
    Once, 
    Loop,
};

struct Keyframe final
{
    Keyframe() = default;
    Keyframe(const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double& time)
        : offset(offset)
        , size(size)
        , time(time)
    {}

    D3DXVECTOR2 offset  = D3DXVECTOR2(0.0f, 0.0f);
    D3DXVECTOR2 size    = D3DXVECTOR2(1.0f, 1.0f);
    double time         = 0.0;
};

class Animation final : public IResource
{
public:
    Animation(class Context* const context);
    ~Animation();

    bool SaveToFile(const std::string& path) override;
    bool LoadFromFile(const std::string& path) override;

    auto GetRepeatType() const -> const RepeatType& { return repeat_type; }
    void SetRepeatType(const RepeatType& type) { this->repeat_type = type; }

    auto GetSpriteTexture() const -> const std::shared_ptr<class D3D11_Texture>& { return sprite_texture; }
    void SetSpriteTexture(const std::shared_ptr<class D3D11_Texture>& texture) { 
		this->sprite_texture = texture; 
	}
    void SetSpriteTexture(const std::string& path);

    auto GetSpriteTextureSize() const -> const D3DXVECTOR2& { return sprite_texture_size; }
    void SetSpriteTextureSize(const D3DXVECTOR2& size) { this->sprite_texture_size = size; }

    auto GetKeyframes() const -> const std::vector<Keyframe>& { return keyframes; }
    void SetKeyframes(const std::vector<Keyframe>& keyframes) { this->keyframes = keyframes; }

    auto GetKeyframe(const uint& index) const -> const Keyframe* const;
    auto GetKeyframeCount() const -> const uint { return static_cast<uint>(keyframes.size()); }

    void AddKeyframe(const Keyframe& keyframe);
    void AddKeyframe(const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double& time);

	auto GetPivot() const -> const D3DXVECTOR2& { return pivot; }
	void SetPivot(const D3DXVECTOR2& pivot) { this->pivot = pivot; }

	auto GetDeltaPosition() const -> const D3DXVECTOR2 { return pivot - keyframes[0].size / 2; }

private:
    std::string sprite_texture_path        = "";

    RepeatType repeat_type  = RepeatType::Loop;
    
    std::shared_ptr<class D3D11_Texture> sprite_texture;
    D3DXVECTOR2 sprite_texture_size = D3DXVECTOR2(1.0f, 1.0f);
	D3DXVECTOR2 pivot = D3DXVECTOR2(0.0f, 0.0f);

    std::vector<Keyframe> keyframes;
};