#include "stdafx.h"
#include "GameData.h"

GameData::GameData(Context * const context)
	:ISubsystem(context)
{
}

bool GameData::Initialize()
{
	if (!LoadFromFile_Item("Items.xml"))
		return false;

	if (!LoadFromFile_Map("Maps.xml"))
		return false;

	if (!LoadFromFile_Mob("Mobs.xml"))
		return false;

	return true;
}

auto GameData::GetItemData(const uint & idx) -> const std::shared_ptr<ItemData>&
{
	return item_array[idx];
}

void GameData::AddInventoryItem(const uint & item_id, const uint & ammount)
{
	if (inventory.count(item_id) == 0)
	{
		inventory[item_id] = ammount;
		inventory_slot_count++;
	}
	else
		inventory[item_id] += ammount;
}

bool GameData::DecreaseInventoryItem(const uint & item_id, const uint & ammount)
{
	if (inventory.count(item_id) == 0)
		return false;

	if (inventory[item_id] - ammount < 0)
		return false;

	if (inventory[item_id] - ammount <= 0)
		inventory_slot_count--;

	inventory[item_id] -= ammount;
	return true;
}

void GameData::PrintInventory()
{
	using namespace std;

	cout << "Item slot: ";
	
	for (uint i = 1; i < inventory.size(); i++)
	{
		cout << item_array[i]->name << " : " << inventory[i] << "/t";
	}

	cout << endl;
}

bool GameData::SaveToFile_Item(const std::string & file_name)
{
	Xml::XMLDocument doc;

	Xml::XMLDeclaration* decl = doc.NewDeclaration();
	doc.LinkEndChild(decl);

	Xml::XMLElement* root = doc.NewElement("ItemData");
	doc.LinkEndChild(root);

	for (const auto& item : item_array)
	{
		Xml::XMLElement* first_element = doc.NewElement("Item");
		root->LinkEndChild(first_element);

		first_element->SetAttribute("ID", item.first);
		first_element->SetAttribute("Name", item.second->name.c_str());
		first_element->SetAttribute("IconPath", item.second->icon_path.c_str());
		first_element->SetAttribute("SellPrice", item.second->price);
	}

	return Xml::XMLError::XML_SUCCESS == doc.SaveFile(file_name.c_str());
}

bool GameData::LoadFromFile_Item(const std::string & file_name)
{
	item_array.clear();

	Xml::XMLDocument doc;
	Xml::XMLError error = doc.LoadFile(file_name.c_str());
	if (error != Xml::XMLError::XML_SUCCESS)
	{
		assert(false);
		return false;
	}

	Xml::XMLElement* root = doc.FirstChildElement();

	Xml::XMLElement* first_element = root->FirstChildElement();
	for (; first_element != nullptr; first_element = first_element->NextSiblingElement())
	{
		uint id;
		ItemDataConsume data;

		id = first_element->IntAttribute("ID");
		data.name = first_element->Attribute("Name");
		data.icon_path = first_element->Attribute("IconPath");
		data.price = first_element->IntAttribute("SellPrice");
		
		data.sort = first_element->IntAttribute("Sort");
		data.heal_ammount = first_element->FloatAttribute("HealAmount");

		item_array[id] = std::make_shared<ItemDataConsume>(data);
	}

	return true;
}

bool GameData::SaveToFile_Map(const std::string & file_name)
{
	//Xml::XMLDocument doc;

	//Xml::XMLDeclaration* decl = doc.NewDeclaration();
	//doc.LinkEndChild(decl);

	//Xml::XMLElement* root = doc.NewElement("MapDatas");
	//doc.LinkEndChild(root);

	//for (const auto& mapdata : mapdata_array)
	//{
	//	Xml::XMLElement* first_element = doc.NewElement("MapData");
	//	root->LinkEndChild(first_element);

	//	first_element->SetAttribute("ID", mapdata.first);
	//	first_element->SetAttribute("MapScaleX", mapdata.second.map_scale.x);
	//	first_element->SetAttribute("MapScaleY", mapdata.second.map_scale.y);
	//	first_element->SetAttribute("MobName", mapdata.second.mob_name.c_str());

	//	for (const auto& platform : mapdata.second.platforms)
	//	{
	//		Xml::XMLElement* platform_element = doc.NewElement("Platform");
	//		first_element->LinkEndChild(platform_element);
	//		platform_element->SetAttribute("PlatformPosX", platform.first.x);
	//		platform_element->SetAttribute("PlatformPosY", platform.first.y);
	//		platform_element->SetAttribute("PlatformScaleX", platform.second.x);
	//		platform_element->SetAttribute("PlatformScaleY", platform.second.y);
	//	}

	//	for (const auto& portal : mapdata.second.portals)
	//	{
	//		Xml::XMLElement* portal_element = doc.NewElement("Portal");
	//		first_element->LinkEndChild(portal_element);
	//		portal_element->SetAttribute("PortalPosX", portal.portal_pos.x);
	//		portal_element->SetAttribute("PortalPosY", portal.portal_pos.y);
	//		portal_element->SetAttribute("TeleportMapID", portal.teleport_map_id);
	//		portal_element->SetAttribute("TeleportPosX", portal.teleport_pos.x);
	//		portal_element->SetAttribute("TeleportPosY", portal.teleport_pos.y);
	//	}
	//}

	//return Xml::XMLError::XML_SUCCESS == doc.SaveFile(file_name.c_str());
	return false;
}

