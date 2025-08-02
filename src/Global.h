#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Timer.h"
extern std::shared_ptr<sf::RenderWindow> window;
extern std::shared_ptr<sf::View> playerView;

static inline Timer second_Timer{ 1,true };