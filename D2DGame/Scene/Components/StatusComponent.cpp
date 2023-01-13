#include "stdafx.h"
#include "StatusComponent.h"
#include "../stdafx_Scene.h"

StatusComponent::StatusComponent(Context * const context, Actor * const actor)
	:IComponent(context,actor)
{
	component_name = typeid(*this).name();
}

auto StatusComponent::DeepCopy(Actor * const target) const -> const std::shared_ptr<IComponent>
{
	std::shared_ptr<IComponent> component = std::make_shared<StatusComponent>(context, target);
	auto cast_component = std::static_pointer_cast<StatusComponent>(component);

	cast_component->hp_max = this->hp_max;
	cast_component->mp_max = this->mp_max;
	cast_component->atk = this->atk;
	cast_component->level = this->level;
	cast_component->exp = this->exp;
	cast_component->exp_max = this->exp_max;

	return component;
}

void StatusComponent::OnStart()
{
	hp = hp_max;
	mp = mp_max;
}

void StatusComponent::OnUpdate(const float & delta_time_sec)
{
	if (exp >= exp_max)
		LevelUp();
}

void StatusComponent::OnEnd()
{
}

void StatusComponent::LevelUp()
{
	level++;
	hp_max += 10;
	atk += 2;
	hp = hp_max;
	mp = mp_max;
	while (exp >= exp_max)
	{
		exp -= exp_max;
	}

	auto effect = context->GetSubsystem<Scene>()->Instantiate<EffectActor>("level_up");
	effect->SetPosition(actor->GetPosition()+D3DXVECTOR2(0,200));
}

void StatusComponent::GetDamaged(const float & damage)
{
	float total_damage = damage;

	if (static_cast<uint>(total_damage) >= hp)
	{
		hp = 0;
		alive = false;
	}
	else
		hp -= static_cast<uint>(total_damage);
}

void StatusComponent::GetHealed(const float & heal)
{
	uint heal_uint = static_cast<uint>(heal);

	if (hp + heal_uint > hp_max)
		hp = hp_max;
	else
		hp += heal_uint;
}

bool StatusComponent::MPGetDamaged(const uint & mp_damage)
{
	if (mp < mp_damage)
	{
		return false;
	}
	else
	{
		mp -= mp_damage;
		return true;
	}
}

void StatusComponent::MPGetHealed(const uint & mp_heal)
{
	if (mp + mp_heal > mp_max)
		mp = mp_max;
	else
		mp += mp_heal;
}
