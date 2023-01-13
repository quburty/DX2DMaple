#include "stdafx.h"
#include "PlayerActor.h"
#include "../stdafx_Scene.h"

PlayerActor::PlayerActor(Context * const context)
	:Actor(context)
{
	AddComponent<AnimatorComponent>();
	AddComponent<RigidBodyComponent>();
	auto audio = AddComponent<AudioSourceComponent>();
	auto status = AddComponent<StatusComponent>();
	status->SetHPMax(100);
	status->SetATK(5);

	audio->SetAudioClip("../_assets/Audios/Jump.mp3");
	audio->SetLoop(false);
}

PlayerActor::~PlayerActor()
{
}

void PlayerActor::Update(const float & delta_time_sec)
{
	auto animator = GetComponent<AnimatorComponent>();
	auto rigidbody = GetComponent<RigidBodyComponent>();
	auto status = GetComponent<StatusComponent>();

	if (!status->IsAlive())
	{
		animator->SetCurrentAnimation("Dead");
		rigidbody->SetXSpeed(0);
		rigidbody->SetYSpeed(0);

		return;
	}

	Actor::Update(delta_time_sec);

	auto input = context->GetSubsystem<Input>();


	if (rigidbody->IsPlatform() && input->GetKey(KeyCode::Arrow_Down))
		is_prone = true;
	else
		is_prone = false;

	if (input->GetKeyDown(KeyCode::Space) && rigidbody->IsPlatform())
	{
		if (is_prone)
			position.y -= 7.0f;
		else
		{
			rigidbody->AddForce(D3DXVECTOR2(0, 350));
			GetComponent<AudioSourceComponent>()->Play();
		}
	}

	static const float speed = 150.f;
	if (input->GetKey(KeyCode::Arrow_Left))
	{
		is_walking = true;
		position.x -= speed*delta_time_sec;
		is_dir_right = false;
	}
	else if (input->GetKey(KeyCode::Arrow_Right))
	{
		is_walking = true;
		position.x += speed*delta_time_sec;
		is_dir_right = true;
	}
	else
		is_walking = false;

	
	//Attack
	if (can_attack)
	{
		if (input->GetKeyDown(KeyCode::Ctrl_Left) && status->MPGetDamaged(4))
		{
			if (attack_count == 0)
			{
				auto skill = std::static_pointer_cast<SkillActor>(context->GetSubsystem<Scene>()->GetPrefab("skill1"));
				skill->SetDamage(GetComponent<StatusComponent>()->GetATK());
				context->GetSubsystem<Scene>()->Instantiate<SkillActor>("skill1");
			}
			else if (attack_count == 1)
			{
				auto skill = std::static_pointer_cast<SkillActor>(context->GetSubsystem<Scene>()->GetPrefab("skill2"));
				skill->SetDamage(GetComponent<StatusComponent>()->GetATK()*1.2f);
				context->GetSubsystem<Scene>()->Instantiate<SkillActor>("skill2");
			}
			else if (attack_count == 2)
			{
				auto skill = std::static_pointer_cast<SkillActor>(context->GetSubsystem<Scene>()->GetPrefab("skill3"));
				skill->SetDamage(GetComponent<StatusComponent>()->GetATK()*1.3f);
				skill->SetSkillAttackTime(0.3f);
				context->GetSubsystem<Scene>()->Instantiate<SkillActor>("skill3");
			}

			can_attack = false;
			time_after_attack = 0.0f;
			attack_count = (attack_count + 1) % 3;
		}
	}
	else
	{
		if (time_after_attack > 0.5f)
		{
			can_attack = true;
		}
	}

	time_after_attack += delta_time_sec;

	if (time_after_attack > 1.0f)
		attack_count = 0;
	
	//Enemy collision
	{
		auto scene = context->GetSubsystem<Scene>();

		if (!is_invincible)
		{
			for (const auto& mob : scene->GetTypeActors<MobActor>())
			{
				if (IsCollide(mob))
				{
					is_invincible = true;

					if (is_dir_right)
						rigidbody->AddForce(D3DXVECTOR2(-500, 300));
					else
						rigidbody->AddForce(D3DXVECTOR2(+500, 300));

					auto damage = mob->GetComponent<StatusComponent>()->GetATK();
					status->GetDamaged(damage);
					auto font = scene->Instantiate<DamageFontActor>("font");
					font->SetDamageValue(damage);
					font->GetComponent<NumberFontComponent>()->SetFontTextureDir("../_assets/Textures/Font/font_violet.png");
					font->SetPosition(position);

					if (!status->IsAlive())
						Die();

					break;
				}
			}
		}
		else
		{
			invincible_time_pass += delta_time_sec;
			if (invincible_time_pass > invincible_time)
			{
				invincible_time_pass = 0.0f;
				is_invincible = false;
			}
		}
	}

	//Item Use
	{
		if (input->GetKeyDown(KeyCode::Keypad1))
			UseConsumeItem(1);
		if (input->GetKeyDown(KeyCode::Keypad2))
			UseConsumeItem(2);
		if (input->GetKeyDown(KeyCode::Keypad3))
			UseConsumeItem(3);
	}

	//Animator code
	{
		if (!can_attack)
		{
			animator->SetCurrentAnimation("Attack");
		}
		else
		{
			if (!rigidbody->IsPlatform())
			{
				animator->SetCurrentAnimation("Jump");
			}
			else if (is_walking)
			{
				animator->SetCurrentAnimation("Walk");
			}
			else
			{
				animator->SetCurrentAnimation("Stand");
			}
		}
		if (is_prone)
		{
			animator->SetCurrentAnimation("Prone");
		}
	}
}

bool PlayerActor::UseConsumeItem(const uint & consume_item_id)
{
	auto gamedata = context->GetSubsystem<GameData>();
	
	if (gamedata->DecreaseInventoryItem(consume_item_id))
	{
		auto status = GetComponent<StatusComponent>();
		auto consume_item = std::static_pointer_cast<ItemDataConsume>(gamedata->GetItemData(consume_item_id));

		if (consume_item->sort & ItemConsumeSort::HP)
			status->GetHealed(consume_item->heal_ammount);
		if (consume_item->sort & ItemConsumeSort::MP)
			status->MPGetHealed(static_cast<uint>(consume_item->heal_ammount));
	}

	return true;
}

void PlayerActor::Die()
{
	context->GetSubsystem<Scene>()->Instantiate<Actor>("Tomb")->SetPosition(position.x, position.y + 100);
}
