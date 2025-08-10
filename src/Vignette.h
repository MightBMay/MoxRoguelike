#pragma once
#include "Component.h"

#include <SFML/Graphics/Shader.hpp>
class Vignette : public Component {
public:
    virtual void init() override;
    virtual void update(float deltaTime)override;
    virtual void Destroy() override;


private:
    std::shared_ptr<sf::Shader> vignetteShader;

};