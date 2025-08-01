#include "Utility.h"
#include <chrono>






float vectorToAngle(const sf::Vector2f& movementVector) {
    // Calculate the angle in radians using atan2
    // Note: atan2 takes (y, x) but we're using (x, -y) to get the correct orientation
    float angleRad = std::atan2(movementVector.x, -movementVector.y);

    // Convert to degrees
    float angleDeg = angleRad * (180.0f / PI);

    // Ensure the angle is in the 0-360 range
    if (angleDeg < 0) {
        angleDeg += 360.0f;
    }

    return angleDeg;
}


sf::Vector2f getMouseWorldPos(const sf::RenderWindow& window, const sf::View& playerView) {
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);

    // Convert to world coordinates using the current playerView
    return window.mapPixelToCoords(mousePixelPos, playerView);
}

sf::Vector2f getMouseWorldPos(
    const std::shared_ptr<sf::RenderWindow>& window, 
    const std::shared_ptr < sf::View>& playerView) {
    assert(window && "window is null in getMouseWorldPos!");
    assert(playerView && "view is null in getMouseWorldPos!");
    return getMouseWorldPos(*window, *playerView);
}




std::mt19937& rng::getEngine() {
    static std::mt19937 engine(std::random_device{}());
    return engine;
}

void rng::setSeed(unsigned int seed) {
    getEngine().seed(seed);
}

int rng::getInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(getEngine());
}

float rng::getFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(getEngine());
}