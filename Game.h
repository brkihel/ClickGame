#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

 /*
	Engine like class
	Wrapper class
 */

class Game{

private:
	//Variables
	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Mouse positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Resources
	sf::Font font;

	//Text
	sf::Text uiText;

	//Game logic
	bool endGame;
	unsigned playerScore;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	unsigned maxEnemies;
	float enemySpeed;
	bool mouseHeld;
	int fpsValue;

	//Game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape clickRect; //clickRect is the enemy actual name!

	//Private functions
	void initVariables();
	void initWindow();
	void initFonts();
	void initUiText();
	void initEnemies();

public:
	//Constructors and destructors
	Game();
	virtual ~Game();

	//Acessors
	const bool getWindowIsOpen() const;
	const bool getEndGame() const;

	//Functions
	void spawnEnemy();

	void pollEvents();
	void updateMousePos();
	void updateText();
	void updateEnemies();
	void update();	
	
	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void render();		
};

