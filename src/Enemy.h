#pragma once

class Component;
class GameObject;
class Player;


namespace Enemies {
	class Enemy : public Component, public std::enable_shared_from_this<Enemy> {
	public:

		int level = 1;

		int _maxHp = 0;
		int _curHp = 0;
		int _damage = 0;
		float _attackSpeed = 0;

		float _speed = 0;
		float _size = 86;
		float halfSize = 43;// used for some checks, was a waste to calculate per frame per enemy.
		float _attackSize = 0;

		int xpValue = 0;

		// max number of attacks in 1 second
		float attackTimer = 0;

		sf::Vector2f direction{ 0,0 };

		bool facingDirection = 0; // 0== right, 1 == left;




		/// <summary>
		/// base enemy constructor
		/// </summary>
		Enemy(int level, std::string enemyType) :level(level), enemyType(enemyType) {

		}


		float getSpeed() {
					// can invert speed to make it simulate knockback.
			return _speed * (hitFlickerTimer.inProgress() ? -0.45f : 1);
		}

		virtual bool takeDamage(int _damage);
		virtual void OnDeath();
		virtual void Attack(float deltaTime, GameObject* player);

		static void SetPlayer(GameObject* player);
		static GameObject* GetPlayer() { return _playerObj; }


		virtual void log() {
			std::cout <<
				"\nEnemy: " <<
				"\nlevel: " << level <<
				"\nhp: " << _curHp << "/ " << _maxHp <<
				"\ndamage: " << _damage <<
				"\nspeed: " << _speed <<
				"\nsize: " << _size <<
				"\nxp: " << xpValue;

		}

		virtual void Destroy() override;

		virtual void init() override;
		virtual void update(float deltatime) override;


	protected:
		std::string enemyType = "";
		inline void ResetHitFlicker();

		Timer hitFlickerTimer{ hitFlickerDuration, false };

		virtual void LoadInfoFromJson(std::string enemyType);



	private:
		static constexpr float hitFlickerDuration = 0.125f;
		static constexpr sf::Color hitColour = sf::Color(255, 155, 155, 255);


		static inline GameObject* _playerObj = nullptr;
		static inline Player* _playerComponent = nullptr;
		void UpdateFacingDirection();


	};
}