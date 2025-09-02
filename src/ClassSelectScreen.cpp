#include "pch.h"
#include "ClassSelectScreen.h"
#include "Player.h"


sf::IntRect ClassSelectScreen::CreateRectFromJson(const json& data) {

	sf::Vector2i size;
	sf::Vector2i pos;
	if (data.contains("spriteSize")) {
		auto rawSize = data["spriteSize"].get<std::vector<int>>();
		size = { rawSize[0],rawSize[1] };
	}else size={};


	if (data.contains("textureStartPos")) {
		auto rawPos = data["textureStartPos"].get<std::vector<int>>();
		pos = { rawPos[0],rawPos[1] };
	}else pos = {};
		
	return { pos, size };
}

void ClassSelectScreen::CreateClassSelectionRect() {
	static constexpr sf::Vector2i containerSize = { 1300,128 };
	static constexpr sf::Vector2f containerPos= { 310,0 };
	auto scrollContainerRect = sf::FloatRect{ containerPos, static_cast<sf::Vector2f>(containerSize) };// define scroll rect size/position.
// make empty gameobject (prob set gameobject sprite size to same as rect size) (set layer to UI layer as well)
	classScrollObj = GameObject::Create("../assets/sprites/shapes/bl_square_128.png",
		{ {},containerSize + sf::Vector2i{8,4} },
		130
	);

	classScrollObj->setPosition(scrollContainerRect.position);
	classScrollRect = classScrollObj->addComponent<UI_ScrollContainer>(
		window,
		scrollContainerRect,
		sf::Vector2f{ 128, 128 },
		128
		// set this to the size of the content's sprites.
	);
	auto scrollContainerSprite = classScrollObj->getSprite(); // set values for scroll container's sprite.
	scrollContainerSprite->SetRepeated(true);
	scrollContainerSprite->setColor(sf::Color(0, 0, 0, 64));


	auto scrollRectShader = std::make_shared<sf::Shader>(); // shader needed to mask content.
	if (!scrollRectShader->loadFromFile("../assets/shaders/ScrollRectMask.frag", sf::Shader::Type::Fragment)) {
		std::cerr << "\nSpriteRectMask shader not found.";
	}
	auto scrollContainer_S = classScrollRect.lock();
	int quantity = Player::playerClassList.size();
	for (int i = 0; i < quantity; ++i) {
		// create content object and do whatever you need to be done to it.
		const json& classData = GameData::getPlayerClassFromIndex(i);
		sf::IntRect rect = CreateRectFromJson(classData);

		auto& temp = GameObject::Create("../assets/sprites/atlases/playerSprites.png", rect, 131);
		float halfHeight = rect.size.y / 2.0f;
		temp->setOrigin(rect.size.x / 2.0f, halfHeight);
		temp->setShader(scrollRectShader);
		// add button, get its click event, subscribe stuff to it.
		temp->addComponent<UI_Button>(window).lock()->getOnClick().subscribe(
			[i, this]() {
				playerClassIndex = i;
				UpdateStartButton();

			}
		);
		// set position accordingly
		temp->setPosition(scrollContainerRect.position.x + (rect.size.x * i), scrollContainerRect.position.y + halfHeight);
		scrollContainer_S->addContent(temp); // be sure to actually add to the content.
	}
}

