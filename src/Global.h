#pragma once
#include <stack>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Timer.h"
extern std::shared_ptr<sf::RenderWindow> window;
extern std::shared_ptr<sf::View> playerView;


extern MEvent<sf::Vector2u> windowResizeEvent;

extern std::stack<sf::Cursor::Type> cursorStack;

extern  sf::Font font;
extern  Timer second_Timer;
extern  int elapsed_seconds;
extern  float timeScale;