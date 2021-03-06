#include "BossLevel1.h"
#include "MediaManager/SFXPlayer.h"
#include <json.hpp>
using json = nlohmann::json;

BossLevel1::BossLevel1(json cfg):cfg(cfg)
{
	max_enemy_count = 10;
	boss = new Commander(450, 10, VectorMath::Vdirection::UP);
	boss_health = sf::Vector2f(boss->getHP(), 0.0f);
	//Reserve Vectors
	prepareContainers();
	randomizeEnemyPos();
	//Load 
	loadTextures();
	Scene::s_window->setView(*Scene::s_view);
	player = new Player(125, 0.5f, 100.0f);
	//Set player Texture
	player->setTexture(player_textures[0], sf::Vector2f(0.5f, 0.5f));
	//create the camera
	Scene::s_view->zoom(1.8f);
	camera = new CameraFollowHorzScroll(Scene::s_window, Scene::s_view, player, sf::Vector2f(100.0f, 0.0f), true);
	//create the starfield
	// starfield = new starfield(Scene::s_window, Scene::s_view, s_view->getSize().x, 25, 1.4f);
	r_starfield = new RadialStarField(Scene::s_window, Scene::s_view, 300, 8);
	hud = new HUDPanel(Scene::s_window, Scene::s_view, player, &player_score, boss_health, boss_health, 0.8f);
	//Music
	music = new MusicPlayer("song4", true);

	pSc = new PauseSc(s_window, s_view, &paused, cfg["sceneName"]);

	// Boss
	boss_texture.loadFromFile("res/Sprites/enemies/bosses/BossCommander_2.png");
	boss->setTexture(boss_texture, sf::Vector2f(0.6f, 0.6f));
	boss->setPosition(VectorMath::getViewportLowerRightPos() - sf::Vector2f(boss->getSize().x, Scene::s_view->getSize().y) / 2.0f);
	boss->setProjectileTexture(boss_projectile_texture, sf::Vector2f(0.6f, 0.6f));
	boss->setBulletParameters(10, 20);

	// Minion Spawner
	json spawner_cfg1 = "{\"spawn_type\":\"Spinner\",\"spawn_range\":[0.0,0.0],\"member\":{\"health\":20,\"speed\":7,\"timer\":7.0,\"scale\":[0.4,0.4],\"rot_speed\":5.0,\"bullet\":{\"damage\":10,\"speed\":15,\"scale\":[0.4,0.4],\"textures\":[\"res/Sprites/projectiles/laser_ball_purple.png\"]},\"textures\":[\"res/Sprites/enemies/spinner_purple.png\"]}}"_json;
	json spawner_cfg2 = "{\"spawn_type\":\"Spinner\",\"spawn_range\":[0.0,0.0],\"member\":{\"health\":30,\"speed\":7,\"timer\":7.0,\"scale\":[0.3,0.3],\"rot_speed\":1.0,\"bullet\":{\"damage\":5,\"speed\":15,\"scale\":[0.4,0.4],\"textures\":[\"res/Sprites/projectiles/laser_ball_red.png\"]},\"textures\":[\"res/Sprites/enemies/spinner_red.png\"]}}"_json;
	spawners.emplace_back(new SimpleSpawner(world_enemies,world_position,total_length, spawner_cfg1));
	spawners.emplace_back(new SimpleSpawner(world_enemies,world_position,total_length, spawner_cfg2));
	// spawners.emplace_back(new MemoryManagement::EnemySpawner<Spinner>(world_enemies, world_position, total_length));

	f_in = new Composit::Fade(s_window, s_view, false, 4);
	f_in->trigger();

}

BossLevel1::~BossLevel1()
{
	delete player;
	delete r_starfield;
	delete camera;
	delete hud;
	delete music;
	delete boss;
	delete pSc;
}

void BossLevel1::update(float delta_time)
{
	pollEvents();
	if (player->getHP() > 0) {
		if (!paused) {
			
			if (boss->getHP() <= 0) {
				//? next lvl
				//!placeholder for finishing the level
				Scene::s_view->zoom(1.0f/1.8f);
				Scene::s_window->setView(*Scene::s_view);
				SaveSys::saveLevel(cfg["sceneName"], player_score);
				m_finished = true; //? goto next level
				// m_return = true;
			} else {
				camera->follow();
				updateEntityCollisions();
				updateEntities();
			}
			
		}
		
		pSc->update();
		//!need to call here because sometimes m_return is checked even after the player died
		if (m_return) {
			//! exit level
			m_return = false;
			Scene::s_view->zoom(1.0f / 1.8f);
			Scene::s_window->setView(*Scene::s_view);
			SceneManagement::goBackToMainMenu();
		}

	}
	else {
		//!game over
		Scene::s_view->zoom(1.0f/1.8f);
		Scene::s_window->setView(*Scene::s_view);
		SaveSys::saveState(cfg["sceneName"]);
		SFX::play(SFXlib::GameOver, 100.0f);
		SceneManagement::goToGameOver();
		// SceneManagement::goBackToMainMenu();

	}

}

