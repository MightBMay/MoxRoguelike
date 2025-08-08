#pragma once
#include "Component.h"
#include "Weapon.h"



class Timer;
class GameObject;
class AbilityBar;
class UI_AbilityDescription;
class ProgressBar;

class Player : public Component, public std::enable_shared_from_this<Player> {
public:
	int maxHealth;
	int curHealth;
	float speed = 300.0f;
	float size = 32;
	sf::Vector2f direction{ 0,0 };
	// 0 == right, 1 = left
	bool facingDirection = false;

	Player(int maxhealth);

	static bool isVulnrable() { return _isVulnrable; }

	virtual void CreateWeapons(std::shared_ptr<sf::RenderWindow> window);
	std::shared_ptr<AbilityBar>& getAbilityBar() { return abilityBarUI; }
	virtual void takeDamage(int _damage);
	void init()override;
	void update(float deltatime) override;
	virtual void Destroy() override {}

protected:
	std::shared_ptr < GameObject> abilityDescription;
	std::shared_ptr<AbilityBar> abilityBarUI;

	std::shared_ptr<GameObject> healtBarObj;
	std::weak_ptr<ProgressBar> healthBar;
	Timer hitFlickerTimer{ hitFlickerDuration, false};

	void ResetHitFlicker() {
		parent->getSprite()->setColor(sf::Color::White);
		_isVulnrable = true;
	}


private:
	static inline bool _isVulnrable = true;
	static constexpr float hitFlickerDuration = 0.125f;
	static constexpr sf::Color hitColour = sf::Color(255, 155, 155, 255);
	void UpdateFacingDirection();


};