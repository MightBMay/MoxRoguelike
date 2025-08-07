#include <iostream>

#include <SFML/Graphics.hpp>

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

#include "TrailRenderer.h"

//#include "CameraController.h"  can be used as an alternative camera controller. 
							  // currently just manually tracking in player.cpp/h;

#include "Player.h"
#include "Enemy.h"
#include "TestEnemy.h"
std::shared_ptr<sf::RenderWindow> window;
std::shared_ptr<sf::View> playerView;

std::shared_ptr<GameObject> player;
std::shared_ptr<GameObject> abilDesc;

void CreatePlayer(std::shared_ptr<GameObject>& playerObj, std::weak_ptr<Player>& player, GameObjectManager& manager) {
	playerObj = GameObject::Create(
		"../assets/sprites/gun.png",
		sf::IntRect{ {0, 0}, {128, 128} },
		5
	);
	playerObj->setOrigin(64, 64);
	playerObj->setPosition(960, 540);
	player = playerObj->addComponent<Player>(3);
	auto playerShared = player.lock();
	playerShared->init();

	player.lock()->CreateWeapons(window);


	Projectile::player = playerObj;
	Enemy::SetPlayer(playerObj.get());



}


int main() {
#pragma region create window
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ 1920u, 1080u }), "Mox"); // make window
	playerView = std::make_shared<sf::View>(sf::FloatRect{ {0, 0},{1920u,1080u} });
	//window->setFramerateLimit(144); // cap fps



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

	auto& enemyManager = EnemyManager::getInstance();

	Projectile::projPool.init(512, 10);

#pragma region make playerObj
	//std::shared_ptr<GameObject> playerObj; // declare playerObj
	std::weak_ptr<Player> p_move; // get pointer to playerObj's movement component.
	CreatePlayer(player, p_move, manager); // seperate method cuz it took a lot of space.

	auto vignetteObj = GameObject::Create("../assets/sprites/shapes/bl_square_128.png", { {},{1920,1080} });
	vignetteObj->addComponent<Vignette>();
#pragma endregion


#pragma region make background



	std::shared_ptr<GameObject> Background = GameObject::Create( // create gameobject for background.
		"../assets/sprites/cardboard.png",
		sf::IntRect{ {0,0},{1920,1080} },
		-110// move to layer -110 to stay behind things. 
	);      //-100 because background is set to be UI so
	// rendering will draw it using default sf::view . 

	Background->getSprite()->SetRepeated(true); // repeat over entire rect.
	Background->addComponent<BackgroundImage>();


#pragma endregion

	EnemyManager::SpawnEnemy(0, 5, 1028);


	while (window->isOpen()) {
		Delta_Timer = dt_clock.restart();
		float deltaTime = Delta_Timer.asSeconds();
		// Event handling (unchanged)
		while (const std::optional event = window->pollEvent()) {


			if (event->is<sf::Event::Closed>()) {
				window->close();
			}


			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

				switch (keyPressed->scancode)
				{
					case sf::Keyboard::Scancode::NumLock:
						EnemyManager::SpawnEnemy(0, 5, 500);
						break;


					case sf::Keyboard::Scancode::Space:
						std::cout<< "\nEnemies: "<<enemyManager.count();
						break;
					
				}


				/* unused atm, moved the input stuff to their own component.


				}
				else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {

				}
				*/





			}
			manager.processEvent(event); //Sends event to ALL gameobjects. easy but kinda inefficient, improve later.

		}

		// Update and render
		manager.updateAll(deltaTime); // call updatme() on all gameobjects
		EnemyManager::HandleSpawning(deltaTime);
		second_Timer.update(deltaTime);
		window->clear();

		manager.renderAll(*window); // draw all gameobjects with sprites to window.

#pragma region FPS logging
		timeSinceLastUpdate += fpsClock.restart();
		frameCount++;

		if (timeSinceLastUpdate >= updateInterval) {
			float fps = frameCount / timeSinceLastUpdate.asSeconds();
			std::cout << "\rFPS: " << std::fixed << std::setprecision(1) << fps << std::flush;

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

