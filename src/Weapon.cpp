#include "pch.h"
#include "GameObject.h"
#include "Player.h"
#include "Animation.h"

#include "Weapon.h"
#include "AutoWeapon.h"
#include "OrbitWeapon.h"
#include "BoomerangWeapon.h"
#include "SpiralWeapon.h"

WeaponBase::WeaponBase(std::string& weaponName) :name(weaponName) {
	// technically re assigns this every time we make a weapon, but wtv
	playerStats = Player::getStats();
};

const float& WeaponBase::getAttackSpeed() const { return playerStats->AttackSpeed(attackSpeed); }



using weaponConstructor = std::function<std::weak_ptr<WeaponBase>(std::shared_ptr<GameObject>)>;
using obj = const std::shared_ptr<GameObject>;

const std::map<int, weaponConstructor> WeaponBase::weaponList
{
	{0,[](obj player) { return player->addComponent<AutoWeapon>(std::string("Auto Weapon")); }},

	{1,[](obj player) { return player->addComponent<BoomerangWeapon>(); } },

	{2,[](obj player) { return player->addComponent<OrbitWeapon>(); } },
	
	{3,[](obj  player) {return player->addComponent<SpiralWeapon>(std::string("Spiral Weapon") ); }},

};
					   
const json& WeaponBase::LoadInfoFromJson() {
	const auto& json = GetJsonData();
#pragma region Load projectile stats and info



	if (json.contains("damage"))// check if json defined a dmg.
		damage = json["damage"]; // if so, get dmg
	else {
		damage = 0; // if not, set a default value.
		// for some stats, you really should define a value. log that there is none.
		std::cerr << "\n\"damage\" not found in JSON for: " << name; 
	}

	// the above pattern repeats.

	if (json.contains("speed"))
		speed = json["speed"].get<float>();
	else {
		speed = 0;
		std::cerr << "\n \"speed\" not found in JSON for: " << name;
	}


	if (json.contains("attackSpeed")) {
		attackSpeed = 1 / json["attackSpeed"].get<float>(); // attack speed number represents firings per second. 
	}
	else {
		attackSpeed = 0;
		std::cerr << "\n \"attackSpeed\" not found in JSON for: " << name;
	}

	if (json.contains("range")) {
		range = json["range"];
		range *= range; // distance checks use sqr magnitude, so sqr range to counteract that.
	}

	else range = std::numeric_limits<float>::max(); // unlimited range by default. 

	if (json.contains("pierce"))
		pierce = json["pierce"];
	else pierce = std::numeric_limits<int>::max(); // unlimited pierce by default.

	if (json.contains("projSize")){
		projRadius = json["projSize"];
		projRadius *= projRadius; // distance checks are squared again.
	}
	else projRadius = 1024; // 32x32 default size

	if (json.contains("description")) 
		description = json["description"];
	else { description = "DESCRIPTION FOR WEAPON "+ name +"NOT FOUND"; }
#pragma endregion

#pragma region Load sprite/animation data



	if (!json.contains("projectile data")) return json; // can return early if none defined.
	const auto& projData = json["projectile data"];

	if (projData.contains("textureStartPos")) { // start position on the projectile atlas.
		auto vec = projData["textureStartPos"].get<std::vector<int>>();
		projectileRect.position = { vec[0],vec[1] };
	}
	else { 
		projectileRect.position = {};
		std::cerr << "\nNo texture start position was found for "<<name<<". using default sprite";
	}

	if (projData.contains("spriteSize")) { // dimensions of one frame of the animation/sprite.
		auto vec = projData["spriteSize"].get < std::vector<int>>();
		projectileRect.size = {vec[0],vec[1]};
	}
	else {
		projectileRect.size = { 32,32 };
		std::cerr << "\n Sprite size not defined for " << name << "defaulting to 32x32";
	}

	animation = std::make_shared<SpriteAnimation>(projectileRect);

	if (projData.contains("animation data")) {
		animation->LoadFromJson(projData["animation data"]);
	}

#pragma endregion


	return json;
}