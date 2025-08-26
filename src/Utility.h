#pragma once



constexpr float PI = 3.14159265358979323846f;

float vectorToAngle(const sf::Vector2f& movementVector);

sf::Vector2f getMouseWorldPos(const sf::RenderWindow& window, const sf::View& playerView);
sf::Vector2f getMouseWorldPos(const std::shared_ptr<sf::RenderWindow>& window, const std::shared_ptr < sf::View>& playerView);

/// <summary>
/// rotates the given vector by a given angle.
/// </summary>
void rotateVectorByAngle(sf::Vector2f& vec, float angle);

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
    template<size_t quantity>
    static std::array<int, quantity> getInts(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        auto engine = getEngine();
        std::array<float, quantity> values;
        for (float& num : values)
            num = dist(engine);
        return values;
    }

    // Float rng: [min, max) (or use nextafter for inclusive)
    static float getFloat(float min, float max);
    template<size_t quantity>
    static std::array<float, quantity> getFloats(int min, int max) {
        std::uniform_real_distribution<float> dist(min, max);
        auto engine = getEngine();
        std::array<float, quantity> values;
        for (float& num : values)
            num = dist(engine);
        return values;
    }
    static std::mt19937& getEngine();

   
};