#include "pch.h"
#include "GameObject.h"


void Vignette::init() {
	auto parentS = parent.lock();
	vignetteShader = std::make_shared<sf::Shader>();
	if (!vignetteShader->loadFromFile("../assets/shaders/vignette.frag", sf::Shader::Type::Fragment)) {
		std::cerr << "vignette shader not found\n";
	}
	parentS->setShader(vignetteShader);
	GameObjectManager::getInstance().setRenderLayer(parentS, 100);


}
void Vignette::update(float deltaTime) {
	vignetteShader->setUniform("cameraPos", playerView->getCenter());
	vignetteShader->setUniform("windowSize", sf::Vector2f(window->getSize()));
}
void Vignette::Destroy() {}