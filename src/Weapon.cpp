#include "Weapon.h"
#include "GameObject.h"


const std::map<int, std::function<void(std::shared_ptr<GameObject>)>> WeaponBase::weaponList
{
	//{0,[](shared_ptr<GameObject> obj, int level){obj->addComponent<>();}},

};
