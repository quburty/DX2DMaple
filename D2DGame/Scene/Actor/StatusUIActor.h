#pragma once
#include "Actor.h"

class StatusUIActor final : public Actor
{
public:
	StatusUIActor(class Context* const context);
	~StatusUIActor();

	void Start() override;
	void Update(const float& delta_time_sec) override;

private:
	class BarActor* hpbar = nullptr;
	class BarActor* mpbar = nullptr;
	class BarActor* expbar = nullptr;
	class Actor* level_actor = nullptr;
};