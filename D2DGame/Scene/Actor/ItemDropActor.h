#pragma once
#include "Actor.h"

class ItemDropActor final : public Actor
{
public:
	ItemDropActor(class Context* const context);
	~ItemDropActor() = default;

	void Start() override;
	void Update(const float& delta_time_sec) override;

	bool SetItemID(const uint& id);

private:

	uint item_id = 1;
};