#include "TrailRenderer.h"
#include "GameObject.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
TrailRenderer::TrailRenderer(float duration, float thickness, sf::Color startColour, sf::Color endColour)
    : duration(duration), startColour(startColour),endColour(endColour), thickness(thickness) {
}

void TrailRenderer::update(float deltaTime) {

    timeSinceLastUpdate += deltaTime;// increment timer
    if (timeSinceLastUpdate < updateInterval) return; // do not continue until timer has passed an interval
    timeSinceLastUpdate = 0; // reset timer and continue adding points.

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

void TrailRenderer::init() {
    std::shared_ptr<sf::Drawable> drawable = std::static_pointer_cast<sf::Drawable>(shared_from_this());
    renderable = std::make_shared<Renderable>(drawable, nullptr);

   // LoadShader("../assets/sprites/cardboard.png");


    GameObjectManager::getInstance().addExternalRenderable(renderable);
}

void TrailRenderer::draw(sf::RenderTarget& window, sf::RenderStates states) const  {
    if (points.size() < 2 || !emitting) return;

    // Create a vertex array for the line strip
    sf::VertexArray line(sf::PrimitiveType::TriangleStrip, (points.size() - 1) * 2);

    for (size_t i = 0; i < points.size() - 1; i++) {
        sf::Vector2f direction = points[i + 1].position - points[i].position;
        float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // Calculate alpha based on point age
        auto now = std::chrono::steady_clock::now();
        float elapsed1 = std::chrono::duration_cast<std::chrono::duration<float>>(now - points[i].creationTime).count();
        float elapsed2 = std::chrono::duration_cast<std::chrono::duration<float>>(now - points[i + 1].creationTime).count();

        float alpha1 = 255 * (1.f - (elapsed1 / duration));
        float alpha2 = 255 * (1.f - (elapsed2 / duration));

        // Safeguard against division by zero
        sf::Vector2f unitDirection = 
            (magnitude > 0.0001f) ? direction / magnitude : sf::Vector2f(1.f, 0.f); // Default direction if points are too close
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = 
            thickness* // base thickness
            (alpha2/255) // multiplied by alpha (0-1) to scale tip down.
            / 2.f // /2 because offset is half thickness in both ways.
            * unitPerpendicular;// direction.

 


        sf::Color color1 = startColour;
        sf::Color color2 = endColour;
        color1.a = alpha1;
        color2.a = alpha2;
 
        // Add the two vertices for this segment
        line[i * 2].position = points[i].position + offset;
        line[i * 2].color = color1;

        line[i * 2 + 1].position = points[i].position - offset;
        line[i * 2 + 1].color = color1;
    }
    /*
    if (renderable->shader != nullptr) {
       
        states.texture = texture.get(); 

        states.shader = renderable->shader.get(); 

        if (auto shader = renderable->shader.get()) { 
            shader->setUniform("trailTexture", sf::Shader::CurrentTexture);

        }

    }
    */



    window.draw(line, states);
}

void TrailRenderer::Destroy() {
    GameObjectManager::getInstance().removeExternalRenderable(renderable);
}