#include "Player.h"
#include "GameObject.h"
#include "Global.h"
#include "Timer.h"
#include "Projectile.h"
#include "UI_CooldownSprite.h"
#include "UI_AbilityBar.h"
#include "ProgressBar.h"
#include "Input.h"

#include "Sow_Projectile.h"
#include "Reap_Projectile.h"
#include "Papyrmancer_Reap.h"
#include "Papyrmancer_Sow.h"
#include "AutoWeapon.h"
#include "BoomerangProjectile.h"
#include "OrbitProjectile.h"


Player::Player() : hitFlickerTimer(hitFlickerDuration) {
	stats = std::make_shared<PlayerStats>(100, 300);
	abilityBarUI = std::make_shared<AbilityBar>();
	weaponBarUI = std::make_shared<WeaponBar>();
	statUpgradeUI= std::make_shared<StatUpgradeBar>();
}
void Player::init() {
	hitFlickerTimer.getEndEvent().subscribe(shared_from_this(), &Player::ResetHitFlicker);
	CreateAbilities(window);
	sf::IntRect& rect = sf::IntRect{ {0,0},{256,32} };
	healtBarObj = GameObject::Create("../assets/sprites/cardboard.png", rect, 130);
	healtBarObj->getSprite()->SetRepeated(true);
	healtBarObj->setPosition(0, 1048);
	int maxHp = stats->getMaxHp();
	healthBar = healtBarObj->addComponent<ProgressBar>(
		rect,
		"../assets/sprites/shapes/bl_square_128.png",
		true,
		0,
		maxHp);
	auto healthBarS = healthBar.lock();
	healthBarS->updateBar(maxHp);
	healthBarS->setFillColor(sf::Color(192, 64, 64, 128));

	AddWeapon(0, 0);
	AddWeapon(1, 0);
	AddWeapon(2, 1);
	AddWeapon(3, 1);
	AddWeapon(4, 2);
	AddWeapon(5, 2);

	weaponBarUI->Hide();

}

void Player::update(float deltatime) {
	hitFlickerTimer.update(deltatime);
	direction = { 0,0 };
	if (Input::GetAction("up"))  
		direction.y = -1; 

	if (Input::GetAction("down"))  
		direction.y = 1; 

	if (Input::GetAction("left"))  
		direction.x = -1; 

	if (Input::GetAction("right"))  
		direction.x = 1; 
	
	// edge case for holding opposites.
	if (Input::GetActionUp("up") && Input::GetAction("down"))
		direction.y = 1;

	if (Input::GetActionUp("down") && Input::GetAction("up"))
		direction.y = -1;

	if (Input::GetActionUp("left") && Input::GetAction("right"))
		direction.x = 1;

	if (Input::GetActionUp("right") && Input::GetAction("left"))
		direction.x = -1;


	
	if (direction.lengthSquared() < 0.05f) return; //only move if direction held.
	direction = direction.normalized();
	parent->move( direction * stats->Speed() * deltatime );
	playerView->setCenter(parent->getPosition()); // set playerView center to player, and re assign to actually move playerView.
}

void Player::takeDamage(int _damage){
	int& curHp = stats->getCurHp();
	curHp -= _damage;
	_isVulnrable = false;
	hitFlickerTimer.start();
	parent->getSprite()->setColor(hitColour);
	healthBar.lock()->updateBar(curHp);
}


void Player::UpdateFacingDirection() {
	if (direction.x == 0) return; // dont flip if input released.
	bool newFacingDirection = direction.x < 0; // signbit returns true if number is negative.
	if (newFacingDirection != facingDirection) { // only flip if facing direction is not the same as before.
		facingDirection = newFacingDirection; // store new facing direction
		parent->scaleObject(-1, 1); // flip player.
	}
}

void Player::AddWeapon(int slotIndex, int weaponIndex) {
	if (slotIndex > 6 || slotIndex < 0) return;// check slotIndex bounds
	std::shared_ptr<WeaponBase> weapon = WeaponBase::CreateWeapon(0, parent).lock();
	weaponHolder[slotIndex] = weapon;
	weaponIndices[slotIndex] = weaponIndex;
	weaponBarUI->LinkWeapon(slotIndex, weapon);
}

void Player::CreateAbilities(std::shared_ptr<sf::RenderWindow> window) {

	sf::IntRect abilityBarIconRect = sf::IntRect{ {0,0},{128,128} };		

	 auto weaponQ = parent->addComponent<Papyrmancer_Sow<Sow_Projectile>>(
		std::make_shared<WeaponStats>(1, 1000, 0, 32, 0.0666f, 1),
		window);
	 abilityBarUI->LinkWeapon(0,abilityBarIconRect, std::static_pointer_cast<WeaponBase>(weaponQ.lock()));


	 auto weaponE = parent->addComponent<Papyrmancer_Reap<Reap_Projectile>>(
		 std::make_shared<WeaponStats>(1, 1500, 1000, 32, 1.5f, 1),
		 window
		 );
	abilityBarUI->LinkWeapon(1, abilityBarIconRect, std::static_pointer_cast<WeaponBase>(weaponE.lock()));


	auto weaponR = WeaponBase::CreateWeapon(1, parent);
	abilityBarUI->LinkWeapon(2, abilityBarIconRect, std::static_pointer_cast<WeaponBase>(weaponR.lock()));

	abilityHolder[0] = weaponQ;
	abilityHolder[1] = weaponE;
	abilityHolder[2] = weaponR;


	abilityDescription = GameObject::Create(120);
	abilityDescription->setPosition(1528, 652);
	abilityDescription->setAsUI(true);

	abilityDescription->addComponent<UI_AbilityDescription>(window);

}
