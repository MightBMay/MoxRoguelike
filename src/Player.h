#pragma once
#include "Component.h"
#include "Weapon.h"

class WeaponBase;
class WeaponStats;
class GameObject;


class Player : public Component {
public:

	int health = 0;
	float speed = 300.0f;
	float size = 32;
	sf::Vector2f direction{ 0,0 };
	// 0 == right, 1 = left
	bool facingDirection = false;

	Player(int health) : health(health) {}

	virtual void CreateWeapons(std::shared_ptr<sf::RenderWindow> window, std::array<std::weak_ptr<WeaponBase>, 4>& weaponArray, std::array<std::shared_ptr<GameObject>, 4>& cdSprites);

	virtual void takeDamage(int damage) {
		health -= damage;
		std::cout << "Hit. hp: " << health << '\n';
		if (health <= 0) {
			std::cout << " Player has died" << std::endl;
		}


	
	}
	void update(float deltatime) override;
	void ProcessEvent(const std::optional<sf::Event>& event)override;
	virtual void Destroy() override {}

private:

	void UpdateFacingDirection();


};