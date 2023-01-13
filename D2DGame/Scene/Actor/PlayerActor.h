#pragma once
#include "Actor.h"

enum class AttackMotion : uint
{
	Default,
	Attack1,
	Attack2,
	Attack3
};

class PlayerActor final : public Actor
{
public:
	PlayerActor(class Context* const context);
	~PlayerActor();

	void Update(const float& delta_time_sec) override;

	bool UseConsumeItem(const uint& item_id);
	
	void Die();

private:
	bool is_prone = false;
	bool is_walking = false;
	bool is_invincible = false;
	bool is_rope_climbing = false;
	float invincible_time = 2.0f;
	float invincible_time_pass = 0.0f;

	AttackMotion attack_motion = AttackMotion::Default;
	float time_after_attack = 0.0f;
	bool can_attack = true;

	uint attack_count = 0;
};