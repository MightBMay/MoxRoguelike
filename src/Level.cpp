#include"pch.h"


void Level::CreateBackground() {
	Background = GameObject::Create( // create gameobject for background.
		"../assets/sprites/cardboard.png",
		sf::IntRect{ {0,0},{1920,1080} },
		-110// move to layer -110 to stay behind things. 
	);      //-100 because background is set to be UI so
	// rendering will draw it using default sf::view . 

	Background->getSprite()->SetRepeated(true); // repeat over entire rect.
	Background->addComponent<BackgroundImage>();

	vignetteObject = GameObject::Create("../assets/sprites/shapes/bl_square_128.png", { {},{1920,1080} });
	vignetteObject->addComponent<Vignette>();
}

