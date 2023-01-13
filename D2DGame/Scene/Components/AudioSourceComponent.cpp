#include "stdafx.h"
#include "AudioSourceComponent.h"
#include "../stdafx_Scene.h"

AudioSourceComponent::AudioSourceComponent(Context * const context, Actor * const actor)
	:IComponent(context,actor)
{
	component_name = typeid(*this).name();
}

auto AudioSourceComponent::DeepCopy(Actor * const target) const -> const std::shared_ptr<IComponent>
{
	std::shared_ptr<IComponent> component = std::make_shared<AudioSourceComponent>(context, target);
	auto audio = std::static_pointer_cast<AudioSourceComponent>(component);

	audio->audio_clip = this->audio_clip;
	audio->is_loop = this->is_loop;
	audio->is_mute = this->is_mute;
	audio->priority = this->priority;
	audio->volume = this->volume;
	audio->pitch = this->pitch;
	audio->pan = this->pan;
	audio->is_playing = this->is_playing;

	return component;
}

void AudioSourceComponent::OnStart()
{
    if (!is_playing)
        return;

    Play();
}

void AudioSourceComponent::OnUpdate(const float & delta_time_sec)
{
    if (!is_playing)
        return;

    if (!audio_clip)
        return;

    audio_clip->Update();
}

void AudioSourceComponent::OnEnd()
{
    Stop();
}

auto AudioSourceComponent::GetAudioClipName() const -> const std::string
{
    return audio_clip ? audio_clip->GetResourceName() : FileSystem::NOT_ASSIGNED_STR;
}

void AudioSourceComponent::SetAudioClip(const std::string & path)
{
    auto resource_manager = context->GetSubsystem<ResourceManager>();
    audio_clip = resource_manager->Load<AudioClip>(path);
}

auto AudioSourceComponent::Play() -> const bool
{
    if (!audio_clip)
        return false;

    audio_clip->SetLoop(is_loop);
    audio_clip->SetMute(is_mute);
    audio_clip->SetPriority(priority);
    audio_clip->SetVolume(volume);
    audio_clip->SetPitch(pitch);
    audio_clip->SetPan(pan);

    return audio_clip->Play();
}

auto AudioSourceComponent::Pause() -> const bool
{
    if (!audio_clip)
        return false;

    return audio_clip->Pause();
}

auto AudioSourceComponent::Stop() -> const bool
{
    if (!audio_clip)
        return false;

    return audio_clip->Stop();
}

void AudioSourceComponent::SetLoop(const bool & is_loop)
{
    if (!audio_clip || this->is_loop == is_loop)
        return;

    this->is_loop = is_loop;
    audio_clip->SetLoop(this->is_loop);
}

void AudioSourceComponent::SetMute(const bool & is_mute)
{
    if (!audio_clip || this->is_mute == is_mute)
        return;

    this->is_mute = is_mute;
    audio_clip->SetMute(this->is_mute);
}

void AudioSourceComponent::SetPriority(const int & priority)
{
    if (!audio_clip || this->priority == priority)
        return;

    /*
        Channel Priority
        - 0     : 우선순위 높음
        - 255   : 우선순위 낮음
        - 128   : 기본값
    */

    this->priority = Math::Clamp(priority, 0, 255);
    audio_clip->SetPriority(this->priority);
}

void AudioSourceComponent::SetVolume(const float & volume)
{
    if (!audio_clip || this->volume == volume)
        return;

    this->volume = Math::Clamp(volume, 0.0f, 1.0f);
    audio_clip->SetVolume(this->volume);
}

void AudioSourceComponent::SetPitch(const float & pitch)
{
    if (!audio_clip || this->pitch == pitch)
        return;

    this->pitch = Math::Clamp(pitch, 0.0f, 3.0f);
    audio_clip->SetPitch(this->pitch);
}

void AudioSourceComponent::SetPan(const float & pan)
{
    if (!audio_clip || this->pan == pan)
        return;

    this->pan = Math::Clamp(pan, -1.0f, +1.0f);
    audio_clip->SetPan(this->pan);
}
