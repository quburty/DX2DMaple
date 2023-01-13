#pragma once
#include "Actor.h"

class BarActor final : public Actor
{
public:
	BarActor(class Context* const context);
	~BarActor() = default;

	void Update(const float& delta_time_sec) override;

	void SetBarLength(const float& length) { max_length = length; }
	void SetTarget(const uint& target) { target_value = target; } //1:hp 2:mp 3:exp

private:
	void SetMaxValue(const float& value);
	void SetValue(const float& value);

private:
	uint target_value; //1:hp 2:mp 3:exp

	float max_length = 500.0f;
	float max_value = 1.0f;
	float current_value = 1.0f;

};