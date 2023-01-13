#pragma once
#include "ISubsystem.h"
#include "GameData/GameData_Item.h"
#include "GameData/GameData_Map.h"
#include "GameData/GameData_Mob.h"

class GameData final : public ISubsystem
{
public:
	GameData(class Context* const context);
	~GameData() = default;

	bool Initialize() override;

	auto GetItemData(const uint& idx) -> const std::shared_ptr<ItemData>&;

	//inventory
	auto GetInventoryData() -> std::map<uint, uint>& { return inventory; }
	void AddInventoryItem(const uint& item_id, const uint& ammount = 1);
	bool DecreaseInventoryItem(const uint& item_id, const uint& ammount = 1);

	//Map
	auto GetMapData(const uint& idx) -> const MapData& { return mapdata_array[idx]; }

	//Mob
	auto GetMobData(const std::string& mob_name) -> const MobData& { return mobdata_array[mob_name]; }

	//for debug
	void PrintInventory();


private:
	bool SaveToFile_Item(const std::string& file_name);
	bool LoadFromFile_Item(const std::string& file_name);

private:
	bool SaveToFile_Map(const std::string& file_name);
	bool LoadFromFile_Map(const std::string& file_name);

private:
	bool LoadFromFile_Mob(const std::string& file_name);

private:
	std::map<uint, std::shared_ptr<ItemData>> item_array;

private:
	std::map<uint, uint> inventory; //first: item_id second: item_count
	uint inventory_slot_count = 0;
	//std::map<ItemEquipPart, uint> equipment;

private:
	std::map<uint, MapData> mapdata_array;
	std::map<std::string, MobData> mobdata_array;
};