#include "Player.h"
#include "GameObject.h"
#include "Global.h"
#include "Timer.h"
#include "Projectile.h"
#include "UI_CooldownSprite.h"
#include "UI_AbilityBar.h"
#include "PlayerUI.h"
#include "ProgressBar.h"
#include "Input.h"
#include "StatUpgrade.h"
#include "Sow_Projectile.h"
#include "Reap_Projectile.h"
#include "Papyrmancer_Reap.h"
#include "Papyrmancer_Sow.h"
#include "AutoWeapon.h"
#include "BoomerangProjectile.h"
#include "OrbitProjectile.h"





void StatGroup::UpdateValues(std::shared_ptr<StatUpgrade> upgrade) {
	flat = upgrade->getFlat();
	mult = upgrade->getMult();
}


/// <returns>Player's max HP modified by any upgrades to their stats.</returns>
const int PlayerStats::MaxHp() const { 
	return maxHp.evaluate();
}
/// <returns>Player's health regen modified by any upgrades to their stats.</returns>
const int PlayerStats::HealthRegen()const {
	return healthRegen.evaluate();
}
/// <returns>Player's defence modified by any upgrades to their stats.</returns>
const int PlayerStats::Defence()const {
	return defence.evaluate();
}
/// <returns>Player's speed modified by any upgrades to their stats.</returns>
const float PlayerStats::Speed() const { 
	return speed.evaluate();
}
/// <returns> given damage amount, modified by any upgrades to their stats.</returns>
const int PlayerStats::Damage(int originalDamage) const { 
	return damage.evaluate(originalDamage);
}
/// <returns>given base attack speed, modified by any upgrades to their stats.</returns>
const float PlayerStats::AttackSpeed(float originalSpeed) const {
	return attackSpeed.evaluate_f(originalSpeed);
}
const float& PlayerStats::Size() const { return size; }
// returns current player xp.
const int& PlayerStats::CurXP() const { return curXp; }
/// <returns> Adds given xp value to curxp, and handles leveling up. takes into account XP stat..</returns>
const void PlayerStats::AddXP(int baseXp) {
	curXp += xp.evaluate(baseXp);
	while (curXp >= xpToNext) {
		curXp -= xpToNext;
		++level;
		
		//xpToNext = GetNextXP();   get next level xp value.
	}
	// re calculate stats here
}

std::shared_ptr<StatUpgrade> PlayerStats::AddUpgrade(StatType type) {

	for (auto& upgrade :statUpgrades) { // iterate upgrade array

		if (!upgrade) { // if array element is null, fill with new upgrade, then exit.
			upgrade = std::make_shared<StatUpgrade>(type);
			RecalculateStats();
			return upgrade;
		}
		else if (upgrade->type == type) { // if upgrade is contained already, level the existing one.
			upgrade->LevelUp();
			break;
		}

	}

	RecalculateStats();
	return nullptr;
	
}

void PlayerStats::RecalculateStats() {
	for (const auto& upgrade : statUpgrades) {
		if (!upgrade) return; // return early, as array is filled 
							  //from lowest to highest index

		switch (upgrade->type) { 
			case StatType::Health:
				maxHp.UpdateValues(upgrade);
				onMaxHealthChange.invoke(maxHp.evaluate());
				break;

			case StatType::HealthRegen:
				healthRegen.UpdateValues(upgrade);
				break;

			case StatType::Speed:
				speed.UpdateValues(upgrade);
				break;

			case StatType::Damage:
				damage.UpdateValues(upgrade);
				break;

			case StatType::Defence:
				defence.UpdateValues(upgrade);
				break;

			default:
				break;
		}
	}
}


Player::Player(std::string className) :className(className), hitFlickerTimer(hitFlickerDuration) {

}

