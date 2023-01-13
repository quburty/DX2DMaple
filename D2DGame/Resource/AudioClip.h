#pragma once
#include "IResource.h"

namespace FMOD { class System; class Sound; class Channel; }

enum class PlayMode : uint { Memory, Stream, };
enum class Rolloff  : uint { Linear, Custom, };

class AudioClip final : public IResource
{
public:
    AudioClip(class Context* const context);
    ~AudioClip();

    bool SaveToFile(const std::string& path) override;
    bool LoadFromFile(const std::string& path) override;

    bool Play();
    bool Pause();
    bool Stop();

    auto SetLoop(const bool& is_loop) -> const bool;
    auto SetMute(const bool& is_mute) -> const bool;
    auto SetPriority(const int& priority) -> const bool;
    auto SetVolume(const float& volume) -> const bool;
    auto SetPitch(const float& pitch) -> const bool;
    auto SetPan(const float& pan) -> const bool;

    auto SetRolloff(const std::vector<D3DXVECTOR3>& curve_points) -> const bool;
    auto SetRolloff(const Rolloff& rolloff) -> const bool;

    auto IsPlaying() -> const bool;

    void Update();

private:
    auto CreateSound(const std::string& path) -> const bool;
    auto CreateStream(const std::string& path) -> const bool;

    auto GetSoundMode() const -> const int;
    auto IsChannelValid() const -> const bool;

    void LogErrorHandler(const int& error) const;

private:
    FMOD::System* system                = nullptr;
    FMOD::Sound* sound                  = nullptr;
    FMOD::Channel* channel              = nullptr;

    float min_distance                  = 1.0f;
    float max_distance                  = 10000.0f;

    PlayMode play_mode                  = PlayMode::Memory;
    int loop_mode                       = 0;
    int rolloff_mode                    = 0;
    int result                          = 0;
};