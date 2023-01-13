#pragma once
#include "Actor.h"

class DamageFontActor final : public Actor
{
public:
	DamageFontActor(class Context* const context);
	~DamageFontActor();

	void Update(const float& delta_time_sec) override;

	auto GetDamageValue() const -> const float& { return damage; }
	void SetDamageValue(const float& damage);

private:
	float show_time = 0.5f;
	float time = 0.0f;
	float damage = 1.0f;
};