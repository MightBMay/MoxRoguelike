#include "pch.h"
#include "GameObject.h"

TrailRenderer::TrailRenderer(float duration, float thickness, sf::Color startColour, sf::Color endColour)
    : duration(duration), startColour(startColour),endColour(endColour), thickness(thickness) {
}

void TrailRenderer::update(float deltaTime) {

    timeSinceLastUpdate += deltaTime;// increment timer
    if (timeSinceLastUpdate < updateInterval) return; // do not continue until timer has passed an interval
    timeSinceLastUpdate = 0; // reset timer and continue adding points.

	auto now = std::chrono::steady_clock::now();
	// Add new point
    if(points.size()<128)
	    points.push_back({ parent->getPosition(), now });

	// Remove expired points
	auto it = points.begin();
	while (it != points.end()) {
		auto elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - it->creationTime).count();
		if (elapsed > duration) {
			it = points.erase(it);
		}
		else {
			++it;
		}
	}
}

void TrailRenderer::init() {
    std::shared_ptr<sf::Drawable> drawable = std::static_pointer_cast<sf::Drawable>(shared_from_this());
    renderable = std::make_shared<Renderable>(drawable, nullptr);

   // LoadShader("../assets/sprites/cardboard.png");


    GameObjectManager::getInstance().addExternalRenderable(renderable);
}

void TrailRenderer::draw(sf::RenderTarget& window, sf::RenderStates states) const {
    if (points.size() < 2 || !emitting) return;

    // --- 1. Compute total "squared length" of the trail ---
    float totalSqLength = 0.f;
    for (size_t i = 0; i < points.size() - 1; i++) {
        sf::Vector2f d = points[i + 1].position - points[i].position;
        totalSqLength += d.x * d.x + d.y * d.y;
    }
    if (totalSqLength < 0.0001f) return; // safeguard for degenerate trails

    // --- 2. Allocate triangles ---
    sf::VertexArray strip(sf::PrimitiveType::TriangleStrip, points.size() * 2);

    float accumulatedSq = 0.f;

    for (size_t i = 0; i < points.size(); i++) {
        float progress = static_cast<float>(i) / (points.size() - 1);
        sf::Color color = interpolateColor(progress);

        // apply alpha fade
        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - points[i].creationTime).count();
        float alpha = 255.f * (1.f - (elapsed / duration));
        color.a = static_cast<uint8_t>(std::clamp(alpha, 0.f, 255.f));

        // compute normal offset for thickness
        sf::Vector2f dir;
        if (i < points.size() - 1)
            dir = points[i + 1].position - points[i].position;
        else
            dir = points[i].position - points[i - 1].position;

        float mag = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        sf::Vector2f normal = (mag > 0.0001f) ? sf::Vector2f(-dir.y, dir.x) / mag : sf::Vector2f(1, 0);

        sf::Vector2f offset = normal * (thickness * 0.5f * (alpha / 255.f));

        // 2 vertices per point
        strip[i * 2].position = points[i].position + offset;
        strip[i * 2].color = color;

        strip[i * 2 + 1].position = points[i].position - offset;
        strip[i * 2 + 1].color = color;
    }

    window.draw(strip, states);
}


void TrailRenderer::Destroy() {
    GameObjectManager::getInstance().removeExternalRenderable(renderable);
}