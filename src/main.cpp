
#include "pch.h"

#include "GameObject.h"
#include "EnemyManager.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Player.h"
#include "Enemy.h"
#include "StatUpgrade.h"





std::shared_ptr<sf::RenderWindow> window;
std::shared_ptr<sf::View> playerView;

std::shared_ptr<GameObject> player;


void InitializeGame(GameObjectManager& manager, std::shared_ptr<Renderable> fpsText);

void ResetAll(GameObjectManager& manager, std::shared_ptr<Renderable> fpsText) {

	GameObjectManager::getInstance().clearAll();
	EnemyManager::getInstance().Reset();
	elapsed_seconds = 0;
	second_Timer.cancel();// stops and resets timer
	second_Timer.start(); // re start the timer.
	InitializeGame(manager,fpsText);



}

void InitializeGame(GameObjectManager& manager, std::shared_ptr<Renderable> fpsText) {
	playerView->setCenter({});
	Projectile::projPool.init(512, 10);
	player = Player::CreatePlayerClass(1); 
	Enemies::Enemy::SetPlayer(player.get());

#pragma region create background
	std::shared_ptr<GameObject> Background = GameObject::Create( // create gameobject for background.
		"../assets/sprites/cardboard.png",
		sf::IntRect{ {0,0},{1920,1080} },
		-110// move to layer -110 to stay behind things. 
	);      //-100 because background is set to be UI so
	// rendering will draw it using default sf::view . 

	Background->getSprite()->SetRepeated(true); // repeat over entire rect.
	Background->addComponent<BackgroundImage>();
#pragma endregion


	auto vignetteObj = GameObject::Create("../assets/sprites/shapes/bl_square_128.png", { {},{1920,1080} });
	vignetteObj->addComponent<Vignette>();

	
	manager.addExternalRenderable(fpsText, 1000);
}




int main() {
#pragma region create window and initialize global.h variables
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ 1920u, 1080u }), "Mox"); // make window
	playerView = std::make_shared<sf::View>(sf::FloatRect{ {0, 0},{1920u,1080u} });
	playerView->setCenter({});// center to 0,0
	window->setFramerateLimit(144); // cap fps
	window->setVerticalSyncEnabled(true);
	font.openFromFile("../assets/fonts/amazon ember.ttf");
#pragma endregion
	
#pragma region create delta time dt_clock
	sf::Clock dt_clock;
	sf::Time Delta_Timer;

#pragma endregion

#pragma region FPS logging
	sf::Clock fpsClock;// s.e
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	int frameCount = 0;
	const sf::Time updateInterval = sf::seconds(0.25f);
#pragma endregion

	GameData::loadAllData();
	auto& manager = GameObjectManager::getInstance(); // manager allows access to all gameobjects at once.
	Input::Initialize();


	//auto scrollContainerRect = sf::FloatRect{ {500,300}, {800,128} };// define scroll rect size/position.
	//// make empty gameobject (prob set gameobject sprite size to same as rect size) (set layer to UI layer as well)
	//auto scrollContainerObj = GameObject::Create("../assets/sprites/shapes/square_128.png", { {},{809,128} }, 130);
	//scrollContainerObj->setPosition(scrollContainerRect.position); // subtract a lil to offset the background from the content.
	//// create scrollContainer
	//auto scrollContainer = scrollContainerObj->addComponent<UI_ScrollContainer>(
	//	scrollContainerRect,
	//	sf::Vector2f{ 128, 128 } // set this to the size of the content's sprites.
	//);
	//auto scrollContainerSprite = scrollContainerObj->getSprite(); // set values for scroll container's sprite.
	//scrollContainerSprite->SetRepeated(true);
	//scrollContainerSprite->setColor(sf::Color(0,0,0,64));


	//auto scrollRectShader = std::make_shared<sf::Shader>(); // shader needed to mask content.
	//if (!scrollRectShader->loadFromFile("../assets/shaders/ScrollRectMask.frag", sf::Shader::Type::Fragment)) {
	//	std::cerr << "\nSpriteRectMask shader not found.";
	//}
	//sf::IntRect contentRect = { {},{128,128} }; // again, size of the content's sprite.
	//auto scrollContainer_S = scrollContainer.lock();
	//for (int i = 0; i < 3; ++i) {
	//	// create content object and do whatever you need to be done to it.
	//	auto& temp = GameObject::Create("../assets/sprites/shapes/square_128.png", contentRect, 131);
	//	temp->setShader(scrollRectShader);
	//	temp->addComponent<UI_Button>(window).lock()->getOnClick().subscribe([i]() {std::cout << "\nclicked: " << i; });
	//	// set position accordingly
	//	temp->setPosition(scrollContainerRect.position.x + (contentRect.size.x * i), scrollContainerRect.position.y);
	//	scrollContainer_S->addContent(temp); // be sure to actually add to the content.
	//}




	std::shared_ptr<sf::Text> fpsText = std::make_shared<sf::Text>(font);
	fpsText->setOutlineThickness(2);
	std::shared_ptr<Renderable> fpsTextRenderable = std::make_shared<Renderable>(fpsText, nullptr);

	InitializeGame(manager, fpsTextRenderable);	
	
