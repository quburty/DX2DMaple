#pragma once

struct MobData
{
	std::string idle_animation_path;
	std::string walk_animation_path;
	std::string dead_animation_path;
	D3DXVECTOR2 scale;
	float exp;
	float atk;
	uint hp_max;
	std::vector<std::pair<uint, float>> drop_table;
};