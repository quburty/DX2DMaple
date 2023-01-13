#include "stdafx.h"
#include "WallActor.h"
#include "../stdafx_Scene.h"

WallActor::WallActor(Context * const context)
	:Actor(context)
{
}

void WallActor::Update(const float & delta_time_sec)
{
	Actor::Update(delta_time_sec);

	auto player = context->GetSubsystem<Scene>()->GetPlayer();
	auto player_pos = player->GetPosition();

	if (this->position.x + this->actor_x_max > player_pos.x + player->GetXMin() &&
		this->position.x + this->actor_x_min < player_pos.x + player->GetXMax() &&
		this->position.y + this->actor_y_max - 3.f > player_pos.y &&
		this->position.y + this->actor_y_min - 5.f < player_pos.y)
	{
		if (player_pos.x > this->position.x)
			player->SetPosition(this->position.x + this->actor_x_max - player->GetXMin() + 1.f, player_pos.y);
		else if (player_pos.x <= this->position.x)
			player->SetPosition(this->position.x + this->actor_x_min - player->GetXMax() - 1.f, player_pos.y);
	}
}