void Player::init() {
	auto sharedThis = shared_from_this();
	playerUI = std::make_shared<PlayerUI>(sharedThis);
	stats = std::make_shared<PlayerStats>(className,statUpgradeHolder);
	hitFlickerTimer.getEndEvent().subscribe(sharedThis, &Player::ResetHitFlicker);

	stats->RecalculateStats();// unlikely, but in case something changes before init call.

	int maxHp = stats->MaxHp();
	// have to set these in player because we need values from stats, and stats and playerUI need eachothers values.
	playerUI->healthBar = playerUI->healtBarObj->addComponent<ProgressBar>(
		sf::IntRect{{ 0,0 }, { 256,32 }},
		"../assets/sprites/shapes/bl_square_128.png",
		true,
		0,
		maxHp);
	auto healthBarS = playerUI->healthBar.lock();

	healthBarS->updateBar(maxHp);
	healthBarS->setFillColor(sf::Color(192, 64, 64, 128));

	stats->getMaxHpChangeEvent().subscribe(playerUI->healthBar, &ProgressBar::setRange);


	// DEBUG
	AddWeapon(0, 0);
	AddWeapon(1, 1);
	AddWeapon(2, 2);

	CreateAbilities(window);

	
	
	

	

}

void Player::HandleRegen(float deltatime) {
	healthRegenTimer -= deltatime;
	if (healthRegenTimer <= 0) { // if timer is up,
		healthRegenTimer = 1; // reset timer
		int& curHp = stats->getCurHp(); // get cur and max hp
		const int& maxHp = stats->MaxHp();
		if (curHp < maxHp) { // if not at or above max hp,
			curHp += stats->HealthRegen(); // heal player
			if (curHp > maxHp) { // if heal went over max hp, set to max hp.
				curHp = maxHp;
			}
			playerUI->UpdateHealthbar(curHp);
		}
	}

}


void Player::update(float deltatime) {

	//DEBUG STUFF
	if (Input::GetKeyDown(sf::Keyboard::Scancode::Num5)) AddStat(StatType::Health);
	if (Input::GetKeyDown(sf::Keyboard::Scancode::Space)) {
		std::cout << "\n";
		for (auto& item : weaponHolder) {
			if(auto itemp = item.lock())
				std::cout << "Derived type: " << typeid(*itemp).name() << "\n";
				
		}
	}
	// END OF DEBUG
	hitFlickerTimer.update(deltatime);
	HandleRegen(deltatime);

	direction = { 0,0 };
	if (Input::GetAction("up"))  
		direction.y = -1; 

	else if (Input::GetAction("down"))  
		direction.y = 1; 

	if (Input::GetAction("left"))  
		direction.x = -1; 

	else if (Input::GetAction("right"))  
		direction.x = 1; 
	
	// edge case for holding opposites.
	if (Input::GetActionUp("up") && Input::GetAction("down"))
		direction.y = 1;

	else if (Input::GetActionUp("down") && Input::GetAction("up"))
		direction.y = -1;

	if (Input::GetActionUp("left") && Input::GetAction("right"))
		direction.x = 1;

	else if (Input::GetActionUp("right") && Input::GetAction("left"))
		direction.x = -1;


	
	if (direction.lengthSquared() < 0.05f) return; //only move if direction held.
	direction = direction.normalized();
	parent->move( direction * stats->Speed() * deltatime );
	playerView->setCenter(parent->getPosition()); // set playerView center to player
	UpdateFacingDirection();
}

void Player::takeDamage(int _damage){
	int& curHp = stats->getCurHp();
	curHp -= _damage;
	_isVulnrable = false;
	hitFlickerTimer.start();
	parent->getSprite()->setColor(hitColour);
	playerUI->UpdateHealthbar(curHp);
}

void Player::EnableBarUI(int value) { playerUI->SetSpriteBarEnabled(value); }

void Player::UpdateFacingDirection() {
	if (direction.x == 0) return; // dont flip if input released.
	bool newFacingDirection = direction.x < 0; // signbit returns true if number is negative.
	if (newFacingDirection != facingDirection) { // only flip if facing direction is not the same as before.
		facingDirection = newFacingDirection; // store new facing direction
		parent->scaleObject(-1, 1); // flip player.
	}
}

