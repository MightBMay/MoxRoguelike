#pragma once
#include "pch.h"


//  usage example.
// 
//	auto scrollContainerRect = sf::FloatRect{ {500,300}, {800,128} };// define scroll rect size/position.
//
//	// make empty gameobject (prob set gameobject sprite size to same as rect size) (set layer to UI layer as well)
//	auto scrollContainerObj = GameObject::Create("../assets/sprites/shapes/square_128.png", { {},{800,128} }, 130);
//  scrollContainerObj->setPosition(scrollContainerRect.position - sf::Vector2f(0,4)); // subtract a lil to offset the background from the content.
//	// create scrollContainer
//	auto scrollContainer = scrollContainerObj->addComponent<UI_ScrollContainer>(
//		scrollContainerRect,
//		sf::Vector2f{ 128, 128 } // set this to the size of the content's sprites.
//	);
// 
//	auto scrollContainerSprite = scrollContainerObj->getSprite(); // set values for scroll container's sprite.
//	scrollContainerSprite->SetRepeated(true);
//	scrollContainerSprite->setColor(sf::Color::Black);
//
//
//
//	auto scrollRectShader = std::make_shared<sf::Shader>(); // shader needed to mask content.
//	if (!scrollRectShader->loadFromFile("../assets/shaders/ScrollRectMask.frag", sf::Shader::Type::Fragment)) {
//		std::cerr << "\nSpriteRectMask shader not found.";
//	}
// 
// 
//	sf::IntRect contentRect = { {},{128,128} }; // again, size of the content's sprite.
// 
//	auto scrollContainer_S = scrollContainer.lock();
//	for (int i = 0; i < 32; ++i) {
// 
//		// create content object and do whatever you need to be done to it.
//		auto& temp = GameObject::Create("../assets/sprites/shapes/square_128.png", contentRect, 131);
// 
//		temp->setShader(scrollRectShader);
//		temp->addComponent<UI_Button>(window).lock()->getOnClick().subscribe([i]() {std::cout << "\nclicked: " << i; });
// 
//		// set position accordingly
//		temp->setPosition(scrollContainerRect.position.x + (contentRect.size.x * i), scrollContainerRect.position.y);
// 
//		scrollContainer_S->addContent(temp); // be sure to actually add to the content.
//	}


class UI_ScrollContainer : public UI_Element {
private:

	sf::FloatRect viewPortBounds;
	sf::Vector2f contentPosition;
	sf::Vector2f dragStartPos;
	sf::Vector2f contentStartPos{};
	sf::Vector2f contentSpacing;
	// size of content. (ideally all should be same sized)
	sf::Vector2f contentSize;
	bool isDragging= false;
	bool enabled = true;
	bool _horizontal = true;
	bool _vertical = false;
	float elasticity = 1;
	float fadeWidth = 32;
	sf::Vector2i contentThatCanFitInView;

	std::shared_ptr<MSprite> sprite;
	std::vector<std::shared_ptr<GameObject>> contentObjects;
	std::vector<sf::Vector2f> originalPositions;

public:
	UI_ScrollContainer(std::shared_ptr<sf::RenderWindow> window,
		const sf::FloatRect& viewportBounds, sf::Vector2f contentObjectSize,
		sf::Vector2f contentSpacing = { 32,0 }, float fadeWidth = 16, float elasticity = 1)
		: UI_Element(window),
		viewPortBounds(viewportBounds),
		elasticity(elasticity),
		contentSize(contentObjectSize),
		contentSpacing(contentSpacing),
		fadeWidth(fadeWidth){
		int h = viewportBounds.size.x / contentObjectSize.x;
		int v = viewportBounds.size.y / contentObjectSize.y;
		contentThatCanFitInView = { h,v };
	
	}

	void Destroy() override {}

	void addContent(std::shared_ptr<GameObject> gameObject) {
		contentObjects.push_back(gameObject);
		originalPositions.push_back(gameObject->getPosition());

		applyBoundsConstraints();
		updateContentBounds();
	}

	void Hide() {
		parent.lock()->setActive(false, true);
		for (auto& obj : contentObjects)
			obj->setActive(false, true);
	}

