#pragma once
#include <iostream>
#include <SFML/System/Vector2.hpp>
std::ostream& operator<<(std::ostream& os, const sf::Vector2f& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2i& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}