#pragma once
#include "IComponent.h"

enum class AnimationMode : uint
{
    Play,
    Stop,
    Pause,
};

class AnimatorComponent final : public IComponent
{
public:
    AnimatorComponent
    (
        class Context* const context,
        class Actor* const actor
    );
    ~AnimatorComponent() = default;

	auto DeepCopy(Actor* const target) const -> const std::shared_ptr<IComponent> override;

    void OnStart() override;
    void OnUpdate(const float& delta_time_sec) override;
    void OnEnd() override;

    auto GetAnimationMode() const -> const AnimationMode& { return animation_mode; }
    void SetAnimationMode(const AnimationMode& mode) { this->animation_mode = mode; }

    auto GetCurrentFrameNumber() const -> const uint& { return current_frame_number; }
    void SetCurrentFrameNumber(const uint& number) { this->current_frame_number = number; }

    auto GetFrameCounter() const -> const float& { return frame_counter; }
    void SetFrameCounter(const float& count) { this->frame_counter = count; }

	auto GetAnimation(const std::string& name) -> const std::shared_ptr<class Animation>& { return animations[name]; }

    auto GetCurrentAnimation() const -> const std::shared_ptr<class Animation>;
    void SetCurrentAnimation(const std::string& animation_name);

    auto GetAnimations() const -> const  std::map<std::string, std::shared_ptr<class Animation>>& { return animations; }
    auto GetCurrentKeyframe() const -> const Keyframe* const;

    void AddAnimation(const std::string& animation_name, const std::shared_ptr<class Animation>& animation);
    void AddAnimation(const std::string& path);

    void Play();
    void Stop();
    void Pause();

    bool IsPlaying() const { return animation_mode == AnimationMode::Play; }

	auto GetCurrentAnimationName() const -> const std::string& { return current_animation_name; }

	auto GetShaderType() const -> const ShaderType& { return shader_type; }
	void SetShaderType(const ShaderType& shader_type);

	auto GetTimeBufferCount() const -> const float& { return time_buffer_count; }

	bool IsAnimationEnd() const { return is_animation_end; }

	bool IsUsingActorTexture() const { return use_actor_texture; }
	void SetUsingActorTexture(const bool& use) { use_actor_texture = use; }

private:
    AnimationMode animation_mode    = AnimationMode::Play;
    uint current_frame_number       = 0;
    float frame_counter             = 0.0f;

	std::string current_animation_name = "None";
    std::weak_ptr<class Animation> current_animation;
    std::map<std::string, std::shared_ptr<class Animation>> animations;

	ShaderType shader_type = ShaderType::Default;
	float time_buffer_count = 0.0f;
	bool is_animation_end = false;
	bool use_actor_texture = false;
};