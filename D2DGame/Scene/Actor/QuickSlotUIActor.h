#pragma once
#include "Actor.h"

class QuickSlotUIActor final : public Actor
{
public:
	QuickSlotUIActor(class Context* const context);
	~QuickSlotUIActor();

	void Start() override;
	void Update(const float& delta_time_sec) override;

private:
	Actor* font_actor1 = nullptr;
	Actor* font_actor2 = nullptr;
	Actor* font_actor3 = nullptr;

	class NumberFontComponent* font1;
	class NumberFontComponent* font2;
	class NumberFontComponent* font3;
};