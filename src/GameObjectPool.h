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
	GameObjectPool(bool expandable = false) : expandable(expandable){

	}

	void init(size_t quantity, int layer) {
		auto& manager = GameObjectManager::getInstance();
		for (int i = 0; i < quantity; ++i) {
			auto obj = GameObject::Create(layer);
			obj->setPoolIndex(i);
			obj->setActive(false);
			_pool.push_back(obj);
		}
		
	}
	int size() { return _pool.size(); }

	template <typename ComponentType, typename... Args>
	std::shared_ptr<GameObject> make(int renderLayer = 0, Args&&... args) {
		static_assert(
			std::is_base_of_v<BaseComponentType, ComponentType>,
			"ComponentType must inherit from BaseComponentType"
			);

		if (next_available >= _pool.size()) {
			if (!expandable)
				return nullptr;
			// can be toggled here to allow expansion.
			auto& obj = GameObject::Create(renderLayer);
			obj->setPoolIndex(_pool.size() - 1);
			_pool.push_back(obj);
			return obj;
			
		}
		auto obj = _pool[next_available++];
		obj->setActive(true);
		obj->removeAllComponents();
		obj->addComponent<ComponentType>(std::forward<Args>(args)...);
		GameObjectManager::getInstance().add(obj, renderLayer);
		
		return obj;

	}

	void release(std::shared_ptr<GameObject> obj) {
		obj->setActive(false);
		GameObjectManager::getInstance().remove(obj);
		obj->removeSprite();

		int index = obj->getPoolIndex();
		if (index >= next_available) return; // already inactive

		// Swap and update indices in one go
		if (index != next_available - 1) { // Only swap if not already the last active
			std::swap(_pool[index], _pool[next_available - 1]);
			_pool[index]->setPoolIndex(index);
			_pool[next_available - 1]->setPoolIndex(next_available - 1);
		}

		next_available--;
	}
	



private:
	bool expandable = false;
};

