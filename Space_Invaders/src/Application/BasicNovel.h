#pragma once
#include "SceneManager/Scene.h"
#include "SceneManager/TextManager.h"
#include "SceneManager/MusicPlayer.h"
// #include <vector>
// #include <string>

class NovelGame :public SceneManagement::Scene {

private:
	MusicPlayer* music;
	float lvl_p = 0.0f;
    //Fonts
    // sf::Font font;

	//Textures
	// sf::Texture dialog_overlay;

	//Background

	//Camera

    //Input bools
    bool key_space = false;

    // Internal functions
    // TextPanel* text_panel;
	TextManager* text_panel;

public:
	//Constructors and Destructors
	NovelGame(const sf::Vector2i& screen_dimensions, const char* app_name);
	~NovelGame();
	void pollEvents()override;
	void update(float delta_time) override;
	void render()override;
};
