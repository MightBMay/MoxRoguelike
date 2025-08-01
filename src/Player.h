#pragma once
#include "Component.h"
#include "Weapon.h"

class GameObject;
class AbilityBar;

class Player : public Component {
public:

	int health = 0;
	float speed = 300.0f;
	float size = 32;
	sf::Vector2f direction{ 0,0 };
	// 0 == right, 1 = left
	bool facingDirection = false;

	Player(int health);

	virtual void CreateWeapons(std::shared_ptr<sf::RenderWindow> window);

	virtual void takeDamage(int damage) {
		health -= damage;
	}
	void update(float deltatime) override;
	void ProcessEvent(const std::optional<sf::Event>& event)override;
	virtual void Destroy() override {}

protected:
	std::shared_ptr<AbilityBar> abilityBarUI;

private:

	void UpdateFacingDirection();


};