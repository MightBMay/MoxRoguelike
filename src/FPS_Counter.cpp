#include "pch.h"
#include "FPS_Counter.h"


FPS_Counter::FPS_Counter(sf::Font& font, bool startWithFpsShown) {
	text = std::make_shared<sf::Text>(font);
	text->setOutlineThickness(2);
	
	renderable = std::make_shared<Renderable>(text,text, nullptr);
	GameObjectManager::getInstance().addExternalRenderable(renderable,1000);

}

void FPS_Counter::Update() {
	timeSinceLastUpdate += fpsClock.restart();
	frameCount++;
	if (timeSinceLastUpdate >= updateInterval) {
		timer_stringStream.clear();
		timer_stringStream.str("");
		if (showFps) {
			float fps = frameCount / timeSinceLastUpdate.asSeconds();
			timer_stringStream << "FPS: " << std::fixed << std::setprecision(1) << fps << "\n";
		}


		int hours = elapsed_seconds / 3600;
		int minutes = (elapsed_seconds / 60) % 60;
		int seconds = elapsed_seconds % 60;

		if (hours > 0)
			timer_stringStream << hours << ":";

		timer_stringStream << std::setfill('0') << std::setw(2) << minutes << ":"
			<< std::setfill('0') << std::setw(2) << seconds;



		text->setString(timer_stringStream.str());
		frameCount = 0;
		timeSinceLastUpdate = sf::Time::Zero;
	}
}

void FPS_Counter::SetShowFps(const bool value) {
	showFps = value;
}
