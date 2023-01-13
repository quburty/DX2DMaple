#include "stdafx.h"
#include "CursorActor.h"
#include "../stdafx_Scene.h"

CursorActor::CursorActor(Context * const context)
	:Actor(context)
{
	is_ui_actor = true;

	auto animator = AddComponent<AnimatorComponent>();
	animator->AddAnimation("../_assets/Animations/Cursor/Default.xml");
	animator->AddAnimation("../_assets/Animations/Cursor/Clicked.xml");
}

void CursorActor::Update(const float & delta_time_sec)
{
	Actor::Update(delta_time_sec);

	auto input = context->GetSubsystem<Input>();
	auto animator = GetComponent<AnimatorComponent>();

	this->position = input->GetMousePosition();

	if (input->GetKey(KeyCode::Click_Left))
		animator->SetCurrentAnimation("Clicked");
	else
		animator->SetCurrentAnimation("Default");
}
