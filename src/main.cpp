#include <SFML/Graphics.hpp>
#include "Drawing.h"
#include "TextureManager.h"
#include "MSprite.h"
#include "GameObject.h"

#include <iostream>

#define ASSETPATH "../assets/sprites/"



void CreatePlayer(std::shared_ptr<GameObject>& player) {
    player =
        std::make_shared<GameObject>(
            "../assets/sprites/gun.png",
            sf::IntRect{ {0, 0}, { 600,500 } }
    );

    player->setOrigin(300, 250);
    player->setPosition(960, 540);

    Drawing::AddDrawable(1, player->getSprite());
}



int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);

    sf::Texture& bg_tex = sf::Texture("../assets/sprites/cardboard.png");
    bg_tex.setRepeated(true);
    sf::Sprite& bg_spr = sf::Sprite(bg_tex, { {0,0 },{5000,5000} });

    sf::Texture& player_tex = sf::Texture("../assets/sprites/gun.png");
    sf::Sprite& player_spr = sf::Sprite(player_tex, { {0,0 },{600,500} });

    std::shared_ptr<GameObject> Player;
    CreatePlayer(Player);


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

                if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace) {

                    if (Player) {
                        GameObject::Destroy(Player);
                    }
                    else {
                        std::cout << "no player" << std::endl;
                    }
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {

                    if (!Player) {
                        CreatePlayer(Player);
                    }
                    else {
                        std::cout << "Player already exists" << std::endl;
                    }
                }

                if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
                    Player->scaleObject(1.1f, 1);
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
                    Player->scaleObject(0.9f, 1); 
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
                    Player->rotate(10);
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
                    Player->rotate(-10);
                }


            }

        }
   

        window.clear();
        
        Drawing::DrawAll(window);

        window.display();
    }
}
