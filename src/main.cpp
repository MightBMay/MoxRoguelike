
#include "pch.h"

#include "GameObject.h"
#include "EnemyManager.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Player.h"
#include "Enemy.h"
#include "StatUpgrade.h"
#include "ClassSelectScreen.h"



#pragma region global.h extern definitions

std::shared_ptr<sf::RenderWindow> window;
std::shared_ptr<sf::View> playerView;
sf::Font font;
MEvent<sf::Vector2u> windowResizeEvent{};
std::stack<sf::Cursor::Type> cursorStack;
Timer second_Timer{ 1,true };
int elapsed_seconds = 0;
float timeScale = 1.0f;


#pragma endregion


std::shared_ptr<GameObject> player;
std::unique_ptr<ClassSelectScreen> classSelectScreen;

std::shared_ptr<FPS_Counter> fpsCounter;
std::shared_ptr<Level> level;

std::shared_ptr<GameObject> backgroundObj;
std::shared_ptr<GameObject> vignetteObj;




void CreateBackground() {
	sf::IntRect rect = sf::IntRect{ {},{2560,1440} };
	backgroundObj = GameObject::Create( // create gameobject for background.
		"../assets/sprites/cardboard.png",
		rect,
		-110// move to layer -110 to stay behind things. 
	);      //-100 because background is set to be UI so
	// rendering will draw it using default sf::view . 

	backgroundObj->getSprite()->SetRepeated(true); // repeat over entire rect.
	backgroundObj->addComponent<BackgroundImage>();

	vignetteObj = GameObject::Create("../assets/sprites/shapes/bl_square_128.png", rect);
	vignetteObj->addComponent<Vignette>();
}


void InitializeGame(GameObjectManager& manager);

void ResetAll(GameObjectManager& manager) {

	GameObjectManager::getInstance().clearAll();
	EnemyManager::getInstance().Reset();
	elapsed_seconds = 0;
	second_Timer.clear(); // reuse same timer, but clear its subscribers.

	GameData::loadAllData();// technically allows hotreloading anything loaded from json
	Input::Initialize();
	InitializeGame(manager);




}

void InitializeGame(GameObjectManager& manager) {
	cursorStack = {};


	GameData::loadAllData();
	Input::Initialize();
	second_Timer.getLoopEvent().subscribe([]() {++elapsed_seconds; });
	
	if (!font.openFromFile("../assets/fonts/amazon ember.ttf"))
		std::cerr << "font  file was not found";
	fpsCounter = std::make_shared<FPS_Counter>(font, true);

	playerView->setCenter({}); // sets center to 0,0.
	Projectile::projPool.init(512, 10);
	CreateBackground();
	classSelectScreen = std::make_unique<ClassSelectScreen>(player, level);
}




int main() {


	// create window and initialize global.h variables							
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ 2560u, 1440u }), "Mox", sf::Style::Default); // make window
	playerView = std::make_shared<sf::View>(sf::FloatRect{ {0, 0},{2560u,1440u} });
	window->setSize({ 1280,720 });
	playerView->setCenter({});// center to 0,0
	window->setFramerateLimit(144); // cap fps
	window->setVerticalSyncEnabled(true);

#pragma region create delta time dt_clock	
	sf::Clock dt_clock;
	sf::Time Delta_Timer;

#pragma endregion
	// store reference so we dont gotta getinstance() multiple times.
	auto& manager = GameObjectManager::getInstance(); 
	
	InitializeGame(manager);
	Audio::Initialize();
	float volume = 0.5f;

	while (window->isOpen()) {
		Delta_Timer = dt_clock.restart();
		float deltaTime = Delta_Timer.asSeconds() * timeScale;
		// handle sfml events and update input.
		while (const std::optional event = window->pollEvent()) {
			Input::HandleEvent(event);
			if (event->is<sf::Event::Closed>()) window->close();

			if (const auto* resize = event->getIf<sf::Event::Resized>()) {
				windowResizeEvent.invoke(resize->size);
			}
		}
		// done in gameplay loop so we can show fps anywhere.
		if (Input::GetActionDown("togglefps")) fpsCounter->ToggleShowFps();
		if (Input::GetActionDown("forceclose")) window->close();


		//Debug
		if (Input::GetKeyDown(sf::Keyboard::Scan::Up)) volume += 0.05f;
		if (Input::GetKeyDown(sf::Keyboard::Scan::Down)) volume -= 0.05f;
		if (Input::GetKeyDown(sf::Keyboard::Scan::Right)) {
			auto eInst = Audio::CreateFMODEvent("event:/TestEvent");
			if (eInst != nullptr) {
				eInst->setParameterByName("volume", volume);
				eInst->start();
				eInst->release();
			}
		}
		if (Input::GetKeyDown(sf::Keyboard::Scancode::Equal)) EnemyManager::SpawnEnemy(1, 1);
		if (Input::GetKeyDown(sf::Keyboard::Scan::Delete)) ResetAll(manager);


		

		//end of debug

		// Update and render

		manager.updateAll(deltaTime); // call update() on all gameobjects
		Input::Update();// input updated for next frame.
		Audio::Update();
		EnemyManager::HandleSpawning(deltaTime);
		second_Timer.update(deltaTime);

		fpsCounter->Update();
		window->clear();
		manager.renderAll(*window); // draw all gameobjects with sprites to window.



		window->display(); // display drawn image.
	}
}