void ClassSelectScreen::CreateLevelSelectionRect() {
	static constexpr sf::Vector2i containerSize = { 1300,128 };
	static constexpr sf::Vector2f containerPos = { 310, 1080 -containerSize.y };
	auto scrollContainerRect = sf::FloatRect{ containerPos, static_cast<sf::Vector2f>(containerSize) };// define scroll rect size/position.
// make empty gameobject (prob set gameobject sprite size to same as rect size) (set layer to UI layer as well)
	levelScrollObj = GameObject::Create("../assets/sprites/shapes/bl_square_128.png",
		{ {},containerSize+ sf::Vector2i{8,4} },
		130
	);

	levelScrollObj->setPosition(scrollContainerRect.position);
	levelScrollRect = levelScrollObj->addComponent<UI_ScrollContainer>(
		window,
		scrollContainerRect,
		sf::Vector2f{ 128, 128 },
		128
		// set this to the size of the content's sprites.
	);
	auto scrollContainerSprite = levelScrollObj->getSprite(); // set values for scroll container's sprite.
	scrollContainerSprite->SetRepeated(true);
	scrollContainerSprite->setColor(sf::Color(0, 0, 0, 64));


	auto scrollRectShader = std::make_shared<sf::Shader>(); // shader needed to mask content.
	if (!scrollRectShader->loadFromFile("../assets/shaders/ScrollRectMask.frag", sf::Shader::Type::Fragment)) {
		std::cerr << "\nSpriteRectMask shader not found.";
	}
	sf::Vector2i playerSpriteSize{ 128,128 }; // again, size of the content's sprite.
	auto scrollContainer_S = levelScrollRect.lock();
	int quantity = GameData::getLevelCount();
	for (int i = 0; i < quantity; ++i) {
		// create content object and do whatever you need to be done to it.
		const json& levelData = GameData::getLevelFromIndex(i);
		sf::IntRect rect = CreateRectFromJson(levelData);
		auto& temp = GameObject::Create("../assets/sprites/atlases/levelIconAtlas.png", rect, 131);
		float halfHeight = rect.size.y / 2.0f;
		temp->setOrigin(rect.size.x / 2.0f, halfHeight);
		temp->setShader(scrollRectShader);
		// add button, get its click event, subscribe stuff to it.
		temp->addComponent<UI_Button>(window).lock()->getOnClick().subscribe(
			[i, this]() {
				levelIndex = i;
				UpdateStartButton();
			}
		);
		// set position accordingly
		temp->setPosition(scrollContainerRect.position.x + (rect.size.x * i), scrollContainerRect.position.y + halfHeight);
		scrollContainer_S->addContent(temp); // be sure to actually add to the content.
	}
}

void ClassSelectScreen::CreateStartButton() {
	static constexpr sf::IntRect startButtonRect = { {},{310,128} };
	static constexpr sf::Vector2f startButtonPosition = { 1610,952 };
	
	startButtonRT = std::make_shared<sf::RenderTexture>(static_cast<sf::Vector2u>(startButtonRect.size));
	text->setCharacterSize(52);
	text->setString("Start");
	text->setOutlineThickness(2);
	auto textSize = text->getLocalBounds().size;
	text->setPosition(
		{
			(startButtonRect.size.x/2.0f ) - (textSize.x / 2.0f),
			textSize.y/2.0f 
		}
	);

	auto bgTexture = *TextureManager::getTexture("../assets/sprites/shapes/bl_square_128.png");
	bgTexture.setRepeated(true);
	sf::Sprite bgSprite = sf::Sprite( bgTexture, startButtonRect);
	
	startButtonRT->draw(bgSprite);
	startButtonRT->draw(*text);
	startButtonRT->display();



	startButtonObj = GameObject::Create();
	startButtonObj->setSprite(startButtonRT->getTexture(), startButtonRect);

	startButtonObj->setPosition(startButtonPosition);
	GameObjectManager::getInstance().add(startButtonObj, 130);

	startButton = startButtonObj->addComponent<UI_Button>(window);
	auto bound= startButtonObj->getSprite()->getGlobalBounds();
	auto buttonS = startButton.lock();
	buttonS->SetEnabled(false);
	buttonS->getOnClick().subscribe([this]() { StartLevel(); });
}

void ClassSelectScreen::CreateClassDescription() {
	static constexpr sf::Vector2i descriptionPos = { 0,128 };
	static constexpr sf::Vector2i descriptionSize = {1000,600};
	descriptionObj = GameObject::Create(
		"../assets/sprites/shapes/bl_square_128.png", 
		{ {0, 0}, descriptionSize},
		131
	);

	descriptionObj->setPosition(descriptionPos);



}


void ClassSelectScreen::UpdateStartButton() {
	if (playerClassIndex >= 0 && levelIndex >= 0) {
		startButton.lock()->SetEnabled(true);
	}
}

void ClassSelectScreen::StartLevel() {
	level = std::make_shared<Level>(levelIndex);
	player = Player::CreatePlayerClass(playerClassIndex);

	levelScrollRect.lock()->Hide();
	classScrollRect.lock()->Hide();
	startButton.lock()->SetEnabled(false);
	startButtonObj->setActive(false, true);

	second_Timer.start();
}