#pragma once
#include "Component.h"
class Vignette : public Component {
public:
    virtual void init() override;
    virtual void update(float deltaTime)override;
    virtual void Destroy() override;
    virtual void ProcessEvent(const std::optional<sf::Event>& event) override {}

private:
    std::shared_ptr<sf::Shader> vignetteShader;

};