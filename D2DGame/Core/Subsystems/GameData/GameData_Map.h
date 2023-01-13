#pragma once

struct PortalData
{
	D3DXVECTOR2 portal_pos;
	uint teleport_map_id;
	D3DXVECTOR2 teleport_pos;
};

struct WallData
{
	D3DXVECTOR2 wall_pos;
	float height;
};

struct MapData
{
	std::string bgm_path;
	std::string bg_path;
	float map_x_min;
	float map_x_max;
	float map_y_min;
	float map_y_max;
	std::vector<std::pair<std::array<float,4>,uint>> platforms; //first:pos second:scale third:mobspawn
	std::string mob_name;
	std::vector<PortalData> portals;
	std::vector<WallData> walls;
};