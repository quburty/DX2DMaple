#include "stdafx.h"
#include "UIComponent.h"
#include "../stdafx_Scene.h"

UIComponent::UIComponent(Context * const context, Actor * const actor)
	:IComponent(context,actor)
{
	component_name = typeid(*this).name();
}

auto UIComponent::DeepCopy(Actor * const target) const -> const std::shared_ptr<IComponent>
{
	std::shared_ptr<IComponent> component = std::make_shared<UIComponent>(context, target);
	auto cast_component = std::static_pointer_cast<UIComponent>(component);

	cast_component->click_event = this->click_event;
	cast_component->drag_event = this->drag_event;
	cast_component->drop_event = this->drop_event;

	return component;
}

void UIComponent::OnStart()
{
}

void UIComponent::OnUpdate(const float & delta_time_sec)
{
	auto input = context->GetSubsystem<Input>();
	if (input->GetKeyDown(KeyCode::Click_Left))
	{
		if(click_event)
			click_event(this);
	}

	return;

	if (is_draged && input->GetKey(KeyCode::Click_Left))
	{
		
	}

	if (is_draged && input->GetKeyUp(KeyCode::Click_Left))
	{
		is_draged = false;

	}
}

void UIComponent::OnEnd()
{
}
