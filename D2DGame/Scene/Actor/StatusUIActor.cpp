#include "stdafx.h"
#include "StatusUIActor.h"
#include "../stdafx_Scene.h"

StatusUIActor::StatusUIActor(Context * const context)
	:Actor(context)
{
}

StatusUIActor::~StatusUIActor()
{
}

void StatusUIActor::Start()
{
	Actor::Start();

	auto scene = context->GetSubsystem<Scene>();

	hpbar = scene->Instantiate<BarActor>("hpbar").get();
	hpbar->SetTarget(1);
	hpbar->SetTexture("../_assets/Textures/UI/hpbar.png");
	hpbar->SetBarLength(170);
	hpbar->SetPosition(D3DXVECTOR2(-77,-1) + this->position);

	mpbar = scene->Instantiate<BarActor>("hpbar").get();
	mpbar->SetTarget(2);
	mpbar->SetTexture("../_assets/Textures/UI/mpbar.png");
	mpbar->SetBarLength(170);
	mpbar->SetPosition(D3DXVECTOR2(-77, -15) + this->position);

	expbar = scene->Instantiate<BarActor>("hpbar").get();
	expbar->SetTarget(3);
	expbar->SetTexture("../_assets/Textures/UI/expbar.png");
	expbar->SetBarLength(1000);
	expbar->SetYMin(-5.f);
	expbar->SetYMax(+5.f);
	expbar->SetPosition(D3DXVECTOR2(-500, -100) + this->position);
	
	level_actor = scene->Instantiate<Actor>("LevelFont").get();
	level_actor->SetPosition(D3DXVECTOR2(0, 24) + this->position);
}

void StatusUIActor::Update(const float & delta_time_sec)
{
	Actor::Update(delta_time_sec);

	level_actor->GetComponent<NumberFontComponent>()
		->SetValue(context->GetSubsystem<Scene>()->GetPlayer()->GetComponent<StatusComponent>()->GetLevel());
}
