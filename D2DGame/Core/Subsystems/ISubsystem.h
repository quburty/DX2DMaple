#pragma once

class ISubsystem
{
public:
    ISubsystem(class Context* context) : context(context) {}
    virtual ~ISubsystem() = default;

    virtual bool Initialize() = 0;
    virtual void Update(const float& delta_time_sec) {}

    auto GetContext() const -> class Context* const { return context; }

protected:
	class Context* context = nullptr;
};