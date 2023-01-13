#pragma once
#include "Actor.h"

class EffectActor final : public Actor
{
public:
	EffectActor(class Context* const context);
	~EffectActor();

	void Start() override;
	void Update(const float& delta_time_sec) override;
	void SetEffectAni(const std::string& path);

private:
};