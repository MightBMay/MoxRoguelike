#include "CameraController.h"
#include "GameObject.h"
void CameraController::CenterOnTarget() {
    if (!target) return;

    // Get target position in world coordinates
    sf::Vector2f targetPos = target->getPosition();

    // Convert target position to playerView coordinates
    sf::Vector2i targetViewPos = w.mapCoordsToPixel(targetPos, v);
    sf::Vector2f viewCenter = v.getCenter();
    sf::Vector2f viewSize = v.getSize();
    sf::Vector2f windowSize = static_cast<sf::Vector2f>(w.getSize());

    // Calculate screen edges in playerView coordinates
    float leftEdge = viewCenter.x - viewSize.x / 2;
    float rightEdge = viewCenter.x + viewSize.x / 2;
    float topEdge = viewCenter.y - viewSize.y / 2;
    float bottomEdge = viewCenter.y + viewSize.y / 2;

    // Calculate threshold distances
    float xThreshold = viewSize.x * hEdgeThreshold;
    float yThreshold = viewSize.y * vEdgeThreshold;

    // Check if target is near edges and calculate new center
    sf::Vector2f newCenter = viewCenter;

    // Horizontal movement
    if (targetPos.x < leftEdge + xThreshold) {
        newCenter.x = targetPos.x - xThreshold + viewSize.x / 2;
    }
    else if (targetPos.x > rightEdge - xThreshold) {
        newCenter.x = targetPos.x + xThreshold - viewSize.x / 2;
    }

    // Vertical movement
    if (targetPos.y < topEdge + yThreshold) {
        newCenter.y = targetPos.y - yThreshold + viewSize.y / 2;
    }
    else if (targetPos.y > bottomEdge - yThreshold) {
        newCenter.y = targetPos.y + yThreshold - viewSize.y / 2;
    }

    // Smoothly move camera toward new center
    sf::Vector2f delta = newCenter - viewCenter;
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    if (distance > 1.0f) {  // Only move if significant distance
        v.setCenter(viewCenter + delta * std::min(baseMoveSpeed / distance, 1.0f));
        window->setView(v);  // Update window's playerView
    }
}