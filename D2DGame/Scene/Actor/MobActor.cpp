#include "stdafx.h"
#include "MobActor.h"
#include "../stdafx_Scene.h"

MobActor::MobActor(Context * const context)
	:Actor(context)
{
	auto status = AddComponent<StatusComponent>().get();
	status->SetHPMax(30);
	status->SetATK(50);
}

MobActor::~MobActor()
{
}

void MobActor::Update(const float & delta_time_sec)
{
	Actor::Update(delta_time_sec);

	static const float speed = 75.f;
	
	if (moving)
	{
		if (auto animator = GetComponent<AnimatorComponent>())
			animator->SetCurrentAnimation("Walk");

		auto platform_x = belong_platform->GetPosition().x;
		if (position.x < platform_x + belong_platform->GetXMin())
			is_dir_right = true;
		else if (position.x > platform_x + belong_platform->GetXMax())
			is_dir_right = false;

		if (is_dir_right)
			position.x += speed * delta_time_sec;
		else
			position.x -= speed * delta_time_sec;
	}
	else
	{
		if (auto animator = GetComponent<AnimatorComponent>())
			animator->SetCurrentAnimation("Idle");
	}


	if (Math::Random(0.f, 1.f) < (moving ? 0.1f : 0.4f)*delta_time_sec)
		moving = !moving;
}

void MobActor::Hitted(const float & damage)
{
	auto font = context->GetSubsystem<Scene>()->Instantiate<DamageFontActor>("font");
	font->SetDamageValue(damage);
	font->SetPosition(position);

	auto status = GetComponent<StatusComponent>();
	status->GetDamaged(damage);

	if (!status->IsAlive())
	{
		auto scene = context->GetSubsystem<Scene>();

		scene->GetPlayer()->GetComponent<StatusComponent>()->AddEXP(status->GetEXP());

		auto dead = context->GetSubsystem<Scene>()->CreateActor<EffectActor>();
		dead->SetPosition(this->position);
		auto ani = dead->GetComponent<AnimatorComponent>();
		ani->AddAnimation("Dead", GetAnimator()->GetAnimation("Dead"));
		ani->SetCurrentAnimation("Dead");
		ani->SetShaderType(ShaderType::Fade);
		context->GetSubsystem<Scene>()->RegisterActor(dead);
		
		if (belong_platform)
			belong_platform->MobDie();

		for (const auto& item : item_drop_table)
		{
			if (Math::Random(0.0f, 1.0f) <= item.second)
			{
				auto drop_item = scene->Instantiate<ItemDropActor>("item");
				drop_item->SetPosition(position.x, position.y+70.0f);
				drop_item->SetItemID(item.first);
			}
		}

		is_active = false;
	}
}

void MobActor::AddDropTable(const uint & item_id, const float & percentage)
{
	if (percentage > 1.0f || percentage <= 0.0f)
	{
		LOG_ERROR_F("percentage value %f is invalid", percentage);
		return;
	}

	item_drop_table.emplace_back(item_id, percentage);
}
