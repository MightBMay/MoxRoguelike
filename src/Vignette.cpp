#include "Vignette.h"
#include "GameObject.h"
#include "Global.h"

void Vignette::init() {
	vignetteShader = std::make_shared<sf::Shader>();
	if (!vignetteShader->loadFromFile("../assets/shaders/vignette.frag", sf::Shader::Type::Fragment)) {
		std::cerr << "vignette shader not found\n";
	}
	parent->setShader(vignetteShader);
	GameObjectManager::getInstance().setRenderLayer(parent, 100);


}
void Vignette::update(float deltaTime) {
	vignetteShader->setUniform("cameraPos", playerView->getCenter());
	vignetteShader->setUniform("windowSize", sf::Vector2f(window->getSize()));
}
void Vignette::Destroy() {}