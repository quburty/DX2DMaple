#pragma once
#include "Actor.h"

class SkillActor final : public Actor
{
public:
	SkillActor(class Context* const context);
	~SkillActor();

	void Start() override;
	void Update(const float& delta_time_sec) override;

	void SetPlayer(class PlayerActor* const player) { this->player = player; }

	auto GetSkillPosition() const -> const D3DXVECTOR2& { return skill_position; }
	void SetSkillPosition(const D3DXVECTOR2& pos) { skill_position = pos; }

	auto GetDamage() const -> const float& { return damage; }
	void SetDamage(const float& damage) { this->damage = damage; }

	void SetSkillAttackTime(const float& attack_time = 0.f) { this->skill_attack_time = attack_time; }

private:
	void Attack();

private:
	class PlayerActor* player = nullptr;

	D3DXVECTOR2 skill_position{ 0.0f,0.0f };

	float skill_attack_time = 0.f;
	float time = 0.0f;
	bool attacked = false;
	
	float damage = 10.0f;
};