#pragma once
#include <random>
#include <cmath> 
#include <SFML/Graphics.hpp>

constexpr float PI = 3.14159265358979323846f;

float vectorToAngle(const sf::Vector2f& movementVector);

sf::Vector2f getMouseWorldPos(const sf::RenderWindow& window, const sf::View& view);


class rng {
public:
    // Delete constructors to prevent instantiation
    rng() = delete;
    rng(const rng&) = delete;
    rng& operator=(const rng&) = delete;

    // Seed control (optional)
    static void setSeed(unsigned int seed);

    // Integer rng: [min, max] (inclusive)
    static int getInt(int min, int max);

    // Float rng: [min, max) (or use nextafter for inclusive)
    static float getFloat(float min, float max);

private:
    // Static engine (initialized once)
    static std::mt19937& getEngine();
};