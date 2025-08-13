#include "Component.h"
#include "Renderable.h"
#include "MSprite.h"




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
	inline void setFillColor(sf::Color& colour) { if (fillSprite) fillSprite->setColor(colour); }
	inline void setRange(float min, float max) {
		minValue = min; 
		maxValue = max;
	}

	inline void setRange(int min, int max) {
		minValue = min;
		maxValue = max;
	}

	inline void setRange(int max) {
		maxValue = max;
	}
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
	
	/// <summary>
	/// normalizes a number from a given range to a range of 0-1.
	/// </summary>
	/// <param name="value"> value to be normalized.</param>
	/// <param name="min">minimum range value. if value < min, returns 0.</param>
	/// <param name="max">maximum range value. if value > max, returns 1.</param>
	inline float normalizeFromRange(float value, float min = 0.0f, float max = 1.0f) {
		if (max == min) {
			std::cerr << "\n tried to normalize number with min and max equal.";
			return -1;
		}
		value = std::clamp(value, min, max);
		return (value - min) / (max - min);
	}

};