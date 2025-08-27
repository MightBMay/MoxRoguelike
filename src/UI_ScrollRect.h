#pragma once
#include "pch.h"


class UI_ScrollContainer : public Component {
private:

	sf::FloatRect viewPortBounds;
	sf::Vector2f contentPosition;
	sf::Vector2f dragStartPos;
	sf::Vector2f contentStartPos;
	sf::Vector2f contentSpacing{};
	// size of content. (ideally all should be same sized)
	sf::Vector2f contentSize;
	bool isDragging;
	bool enabled;
	bool _horizontal = true;
	bool _vertical = false;
	float elasticity = 1;
	float fadeWidth = 32;

	std::vector<std::shared_ptr<GameObject>> contentObjects;
	std::vector<sf::Vector2f> originalPositions;

public:
	UI_ScrollContainer(const sf::FloatRect& viewportBounds, sf::Vector2f contentObjectSize)
		: viewPortBounds(viewportBounds),
		isDragging(false),
		enabled(true),
		elasticity(0.1f),
		contentPosition(0, 0),
		contentSize(contentObjectSize) {}

	virtual void init() override {
		//DEBUG remove this, objects should be created externally and just added with addcontent.
		auto shader = std::make_shared<sf::Shader>();
		if (!shader->loadFromFile("../assets/shaders/ScrollRectMask.frag", sf::Shader::Type::Fragment)) {
			std::cerr << "\nSpriteRectMask shader not found."; return;
		}

		for (int i = 0; i < 32; ++i) {
			auto& temp = GameObject::Create("../assets/sprites/shapes/square_64.png", { {},{64,64} }, 131);
			temp->setShader(shader);
			auto button = temp->addComponent<UI_Button>(window);
			button.lock()->getOnClick().subscribe([i]() {std::cout << "\nclicked: " << i; });
			temp->setPosition(viewPortBounds.position.x + (contentSize.x * i), viewPortBounds.position.y);
			addContent(temp);
		}
	}
	void Destroy() override {}

	void addContent(std::shared_ptr<GameObject> gameObject) {
		contentObjects.push_back(gameObject);
		originalPositions.push_back(gameObject->getPosition());
		updateContentBounds();
	}

	void clearContent() {
		contentObjects.clear();
		originalPositions.clear();
	}

	void updateContentBounds() {
		// Optional: Calculate content bounds for constraints
		// You might not need this if you want infinite scrolling
	}

	void applyBoundsConstraints() {
		if (contentObjects.empty()) return;

		// Calculate total content width/height
		float totalContentWidth = contentObjects.size() * (contentSize.x + contentSpacing.x) - contentSpacing.x;
		float totalContentHeight = contentObjects.size() * (contentSize.y + contentSpacing.y) - contentSpacing.y;

		// Store original content position before potentially modifying it
		sf::Vector2f originalContentPos = contentPosition;

		// Horizontal constraints

		auto firstPos = originalPositions.front() + contentPosition;
		auto lastPos = originalPositions.back() + contentPosition;

		if (firstPos.x + contentSize.x > viewPortBounds.position.x + viewPortBounds.size.x) {
			// Too far right
			contentPosition.x = (viewPortBounds.position.x + viewPortBounds.size.x - contentSize.x) - originalPositions.front().x;
		}
		else if (lastPos.x < viewPortBounds.position.x) {
			// Too far left
			contentPosition.x = viewPortBounds.position.x - originalPositions.back().x;
		}



	// Vertical constraints

		if (lastPos.y + contentSize.y > viewPortBounds.position.y + viewPortBounds.size.y) {
			// Too far down
			contentPosition.y = (viewPortBounds.position.y + viewPortBounds.size.y - contentSize.y) - originalPositions.back().y;
		}
		else if (firstPos.y < viewPortBounds.position.y) {
			// Too far up
			contentPosition.y = viewPortBounds.position.y - originalPositions.front().y;
		}

	}

	void updateContentPositions() {
		for (size_t i = 0; i < contentObjects.size(); ++i) {
			if (auto obj = contentObjects[i]) {
				// Calculate new position based on original + scroll offset
				sf::Vector2f newPos = originalPositions[i] + contentPosition;
				obj->setPosition(newPos);
				if (!viewPortBounds.contains(newPos))
					obj->setActive(false); // disable object when it leaves the rect.
				else
					obj->setActive(true);
			}
		}
	}

	void update(float deltaTime) {
		if (!enabled) return;

		auto mPosInt = Input::mousePos_Screen;
		sf::Vector2f mousePos = { static_cast<float>(mPosInt.x) ,static_cast<float>(mPosInt.y) };

		if (Input::GetMouseDown(0)) {
			if (!viewPortBounds.contains(mousePos)) return;
			isDragging = true;
			dragStartPos = mousePos;
			contentStartPos = contentPosition;
			return; // Consume the event
		}



		else if (Input::GetMouseUp(0)) {
			isDragging = false;
			// Don't consume release event so buttons can still work
			return;
		}

		if (Input::MouseMoved) {
			if (isDragging) {
				sf::Vector2f delta = mousePos - dragStartPos;
				if (!_horizontal) delta.x = 0; // can toggle axis seperately.
				if (!_vertical) delta.y = 0;
				contentPosition = contentStartPos + delta;
				applyBoundsConstraints();
				updateContentPositions();
				return; // Consume the event while dragging
			}
		}

		UpdateContentMaskShader();


	//case sf::Event::MouseWheelScrolled:
	//    contentPosition.y += event.mouseWheelScroll.delta * 30.0f;
	//    applyBoundsConstraints();
	//    updateContentPositions();
	//    return true;

	}
	void UpdateContentMaskShader() {
		for (auto& item : contentObjects) {
			auto& shader = item->getRenderable()->shader;
			if (shader) {
				shader->setUniform("viewportPosition", viewPortBounds.position);
				shader->setUniform("viewportSize", viewPortBounds.size);
				shader->setUniform("fadeWidth", fadeWidth);
				shader->setUniform("texture", sf::Shader::CurrentTexture);
			}
		}
	}
	void resetPositions() {
		contentPosition = sf::Vector2f(0, 0);
		updateContentPositions();
	}
	void SetAxis(bool horizontal, bool vertical) {
		_horizontal = horizontal;
		_vertical = vertical;
	}
	// Getters and setters
	void setEnabled(bool enabled) { enabled = enabled; }
	bool isEnabled() const { return enabled; }
	sf::Vector2f getContentPosition() const { return contentPosition; }

	sf::FloatRect getViewportBounds() const { return viewPortBounds; }
	void setViewportBounds(const sf::FloatRect& bounds) {
		viewPortBounds = bounds;
	}
};