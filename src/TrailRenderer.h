#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include "Component.h"

class TrailRenderer : public Component, public sf::Drawable {
private:
	struct TrailPoint {
		sf::Vector2f position;
		std::chrono::time_point<std::chrono::steady_clock> creationTime;
	};

	std::vector<TrailPoint> points;
	float duration; //(seconds)
	sf::Color colour;
	float thickness;

protected:
	void draw(sf::RenderTarget& window, sf::RenderStates states)const override;

public :
	virtual void init() {}
	virtual void update(float deltaTime) override;
	
	virtual void Destroy() {}
	virtual void ProcessEvent(const std::optional<sf::Event>& event) {}

	
	void clear() {
		points.clear();
	}

	void setDuration(float newDuration) {
		duration = newDuration;
	}

	void setColor(const sf::Color& newColor) {
		colour = newColor;
	}

	void setThickness(float newThickness) {
		thickness = newThickness;
	}

	

	TrailRenderer(float duration = 1, float thickness = 2, sf::Color colour = sf::Color::White)
	: duration(duration), colour(colour), thickness(thickness){}

	



};