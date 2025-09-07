#pragma once

class Component;
class Renderable;
class MSprite;

class ProgressBar : public Component {
public:
	
	ProgressBar(
		sf::IntRect rect,
		std::string fillSpritePath = "../assets/sprites/shapes/bl_square_128.png",
		bool isHorizontal = true, 
		float min = 0, 
		float max = 1
	);
	virtual void updateBar(int value);
	virtual void updateBar(float value);
	void setFillColor(sf::Color& colour);
	virtual void setRange(float min, float max);
	virtual void setRange(int min, int max);

	virtual void setRange(int max);
	virtual void init() override;
	virtual void update(float deltaTime) override{}
	virtual void Destroy() {}

protected:
	// might want to store a gameobject instead, but for now uses a msprite directly.
	std::shared_ptr<MSprite> fillSprite;
	std::shared_ptr<Renderable> renderable;
	sf::Vector2f startScale;
	bool isHorizontal;
	float minValue;
	float maxValue;
	float curValue =0;
	


};


