#include "stdafx.h"
#include "Scene.h"
#include "Scene/stdafx_Scene.h"

Scene::Scene(Context * context)
	:ISubsystem(context)
{
	
}

Scene::~Scene()
{
}

bool Scene::Initialize()
{
	auto mapdata = context->GetSubsystem<GameData>()->GetMapData(0);

	auto title_screen = CreateActor<BackgroundActor>();
	title_screen->SetRenderSizeTextureOrActor(true);
	title_screen->SetTexture("../_assets/Textures/StartScreen.png");
	title_screen->SetRenderSizeTextureOrActor(false);
	title_screen->SetScale(1280, 720);
	RegisterPrefab(title_screen, "TitleScreen");

	auto background = CreateActor<BackgroundActor>();
	background->SetScale(2000, 1600);
	RegisterPrefab(background, "Background");

	auto cursor = CreateActor<CursorActor>();
	RegisterPrefab(cursor, "Cursor");

	auto tomb = CreateActor<Actor>();
	tomb->SetTexture("../_assets/Textures/tomb.png");
	tomb->AddComponent<RigidBodyComponent>();
	auto tomb_audio = tomb->AddComponent<AudioSourceComponent>();
	tomb_audio->SetAudioClip("../_assets/Audios/Tombstone.mp3");
	tomb_audio->SetPlaying(true);
	tomb->SetRenderSizeTextureOrActor(true);
	RegisterPrefab(tomb, "Tomb");

	auto item = CreateActor<ItemDropActor>();
	item->SetTexture("../_assets/Textures/item1.png");
	item->SetScale(100, 30);
	RegisterPrefab(item, "item");

	auto skill1 = CreateActor<SkillActor>();
	skill1->SetScale(D3DXVECTOR2(300, 300));
	auto skillani1 = skill1->AddComponent<AnimatorComponent>();
	skillani1->AddAnimation("../_assets/Animations/Skill/SmashSwing1.xml");
	auto skillaudio1 = skill1->AddComponent<AudioSourceComponent>();
	skillaudio1->SetAudioClip("../_assets/Audios/Skill/attack1.mp3");
	skillaudio1->SetPlaying(true);
	RegisterPrefab(skill1, "skill1");

	auto skill2 = CreateActor<SkillActor>();
	skill2->SetScale(D3DXVECTOR2(300, 130));
	auto skillani2 = skill2->AddComponent<AnimatorComponent>();
	skillani2->AddAnimation("../_assets/Animations/Skill/SmashSwing2.xml");
	auto skillaudio2 = skill2->AddComponent<AudioSourceComponent>();
	skillaudio2->SetAudioClip("../_assets/Audios/Skill/attack2.mp3");
	skillaudio2->SetPlaying(true);
	RegisterPrefab(skill2, "skill2");

	auto skill3 = CreateActor<SkillActor>();
	skill3->SetScale(D3DXVECTOR2(320, 300));
	auto skillani3 = skill3->AddComponent<AnimatorComponent>();
	skillani3->AddAnimation("../_assets/Animations/Skill/SmashSwing3.xml");
	auto skillaudio3 = skill3->AddComponent<AudioSourceComponent>();
	skillaudio3->SetAudioClip("../_assets/Audios/Skill/attack3.mp3");
	skillaudio3->SetPlaying(true);
	RegisterPrefab(skill3, "skill3");

	auto damageFont = CreateActor<DamageFontActor>();
	damageFont->SetScale(D3DXVECTOR2(50, 50));
	RegisterPrefab(damageFont, "font");

	auto mob = CreateActor<MobActor>();
	RegisterPrefab(mob,"mob");

	auto player = CreateActor<PlayerActor>();
	auto animator = player->GetComponent<AnimatorComponent>();
	animator->AddAnimation("../_assets/Animations/Player/Stand.xml");
	animator->AddAnimation("../_assets/Animations/Player/Walk.xml");
	animator->AddAnimation("../_assets/Animations/Player/Jump.xml");
	animator->AddAnimation("../_assets/Animations/Player/Attack.xml");
	animator->AddAnimation("../_assets/Animations/Player/Prone.xml");
	animator->AddAnimation("../_assets/Animations/Player/Dead.xml");
	animator->AddAnimation("../_assets/Animations/Player/Rope.xml");
	animator->SetCurrentAnimation("Stand");

	player->AddComponent<RigidBodyComponent>();

	player->SetPosition(D3DXVECTOR2(-400,-200));
	player->SetScale(50, 70);

	RegisterPrefab(player, "player");

	auto platform = CreateActor<PlatformActor>();
	platform->SetRenderSizeTextureOrActor(false);
	RegisterPrefab(platform,"platform");

	auto level_up_effect = CreateActor<EffectActor>();
	level_up_effect->GetAnimator()->AddAnimation("../_assets/Animations/LevelUp.xml");
	auto levelupbgm = level_up_effect->AddComponent<AudioSourceComponent>();
	levelupbgm->SetAudioClip("../_assets/Audios/LevelUp.mp3");
	RegisterPrefab(level_up_effect, "level_up");

	auto attack_hit = CreateActor<EffectActor>();
	attack_hit->GetAnimator()->AddAnimation("../_assets/Animations/Attack1Hit.xml");
	auto hit_audio = attack_hit->AddComponent<AudioSourceComponent>();
	hit_audio->SetAudioClip("../_assets/Audios/Skill/Hit.mp3");
	hit_audio->SetPlaying(true);
	RegisterPrefab(attack_hit, "attack_hit");

	auto portal = CreateActor<PortalActor>();
	RegisterPrefab(portal,"portal");

	auto quick_slot = CreateActor<QuickSlotUIActor>();
	quick_slot->SetTexture("../_assets/Textures/QuickSlot.png");
	quick_slot->SetPosition(500,-300);
	quick_slot->SetIsUIActor(true);
	quick_slot->SetRenderSizeTextureOrActor(true);
	RegisterPrefab(quick_slot, "QuickSlotUI");

	auto font_actor1 = CreateActor<Actor>();
	font_actor1->SetIsUIActor(true);
	font_actor1->AddComponent<NumberFontComponent>();
	RegisterPrefab(font_actor1, "ItemAmmountFont");

	auto hpbar = CreateActor<BarActor>();
	hpbar->SetIsUIActor(true);
	hpbar->SetScale(1, 18);
	hpbar->SetXMin(0.0f);
	RegisterPrefab(hpbar, "hpbar");

	auto level_actor = CreateActor<Actor>();
	auto level_font = level_actor->AddComponent<NumberFontComponent>();
	level_actor->SetIsUIActor(true);
	level_font->SetFontTextureDir("../_assets/Textures/Font/font_level.png");
	RegisterPrefab(level_actor, "LevelFont");

	auto status_ui = CreateActor<StatusUIActor>();
	status_ui->SetIsUIActor(true);
	status_ui->SetPosition(0, -200);
	status_ui->SetTexture("../_assets/Textures/UI/mainbar.png");
	RegisterPrefab(status_ui, "status_ui");

	auto wall = CreateActor<WallActor>();
	wall->SetScale(1.f, 200.f);
	wall->SetRenderSizeTextureOrActor(false);
	RegisterPrefab(wall, "wall");

	TitleScreen();

	return true;
}

