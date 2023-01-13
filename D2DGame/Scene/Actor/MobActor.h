#pragma once
#include "Actor.h"

class MobActor : public Actor
{
public:
	MobActor(class Context* const context);
	~MobActor();

	void Update(const float& delta_time_sec) override;

	void Hitted(const float& damage);
	void AddDropTable(const uint& item_id, const float& percentage = 1.0f);

	void SetSpawnedPlatform(class PlatformActor* const platform) { belong_platform = platform; }

private:
	std::vector<std::pair<uint, float>> item_drop_table; //first:item_id second:percentage
	PlatformActor* belong_platform;
	bool moving = false;
};