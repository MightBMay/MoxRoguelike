#pragma once

class Component;
class GameObject;
class Player;

class Enemy : public Component, public std::enable_shared_from_this<Enemy> {
public:
	
	int level = 1;

	int _maxHp=0;
	int _curHp=0;
	int _damage=0;
	float _attackSpeed=0;

	float _speed=0;
	float _size=86;
	float halfSize = 43;// used for some checks, was a waste to calculate per frame per enemy.
	float _attackSize=0;

	int xpValue = 0;
	
	// max number of attacks in 1 second
	float attackTimer = 0;
	
	sf::Vector2f direction{ 0,0 };

	bool facingDirection = 0; // 0== right, 1 == left;




	/// <summary>
	/// base enemy constructor
	/// </summary>
	Enemy(int level, std::string enemyType) :level(level),enemyType(enemyType){
		
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

	virtual void setSprite();

	virtual void log() {
		std::cout <<
			"\nEnemy: " <<
			"\nlevel: " << level <<
			"\nhp: " << _curHp << "/ " << _maxHp <<
			"\ndamage: " << _damage <<
			"\nspeed: " << _speed<<
			"\nsize: " << _size<<
			"\nxp: "<< xpValue;

	}

	virtual void Destroy() override;

	virtual void init() override;
	virtual void update(float deltatime) override;


protected:
	std::string enemyType= "";
	inline void ResetHitFlicker();

	Timer hitFlickerTimer{hitFlickerDuration, false};

	// amount of hp gained per level.
	virtual int hpPerLevel() const = 0;
	// amount of _damage gained per level
	virtual int damagePerLevel() const =0;
	/// <summary>
	/// attack speed (returns number of attacks per second).
	/// </summary>
	virtual float attackSpeed() const = 0;

	/// <summary>
	/// used to scale the attack hitbox visual. KEEP IN MIND this can be influenced depending on texture size for the hitbox.
	/// </summary>
	virtual const float attackSize() const = 0;
	virtual float speed() const = 0;
	virtual float size() const = 0;

	virtual void LoadInfoFromJson(std::string enemyType) {
		auto& json = GameDataLoader::getEnemy(enemyType);
		if (json.contains("hp")) {
			_maxHp = json["hp"];
			_curHp = _maxHp;
		}
		else { std::cerr << "\n\"hp\" not found/defined in json for " << enemyType; }

		if (json.contains("damage")) {
			_damage= json["damage"];
		}
		else { std::cerr << "\nDamage not found/defined in json for " << enemyType; }

		if (json.contains("attack speed")) {
			_attackSpeed = 1/ json["attack speed"].get<float>();
		}
		else { std::cerr << " not found/defined in json for " << enemyType; }

		if (json.contains("attack size")) {
			_attackSize = json["attack size"];
		}
		else { std::cerr << " \n\"attack size\"not found/defined in json for " << enemyType; }

		if (json.contains("speed")) {
			_speed = json["speed"];
			
		}
		else { std::cerr << "\n\"speed\" not found/defined in json for " << enemyType; }

		if (json.contains("size")) {
			_size = json["size"];
			halfSize = _size / 2.0f;
			
		}
		else { std::cerr << "\n\"size \" not found/defined in json for " << enemyType; }

		if (json.contains("xp")) {
			xpValue = json["xp"];
		}else{ std::cerr << "\n\"xp \" not found/defined in json for " << enemyType; }

	}



private:
	static constexpr float hitFlickerDuration = 0.125f;
	static constexpr sf::Color hitColour = sf::Color(255, 155, 155, 255);


	static inline GameObject* _playerObj = nullptr;
	static inline Player* _playerComponent = nullptr;
	void UpdateFacingDirection();


};