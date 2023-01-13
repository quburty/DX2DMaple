#pragma once
#include "IComponent.h"

class UIComponent final : public IComponent
{
public:
	UIComponent
	(
		class Context* const context,
		class Actor* const actor
	);
	~UIComponent() = default;

	auto DeepCopy(Actor* const target) const -> const std::shared_ptr<IComponent> override;

	void OnStart() override;
	void OnUpdate(const float& delta_time_sec) override;
	void OnEnd() override;

	void SetClickEvent(const std::function<void(UIComponent*)>& func_ptr) { click_event = func_ptr; }

private:
	std::function<void(UIComponent*)> click_event;
	std::function<void(UIComponent*)> drag_event;
	std::function<void(UIComponent*)> drop_event;

	bool is_draged = false;
};