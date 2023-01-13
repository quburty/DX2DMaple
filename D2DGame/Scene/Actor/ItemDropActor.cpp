#include "stdafx.h"
#include "ItemDropActor.h"
#include "../stdafx_Scene.h"

ItemDropActor::ItemDropActor(Context * const context)
	:Actor(context)
{
	AddComponent<RigidBodyComponent>();
	auto animator = AddComponent<AnimatorComponent>();
	animator->AddAnimation("../_assets/Animations/ItemDrop.xml");
	animator->SetUsingActorTexture(true);
	SetTexture(context->GetSubsystem<GameData>()->GetItemData(1)->icon_path);
}

void ItemDropActor::Start()
{
	Actor::Start();

	AddComponent<RigidBodyComponent>()->AddForce(Math::Random(-400, 400), 300);
}

void ItemDropActor::Update(const float & delta_time_sec)
{
	Actor::Update(delta_time_sec);

	auto player = context->GetSubsystem<Scene>()->GetPlayer();
	auto input = context->GetSubsystem<Input>();

	if (IsCollide(player) && input->GetKeyDown(KeyCode::Z))
	{
		auto scene = context->GetSubsystem<Scene>();
		auto effect = scene->CreateActor<EffectActor>();
		effect->SetPosition(this->position);
		effect->GetAnimator()->AddAnimation("ItemGet",GetComponent<AnimatorComponent>()->GetCurrentAnimation());
		effect->GetAnimator()->SetShaderType(ShaderType::Fade);
		effect->GetAnimator()->SetUsingActorTexture(true);
		effect->SetTexture(context->GetSubsystem<GameData>()->GetItemData(item_id)->icon_path);
		effect->AddComponent<RigidBodyComponent>()->AddForce(0, 200);
		effect->AddComponent<AudioSourceComponent>()->SetAudioClip("../_assets/Audios/PickUpItem.mp3");
		scene->RegisterActor(effect);

		context->GetSubsystem<GameData>()->AddInventoryItem(item_id, 1);

		is_active = false;
	}
}

bool ItemDropActor::SetItemID(const uint & id)
{
	auto item_data = context->GetSubsystem<GameData>()->GetItemData(id);
	
	if (!item_data)
	{
		LOG_ERROR_F("Item data %d doesn't exist");
		return false;
	}

	item_id = id;
	SetTexture(context->GetSubsystem<GameData>()->GetItemData(item_id)->icon_path);

	return false;
}
