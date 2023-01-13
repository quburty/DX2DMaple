#include "stdafx.h"
#include "EffectActor.h"
#include "../stdafx_Scene.h"

EffectActor::EffectActor(Context * const context)
	:Actor(context)
{
	AddComponent<AnimatorComponent>();
}

EffectActor::~EffectActor()
{
}

void EffectActor::Start()
{
	Actor::Start();

	//���� ����� ������� ������ ���
	if (auto audio = GetComponent<AudioSourceComponent>())
	{
		audio->Play();
	}
}

void EffectActor::Update(const float & delta_time_sec)
{
	Actor::Update(delta_time_sec);

	if (GetComponent<AnimatorComponent>()->IsAnimationEnd())
		is_active = false;
}

void EffectActor::SetEffectAni(const std::string & path)
{
	GetComponent<AnimatorComponent>()->AddAnimation(path);
}
