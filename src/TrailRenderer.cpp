#include "TrailRenderer.h"
#include "GameObject.h"

void TrailRenderer::update(float deltaTime) {

	auto now = std::chrono::steady_clock::now();
	
	// Add new point
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


void TrailRenderer::draw(sf::RenderTarget& window, sf::RenderStates states) const  {
    if (points.size() < 2) return;

    // Create a vertex array for the line strip
    sf::VertexArray line(sf::PrimitiveType::TriangleStrip, (points.size() - 1) * 2);

    for (size_t i = 0; i < points.size() - 1; i++) {
        sf::Vector2f direction = points[i + 1].position - points[i].position;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

        // Calculate alpha based on point age
        auto now = std::chrono::steady_clock::now();
        float elapsed1 = std::chrono::duration_cast<std::chrono::duration<float>>(now - points[i].creationTime).count();
        float elapsed2 = std::chrono::duration_cast<std::chrono::duration<float>>(now - points[i + 1].creationTime).count();

        float alpha1 = 255 * (1.f - (elapsed1 / duration));
        float alpha2 = 255 * (1.f - (elapsed2 / duration));

        sf::Color color1 = colour;
        sf::Color color2 = colour;
        color1.a = static_cast<uint8_t>(alpha1);
        color2.a = static_cast<uint8_t>(alpha2);

        // Add the two vertices for this segment
        line[i * 2].position = points[i].position + offset;
        line[i * 2].color = color1;

        line[i * 2 + 1].position = points[i].position - offset;
        line[i * 2 + 1].color = color1;
    }

    window.draw(line, states);
}
