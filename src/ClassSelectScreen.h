#pragma once
// used to make and store class and level selection rects.
struct ClassSelectScreen {
	ClassSelectScreen(std::shared_ptr<GameObject>& player, std::shared_ptr<Level>& level) : player(player), level(level) {
		text = std::make_shared<sf::Text>(font);
		CreateClassSelectionRect();
		CreateLevelSelectionRect();
		CreateStartButton();
		CreateDescription();
		
	}
	void CreateClassSelectionRect();
	void CreateLevelSelectionRect();
	void CreateStartButton();


	void CreateDescription();
	void UpdateDescription(std::string str);
	void UpdateDescription(std::string className, const json& data);
	
	void UpdateStartButton();
	void StartLevel();


	std::shared_ptr<Level>& level;
	std::shared_ptr<GameObject>& player;

	std::shared_ptr<GameObject> startButtonObj;
	std::shared_ptr<sf::RenderTexture> startButtonRT;
	std::weak_ptr<UI_Button> startButton;



	std::shared_ptr<GameObject> classScrollObj;
	std::weak_ptr<UI_ScrollContainer> classScrollRect;

	std::shared_ptr<sf::RenderTexture> descriptionRT;
	std::shared_ptr<GameObject> descriptionObj;
	std::shared_ptr<MSprite> descriptionBackground;




	std::shared_ptr<GameObject> levelScrollObj;
	std::weak_ptr<UI_ScrollContainer> levelScrollRect;

	// re used objects- repositioned to draw.
	std::shared_ptr<sf::Text> text;

	

private:
	int playerClassIndex = -1;
	int levelIndex = -1;
;	sf::IntRect CreateRectFromJson(const json& data);
};
