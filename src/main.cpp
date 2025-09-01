
#include "pch.h"

#include "GameObject.h"
#include "EnemyManager.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Player.h"
#include "Enemy.h"
#include "StatUpgrade.h"




#pragma region global.h extern definitions

std::shared_ptr<sf::RenderWindow> window;
std::shared_ptr<sf::View> playerView;
sf::Font font;
Timer second_Timer{ 1,true };
int elapsed_seconds = 0;
float timeScale = 1.0f;


#pragma endregion


std::shared_ptr<GameObject> player;
std::shared_ptr<GameObject> Background;
std::shared_ptr<GameObject> vignetteObject;

std::shared_ptr<FPS_Counter> fpsCounter;

void CreateClassSelectionScreen() {
	sf::Vector2i rectSize = { 800,128 };
	auto scrollContainerRect = sf::FloatRect{ {500,300}, static_cast<sf::Vector2f>(rectSize) };// define scroll rect size/position.
// make empty gameobject (prob set gameobject sprite size to same as rect size) (set layer to UI layer as well)
	auto scrollContainerObj = GameObject::Create("../assets/sprites/shapes/bl_square_128.png",
		{ {},rectSize + sf::Vector2i{8,4} },
		130
	);

	scrollContainerObj->setPosition(scrollContainerRect.position);
	auto scrollContainer = scrollContainerObj->addComponent<UI_ScrollContainer>(
		window,
		scrollContainerRect,
		sf::Vector2f{ 128, 128 },
		128
		// set this to the size of the content's sprites.
	);
	auto scrollContainerSprite = scrollContainerObj->getSprite(); // set values for scroll container's sprite.
	scrollContainerSprite->SetRepeated(true);
	scrollContainerSprite->setColor(sf::Color(0, 0, 0, 64));


	auto scrollRectShader = std::make_shared<sf::Shader>(); // shader needed to mask content.
	if (!scrollRectShader->loadFromFile("../assets/shaders/ScrollRectMask.frag", sf::Shader::Type::Fragment)) {
		std::cerr << "\nSpriteRectMask shader not found.";
	}
	sf::Vector2i playerSpriteSize{ 128,128 }; // again, size of the content's sprite.
	auto scrollContainer_S = scrollContainer.lock();
	int quantity = Player::playerClassList.size();
	for (int i = 0; i < quantity; ++i) {
		// create content object and do whatever you need to be done to it.
		const json& classData = GameData::getPlayerClassFromIndex(i);
		sf::Vector2i size;
		sf::Vector2i pos;
		if (classData.contains("spriteSize")) {
			auto rawSize = classData["spriteSize"].get<std::vector<int>>();
			size = { rawSize[0],rawSize[1] };
		}


		if (classData.contains("textureStartPos")) {
			auto rawSize = classData["textureStartPos"].get<std::vector<int>>();
			pos = { rawSize[0],rawSize[1] };
		}


		auto& temp = GameObject::Create("../assets/sprites/atlases/playerSprites.png", { pos, size }, 131);
		float halfHeight = size.y / 2.0f;
		temp->setOrigin(size.x/2.0f , halfHeight);
		temp->setShader(scrollRectShader);
		temp->addComponent<UI_Button>(window).lock()->getOnClick().subscribe(
			[i, scrollContainerObj, scrollContainer]() {
				player = Player::CreatePlayerClass(i);
				scrollContainer.lock()->Hide();
				scrollContainerObj->setActive(false, true);
				second_Timer.start();

			}
		);
		// set position accordingly
		temp->setPosition(scrollContainerRect.position.x + (playerSpriteSize.x * i), scrollContainerRect.position.y + halfHeight);
		scrollContainer_S->addContent(temp); // be sure to actually add to the content.
	}


}

void InitializeGame(GameObjectManager& manager);

void ResetAll(GameObjectManager& manager) {

	GameObjectManager::getInstance().clearAll();
	EnemyManager::getInstance().Reset();
	elapsed_seconds = 0;
	second_Timer.cancel();// stops and resets timer

	GameData::loadAllData();// technically allows hotreloading anything loaded from json
	Input::Initialize();
	InitializeGame(manager);




}

void InitializeGame(GameObjectManager& manager) {
	GameData::loadAllData();
	Input::Initialize();

	if (!font.openFromFile("../assets/fonts/amazon ember.ttf"))
		std::cerr << "font  file was not found";
	fpsCounter = std::make_shared<FPS_Counter>(font, true);

	playerView->setCenter({}); // sets center to 0,0.
	Projectile::projPool.init(512, 10);

#pragma region create background and vignette
	Background = GameObject::Create( // create gameobject for background.
		"../assets/sprites/cardboard.png",
		sf::IntRect{ {0,0},{1920,1080} },
		-110// move to layer -110 to stay behind things. 
	);      //-100 because background is set to be UI so
	// rendering will draw it using default sf::view . 

	Background->getSprite()->SetRepeated(true); // repeat over entire rect.
	Background->addComponent<BackgroundImage>();

	vignetteObject = GameObject::Create("../assets/sprites/shapes/bl_square_128.png", { {},{1920,1080} });
	vignetteObject->addComponent<Vignette>();
#pragma endregion

	CreateClassSelectionScreen();

}




int main() {
#pragma region create window and initialize global.h variables
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ 1920u, 1080u }), "Mox"); // make window
	playerView = std::make_shared<sf::View>(sf::FloatRect{ {0, 0},{1920u,1080u} });
	playerView->setCenter({});// center to 0,0
	window->setFramerateLimit(144); // cap fps
	window->setVerticalSyncEnabled(true);


#pragma endregion

#pragma region create delta time dt_clock	
	sf::Clock dt_clock;
	sf::Time Delta_Timer;

#pragma endregion
	// store reference so we dont gotta getinstance() multiple times.
	auto& manager = GameObjectManager::getInstance(); 
	InitializeGame(manager);

	// increment elapsed seconds with timer.
	second_Timer.getLoopEvent().subscribe([]() {elapsed_seconds++; });


	while (window->isOpen()) {
		Delta_Timer = dt_clock.restart();
		float deltaTime = Delta_Timer.asSeconds() * timeScale;
		// handle sfml events and update input.
		while (const std::optional event = window->pollEvent()) {
			Input::HandleEvent(event);
			if (event->is<sf::Event::Closed>()) window->close();
		}
		// done in gameplay loop so we can show fps anywhere.
		if (Input::GetActionDown("togglefps")) fpsCounter->ToggleShowFps();


		//Debug
		if (Input::GetKeyDown(sf::Keyboard::Scancode::Equal)) EnemyManager::SpawnEnemy(1, 2000);
		if (Input::GetKeyDown(sf::Keyboard::Scan::Delete)) ResetAll(manager);

		//end of debug

		// Update and render

		manager.updateAll(deltaTime); // call updatme() on all gameobjects
		Input::Update();// input updated for next frame.

		EnemyManager::HandleSpawning(deltaTime);
		second_Timer.update(deltaTime);

		fpsCounter->Update();
		window->clear();
		manager.renderAll(*window); // draw all gameobjects with sprites to window.



		window->display(); // display drawn image.
	}
}


