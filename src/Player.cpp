#include "pch.h"

#include "Enemy.h"
#include "GameObject.h"
#include "Player.h"
#include "PlayerUI.h"
#include "StatUpgrade.h"


#include "Papyrmancer.h"
#include "Ninja.h"

using playerObj = const std::shared_ptr<GameObject>;
const std::map<int, std::function<std::weak_ptr<Player>(playerObj)>> Player::playerClassList
{
	{0,[](playerObj obj) {return obj->addComponent<playerClasses::Papyrmancer>(); }},

	{1,[](playerObj obj) {return obj->addComponent<playerClasses::Ninja>(); }},

};




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


void PlayerStats::LoadInfoFromJson(const json& json) {
	
	if (json.contains("health")) {
		maxHp = StatGroup(json["health"]);
		curHp = maxHp.getBase();

	}
	else { maxHp = StatGroup(); std::cerr << "\nHealth value not found"; }

	if (json.contains("defence")) {
		defence = StatGroup(json["defence"]);
	}
	else {
		defence = StatGroup(); std::cerr << "\nDefence value not found";
	}

	if (json.contains("healthRegen")) {
		healthRegen = StatGroup(json["healthRegen"]);
	}
	else {
		healthRegen = StatGroup(); std::cerr << "\nHealth Regen value not found";
	}

	if (json.contains("speed")) {
		speed = StatGroup(json["speed"]);
	}
	else {
		speed = StatGroup(); std::cerr << "\nSpeed value not found";
	}

	if (json.contains("xp")) {
		xp = StatGroup(0, 0, json["xp"]);
	}
	else { xp = StatGroup(); }

	if (json.contains("attackSpeed")) {
		attackSpeed = StatGroup(0, 0, json["attackSpeed"]);
	}
	else { attackSpeed = StatGroup(); }



}


Player::Player(std::string className) :className(className), hitFlickerTimer(hitFlickerDuration) {

}

void Player::init() {
	auto sharedThis = shared_from_this();
	
	playerUI = std::make_shared<PlayerUI>(sharedThis);
	stats = std::make_shared<PlayerStats>(className,statUpgradeHolder);
	LoadInfoFromJson(className);
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


	//// DEBUG
	//AddWeapon(0, 3);
	//AddWeapon(1, 1);
	AddWeapon(2, 2);

	//auto& tempWep = weaponHolder[0].lock();
	//for (int i = 0; i < 10; ++i) {
	// tempWep->UpdateStats();
	//}
	//std::cout << tempWep->isMaxLevel();

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

std::shared_ptr<GameObject> Player::CreatePlayerClass(int classIndex) {
	auto it = playerClassList.find(classIndex); // search map for index
		//if index not found, return and error log.
	if (it == playerClassList.end()) {
		std::cerr << "weapon index out of bounds / not found.";
		return std::shared_ptr<GameObject>();
	}
	std::shared_ptr<GameObject> playerObj = GameObject::Create();
	it->second(playerObj);// this actually calls the specific player constructor.
	Projectile::player = playerObj;
	Enemies::Enemy::SetPlayer(playerObj.get());

	return playerObj; // calls function stored in enemylist which 
}

void Player::update(float deltatime) {

	//DEBUG STUFF
	if (Input::GetKeyDown(sf::Keyboard::Scancode::Num5)) AddStat(StatType::Health);

	// END OF DEBUG
	hitFlickerTimer.update(deltatime);
	HandleRegen(deltatime);

	MovePlayer(deltatime);
}

void Player::MovePlayer(float deltaTime) {
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
	parent->move(direction * stats->Speed() * deltaTime);
	playerView->setCenter(parent->getPosition()); // set playerView center to player
	UpdateFacingDirection();
}

void Player::takeDamage(int _damage){
	int& curHp = stats->getCurHp();
	curHp -= _damage;
	curHp = std::max(curHp, 0);
	if(curHp <=0){
		// player death here
	}

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
		if (weaponIndices[i] == weaponIndex) { // if already has weapon, level it up
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
	playerUI->UI_AddWeapon(firstEmpty, weaponIndex, weapon);
	
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
	playerUI->UI_AddWeapon(slotIndex, weaponIndex, weapon);
}

void Player::AddStat(StatType type)
{
	std::shared_ptr<StatUpgrade> ptr = stats->AddUpgrade(type);
	if (ptr)
		playerUI->spriteBar->statBar->LinkStat(ptr);
}

bool Player::isWeaponMaxLevel(int weaponIndex) {
	int slotNumber = hasWeapon(weaponIndex);
	if (slotNumber < 0 || slotNumber >= weaponHolder.size()) return false;

	auto weaponS = weaponHolder[slotNumber].lock();
	if (!weaponS) return false;
	return weaponS->isMaxLevel();

}
int Player::hasWeapon(int weaponIndex) {
	for (int i =0;i<weaponIndices.size();++i)
		if (weaponIndices[i] == weaponIndex) return i;
	return -1;
}

bool Player::isStatUpgradeMaxLevel(StatType type) {
	int slotNumber = hasStat(type);
	if (slotNumber < 0 || slotNumber >= statUpgradeHolder.size()) return false;

	auto statS = statUpgradeHolder[slotNumber];
	if (!statS) return false;
	return statS->isMaxLevel();
}
int Player::hasStat(StatType type) {
	for (int i = 0; i<statUpgradeHolder.size(); ++i)
		
		if (statUpgradeHolder[i] && statUpgradeHolder[i]->type == type) return i;
	
	return -1;
}

const void Player::AddXP(int baseXp) {
	int levelsGained = stats->AddXp(baseXp);
	UI_LevelUpSelection::numRemainingLevels = levelsGained;


}


const json& Player::LoadInfoFromJson(std::string className) {
	const auto& json = GameData::getPlayerClass(className);

	stats->LoadInfoFromJson(json);

	if (json.contains("spritePath") && json.contains("spriteSize")) {
		auto rawSize = json["spriteSize"].get<std::vector<int>>();
		sf::Vector2i size = { rawSize[0],rawSize[1] };

		parent->setSprite(json["spritePath"], { {},size });
		GameObjectManager::getInstance().add(parent, 10);
		parent->setOrigin(size.x / 2, size.y / 2);

		if (json.contains("AnimationData")) {

		}

	}
	else { std::cerr << "\nsprite not loaded for: " << className; }




	return json;
}