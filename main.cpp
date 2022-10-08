#include <iostream>
#include "Game.h";

int main() {
	//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

	//Init game "engine"
	Game game;

	//Game loop
	while (game.getWindowIsOpen() && !game.getEndGame())
	{		
		//Update
		game.update();

		//Render
		game.render();
	}

	system("PAUSE");
	//End of app
	return 0;
}