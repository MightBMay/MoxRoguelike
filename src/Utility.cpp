#include "pch.h"






void WrapTextH(sf::Text& text, float maxWidth) {
    const std::string& str = text.getString();
    const sf::Font& font = text.getFont();
    if (!&font) return; // safety

    unsigned int charSize = text.getCharacterSize();
    bool bold = (text.getStyle() & sf::Text::Bold) != 0;
    float outlineThickness = text.getOutlineThickness();

    std::unordered_map<char32_t, float> glyphCache;

    // lambda that handles caching glyph widths.
    auto getGlyphWidth = [&](char32_t c) -> float {
        auto it = glyphCache.find(c);
        if (it != glyphCache.end())
            return it->second;

        float adv = font.getGlyph(c, charSize, bold, outlineThickness).advance;
        glyphCache[c] = adv;
        return adv;
        };

    float spaceWidth = getGlyphWidth(U' ');

    std::string word, currentLine, wrappedText;
    float lineWidth = 0.f;

    for (size_t i = 0; i < str.size(); ++i) { 
        char c = str[i];

        if (c == '\n') {
            // Commit current line + force newline
            if (!wrappedText.empty()) wrappedText += '\n';
            wrappedText += currentLine;
            currentLine.clear();
            lineWidth = 0.f;
            word.clear();
            continue;
        }

        if (std::isspace(static_cast<unsigned char>(c))) {
            if (!word.empty()) {
                // Measure the word width
                float wordWidth = 0.f;
                for (unsigned char wc : word) {
                    wordWidth += getGlyphWidth(wc);
                }

                float testWidth = currentLine.empty() ? wordWidth : lineWidth + spaceWidth + wordWidth;

                if (testWidth > maxWidth) {
                    if (!wrappedText.empty()) wrappedText += '\n';
                    wrappedText += currentLine;

                    currentLine = word;
                    lineWidth = wordWidth;
                }
                else {
                    if (!currentLine.empty()) {
                        currentLine += ' ';
                        lineWidth += spaceWidth;
                    }
                    currentLine += word;
                    lineWidth = testWidth;
                }
                word.clear();
            }
        }
        else {
            word += c;
        }
    }

    // Flush last word
    if (!word.empty()) {
        float wordWidth = 0.f;
        for (unsigned char wc : word) {
            wordWidth += getGlyphWidth(wc);
        }
        float testWidth = currentLine.empty() ? wordWidth : lineWidth + spaceWidth + wordWidth;
        if (testWidth > maxWidth) {
            if (!wrappedText.empty()) wrappedText += '\n';
            wrappedText += currentLine;
            currentLine = word;
        }
        else {
            if (!currentLine.empty()) currentLine += ' ';
            currentLine += word;
        }
    }

    if (!wrappedText.empty()) wrappedText += '\n';
    wrappedText += currentLine;

    text.setString(wrappedText);
}



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

void rotateVectorByAngle(sf::Vector2f& vec, float angle) {
    float cos = std::cos(angle);
    float sin = std::sin(angle);
    vec = {
        vec.x * cos - vec.y * sin,
        vec.x * sin + vec.y * cos
    };

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
