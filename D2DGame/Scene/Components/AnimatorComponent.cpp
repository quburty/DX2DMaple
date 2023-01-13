#include "stdafx.h"
#include "AnimatorComponent.h"
#include "../stdafx_Scene.h"

AnimatorComponent::AnimatorComponent(Context * const context, Actor * const actor)
	: IComponent(context, actor)
{
	component_name = typeid(*this).name();
}

auto AnimatorComponent::DeepCopy(Actor* const target) const -> const std::shared_ptr<IComponent>
{
	std::shared_ptr<IComponent> component = std::make_shared<AnimatorComponent>(context, target);
	auto animator = std::static_pointer_cast<AnimatorComponent>(component);

	animator->animation_mode = this->animation_mode;
	animator->animations.insert(this->animations.begin(), this->animations.end()); //?
	animator->SetCurrentAnimation(this->GetCurrentAnimationName());
	animator->SetShaderType(this->shader_type);
	animator->use_actor_texture = this->use_actor_texture;

	return component;
}

void AnimatorComponent::OnStart()
{
    Play();
}

void AnimatorComponent::OnUpdate(const float & delta_time_sec)
{
    if (current_animation.expired() || !IsPlaying())
        return;

    frame_counter += delta_time_sec * 1000.f;
	time_buffer_count += delta_time_sec;

    if (frame_counter > GetCurrentKeyframe()->time)
    {
        current_frame_number++;

        switch (current_animation.lock()->GetRepeatType())
        {
        case RepeatType::Once:
            if (current_frame_number >= current_animation.lock()->GetKeyframeCount())
            {
                current_frame_number = current_animation.lock()->GetKeyframeCount() - 1;
                Pause();
				is_animation_end = true;
            }
            break;

        case RepeatType::Loop:
            current_frame_number %= current_animation.lock()->GetKeyframeCount();
            break;
        }

        frame_counter = 0.f;
    }
}

void AnimatorComponent::OnEnd()
{
    Stop();
}

auto AnimatorComponent::GetCurrentAnimation() const -> const std::shared_ptr<class Animation>
{
    return current_animation.expired() ? nullptr : current_animation.lock();
}

void AnimatorComponent::SetCurrentAnimation(const std::string & animation_name)
{
    assert(animations.find(animation_name) != animations.end());

	if (current_animation_name == animation_name)
		return;

	current_animation_name = animation_name;
    current_animation       = animations[animation_name];
    current_frame_number    = 0;
    frame_counter           = 0.0f;
}

auto AnimatorComponent::GetCurrentKeyframe() const -> const Keyframe * const
{
    assert(!current_animation.expired());
    return current_animation.lock()->GetKeyframe(current_frame_number);
}

void AnimatorComponent::AddAnimation(const std::string & animation_name, const std::shared_ptr<class Animation>& animation)
{
    assert(animations.find(animation_name) == animations.end());
    animations[animation_name] = animation;

	if (current_animation.expired())
		SetCurrentAnimation(animation_name);
}

void AnimatorComponent::AddAnimation(const std::string & path)
{
    auto animation = context->GetSubsystem<ResourceManager>()->Load<Animation>(path);

    if (!animation)
    {
        assert(false);
        return;
    }
	
	std::string resource_name = FileSystem::GetFileNameFromPath(animation->GetResourceName());
    animations[resource_name] = animation;

	if (current_animation.expired())
		SetCurrentAnimation(resource_name);
}

void AnimatorComponent::Play()
{
    animation_mode  = AnimationMode::Play;
    frame_counter   = 0.0f;
}

void AnimatorComponent::Stop()
{
    animation_mode          = AnimationMode::Stop;
    current_frame_number    = 0;
    frame_counter           = 0.0f;
}

void AnimatorComponent::Pause()
{
    animation_mode = AnimationMode::Pause;
}

void AnimatorComponent::SetShaderType(const ShaderType & shader_type)
{
	if (this->shader_type != shader_type)
	{
		time_buffer_count = 0.0f;
		this->shader_type = shader_type;
	}
}