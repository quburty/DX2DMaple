#pragma once
#include "IComponent.h"

class StatusComponent final : public IComponent
{
public:
	StatusComponent
	(
		class Context* const context,
		class Actor* const actor
	);
	~StatusComponent() = default;

	auto DeepCopy(Actor* const target) const -> const std::shared_ptr<IComponent> override;

	void OnStart() override;
	void OnUpdate(const float& delta_time_sec) override;
	void OnEnd() override;

	auto GetLevel() const -> const uint& { return level; }
	void SetLevel(const uint& level) { this->level = level; }
	void LevelUp();

	auto GetEXPMax() const -> const float& { return exp_max; }
	auto GetEXP() const -> const float& { return exp; }
	void SetEXP(const float& exp) { this->exp = exp; }
	void AddEXP(const float& exp) { this->exp += exp; }

	auto GetCurrentHP() const -> const uint& { return hp; }

	auto GetHPMax() const -> const uint& { return hp_max; }
	void SetHPMax(const uint& hp_max) { this->hp_max = hp_max; hp = this->hp_max; }
	
	auto GetATK() const -> const float& { return atk; }
	void SetATK(const float& atk) { this->atk = atk; }

	void GetDamaged(const float& damage);
	void GetHealed(const float& heal);
	
	bool IsAlive() const { return alive; }

	auto GetCurrentMP() const -> const uint& { return mp; }
	auto GetMPMax() const -> const uint& { return mp_max; }
	void SetMPMax(const uint& mp_max) { this->mp_max = mp_max; }
	
	bool MPGetDamaged(const uint& mp_damage);
	void MPGetHealed(const uint& mp_heal);

private:
	bool alive = true;

	uint level = 1;
	float exp_max = 100.0f;
	float exp = 0.0f;

	float atk = 1.0f;

	uint hp_max = 1;
	uint hp = 0;

	uint mp_max = 100;
	uint mp = 0;
};