void Scene::Update(const float & delta_time_sec)
{
	if (in_game)
	{
		if (current_map_id != update_map_id)
		{
			LoadMap(update_map_id);
			current_map_id = update_map_id;
		}
	}

	/*
		Actor Update
	*/
	for (auto& actor_tags : actors)
	{
		auto& actor_tags_list = actor_tags.second;

		for (auto iter = actor_tags_list.begin(); iter != actor_tags_list.end();)
		{
			if ((*iter)->IsActive())
			{
				(*iter)->Update(delta_time_sec);
				iter++;
			}
			else
			{
				(*iter)->End();
				iter = actor_tags_list.erase(iter);
			}
		}
	}

	background_music->Update();

	if (in_game)
	{
		auto player_pos = GetPlayer()->GetPosition();
		camera_pos.x = Math::Clamp(player_pos.x, camera_x_min, camera_x_max);
		camera_pos.y = Math::Clamp(player_pos.y, camera_y_min, camera_y_max);

		auto input = context->GetSubsystem<Input>();

		if (input->GetKeyDown(KeyCode::R))
		{
			GetPlayer()->SetPosition(player_spawn_pos.x,
				actors[typeid(BackgroundActor).name()].front()->GetTexture()->GetHeight() - player_spawn_pos.y + 15.f);
		}
		if (input->GetKeyDown(KeyCode::B))
		{
			auto wall = Instantiate<WallActor>("wall");
			wall->SetPosition(player_pos);
		}

		if (input->GetKeyDown(KeyCode::P))
		{
			auto hp = GetPlayer()->GetComponent<StatusComponent>()->GetCurrentHP();
			auto mp = GetPlayer()->GetComponent<StatusComponent>()->GetCurrentMP();
			auto y = GetPlayer()->GetPosition().y;
			std::cout << "CurrentHP: " << hp << "\t CurrentMP: " << mp << "\t Current y: " << y<< std::endl;
		}

		if (input->GetKeyDown(KeyCode::Esc))
			TitleScreen();

	}
	else
	{
		auto input = context->GetSubsystem<Input>();
		if (input->GetKeyDown(KeyCode::Enter))
			InGameScreen();
	}
}

void Scene::TitleScreen()
{
	in_game = false;

	actors.clear();

	LoadBgm("../_assets/Audios/BGM/Title.mp3"); 
	
	camera_pos.x = 0.f; camera_pos.y = 0.f;
	Instantiate<BackgroundActor>("TitleScreen");
}

