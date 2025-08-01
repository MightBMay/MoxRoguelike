#pragma once
#include "Global.h"
class GameObject;

class CameraController {
public:

	void CenterOnTarget();
	void SetTarget(std::shared_ptr<GameObject> newTarget) { target = newTarget; }
private:

	const float vEdgeThreshold = 0.2f;  // 20% from screen edges
	const float hEdgeThreshold = 0.25f;  // 20% from screen edges
	const float baseMoveSpeed = 5.0f;     // Camera movement speed


	std::shared_ptr<GameObject> target;
	sf::View& v = *playerView;
	sf::RenderWindow& w = *window;
};