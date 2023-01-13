#include "stdafx.h"
#include "DamageFontActor.h"
#include "../stdafx_Scene.h"

DamageFontActor::DamageFontActor(Context * const context)
	:Actor(context)
{
	auto font = AddComponent<NumberFontComponent>();
	font->SetFontTextureDir("../_assets/Textures/Font/font_red.png");
}

DamageFontActor::~DamageFontActor()
{
}

void DamageFontActor::Update(const float & delta_time_sec)
{
	Actor::Update(delta_time_sec);

	time += delta_time_sec;

	position.y += 300.0f*delta_time_sec;

	if (time > show_time)
		is_active = false;
}

void DamageFontActor::SetDamageValue(const float & damage)
{
	this->damage = damage;
	GetComponent<NumberFontComponent>()->SetValue(static_cast<uint>(damage));
}
