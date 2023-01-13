#include "stdafx.h"
#include "Actor.h"
#include "../stdafx_Scene.h"

Actor::Actor(Context * const context)
    : context(context)
{

}

Actor::~Actor()
{
}

void Actor::Start()
{
	for (const auto& component : components)
        component->OnStart();
}

void Actor::Update(const float & delta_time_sec)
{
    if (!is_active)
        return;

    for (const auto& component : components)
        component->OnUpdate(delta_time_sec);
}

void Actor::End()
{
	for (const auto& component : components)
        component->OnEnd();
}

auto Actor::GetAnimator() -> const std::shared_ptr<class AnimatorComponent>
{
	if (auto animator = GetComponent<AnimatorComponent>())
		return animator;
	else
		return nullptr;
}

void Actor::RemoveComponent(const std::string & component_name)
{
	for (auto iter = components.begin(); iter != components.end();)
	{
		auto component = *iter;
		if (component->GetComponentName() == component_name)
		{
			component.reset();
			iter = components.erase(iter);
		}
		else
			iter++;
	}
}

auto Actor::GetScale() const -> const D3DXVECTOR2
{
	return D3DXVECTOR2(actor_x_max - actor_x_min, actor_y_max - actor_y_min);
}

void Actor::SetScale(const D3DXVECTOR2 & scale)
{
	actor_x_max = +scale.x / 2;
	actor_x_min = -scale.x / 2;
	actor_y_max = +scale.y / 2;
	actor_y_min = -scale.y / 2;
}

auto Actor::GetCenterPosition() const -> const D3DXVECTOR2
{
	float x = position.x + (actor_x_min + actor_x_max)*0.5f;
	float y = position.y + (actor_y_min + actor_y_max)*0.5f;
	return D3DXVECTOR2(x, y);
}

void Actor::SetTexture(const std::string & path)
{
	texture = context->GetSubsystem<ResourceManager>()->Load<Texture2D>(path);
}

bool Actor::IsCollide(const std::shared_ptr<Actor>& rhs)
{
	if (   position.x + actor_x_min < rhs->position.x + rhs->actor_x_max
		&& position.x + actor_x_max > rhs->position.x + rhs->actor_x_min
		&& position.y + actor_y_min < rhs->position.y + rhs->actor_y_max
		&& position.y + actor_y_max > rhs->position.y + rhs->actor_y_min)
		return true;
	else
		return false;
}

void Actor::SetRenderOrder(const int & render_order)
{
	if (render_order > 2 || render_order < -2)
	{
		LOG_ERROR("Render order is out of range");
		return;
	}

	this->render_order = render_order;
}

Actor & Actor::operator=(const Actor & org_actor)
{
	this->actor_x_min = org_actor.actor_x_min;
	this->actor_x_max = org_actor.actor_x_max;
	this->actor_y_min = org_actor.actor_y_min;
	this->actor_y_max = org_actor.actor_y_max;

	this->position = org_actor.position;

	this->texture = org_actor.texture; //texture은 shallow copy로 처리

	this->components.clear();
	this->components.resize(org_actor.components.size());
	for (uint i = 0; i < components.size(); i++)
	{
		components[i] = org_actor.components[i]->DeepCopy(this);
	}

	this->is_dir_right = org_actor.is_dir_right;
	this->render_texture_size = org_actor.render_texture_size;

	this->is_ui_actor = org_actor.is_ui_actor;

	return *this;
}
