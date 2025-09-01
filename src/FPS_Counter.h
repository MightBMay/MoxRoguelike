#pragma once



class Renderable;


class FPS_Counter
{
public:
	FPS_Counter(sf::Font& font, bool startWithFpsShown = false);

	void Update();

	void SetShowFps(const bool value);

private:
	static inline bool showFps = false;
	std::shared_ptr<sf::Text> text;
	std::shared_ptr<Renderable> renderable;
	sf::Clock fpsClock;// s.e
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	int frameCount = 0;
	const sf::Time updateInterval = sf::seconds(0.25f);
	std::ostringstream timer_stringStream;

};

