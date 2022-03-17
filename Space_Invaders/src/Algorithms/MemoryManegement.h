#pragma once
#include <vector>
#include "Algorithms/Utilities.h"
#include "Algorithms/MathUtils.h"
#include "SceneManager/Scene.h"
//Forward declaration
class Enemy;

namespace MemoryManagement {
	class BaseEnemySpawner {
	protected:
		std::vector<Enemy*>& buffer_alias;
	public: 
		BaseEnemySpawner(std::vector<Enemy*>& enemy_buffer) :buffer_alias(enemy_buffer) {
	
		}
		virtual ~BaseEnemySpawner(){}
		virtual void update(){}
		virtual void spawn(){}

	};
	template<typename T>
	class EnemySpawner:public BaseEnemySpawner {
		
	public:
		//prototype of the enemy later move this to their respective classes
		//Type-1 small
		sf::Texture enemy_texture1;
		sf::Texture projectile_texture1;
		uint8_t t1_speed = 10;
		uint16_t t1_health = 20;
		//Type-2 big
		sf::Texture enemy_texture2;
		sf::Texture projectile_texture2;
		uint8_t t2_speed = 5;
		uint16_t t2_health = 30;
		float timer_duration = 1.0f;
		//Control- Remember to set the duration of the timer
		//Todo
		//Add a bullet type, so it can know what kind of bullet it needs to spawn
		//or specify the bullet type withing a function for that enemy class
		Control::GameTimer timer;
		EnemySpawner(std::vector<Enemy*>& enemy_buffer):BaseEnemySpawner(enemy_buffer){
			timer.setDuration(timer_duration);
			std::string path_texture_1("res/Sprites/enemy1.png");
			std::string path_texture_2("res/Sprites/enemy2.png");
			std::string path_projectile_texture1("res/Sprites/bullet.png");
			std::string path_projectile_texture2("res/Sprites/bullet2.png");
			///json

			//
			enemy_texture1.loadFromFile(path_texture_1);
			enemy_texture2.loadFromFile(path_texture_2);
			projectile_texture1.loadFromFile(path_projectile_texture1);
			projectile_texture2.loadFromFile(path_projectile_texture2);

		}

		void update()override {
			timer.start();
			if (timer.timeOut()) {
				spawn();
			}
				
		}
		void spawn()override {
			//health,speed,direction
			int range = 1 - 0 + 1;
			int type = rand() % range + 0;

			//Only two types
			if (type == 0) {
				buffer_alias.emplace_back(new T(t1_health,t1_speed, VectorMath::Vdirection::LEFT));
				buffer_alias.back()->setTexture(enemy_texture1);
				//buffer_alias.back()->setProjectileTexture(enemy_texture1);
				buffer_alias.back()->setProjectileTexture(projectile_texture1);
				
			}
			else {
	
				buffer_alias.emplace_back(new T(t2_health, t2_speed, VectorMath::Vdirection::LEFT));
				buffer_alias.back()->setTexture(enemy_texture2);
				//buffer_alias.back()->setSprite().setScale(0.3, 0.3);
				buffer_alias.back()->setProjectileTexture(projectile_texture2);

			}
		
			buffer_alias.back()->setPosition(sf::Vector2f(VectorMath::getViewportLowerRightPos().x,SceneManagement::Scene::s_view->getSize().y));
		}
	};
	class BasicCollector {
	public:
		BasicCollector(){}
		virtual ~BasicCollector(){}
		virtual void update(){}
		

	};

	template<typename T>
	class Collector:public BasicCollector {
	protected:
		std::vector<T*>& buffer_alias;
	public:
		Collector(std::vector<T*>& buffer):buffer_alias(buffer) {

		}
		void update() override {
			
			buffer_alias.erase(std::remove_if(buffer_alias.begin(), buffer_alias.end(),
				[&](T* obj) {
					//first delete the heap mem
					bool out_of_range = leftViewport(obj);
					if (out_of_range) {
						delete obj;}
					return out_of_range; }),buffer_alias.end());

	
		}
		bool leftViewport(const T* obj) {
			sf::Vector2f top_left_pos = VectorMath::getViewPortTopLeftPos();
			sf::Vector2f lower_right_pos = VectorMath::getViewportLowerRightPos();
			//x axis
			bool far_from_right_side = obj->getTopLeftPos().x > lower_right_pos.x + obj->getSize().x;
			bool far_from_left_side = obj->getTopLeftPos().x < top_left_pos.x - obj->getSize().x;
			//y axis
			bool far_from_top = obj->getTopLeftPos().y < top_left_pos.y - obj->getSize().y;
			bool far_from_bottom = obj->getTopLeftPos().y > lower_right_pos.y + obj->getSize().y;

			return (far_from_right_side || far_from_left_side || far_from_top || far_from_bottom);
		}
	};

}

