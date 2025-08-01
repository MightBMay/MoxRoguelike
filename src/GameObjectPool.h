#pragma once
#include <vector>
#include "GameObject.h"
#include <iostream>

template <typename BaseComponentType>
class GameObjectPool
{
	std::vector<std::shared_ptr<GameObject>> _pool;
	size_t next_available = 0;

public:
	GameObjectPool() {

	}

	void init(size_t quantity) {
		for (int i = 0; i < quantity; ++i) {
			auto obj = GameObject::Create();
			obj->setActive(false);
			_pool.push_back(obj);
		}
		
	}
	int size() { return _pool.size(); }

	template <typename ComponentType, typename... Args>
	std::shared_ptr<GameObject> make(Args&&... args) {
		static_assert(
			std::is_base_of_v<BaseComponentType, ComponentType>,
			"ComponentType must inherit from BaseComponentType"
			);

		if (next_available >= _pool.size()) {
			_pool.push_back(GameObject::Create());
		}
		auto obj = _pool[next_available++];
		if (!obj) std::cout << "NULL: "<<next_available;
		obj->setActive(true);
		obj->removeAllComponents();
		obj->addComponent<ComponentType>(std::forward<Args>(args)...);
		return obj;

	}

	void release(std::shared_ptr<GameObject> obj) {
		obj->setActive(false);
		obj->removeSprite();
		auto it = std::find_if(_pool.begin(), _pool.end(),
			[obj](const auto& ptr) { return ptr == obj; });

		if (it != _pool.end() && it >= _pool.begin() + next_available) {
			std::swap(*it, _pool[next_available - 1]);
			next_available--;
		}
	}
	



private:

};