void Scene::InGameScreen()
{
	in_game = true;

	actors.clear();
	auto player = Instantiate<PlayerActor>("player");
	LoadMap(1);
	player->SetPosition(300, 500);
}

auto Scene::GetPlayer() -> const std::shared_ptr<class PlayerActor>
{
	return std::static_pointer_cast<PlayerActor>(actors["class PlayerActor"].front());
}

auto Scene::GetPrefab(const std::string & name) -> const std::shared_ptr<class Actor>&
{
	return prefab_actors[name];
}

auto Scene::SpawnMob() -> const std::shared_ptr<class MobActor>
{
	auto mob_data = context->GetSubsystem<GameData>()->GetMobData(current_mob_name);
	
	auto mob = Instantiate<MobActor>("mob");
	mob->SetScale(mob_data.scale);
	auto mob_ani = mob->AddComponent<AnimatorComponent>();
	mob_ani->AddAnimation(mob_data.idle_animation_path);
	mob_ani->AddAnimation(mob_data.walk_animation_path);
	mob_ani->AddAnimation(mob_data.dead_animation_path);
	auto mob_status = mob->AddComponent<StatusComponent>();
	mob_status->SetATK(mob_data.atk);
	mob_status->SetHPMax(mob_data.hp_max);
	mob_status->SetEXP(mob_data.exp);
	for (const auto& item : mob_data.drop_table)
		mob->AddDropTable(item.first, item.second);

	return mob;
}

bool Scene::LoadMap(const uint & map_idx)
{
	auto map_data = context->GetSubsystem<GameData>()->GetMapData(map_idx);

	this->map_x_min = map_data.map_x_min;
	this->map_x_max = map_data.map_x_max;
	this->map_y_min = map_data.map_y_min;
	this->map_y_max = map_data.map_y_max;

	for (auto iter = actors.begin(); iter != actors.end();)
	{
		if ((*iter).first == typeid(PlayerActor).name())
		{
			iter++;
			continue;
		}

		iter = actors.erase(iter);
	}
	

	LoadBgm(map_data.bgm_path);
	LoadBackground(map_data.bg_path);

	camera_x_min = map_x_min + Settings::Get().GetWidth()*0.5f;
	camera_x_max = map_x_max - Settings::Get().GetWidth()*0.5f;
	camera_y_min = map_y_min + Settings::Get().GetHeight()*0.5f;
	camera_y_max = map_y_max - Settings::Get().GetHeight()*0.5f;

	auto bg = context->GetSubsystem<ResourceManager>()->Load<Texture2D>(map_data.bg_path);
	auto player = GetPlayer();
	player->SetPosition(player_spawn_pos.x, bg->GetHeight() - player_spawn_pos.y + 15.f);

	current_mob_name = map_data.mob_name;
	
	Instantiate<StatusUIActor>("status_ui");
	Instantiate<QuickSlotUIActor>("QuickSlotUI");

	for (const auto& platform : map_data.platforms)
	{
		auto actor = Instantiate<PlatformActor>("platform");
		actor->SetPosition(platform.first[0],platform.first[1]);
		actor->SetScale(platform.first[2], 1.f);
		actor->SetMaxMobCount(platform.second);
	}

	for (const auto& portal : map_data.portals)
	{
		auto actor = Instantiate<PortalActor>("portal");
		actor->SetPosition(portal.portal_pos);
		actor->SetMapID(portal.teleport_map_id);
		actor->SetTeleportPosition(portal.teleport_pos);
	}

	for (const auto& wall : map_data.walls)
	{
		auto actor = Instantiate<WallActor>("wall");
		actor->SetPosition(wall.wall_pos);
		actor->SetScale(1.f, wall.height);
	}

	auto wall = Instantiate<WallActor>("wall");
	wall->SetPosition(map_x_min, (map_y_min + map_y_max)*0.5f);
	wall->SetScale(1.f, map_y_max - map_y_min);

	wall = Instantiate<WallActor>("wall");
	wall->SetPosition(map_x_max, (map_y_min + map_y_max)*0.5f);
	wall->SetScale(1.f, map_y_max - map_y_min);

	return true;
}

void Scene::LoadBgm(const std::string & path)
{
	if (background_music)
		background_music->Stop();

	background_music = context->GetSubsystem<ResourceManager>()
		->Load<AudioClip>(path);
	background_music->Play();
	background_music->SetLoop(true);
}

void Scene::LoadBackground(const std::string & path)
{
	if (actors[typeid(BackgroundActor).name()].size() != 0)
	{
		actors[typeid(BackgroundActor).name()].clear();
	}

	auto background_texture = context->GetSubsystem<ResourceManager>()->Load<Texture2D>(path);

	auto background = CreateActor<BackgroundActor>();
	background->SetTexture(path);
	background->SetRenderSizeTextureOrActor(true);
	background->SetPosition(background_texture->GetWidth() * 0.5f, background_texture->GetHeight() * 0.5f);
	RegisterActor(background);
}
