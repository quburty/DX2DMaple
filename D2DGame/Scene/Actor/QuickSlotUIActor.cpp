#include "stdafx.h"
#include "QuickSlotUIActor.h"
#include "../stdafx_Scene.h"

QuickSlotUIActor::QuickSlotUIActor(Context * const context)
	:Actor(context)
{
}

QuickSlotUIActor::~QuickSlotUIActor()
{
}

void QuickSlotUIActor::Start()
{
	Actor::Start();

	auto scene = context->GetSubsystem<Scene>();
	font_actor1 = scene->Instantiate<Actor>("ItemAmmountFont").get();
	font_actor1->SetPosition(position.x - 40, position.y + 10);
	font1 = font_actor1->GetComponent<NumberFontComponent>().get();

	font_actor2 = scene->Instantiate<Actor>("ItemAmmountFont").get();
	font_actor2->SetPosition(position.x - 8, position.y + 10);
	font2 = font_actor2->GetComponent<NumberFontComponent>().get();

	font_actor3 = scene->Instantiate<Actor>("ItemAmmountFont").get();
	font_actor3->SetPosition(position.x + 25, position.y + 10);
	font3 = font_actor3->GetComponent<NumberFontComponent>().get();
}

void QuickSlotUIActor::Update(const float & delta_time_sec)
{
	Actor::Update(delta_time_sec);

	font1->SetValue(context->GetSubsystem<GameData>()->GetInventoryData()[1]);
	font2->SetValue(context->GetSubsystem<GameData>()->GetInventoryData()[2]);
	font3->SetValue(context->GetSubsystem<GameData>()->GetInventoryData()[3]);
}
