
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <sstream>
#include <iostream>

#include "GameObject.h"
#include "MSprite.h"
#include "Utility.h"
#include "TextureManager.h"
#include "EnemyManager.h"
#include "Projectile.h"
#include "Weapon.h"
#include "UI_Button.h"
#include "UI_CooldownSprite.h"
#include "UI_AbilityBar.h"
#include "Background.h"
#include "Vignette.h"
#include "Global.h"
#include "ProgressBar.h"
#include "TrailRenderer.h"
#include "Player.h"
#include "Enemy.h"
#include "Input.h"


std::shared_ptr<sf::RenderWindow> window;
std::shared_ptr<sf::View> playerView;

std::shared_ptr<GameObject> player;
std::shared_ptr<GameObject> abilDesc;

void CreatePlayer(std::shared_ptr<GameObject>& playerObj, GameObjectManager& manager) {
	playerObj = GameObject::Create(
		"../assets/sprites/gun.png",
		sf::IntRect{ {0, 0}, {128, 128} },
		5
	);
	playerObj->setOrigin(64, 64);
	playerObj->setPosition(960, 540);
	playerObj->addComponent<Player>();



	Projectile::player = playerObj;
	Enemy::SetPlayer(playerObj.get());



}


int main() {
#pragma region create window and initialize global.h variables
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ 1920u, 1080u }), "Mox"); // make window
	playerView = std::make_shared<sf::View>(sf::FloatRect{ {0, 0},{1920u,1080u} });
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

	auto& manager = GameObjectManager::getInstance(); // manager allows access to all gameobjects at once.
	Projectile::projPool.init(512, 10);
	Input::Initialize();

	
#pragma region fps text stuff

	std::shared_ptr<sf::Text> fpsText = std::make_shared<sf::Text>(font);
	fpsText->setOutlineThickness(2);
	std::shared_ptr<Renderable> fpsTextRenderable = std::make_shared<Renderable>(fpsText, nullptr);
	manager.addExternalRenderable(fpsTextRenderable, 1000);

#pragma endregion

	CreatePlayer(player, manager); // seperate method cuz it took a lot of space.


#pragma region make background
	std::shared_ptr<GameObject> Background = GameObject::Create( // create gameobject for background.
		"../assets/sprites/cardboard.png",
		sf::IntRect{ {0,0},{1920,1080} },
		-110// move to layer -110 to stay behind things. 
	);      //-100 because background is set to be UI so
	// rendering will draw it using default sf::view . 

	Background->getSprite()->SetRepeated(true); // repeat over entire rect.
	Background->addComponent<BackgroundImage>();

	auto vignetteObj = GameObject::Create("../assets/sprites/shapes/bl_square_128.png", { {},{1920,1080} });
	vignetteObj->addComponent<Vignette>();

#pragma endregion


	second_Timer.start();
	second_Timer.getEndEvent().subscribe([]() {elapsed_seconds++; });
	std::ostringstream timer_stringStream;


	while (window->isOpen()) {
		Delta_Timer = dt_clock.restart();
		float deltaTime = Delta_Timer.asSeconds();
		// handle sfml events and update input.
		while (const std::optional event = window->pollEvent()) {

			Input::HandleEvent(event);
			
			if (event->is<sf::Event::Closed>()) 
				window->close();
			
		}
		
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

