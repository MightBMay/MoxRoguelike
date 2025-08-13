#pragma once
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Timer.h"
extern std::shared_ptr<sf::RenderWindow> window;
extern std::shared_ptr<sf::View> playerView;


static inline sf::Font font;
static inline Timer second_Timer{ 1,true };
static inline int elapsed_seconds = 0;
static inline float timeScale = 0.5f;