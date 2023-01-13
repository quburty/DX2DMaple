#include "stdafx.h"
#include "PlatformActor.h"
#include "../stdafx_Scene.h"

PlatformActor::PlatformActor(Context * const context)
	:Actor(context)
{
}

void PlatformActor::Update(const float & delta_time_sec)
{
	Actor::Update(delta_time_sec);

	if (max_mob_count > current_mob_count
		&& Math::Random(0.f, 1.f) < spawn_rate_sec*delta_time_sec)
	{
		auto mob = context->GetSubsystem<Scene>()->SpawnMob();
		mob->SetPosition(Math::Random(position.x + actor_x_min, position.x + actor_x_max), position.y);
		mob->SetSpawnedPlatform(this);
		current_mob_count++;
	}
}

