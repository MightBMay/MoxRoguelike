#include <iostream>

#include <SFML/Graphics.hpp>

#include "TextureManager.h"
#include "MSprite.h"
#include "GameObject.h"

#include "Player_Movement.h"

#define ASSETPATH "../assets/sprites/"



void CreatePlayer(std::shared_ptr<GameObject>& player, PlayerMovement*& pmove, GameObjectManager& manager) {
	player = std::make_shared<GameObject>(
		"../assets/sprites/gun.png",
		sf::IntRect{ {0, 0}, {600, 500} }
	);
	player->setOrigin(300, 250);
	player->setPosition(960, 540);
	pmove = player->addComponent<PlayerMovement>();
	manager.setRenderLayer(player.get(), 1);
}



int main() {
	auto window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "CMake SFML Project"); // make window
	window.setFramerateLimit(144); // cap fps
	sf::Clock clock; // used for deltatime
	sf::Time Delta_Timer;

	auto& manager = GameObjectManager::getInstance(); // manager allows access to all gameobjects at once.

	std::shared_ptr<GameObject> Background = std::make_shared<GameObject>( // create gameobject for background.
		"../assets/sprites/cardboard.png",
		sf::IntRect{ {0,0},{1920,1080} }
	);
	Background->getSprite()->SetRepeated(true); // repeat over entire rect.
	manager.add(Background.get(), -10); // move to layer -10 to stay behind things.


	std::shared_ptr<GameObject> Player; // declare player
	PlayerMovement* p_move; // get pointer to player's movement component.
	CreatePlayer(Player, p_move, manager); // seperate method cuz it took a lot of space.




	while (window.isOpen()) {
		Delta_Timer = clock.restart();
		float deltaTime = Delta_Timer.asSeconds();

		// Event handling (unchanged)
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				
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
							Player->addComponent<PlayerMovement>();
							std::cout << "no";
						}std::cout << "yes";
					}
					
					if (p_move) {
					
						// Handle movement keys
						switch (keyPressed->scancode) { 
						case sf::Keyboard::Scancode::Up:    p_move->direction.y = -1; break;
						case sf::Keyboard::Scancode::Down:  p_move->direction.y = 1;  break;
						case sf::Keyboard::Scancode::Left:  p_move->direction.x = -1; break;
						case sf::Keyboard::Scancode::Right: p_move->direction.x = 1;  break;
						default: break;
						}
					
					}

			}
			else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
				if (p_move) {
					switch (keyReleased->scancode) { // if up AND down are released, y must be 0. if left AND right are released, x must be 0.
					case sf::Keyboard::Scancode::Up:
					case sf::Keyboard::Scancode::Down:    p_move->direction.y = 0; break;
					case sf::Keyboard::Scancode::Left:
					case sf::Keyboard::Scancode::Right:   p_move->direction.x = 0; break;
					default: break;
					}
				}
			}


			if (p_move) {
				std::cout << p_move->direction.x <<", "<<p_move-> direction.y << std::endl;
			}
		}

		// Update and render
		manager.updateAll(deltaTime);
		window.clear();
		manager.renderAll(window); // Replaces Drawing::DrawAll()
		window.display();
	}
}


