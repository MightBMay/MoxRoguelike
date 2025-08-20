#include "Weapon.h"
#include "GameObject.h"
#include "AutoWeapon.h"
#include "OrbitWeapon.h"
#include "BoomerangWeapon.h"
#include "AoeProjectile.h"
#include "OrbitProjectile.h"
#include "BoomerangProjectile.h"
#include "Player.h"

WeaponBase::WeaponBase(std::string& weaponName) :name(weaponName) {
	// technically re assigns this every time we make a weapon, but wtv
	playerStats = Player::getStats();
};

const float& WeaponBase::getAttackSpeed() const { return playerStats->AttackSpeed(attackSpeed); }


const std::map<int, std::function<std::weak_ptr<WeaponBase>(std::shared_ptr<GameObject>)>> WeaponBase::weaponList
{
	{0,[](const shared_ptr<GameObject> obj) { return obj->addComponent<AutoWeapon>(std::string("Auto Weapon")); }},

	{1,[](const shared_ptr<GameObject> obj) { return obj->addComponent<BoomerangWeapon>(); } },

	{2,[](const shared_ptr<GameObject> obj) { return obj->addComponent<OrbitWeapon>(); } },

};
					   
const json& WeaponBase::LoadInfoFromJson() {
	auto& json = GetJsonData();
	if (json.contains("damage")) // check if json defined a dmg.
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

	else range = std::numeric_limits<int>::max(); // unlimited range by default. 

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
	

	return json;
}