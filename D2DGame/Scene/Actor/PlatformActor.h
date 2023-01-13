#pragma once
#include "Actor.h"

class PlatformActor final : public Actor
{
public:
	PlatformActor(class Context* const context);
	~PlatformActor() = default;

	void Update(const float& delta_time_sec) override;

	void SetMaxMobCount(const uint& max_count) { max_mob_count = max_count; }
	void MobDie() { current_mob_count--; }

private:
	uint max_mob_count = 0;
	uint current_mob_count = 0;
	float spawn_rate_sec = 0.5f;
};