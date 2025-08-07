#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Timer.h"
extern std::shared_ptr<sf::RenderWindow> window;
extern std::shared_ptr<sf::View> playerView;


static inline sf::Font font;
static inline Timer second_Timer{ 1,true };
static inline int elapsed_seconds = 0;