/// <summary>
/// iterates weapon list and adds to the first empty slot, or levels up the weapon if you already have it.
/// </summary>
/// <param name="weaponIndex"></param>
void Player::AddWeapon(int weaponIndex) {
	int firstEmpty = -1;
	for (size_t i = 0; i < weaponIndices.size();i++)
	{
		if (weaponIndices[i] == weaponIndex) { // if
			weaponHolder[i].lock()->LevelUp(); 
			return;
		}
		if (weaponIndices[i] == -1 && firstEmpty == -1) { // if this is the first empty slot, store it.
			firstEmpty = i;
		}
	}
	if (firstEmpty == -1) { std::cerr << "\nTried to add weapon with full weaponHolder"; return; }
	std::shared_ptr<WeaponBase> weapon = WeaponBase::CreateWeapon(weaponIndex, parent).lock();
	weaponHolder[firstEmpty] = weapon;
	weaponIndices[firstEmpty] = weaponIndex;
	playerUI->UI_AddWeapon(firstEmpty, weapon);
	
}
/// <summary>
/// manually adds a weapon to a slot. if you already have the weapon, instead levels the existing one up.
/// </summary>
/// <param name="slotIndex"></param>
/// <param name="weaponIndex"></param>
void Player::AddWeapon(int slotIndex, int weaponIndex) {
	if (slotIndex > 6 || slotIndex < 0) return;// check slotIndex bounds

	for (int i = 0; i < weaponIndices.size();++i) // if weapon already exists on player, level it up instead.
		if (weaponIndices[i] == weaponIndex) { 
			weaponHolder[i].lock()->LevelUp(); 
			
			return; 
		}
	

	std::shared_ptr<WeaponBase> weapon = WeaponBase::CreateWeapon(weaponIndex, parent).lock();
	weaponHolder[slotIndex] = weapon;
	weaponIndices[slotIndex] = weaponIndex;
	playerUI->UI_AddWeapon(slotIndex, weapon);
}

void Player::AddStat(StatType type)
{
	std::shared_ptr<StatUpgrade> ptr = stats->AddUpgrade(type);
	if (ptr)
		playerUI->spriteBar->statBar->LinkStat(ptr);
}

void Player::CreateAbilities(std::shared_ptr<sf::RenderWindow> window) {

	sf::IntRect abilityBarIconRect = sf::IntRect{ {0,0},{64,64} };		

	 auto weaponQ = parent->addComponent<Papyrmancer_Sow>();
	 playerUI->spriteBar->abilityBar->LinkAbility(0,abilityBarIconRect, weaponQ.lock());


	 auto weaponE = parent->addComponent<Papyrmancer_Reap>();
	 playerUI->spriteBar->abilityBar->LinkAbility(1, abilityBarIconRect, weaponE.lock());


	//auto weaponR = WeaponBase::CreateWeapon(1, parent);
	//playerUI->spriteBar->abilityBar->LinkAbility(2, abilityBarIconRect, weaponR.lock());

	abilityHolder[0] = weaponQ;
	abilityHolder[1] = weaponE;
	//abilityHolder[2] = weaponR;
}

const json& PlayerStats::LoadInfoFromJson(std::string className) {
	auto& json = GameDataLoader::getPlayerClass(className);

	if (json.contains("health")) {
		maxHp = StatGroup(json["health"]);
		curHp = maxHp.getBase();
		
	}
	else { maxHp = StatGroup(); std::cerr << "\nHealth value not found for: " << className; }

	if (json.contains("defence")) {
		defence = StatGroup(json["defence"]);
	}
	else { defence = StatGroup(); std::cerr << "\Defence value not found for: " << className; }

	if (json.contains("healthRegen")) {
		healthRegen = StatGroup(json["healthRegen"]);
	}
	else { healthRegen = StatGroup(); std::cerr << "\nHealth Regen value not found for: " << className; }

	if (json.contains("speed")) {
		speed = StatGroup(json["speed"]);
	}
	else { speed = StatGroup(); std::cerr << "\Speed value not found for: " << className; }

	if (json.contains("xp")) {
		xp = StatGroup(0,0,json["xp"]);
	}
	else { xp = StatGroup(); }

	if (json.contains("attackSpeed")) {
		std::cout << "attackspeed: " << json["attackSpeed"];
		attackSpeed = StatGroup(0,0,json["attackSpeed"]);
	}
	else { attackSpeed = StatGroup(); }



	return json;

}