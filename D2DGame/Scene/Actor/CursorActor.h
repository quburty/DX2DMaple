#pragma once
#include "Actor.h"

class CursorActor final : public Actor
{
public:
	CursorActor(class Context* const context);
	~CursorActor() = default;

	void Update(const float& delta_time_sec) override;
};