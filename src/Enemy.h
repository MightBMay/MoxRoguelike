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
		Enemy(int level, std::string enemyType);


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
		// this is inefficient, but also simple and functional.
		// ideally, we would store one sprite rect and one 
		// SpriteAnimation per enemy subclass, then we could load from json only one time.
		// right now we have a rect PER CLASS and load from json PER ENEMY. ideally fix this.
			//^^
			// I guess since rn enemies also load their stats per enemy, we don't really lose too 
			// much time to loading sprite data per enemy. ideally we load both stats and 
			// sprite data once per subclass though.
		sf::IntRect spriteRect{};



		std::string enemyType = "";
		inline void ResetHitFlicker();

		Timer hitFlickerTimer{ hitFlickerDuration, false };
		virtual inline float getStatIncreaseMultiplier() {
			return std::pow(1.5, (level - 1) / 5.0f);

		}
		virtual const json& LoadInfoFromJson(std::string enemyType);
		virtual void LoadAudioDataFromJson(const json& audioData) {}
		static inline  std::shared_ptr<sf::Texture> enemyAtlasTexture;
		static inline bool atlasLoaded;

	private:
		static inline const sf::Vector2f* playerPos;
		static constexpr float hitFlickerDuration = 0.125f;
		static constexpr sf::Color hitColour = sf::Color(255, 64, 64, 255);

		static inline GameObject* _playerObj = nullptr;
		static inline Player* _playerComponent = nullptr;
		void UpdateFacingDirection();
	};
}