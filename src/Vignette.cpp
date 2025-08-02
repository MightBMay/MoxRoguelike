#include "Vignette.h"
#include "GameObject.h"
#include "Global.h"

void Vignette::init() {
	vignetteShader = std::make_shared<sf::Shader>();
	if (!vignetteShader->loadFromFile("../assets/shaders/vignette.frag", sf::Shader::Type::Fragment)) {
		std::cerr << "vignette shader not found\n";
	}
	parent->setShader(vignetteShader);
	parent->setAsUI(true);
	GameObjectManager::getInstance().setRenderLayer(parent, 100);


}
void Vignette::update(float deltaTime) {
	vignetteShader->setUniform("cameraPos", playerView->getCenter());
}
void Vignette::Destroy() {}