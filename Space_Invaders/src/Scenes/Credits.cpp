#include "Scenes/Credits.h"

bool back = false;

Credits::Credits(json cfg) {
    // Load Script
    fs_text = new FSScrollTextManager(cfg, Scene::s_window, Scene::s_view, &key_space);

	// Music
    // music = new MusicPlayer("song3", true);
    // music->update(0.9f);

	// Background
	rstarfield = new RadialStarField(Scene::s_window,Scene::s_view,(int)cfg["rstarfield"]["starNum"],(float)cfg["rstarfield"]["speed"]);
}

Credits::~Credits() {
    delete fs_text;
	// delete music;
	delete rstarfield;
}

void Credits::pollEvents() {
    	while (Scene::s_window->pollEvent(Scene::s_events))
	{
		// window events
		switch (Scene::s_events.type)
		{
		case sf::Event::Closed:
			// Scene::s_window->close();
			m_return = true;
			break;
		case sf::Event::KeyPressed:
			if (Scene::s_events.key.code == sf::Keyboard::Escape)
			{
				m_return = true;
			}

			if (Scene::s_events.key.code == sf::Keyboard::Space)
			{
				key_space = true;
			}

			break;
		case sf::Event::KeyReleased:
			if (Scene::s_events.key.code == sf::Keyboard::Space)
			{
				key_space = false;
			}
		// No more type of events
		default:
			// This will change later
			break;
		}
	}
}

void Credits::update(float delta_time) {
    pollEvents();
    if (fs_text->tick()) {
		SceneManagement::goBackToMainMenu();
	};
	if (m_return) {
		m_return = false;
		SceneManagement::goBackToMainMenu();
	}
}

void Credits::render() {
	rstarfield->draw();
    fs_text->draw();
}