void BossLevel1::render()
{
	r_starfield->draw();
	Scene::s_window->draw(boss->getSprite());
	// renders objects
	for (auto& enemy : world_enemies) {
		Scene::s_window->draw(enemy->getSprite());
	}
	for (auto& playerbullet : player_bullets) {
		Scene::s_window->draw(playerbullet->getSprite());
	}
	for (auto& item_ : world_items) {
		Scene::s_window->draw(item_->getSprite());
	}
	for (auto& enemybullet : world_enemy_bullets) {
		Scene::s_window->draw(enemybullet->getSprite());
	}
	Scene::s_window->draw(player->getSprite());
	// displays objects on the screen
	hud->draw();

	if (paused) {
		if (pSc->draw()) {
			m_return = true;
		}
	}

	//Transition
	f_in->draw();
}

void BossLevel1::prepareContainers()
{
	player_textures.reserve(3);
	projectile_textures.reserve(3);
	//Reserving Entities and Projectiles
	player_bullets.reserve(80);
	world_enemies.reserve(150);
	world_enemy_bullets.reserve(300);
	grid_pos.reserve(max_enemy_count);
}

void BossLevel1::loadTextures()
{
	//Player Textures
	player_textures.emplace_back();
	player_textures.back().loadFromFile("res/Sprites/player/player_base.png");
	player_textures.emplace_back();
	player_textures.back().loadFromFile("res/Sprites/player/player_upgrade_1.png");

	//Projectile Textures
	//Player
	projectile_textures.emplace_back();
	projectile_textures.back().loadFromFile("res/Sprites/projectiles/laser_red.png");
	//boss
	boss_projectile_texture.loadFromFile("res/Sprites/projectiles/BossCommander_missile.png");

}

void BossLevel1::updateEntityCollisions()
{
	//Collisions
	//Boss
	for (size_t i = 0; i < player_bullets.size(); i++) {
		if (boss->collidesWith(player_bullets[i])) {
			boss->hurt(player_bullets[i]);
			//deletes the object in the heap
			delete player_bullets[i];
			//deletes the slot that was used for it in the array of bullets
			player_bullets.erase(player_bullets.begin() + i);
		}
	}
	//Minions
	world_enemies.erase(std::remove_if(world_enemies.begin(), world_enemies.end(), [&](Enemy* enemy) {
		//check for collisions with playerbullets
		bool is_dead = false;
		for (size_t i = 0; i < player_bullets.size(); i++) {
			if (enemy->collidesWith(player_bullets[i])) {
				enemy->hurt(player_bullets[i]);
				//deletes the object in the heap
				delete player_bullets[i];
				//deletes the slot that was used for it in the array of bullets
				player_bullets.erase(player_bullets.begin() + i);
			}

		}
		if (enemy->getHP() <= 0) {
			is_dead = true;
			//delete the memory in the heap
			delete enemy;
			player_score += 10;

			SFX::play(SFXlib::EnemyDestroy);
		}

		return is_dead; }), world_enemies.end());
	//items
	world_items.erase(remove_if(world_items.begin(), world_items.end(), [&](GameItem* item_) {
		bool collided = player->collidesWith(item_);
		if (collided) {
			item_->applyEffect(player);
		}
		return collided; }), world_items.end());
	//Player
	world_enemy_bullets.erase(std::remove_if(world_enemy_bullets.begin(), world_enemy_bullets.end(), [&](EnemyBullet* enemy_bullet) {
		bool collided = player->collidesWith(enemy_bullet);
		if (collided) {
			player->hurt(enemy_bullet);


			// for testing music access
			if (player->getHP() <= 0) {
				music->stop();
			}
		}

		return collided; }), world_enemy_bullets.end());
}

void BossLevel1::updateEntities()
{
	
	boss->updateStates();
	boss_health.x = boss->getHP();
	player->updatePhysics();

	if (boss->canShoot()) {
		spawnEnemyBullet(boss);

	}
	//enemies
	for (uint16_t i = 0; i < world_enemies.size(); i++) {
		if (world_enemies[i]->getCenterPos().x <= grid_pos[i].x) {
			world_enemies[i]->idle();
			if (world_enemies[i]->canShoot()) {
				world_enemies[i]->lookAt(player->getCenterPos());
				spawnEnemyBullet(world_enemies[i]);
			}
		}
		else {
			world_enemies[i]->move();
		}
	}

	//bullets
	for (auto& bullet : player_bullets) {
		bullet->move();
	}
	for (auto& bullet : world_enemy_bullets) {
		bullet->move();
	}

	//spawners
	for (auto& spawner : spawners) {
		spawner->update();
	}


	for (auto& collector : collectors) {
		collector->update();
	}

}

void BossLevel1::randomizeEnemyPos()
{
	for (uint16_t i = 0; i < max_enemy_count; i++) {
		grid_pos.emplace_back(sf::Vector2f(Scene::s_view->getCenter().x + rand() % (static_cast<int>(Scene::s_view->getSize().x+1)), 0.0f));
	}
}
