#include "stdafx.h"
#include "NumberFontComponent.h"
#include "../stdafx_Scene.h"

NumberFontComponent::NumberFontComponent(Context * const context, Actor * const actor)
	:IComponent(context, actor)
{
	component_name = typeid(*this).name();
}

auto NumberFontComponent::DeepCopy(Actor* const target) const -> const std::shared_ptr<IComponent>
{
	std::shared_ptr<IComponent> component = std::make_shared<NumberFontComponent>(context, target);
	auto font = std::static_pointer_cast<NumberFontComponent>(component);

	font->font_texture_dir = this->font_texture_dir;
	font->shader_type = this->shader_type;

	return component;
}

void NumberFontComponent::OnStart()
{
}

void NumberFontComponent::OnUpdate(const float & delta_time_sec)
{
}

void NumberFontComponent::OnEnd()
{
}
