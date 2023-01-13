#pragma once
#include "IComponent.h"

class RigidBodyComponent final : public IComponent
{
public:
	RigidBodyComponent
	(
		class Context* const context,
		class Actor* const actor
	);
	~RigidBodyComponent() = default;

	auto DeepCopy(Actor* const target) const -> const std::shared_ptr<IComponent> override;

	void OnStart() override;
	void OnUpdate(const float& delta_time_sec) override;
	void OnEnd() override;

	auto GetGravity() const -> const float& { return gravity; }
	auto GetXSpeed() const -> const float& { return xSpeed; }
	auto GetYSpeed() const -> const float& { return ySpeed; }
	void SetGravity(const float& gravity) { this->gravity = gravity; }
	void SetXSpeed(const float& xSpeed) { this->xSpeed = xSpeed; }
	void SetYSpeed(const float& ySpeed) { this->ySpeed = ySpeed; }

	void AddForce(const D3DXVECTOR2& force);
	void AddForce(const float& x, const float& y) { AddForce(D3DXVECTOR2(x, y)); }

	bool IsPlatform() const { return is_platform; }

private:
	bool is_platform = false;
	class Actor* current_platform;

	float gravity = 1000.0f;
	float xSpeed = 0.0f;
	float ySpeed = 0.0f;

	const float xSpeedLimit = 300.0f;
	const float ySpeedLimit = 1000.0f;
};