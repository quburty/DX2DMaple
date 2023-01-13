#pragma once
#include "ISubsystem.h"

class Timer final : public ISubsystem
{
public:
    Timer(class Context* const context);
    ~Timer() = default;

    bool Initialize() override;
    void Update(const float& delta_time_sec) override;

    auto GetDeltaTimeMS() const -> const float { return static_cast<float>(delta_time_ms); }
    auto GetDeltaTimeSEC() const -> const float { return static_cast<float>(delta_time_ms / 1000.0); }

private:
    double delta_time_ms = 0.0;
    std::chrono::time_point<std::chrono::high_resolution_clock> previous_time;
};