bool GameData::LoadFromFile_Map(const std::string & file_name)
{
	mapdata_array.clear();

	Xml::XMLDocument doc;
	Xml::XMLError error = doc.LoadFile(file_name.c_str());
	if (error != Xml::XMLError::XML_SUCCESS)
	{
		assert(false);
		return false;
	}

	Xml::XMLElement* root = doc.FirstChildElement();

	Xml::XMLElement* first_element = root->FirstChildElement();
	for (; first_element != nullptr; first_element = first_element->NextSiblingElement())
	{
		uint id;
		MapData mapdata;

		id = first_element->IntAttribute("ID");

		mapdata.bgm_path = first_element->Attribute("BGM");
		mapdata.bg_path = first_element->Attribute("BG");
		
		auto temp_texture = context->GetSubsystem<ResourceManager>()->Load<Texture2D>(mapdata.bg_path);

		if (auto map_scale_x = first_element->FloatAttribute("MapScaleX"))
		{
			mapdata.map_x_min = -map_scale_x * 0.5f;
			mapdata.map_x_max = +map_scale_x * 0.5f;
		}
		else
		{
			mapdata.map_x_min = first_element->FloatAttribute("MapStartX");
			mapdata.map_x_max = first_element->FloatAttribute("MapEndX");
		}

		if (auto map_scale_y = first_element->FloatAttribute("MapScaleY"))
		{
			mapdata.map_y_min = -map_scale_y * 0.5f;
			mapdata.map_y_max = +map_scale_y * 0.5f;
		}
		else
		{
			mapdata.map_y_min = first_element->FloatAttribute("MapStartY");
			mapdata.map_y_max = first_element->FloatAttribute("MapEndY");
		}

		mapdata.mob_name = first_element->Attribute("MobName");

		mapdata.platforms.clear();

		Xml::XMLElement* platform_element = first_element->FirstChildElement();
		for (; platform_element != nullptr; platform_element = platform_element->NextSiblingElement())
		{
			if (platform_element->Name() == std::string("Platform"))
			{
				std::array<float,4> pos_and_scale;
				uint mob_spawn_count;

				//0:xpos 1:ypos 2:xscale 3:yscale
				if (auto platform_length = platform_element->FloatAttribute("PlatformLength"))
				{
					pos_and_scale[0] = platform_element->FloatAttribute("PlatformStartX") + platform_length * 0.5f;
					pos_and_scale[2] = platform_length;
				}
				else
				{
					auto platform_start = platform_element->FloatAttribute("PlatformStartX");
					auto platform_end = platform_element->FloatAttribute("PlatformEndX");

					pos_and_scale[0] = (platform_start + platform_end)*0.5f;
					pos_and_scale[2] = platform_end - platform_start;
				}

				pos_and_scale[1] = temp_texture->GetHeight() - platform_element->FloatAttribute("PlatformPosY");
				
				mob_spawn_count = platform_element->IntAttribute("MobSpawn");

				mapdata.platforms.emplace_back(pos_and_scale,mob_spawn_count);
			}
			else if (platform_element->Name() == std::string("Portal"))
			{
				auto portal_element = platform_element;
				PortalData portal_data;
				portal_data.portal_pos.x = portal_element->FloatAttribute("PortalPosX");
				portal_data.portal_pos.y = temp_texture->GetHeight() - portal_element->FloatAttribute("PortalPosY");
				portal_data.teleport_map_id = portal_element->IntAttribute("TeleportMapID");
				portal_data.teleport_pos.x = portal_element->FloatAttribute("TeleportPosX");
				portal_data.teleport_pos.y = portal_element->FloatAttribute("TeleportPosY");

				mapdata.portals.emplace_back(portal_data);
			}
			else if (platform_element->Name() == std::string("Wall"))
			{
				auto wall_element = platform_element;
				WallData walldata;

				auto startY = wall_element->FloatAttribute("WallStartY");
				auto endY = wall_element->FloatAttribute("WallEndY");

				walldata.wall_pos.x = wall_element->FloatAttribute("WallPosX");
				walldata.wall_pos.y = temp_texture->GetHeight() - (startY + endY)*0.5f;
				walldata.height = endY - startY;

				mapdata.walls.emplace_back(walldata);
			}
		}

		mapdata_array[id] = mapdata;
	}

	return true;
}

bool GameData::LoadFromFile_Mob(const std::string & file_name)
{
	mobdata_array.clear();

	Xml::XMLDocument doc;
	Xml::XMLError error = doc.LoadFile(file_name.c_str());
	if (error != Xml::XMLError::XML_SUCCESS)
	{
		assert(false);
		return false;
	}

	Xml::XMLElement* root = doc.FirstChildElement();

	Xml::XMLElement* first_element = root->FirstChildElement();
	for (; first_element != nullptr; first_element = first_element->NextSiblingElement())
	{
		std::string mob_name;
		MobData mobdata;

		mob_name = first_element->Attribute("MobName");
		mobdata.idle_animation_path = first_element->Attribute("IdleAniPath");
		mobdata.walk_animation_path = first_element->Attribute("WalkAniPath");
		mobdata.dead_animation_path = first_element->Attribute("DeadAniPath");
		mobdata.scale.x = first_element->FloatAttribute("ScaleX");
		mobdata.scale.y = first_element->FloatAttribute("ScaleY");
		mobdata.exp = first_element->FloatAttribute("EXP");
		mobdata.atk = first_element->FloatAttribute("Atk");
		mobdata.hp_max = first_element->IntAttribute("HPMax");

		Xml::XMLElement* item_element = first_element->FirstChildElement();
		for (; item_element != nullptr; item_element = item_element->NextSiblingElement())
		{
			uint item_id = item_element->IntAttribute("ItemID");
			float percentage = item_element->FloatAttribute("Percent");

			mobdata.drop_table.emplace_back(item_id, percentage);
		}

		mobdata_array[mob_name] = mobdata;
	}

	return true;
}