#pragma endregion

	second_Timer.start();
	second_Timer.getEndEvent().subscribe([]() {elapsed_seconds++; });
	std::ostringstream timer_stringStream;

	while (window->isOpen()) {
		Delta_Timer = dt_clock.restart();
		float deltaTime = Delta_Timer.asSeconds() * timeScale;
		// handle sfml events and update input.
		while (const std::optional event = window->pollEvent()) {
			Input::HandleEvent(event);
			if (event->is<sf::Event::Closed>()) window->close();			
		}

		if (Input::GetKeyDown(sf::Keyboard::Scancode::Equal)) EnemyManager::SpawnEnemy(1);
		if (Input::GetKeyDown(sf::Keyboard::Scancode::Down)) player->getDerivativesOfComponent<Player>()->takeDamage(5);
		if (Input::GetKeyDown(sf::Keyboard::Scan::Delete)) ResetAll(manager, fpsTextRenderable);


		// Update and render
		
		manager.updateAll(deltaTime); // call updatme() on all gameobjects
		Input::Update();
		//EnemyManager::HandleSpawning(deltaTime);
		second_Timer.update(deltaTime);
		window->clear();

		manager.renderAll(*window); // draw all gameobjects with sprites to window.

#pragma region FPS logging
		timeSinceLastUpdate += fpsClock.restart();
		frameCount++;

		if (timeSinceLastUpdate >= updateInterval) {
			float fps = frameCount / timeSinceLastUpdate.asSeconds();
			timer_stringStream.str("");// clear actual string
			timer_stringStream << "FPS: " << std::fixed << std::setprecision(1) << fps<< "\n";


			int hours = elapsed_seconds / 3600;
			int minutes = (elapsed_seconds / 60) % 60;
			int seconds = elapsed_seconds % 60;

			if (hours > 0)
				timer_stringStream << hours << ":";

			timer_stringStream << std::setfill('0') << std::setw(2) << minutes << ":"
				<< std::setfill('0') << std::setw(2) << seconds;



			fpsText->setString(timer_stringStream.str());
			frameCount = 0;
			timeSinceLastUpdate = sf::Time::Zero;
		}

#pragma endregion

		window->display(); // display drawn image.
	}
}


/*
*
* destruction and re creation of playerObj example.

					if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace) {
						if (playerObj) {
							GameObject::Destroy(playerObj);
							playerObj.reset(); // Clear the shared_ptr
							p_move = nullptr;
						}
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
						if (!playerObj) {
							CreatePlayer(playerObj, p_move, manager);

							std::cout << "no";
						}std::cout << "yes";
					}



*/

