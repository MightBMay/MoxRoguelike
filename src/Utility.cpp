#include "Utility.h"
#include <chrono>

// Static engine initialization
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