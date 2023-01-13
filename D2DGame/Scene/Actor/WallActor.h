#pragma once
#include "Actor.h"

class WallActor final : public Actor
{
public:
	WallActor(class Context* const context);
	~WallActor() = default;

	void Update(const float& delta_time_sec) override;
};