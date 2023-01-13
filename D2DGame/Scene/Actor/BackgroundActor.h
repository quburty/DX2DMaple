#pragma once
#include "Actor.h"

class BackgroundActor final : public Actor
{
public:
	BackgroundActor(class Context* const context);
	~BackgroundActor() = default;

};