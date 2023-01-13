#pragma once
#include "Actor.h"

class PortalActor final : public Actor
{
public:
	PortalActor(class Context* const context);
	~PortalActor() = default;

	void Update(const float& delta_time_sec) override;

	void SetMapID(const uint& map_id) { this->map_id = map_id; }
	void SetTeleportPosition(const D3DXVECTOR2& pos) { this->teleport_position = pos; }

private:
	uint map_id = 0;
	D3DXVECTOR2 teleport_position{ 0.0f,0.0f };

	bool teleport = false;
};