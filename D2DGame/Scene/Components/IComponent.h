#pragma once

class IComponent : public Object
{
public:
	IComponent
	(
		class Context* const context,
		class Actor* const actor
	);

    virtual ~IComponent() = default;

	virtual auto DeepCopy(Actor* const target) const -> const std::shared_ptr<IComponent> = 0;

    virtual void OnStart() = 0;
    virtual void OnUpdate(const float& delta_time_sec) = 0;
    virtual void OnEnd() = 0;

    auto GetActor() const -> class Actor* const { return actor; }

    auto GetComponentName() const -> const std::string& { return component_name; }

protected:
    class Context* context              = nullptr;
    class Actor* actor                  = nullptr;
	std::string component_name = "Unknown";
};