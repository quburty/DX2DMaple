#pragma once
#include "ISubsystem.h"

namespace FMOD { class System; }

class Audio final : public ISubsystem
{
public:
    Audio(class Context* const context);
    ~Audio();

    bool Initialize() override;
    void Update(const float& delta_time_sec) override;

    auto GetFMODSystem() const { return system; }
    void SetListnerTransform(class TransformComponent* const transform);

private:
    void LogErrorHandler(const int& error) const;

private:
    FMOD::System* system                = nullptr;
    class TransformComponent* listener  = nullptr;
    float distance_factor               = 1.0f;
    uint max_channels                   = 32;
    int result                          = 0;
    bool is_initialized                 = false;     
};