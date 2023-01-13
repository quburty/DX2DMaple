#include "stdafx.h"
#include "BarActor.h"
#include "../stdafx_Scene.h"

BarActor::BarActor(Context * const context)
	:Actor(context)
{
	render_texture_size = false;
	actor_x_min = 0.0f;
}

void BarActor::Update(const float & delta_time_sec)
{
	Actor::Update(delta_time_sec);

	auto status = context->GetSubsystem<Scene>()->GetPlayer()->GetComponent<StatusComponent>();

	if (target_value == 1)
	{
		max_value = static_cast<float>(status->GetHPMax());
		current_value = static_cast<float>(status->GetCurrentHP());
	}
	else if (target_value == 2)
	{
		max_value = static_cast<float>(status->GetMPMax());
		current_value = static_cast<float>(status->GetCurrentMP());
	}
	else if (target_value == 3)
	{
		max_value = static_cast<float>(status->GetEXPMax());
		current_value = static_cast<float>(status->GetEXP());
	}

	float length = max_length * current_value / max_value;
	SetXMax(actor_x_min + length);
}

void BarActor::SetMaxValue(const float & value)
{
	if (value <= 0.0f)
	{
		LOG_WARNING_F("max value should be positive float. inserting value: %f", value);
		return;
	}

	max_value = value;
}

void BarActor::SetValue(const float & value)
{
	if (value < 0.0f || value > max_value)
	{
		LOG_WARNING_F("value should be positive float. inserting value: %f", value);
		return;
	}

	current_value = value;
}
