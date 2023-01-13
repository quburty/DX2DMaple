#include "stdafx.h"
#include "SkillActor.h"
#include "../stdafx_Scene.h"

SkillActor::SkillActor(Context * const context)
	:Actor(context)
{
}

SkillActor::~SkillActor()
{
}

void SkillActor::Start()
{
	Actor::Start();	
}

void SkillActor::Update(const float & delta_time_sec)
{
	Actor::Update(delta_time_sec);

	player = context->GetSubsystem<Scene>()->GetPlayer().get();
	position = player->GetPosition();
	is_dir_right = player->IsDirectionRight();

	time += delta_time_sec;
	if (time > skill_attack_time && !attacked)
	{
		Attack();
		attacked = true;
	}

	if (GetComponent<AnimatorComponent>()->IsAnimationEnd())
		is_active = false;
}

void SkillActor::Attack()
{

	auto scene = context->GetSubsystem<Scene>();

	for (auto& mob : scene->GetTypeActors<MobActor>())
	{
		if (IsCollide(mob))
		{
			std::static_pointer_cast<MobActor>(mob)->Hitted(damage);
			scene->Instantiate<EffectActor>("attack_hit")->SetPosition(mob->GetPosition());
		}
	}
}