	void Show() {
		parent.lock()->setActive(true, true);
		for (auto& obj : contentObjects)
			obj->setActive(true, true);
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
		float totalContentWidth = contentObjects.size() * (contentSize.x + contentSpacing.x)-contentSpacing.x;
		float totalContentHeight = contentObjects.size() * (contentSize.y + contentSpacing.y);// -contentSpacing.y;

		// Store original content position before potentially modifying it
		sf::Vector2f originalContentPos = contentPosition;

		// Determine which indices to use for bounds checking
		size_t firstIndex = 0;
		size_t lastIndex = contentObjects.size() - 1;

		// contentThatCanFitInView.x used to determine the visible range for horizontal constraints
		size_t rightVisibleIndex = std::min(firstIndex + contentThatCanFitInView.x - 1, lastIndex);
		size_t leftVisibleIndex = (contentThatCanFitInView.x >= contentObjects.size()) ?
			lastIndex : lastIndex - contentThatCanFitInView.x;

		
		auto rightVisiblePos = originalPositions[rightVisibleIndex] + contentPosition;
		if (rightVisiblePos.x + contentSize.x > viewPortBounds.position.x + viewPortBounds.size.x) {
			// Too far right - adjust so rightmost visible element fits
			
			contentPosition.x = (viewPortBounds.position.x + viewPortBounds.size.x - contentSize.x) - originalPositions[rightVisibleIndex].x;
		}

		// Horizontal constraints - using the leftmost visible element
		auto leftVisiblePos = originalPositions[leftVisibleIndex] + contentPosition;
		if (leftVisiblePos.x < viewPortBounds.position.x) {
			// Too far left - adjust so leftmost visible element fits
			contentPosition.x = viewPortBounds.position.x - originalPositions[leftVisibleIndex].x;
		}

		// contentThatCanFitInView.y used to determine the visible range for vertical constraints
		size_t bottomVisibleIndex = std::min(firstIndex + contentThatCanFitInView.y - 1, lastIndex);
		size_t topVisibleIndex = (contentThatCanFitInView.y >= contentObjects.size()) ?
			firstIndex : lastIndex - contentThatCanFitInView.y + 1;

		auto bottomVisiblePos = originalPositions[bottomVisibleIndex] + contentPosition;
		if (bottomVisiblePos.y + contentSize.y > viewPortBounds.position.y + viewPortBounds.size.y) {
			// Too far down - adjust so bottommost visible element fits
			contentPosition.y = (viewPortBounds.position.y + viewPortBounds.size.y - contentSize.y) - originalPositions[bottomVisibleIndex].y;
		}

		auto topVisiblePos = originalPositions[topVisibleIndex] + contentPosition;
		if (topVisiblePos.y < viewPortBounds.position.y) {
			// Too far up - adjust so topmost visible element fits
			contentPosition.y = viewPortBounds.position.y - originalPositions[topVisibleIndex].y;
		}
	}

	void updateContentPositions() {
		for (size_t i = 0; i < contentObjects.size(); ++i) {
			if (auto& obj = contentObjects[i]) {
				// Calculate new position based on original + scroll offset
				sf::Vector2f newPos = originalPositions[i] + contentPosition + contentSpacing;
				obj->setPosition(newPos);
				if (!viewPortBounds.contains(newPos))
					obj->setActive(false); // disable object when it leaves the rect.
				else
					obj->setActive(true);
			}
		}
	}

	virtual void update(float deltaTime) override {
		UI_Element::update(deltaTime);
		if (!enabled) return;

		auto mPosInt = Input::mousePos_Screen;
		sf::Vector2f mousePos = { static_cast<float>(mPosInt.x) ,static_cast<float>(mPosInt.y) };
		if (Input::GetMouseDown(0)) {
			if (!isHovering()) return;
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
		static const sf::View& view = window->getDefaultView();
		if (contentObjects.empty()) return;
		auto bounds = sprite->getGlobalBounds();
		sf::Vector2i topLeft = window->mapCoordsToPixel(bounds.position, view);
		sf::Vector2i bottomRight = window->mapCoordsToPixel(bounds.position + bounds.size, view);

		sf::Vector2f screenPos = static_cast<sf::Vector2f>(topLeft);
		sf::Vector2f screenSize = static_cast<sf::Vector2f>(bottomRight - topLeft);


		for (auto& item : contentObjects) {
			auto& shader = item->getRenderable()->shader;
			if (shader) {

				shader->setUniform("viewportPosition", screenPos);
				shader->setUniform("viewportSize", screenSize);
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
	void setEnabled(bool value) { enabled = value; }
	bool isEnabled() const { return enabled; }
	sf::Vector2f getContentPosition() const { return contentPosition; }

	sf::FloatRect getViewportBounds() const { return viewPortBounds; }
	void setViewportBounds(const sf::FloatRect& bounds) {
		viewPortBounds = bounds;
	}

	virtual void init()override {
		UI_Element::init();
		sprite = parent.lock()->getSprite();
	}

	// not used, but we inherit UI_Element such that position is perserved when resizing window.
	virtual void OnHover()override {}
	virtual void OnHoverExit()override {}
	virtual void OnClick(const int button)override {}

};



