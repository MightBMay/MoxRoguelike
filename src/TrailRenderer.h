#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include "Component.h"
#include "Renderable.h"



class TrailRenderer : public Component, public sf::Drawable, public std::enable_shared_from_this<TrailRenderer> {
private:
	struct TrailPoint {
		sf::Vector2f position;
		std::chrono::time_point<std::chrono::steady_clock> creationTime;
	};

	std::vector<TrailPoint> points;
	float duration; //(seconds)
	sf::Color startColour;
	sf::Color endColour;
	float thickness;
	std::shared_ptr<sf::Texture> texture = nullptr;
	std::shared_ptr<Renderable> renderable = nullptr;

	bool emitting = true;

protected:
	void draw(sf::RenderTarget& window, sf::RenderStates states)const override;

public :
	virtual void init() override;
	virtual void update(float deltaTime) override;
	
	virtual void Destroy()override;


	
	void clear() {
		points.clear();
	}

	void setEmission(bool value) {
		emitting = value;
	}

	void setDuration(float newDuration) {
		duration = newDuration;
	}

	void setColor(const sf::Color& newStartColour, const sf::Color& newEndColour) {
		startColour = newStartColour;
		endColour = newEndColour;
	}

	void setThickness(float newThickness) {
		thickness = newThickness;
	}

	void LoadShader(std::string texturePath = "../assets/sprites/cardboard.png") {
		auto shader = std::make_shared<sf::Shader>();
		if (!shader->loadFromFile("../assets/shaders/trails/cardboard_trail.frag", sf::Shader::Type::Fragment)) {
			std::cerr << "\nerror loading trail shader\n";
		}

		auto texture = std::make_shared<sf::Texture>();
		if (!texture->loadFromFile(texturePath)) {
			std::cerr << "\n error loading texture for shader\n";
		}

		



		this->texture = texture;
		renderable->shader = shader;
	}
	

	TrailRenderer(float duration = 1, float thickness = 2, sf::Color startColour = sf::Color::White, sf::Color endColour = sf::Color::White);

	



};