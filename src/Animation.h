#pragma once
#include "pch.h"

struct Animation
{

	int frameCount= 0;
	bool loop = false;
	float defaultSpeed = 1.0f;

	/// <summary>
	/// key = what frame to change sprite,
	/// value = what sprite to change to.
	/// </summary>
	std::map<int, int> frameData{};



	void LoadFromJson(const json& json){

		if (json.contains("default speed"))
			defaultSpeed = json["default speed"];
		
		if (json.contains("frame count"))
			frameCount = json["frame count"];
		else std::cerr << "\n Animation contains no frame count"; 

		if (json.contains("loop"))
			loop = json["loop"];
		
		if (json.contains("frame data"))
			for (auto& [key, value] : json["frame data"].items())
				frameData.insert({ std::stoi(key), value });

		
		else std::cerr << "\n Animation contains no frame data.";
	
	}

};


class Animator : public Component {
private:
	static constexpr float animationFrameRate = 1 / 60;
	std::shared_ptr<MSprite> sprite;


	Animation animation;
	sf::Vector2i frameSize;
	float elapsedTime = 0;
	float animationSpeed = 1;
	float frameDuration = animationFrameRate * animationSpeed;
	int currentFrame = 0;
	int frameCounter = 0;
	int columns;
	bool isPlaying = true;

public:

	Animator(Animation animation) :
		animation(animation) {
		columns = animation.frameCount;
	}



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
		if (animation.frameData.size() <= 0) {
			std::cerr << "\nAnimation has no frame data";
			isPlaying = false;
		}

		elapsedTime += deltaTime;
		if (elapsedTime >= frameDuration) {
			elapsedTime = 0;
			AdvanceFrame();
			frameCounter++;

		}
	}

	void AdvanceFrame() {

		// Check if we have custom frame data for this frame
		auto it = animation.frameData.find(frameCounter); // check frame data to see if we change sprite this frame.
		if (it != animation.frameData.end()) { // if we find the frame number in frame data, 
			if (it == --animation.frameData.end()) { // check if it is the last frame in frame data
				if (animation.loop) {  // if looping, reset frame counter and set current frame to 0.
					frameCounter = 0;
					currentFrame = 0;
					UpdateSprite(); // force update sprite to go back to frame 0.
				}
				else { // otherwise, 
					frameCounter = 0;
					currentFrame = animation.frameCount - 1; // stop at last frame of animation.
					UpdateSprite();
					isPlaying = false;
				}
			}
			
			// Use the specified column from the frame data
			else {
				currentFrame = it->second;
				UpdateSprite();
			}
		}		

	}


	void Play() { isPlaying = true; }
	void Pause() { isPlaying = false; }
	void Reset() {
		isPlaying = false;
		currentFrame = 0;
		frameCounter = 0;
		UpdateSprite();
	}

	void UpdateSprite() {
		if (!sprite) { std::cerr << "\nTrying to animate null sprite"; return; }

		sf::IntRect rect = { { (currentFrame % columns) * frameSize.x, 0 }, frameSize };
		sprite->setTextureRect(rect);
		std::cout << "\n anim rect size x: " << rect.position.x;

	}

	void init()override {
		sprite = parent->getSprite();
		frameSize = sprite->getTextureRect().size;
	}

	void Destroy() override {}

};