#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class GameObject;

class Component {
public:
    GameObject* parent = nullptr; // Set by GameObject when added

    virtual void init() {}       // Called when component is added
    virtual void update(float deltaTime) = 0;
    virtual ~Component() = default;
    virtual void ProcessEvent(const std::optional<sf::Event>& event) = 0;
};