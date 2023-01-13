#include "stdafx.h"
#include "RigidBodyComponent.h"
#include "../stdafx_Scene.h"

RigidBodyComponent::RigidBodyComponent(Context * const context, Actor * const actor)
	:IComponent(context, actor)
{
	component_name = typeid(*this).name();
}

auto RigidBodyComponent::DeepCopy(Actor* const target) const -> const std::shared_ptr<IComponent>
{
	std::shared_ptr<IComponent> component = std::make_shared<RigidBodyComponent>(context,target);
	auto cast_component = std::static_pointer_cast<RigidBodyComponent>(component);

	cast_component->gravity = gravity;

	return component;
}

void RigidBodyComponent::OnStart()
{
}

void RigidBodyComponent::OnUpdate(const float & delta_time_sec)
{
	auto position = actor->GetPosition();

	if (!is_platform)
		ySpeed -= gravity * delta_time_sec;
	else if (ySpeed < 0.0f)
		ySpeed = 0.0f;

	/*
		속도제한
	*/
	if (ySpeed > ySpeedLimit)
		ySpeed = ySpeedLimit;
	if (ySpeed < -ySpeedLimit)
		ySpeed = -ySpeedLimit;

	if (xSpeed > xSpeedLimit)
		xSpeed = xSpeedLimit;
	if (xSpeed < -xSpeedLimit)
		xSpeed = -xSpeedLimit;


	const float friction = 5.0f;
	if (xSpeed > 0)
		xSpeed -= xSpeed * friction * delta_time_sec;
	if (xSpeed < 0)
		xSpeed -= xSpeed * friction * delta_time_sec;

	//착지 구현
	if (!is_platform)
	{
		for (const auto platform : context->GetSubsystem<Scene>()->GetTypeActors<PlatformActor>())
		{
			if (actor->GetPosition().x > platform->GetPosition().x + platform->GetXMin() &&
				actor->GetPosition().x < platform->GetPosition().x + platform->GetXMax() &&
				actor->GetPosition().y > platform->GetPosition().y &&
				actor->GetPosition().y + ySpeed * delta_time_sec < platform->GetPosition().y &&
				ySpeed < 0.0f)
			{
				position.y = platform->GetPosition().y;
				ySpeed = 0.0f;
				is_platform = true;
				current_platform = platform.get();
				break;
			}
		}
	}
	else //계속 platform위에 있는지 체크
	{
		if (current_platform)
		{
			if (actor->GetPosition().x < current_platform->GetPosition().x + current_platform->GetXMin() ||
				actor->GetPosition().x > current_platform->GetPosition().x + current_platform->GetXMax() ||
				abs(actor->GetPosition().y - current_platform->GetPosition().y) > 5.0f)
			{
				is_platform = false;
			}
		}
		else
			LOG_ERROR("Rigidbody has no current_platform ptr");
	}

	position.x += xSpeed * delta_time_sec;
	position.y += ySpeed * delta_time_sec;
	
	actor->SetPosition(position);
}

void RigidBodyComponent::OnEnd()
{
}

void RigidBodyComponent::AddForce(const D3DXVECTOR2 & force)
{
	xSpeed += force.x;
	ySpeed += force.y;
}
