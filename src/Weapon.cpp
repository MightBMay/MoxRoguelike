#include "Weapon.h"
#include "GameObject.h"
#include "AutoWeapon.h"
#include "OrbitWeapon.h"
#include "BoomerangWeapon.h"
#include "AoeProjectile.h"
#include "OrbitProjectile.h"
#include "BoomerangProjectile.h"


const std::map<int, std::function<std::weak_ptr<WeaponBase>(std::shared_ptr<GameObject>)>> WeaponBase::weaponList
{
	{0,[](const shared_ptr<GameObject> obj) {
			auto stats = std::make_shared<WeaponStats>(3, 500, 650, 32, 1.5f, 30);

			return obj->addComponent<AutoWeapon>(stats); 
		}
	},

	{1,[](const shared_ptr<GameObject> obj) {
			auto stats = std::make_shared<WeaponStats>(3, 900, 1000, 32, 0.5f, 35);
			return obj->addComponent<BoomerangWeapon>(stats); 
		}
	},

	{2,[](const shared_ptr<GameObject> obj) {
			auto stats = std::make_shared<WeaponStats>(3,900,1000,32,3,5);
			return obj->addComponent<OrbitWeapon>(stats); 
		}
	},

};
