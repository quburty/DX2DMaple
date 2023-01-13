#pragma once
#include "ISubsystem.h"

class Scene final : public ISubsystem
{
public:
	Scene(class Context* context);
	~Scene();

	bool Initialize() override;
	void Update(const float& delta_time_sec);

	void TitleScreen();
	void InGameScreen();

	auto GetActors() const -> const std::map<std::string, std::list<std::shared_ptr<class Actor>>>& { return actors; }
	
	auto GetPlayer() -> const std::shared_ptr<class PlayerActor>;

	auto GetTagActors(const std::string& tag) -> const std::list<std::shared_ptr<class Actor>>& { return actors[tag]; }

	template <typename T>
	auto GetTypeActors() -> const std::list<std::shared_ptr<class Actor>>&;

	template <typename T>
	auto CreateActor() -> const std::shared_ptr<T>;

	template <typename T>
	void RegisterActor(const std::shared_ptr<T>& actor);

	template <typename T>
	auto Instantiate(const std::string& prefab_name) -> const std::shared_ptr<T>;

	template <typename T>
	void RegisterPrefab(const std::shared_ptr<T>& actor, const std::string& name);

	auto GetPrefab(const std::string& name) -> const std::shared_ptr<class Actor>&;

	auto GetCurrentMapID() const -> const uint& { return current_map_id; }
	void SetMapID(const uint& id) { update_map_id = id; }

	void SetSpawnPosition(const D3DXVECTOR2& position) { player_spawn_pos = position; }

	auto GetSceneCameraPos() const -> const D3DXVECTOR2& { return camera_pos; }

	auto SpawnMob() -> const std::shared_ptr<class MobActor>;

private:
	bool LoadMap(const uint& map_idx);

	void LoadBgm(const std::string& path);
	void LoadBackground(const std::string& path);

private:
	bool in_game = false;
	std::shared_ptr<class AudioClip> background_music;

	std::map<std::string, std::shared_ptr<class Actor>> prefab_actors;
	
	std::map<std::string, std::list<std::shared_ptr<class Actor>>> actors;
	uint current_map_id = 0;
	uint update_map_id = 0;
	D3DXVECTOR2 player_spawn_pos{ 300.0f,500.0f };

	float map_x_min = 0.f;
	float map_x_max = 1280.f;
	float map_y_min = 0.f;
	float map_y_max = 720.f;

	float camera_x_min = 0.f;
	float camera_x_max = 0.f;
	float camera_y_min = 0.f;
	float camera_y_max = 0.f;

	D3DXVECTOR2 camera_pos{ 0,0 };

	std::string current_mob_name;
};

template<typename T>
inline auto Scene::GetTypeActors() -> const std::list<std::shared_ptr<class Actor>>&
{
	static_assert(std::is_base_of<Actor, T>::value, "Class does not inherit Actor class.");

	return GetTagActors(typeid(T).name());
}

template<typename T>
inline auto Scene::CreateActor() -> const std::shared_ptr<T>
{
	static_assert(std::is_base_of<Actor, T>::value, "Class does not inherit Actor class.");
	
	auto actor = std::make_shared<T>(context);

	return actor;
}

template<typename T>
inline void Scene::RegisterActor(const std::shared_ptr<T>& actor)
{
	static_assert(std::is_base_of<Actor, T>::value, "Class does not inherit Actor class.");

	actors[typeid(T).name()].emplace_back(std::static_pointer_cast<Actor>(actor));
	actor->Start();
}

template<typename T>
inline auto Scene::Instantiate(const std::string& prefab_name) -> const std::shared_ptr<T>
{
	static_assert(std::is_base_of<Actor, T>::value, "Class does not inherit Actor class.");

	auto actor = CreateActor<T>();
	*actor = *std::static_pointer_cast<T>(prefab_actors[prefab_name]);
	RegisterActor(actor);
	
	return actor;
}

template<typename T>
inline void Scene::RegisterPrefab(const std::shared_ptr<T>& actor, const std::string& name)
{
	static_assert(std::is_base_of<Actor, T>::value, "Class does not inherit Actor class.");

	prefab_actors[name] = std::static_pointer_cast<Actor>(actor);
}
