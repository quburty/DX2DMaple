#include "stdafx.h"
#include "PortalActor.h"
#include "../stdafx_Scene.h"

PortalActor::PortalActor(Context * const context)
	:Actor(context)
{
	auto animator = AddComponent<AnimatorComponent>();
	animator->AddAnimation("../_assets/Animations/Portal.xml");
	SetScale(50, 50);
}

void PortalActor::Update(const float & delta_time_sec)
{
	Actor::Update(delta_time_sec);

	auto scene = context->GetSubsystem<Scene>();
	auto input = context->GetSubsystem<Input>();

	if (IsCollide(scene->GetPlayer()) && input->GetKeyDown(KeyCode::Arrow_Up))
	{
		scene->SetMapID(map_id);
		scene->SetSpawnPosition(teleport_position);
	}
}
