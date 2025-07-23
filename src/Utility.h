#pragma once
#include <random>

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