#pragma once

enum ItemConsumeSort : uint
{
	HP = 1 << 0,
	MP = 1 << 1,
};

enum ItemEquipPart : uint
{
	Weapon,
};

struct ItemData
{
	std::string name;
	uint price;
	std::string icon_path;
};

struct ItemDataConsume :public ItemData
{
	uint sort;
	float heal_ammount;
};