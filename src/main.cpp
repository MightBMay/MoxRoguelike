#include <iostream>

#include <SFML/Graphics.hpp>

#include "Utility.h"

#include "TextureManager.h"
#include "EnemyManager.h"
#include "Projectile.h"
#include "Weapon.h"

#include "MSprite.h"
#include "GameObject.h"


#include "Player_Movement.h"
#include "EnemyMovement.h"

std::shared_ptr<sf::RenderWindow> window;

void CreatePlayer(std::shared_ptr<GameObject>& player, PlayerMovement*& pmove, GameObjectManager& manager) {
	player = std::make_shared<GameObject>(
		"../assets/sprites/gun.png",
		sf::IntRect{ {0, 0}, {128, 128} }
	);
	player->setOrigin(64, 64);
	player->setPosition(960, 540);
	pmove = player->addComponent<PlayerMovement>();
	manager.setRenderLayer(player.get(), 3);


	
	player->addComponent<Weapon<Projectile>>(
		std::make_shared<WeaponStats>(1, 250, 500, 32, 1), 
		window
	);




	EnemyMovement::SetPlayer(player);


}

void CreateTestEnemy(GameObjectManager& manager, EnemyManager& enemyManager) {
	static std::weak_ptr<GameObject> player = EnemyMovement::GetPlayer();
	std::shared_ptr<GameObject> enemy = std::make_shared<GameObject>(
		"../assets/sprites/twig.png",
		sf::IntRect{ {0,0},{128,150} }
	);

	enemy->setOrigin(64, 75);
	auto enemyMove = enemy->addComponent<EnemyMovement>();
	enemyMove->init();



	sf::Vector2f playerPos = player.lock()->getPosition();
	enemy->setPosition(
		playerPos +
		sf::Vector2f{ rng::getFloat(-2000, 2000), rng::getFloat(-800, 800) }

	);

	manager.setRenderLayer(enemy.get(), 1);

}


int main() {
#pragma region create window
	window =std::make_shared<sf::RenderWindow>(sf::VideoMode({ 1920u, 1080u }), "Mox"); // make window
	window = window;
	window->setFramerateLimit(144); // cap fps
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

	auto& projectilePool = ProjectilePool::getInstance();
	

#pragma region make player
	std::shared_ptr<GameObject> Player; // declare player
	PlayerMovement* p_move; // get pointer to player's movement component.
	CreatePlayer(Player, p_move, manager); // seperate method cuz it took a lot of space.
	projectilePool.init(256, Player);



#pragma endregion


#pragma region make background



	std::shared_ptr<GameObject> Background = std::make_shared<GameObject>( // create gameobject for background.
		"../assets/sprites/cardboard.png",
		sf::IntRect{ {0,0},{1920,1080} }
	);
	Background->getSprite()->SetRepeated(true); // repeat over entire rect.
	manager.setRenderLayer(Background.get(), -10); // move to layer -10 to stay behind things.
#pragma endregion

	//for (int i = 0; i < 500; i++)
		//CreateTestEnemy(manager, enemyManager);


	while (window->isOpen()) {
		Delta_Timer = dt_clock.restart();
		float deltaTime = Delta_Timer.asSeconds();
		// Event handling (unchanged)
		while (const std::optional event = window->pollEvent()) {


			if (event->is<sf::Event::Closed>()) {
				window->close();
			}


			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

				if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {



				}/* unused atm, moved the input stuff to their own component.
				else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {

				}
				*/





			}
			manager.processEvent(event); //Sends event to ALL gameobjects. easy but kinda inefficient, improve later.

		}

		// Update and render
		manager.updateAll(deltaTime); // call updatme() on all gameobjects

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
* destruction and re creation of player example.

					if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace) {
						if (Player) {
							GameObject::Destroy(Player);
							Player.reset(); // Clear the shared_ptr
							p_move = nullptr;
						}
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
						if (!Player) {
							CreatePlayer(Player, p_move, manager);

							std::cout << "no";
						}std::cout << "yes";
					}



*/

