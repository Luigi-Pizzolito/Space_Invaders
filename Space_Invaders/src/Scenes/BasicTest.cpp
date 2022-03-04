#include<iostream>
//STL-libs
#include <cstdlib>
#include <ctime>
//Group-Libs
#include "Application/BasicShooter.h"
///variables

int main() {

	//Client Side Application
	ShooterGame application(sf::Vector2i(320*2, 240*2), "Shooter Test");

	//Game Loop
	while (application.isRunning()) {

		//updates the events and game states
		application.update();

		//clears-renders-displays objects
		application.render();

	}

}