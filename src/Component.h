#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

class GameObject;

class Component {
public:
    std::shared_ptr<GameObject> parent; // Set by GameObject when added
    //if initialization is needed
    virtual void init() {} 
    virtual void update(float deltaTime) = 0;
    virtual void Destroy() = 0;
    virtual ~Component() = default;
    virtual void ProcessEvent(const std::optional<sf::Event>& event) = 0;
};