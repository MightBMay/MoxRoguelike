#pragma once
// used to make and store class and level selection rects.
struct ClassSelectScreen {
	ClassSelectScreen(std::shared_ptr<GameObject>& player, std::shared_ptr<Level>& level) : player(player), level(level) {
		CreateClassSelectionRect();
		CreateLevelSelectionRect();
		CreateStartButton();
	}
	void CreateClassSelectionRect();
	void CreateLevelSelectionRect();
	void CreateStartButton();

	void StartLevel();

	void UpdateStartButton();

	std::shared_ptr<Level>& level;
	std::shared_ptr<GameObject>& player;

	std::shared_ptr<GameObject> startButtonObj;
	std::weak_ptr<UI_Button> startButton;


	std::shared_ptr<sf::RenderTexture> startButtonRT;
	std::shared_ptr<sf::Text> startButtonText;

	std::shared_ptr<GameObject> classScrollObj;
	std::weak_ptr<UI_ScrollContainer> classScrollRect;

	std::shared_ptr<GameObject> levelScrollObj;
	std::weak_ptr<UI_ScrollContainer> levelScrollRect;
	

private:
	int playerClassIndex = -1;
	int levelIndex = -1;
;	sf::IntRect CreateRectFromJson(const json& data);
};
