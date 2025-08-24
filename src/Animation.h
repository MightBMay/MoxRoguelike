#pragma once
#include "pch.h"

struct Animation
{

	int frameCount=2;
	bool loop = true;
	float defaultSpeed = 1.0f;

	/// <summary>
	/// key = what frame to change sprite,
	/// value = what sprite to change to.
	/// </summary>
	std::map<int, int> frameData= {

		{0,0},
		{15,1},
	};



	void LoadFromJson(const json& frameDataJ){
		for (auto& [key, value] : frameDataJ.items()) 
			frameData.insert({ std::stoi(key), value });
	
	}

};


class Animator : public Component {
private:
	static constexpr float animationFrameRate = 1 / 60;
	const std::shared_ptr<MSprite>& sprite;


	Animation animation;
	sf::Vector2i frameSize;
	float elapsedTime = 0;
	float animationSpeed = 1;
	float frameDuration = animationFrameRate * animationSpeed;
	int currentFrame = 0;
	int columns = 0;
	bool isPlaying = true;

public:

	Animator(std::shared_ptr<MSprite>& sprite, Animation animation) :
		sprite(sprite), animation(animation) {}

	Animator(std::shared_ptr<GameObject>& obj, Animation animation) :
		sprite(obj->getSprite()), animation(animation) {}

	void SetAnimation(Animation& newAnimation) {
		animation = newAnimation;
		animationSpeed = newAnimation.defaultSpeed;
		frameDuration = animation.frameCount;
		columns = animation.frameCount;
	}


	/// <param name="newSpeed">new speed of the animation. can be set to a negative value to return to the animation's default speed.</param>
	void SetAnimationSpeed(float newSpeed) {
		if (newSpeed < 0) { animationSpeed = animation.defaultSpeed; }
		animationSpeed = newSpeed;
		frameDuration = animationFrameRate * animationSpeed;
	}

	void update(float deltaTime) {
		if (!isPlaying) return;
		elapsedTime += deltaTime;
		if (elapsedTime >= frameDuration) {
			elapsedTime = 0;

		}
	}

	void AdvanceFrame() {
		// Check if we have custom frame data for this frame
		auto it = animation.frameData.find(currentFrame);
		if (it != animation.frameData.end()) {
			// Use the specified column from the frame data
			UpdateSprite(it->second);
		}
		else {
		 // Default behavior: move to next column
			int nextColumn = (currentFrame % columns);
			UpdateSprite(nextColumn);
		}

		currentFrame++;

		// Handle animation end
		if (currentFrame >= animation.frameCount) {
			if (animation.loop) {
				currentFrame = 0;
			}
			else {
				currentFrame = animation.frameCount - 1;
				isPlaying = false;
			}
		}
	}


	void Play() { isPlaying = true; }
	void Pause() { isPlaying = false; }
	void Reset() {
		isPlaying = false;
		currentFrame = 0;
		UpdateSprite();
	}

	void UpdateSprite(int column = 0) {
		if (!sprite) { std::cerr << "\nTrying to animate null sprite"; return; }

		sf::IntRect rect = { { (currentFrame % columns) * frameSize.x, frameSize.y }, frameSize };
		sprite->setTextureRect(rect);

	}

	void init()override {
		frameSize = sprite->getTextureRect().size;
	}

	void Destroy() override {}

};