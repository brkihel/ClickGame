#include "Game.h"

//Private functions
void Game::initVariables(){
	this->window = nullptr;

	//Game Logic
	this->endGame == false;
	this->playerScore = 0;
	this->health = 5;
	this->enemySpawnTimerMax = 10.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 6;
	this->enemySpeed = 3.f;
	this->mouseHeld = false;
	this->fpsValue = 60;
}

void Game::initWindow(){
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "Click Game", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(fpsValue);
}

void Game::initFonts(){
	if (this->font.loadFromFile("Fonts/Silkscreen-Bold.ttf")) {
		std::cout << "ERROR::INITIALIZING::FONTS" << std::endl;
		std::cout << "Failed to load the font... Check if the local is correct or the files are valid." << std::endl;
	}
}

void Game::initUiText(){
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(sf::Color::Black);
	this->uiText.setOutlineColor(sf::Color::White);
	this->uiText.setOutlineThickness(2.f);
	this->uiText.setString("NONE");
}

void Game::initEnemies(){
	this->clickRect.setSize(sf::Vector2f(70.f, 70.f));
	this->clickRect.setFillColor(sf::Color::Red);
	this->clickRect.setOutlineColor(sf::Color::Black);
	this->clickRect.setOutlineThickness(5.f);


}

//Constructors and desctructors
Game::Game() {
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initUiText();
	this->initEnemies();
}

Game::~Game() {
	delete this->window;
}


//Acessors
const bool Game::getWindowIsOpen() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGame;
}

//Functions
void Game::spawnEnemy(){
	/*
		@return void
		Spawns random enemies!
		-Sets a random size
		-Sets a random position
		-Sets a random color
		-Adds enemy to the vector
	*/
	this->clickRect.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->clickRect.getSize().x)),
		0.f
	);

	//Randomize enemy type
	int type = rand() % 3;

	switch (type){
	case 0:
		this->clickRect.setSize(sf::Vector2f(30.f, 30.f));
		this->clickRect.setFillColor(sf::Color::Red);
		break;
	case 1:
		this->clickRect.setSize(sf::Vector2f(50.f, 50.f));
		this->clickRect.setFillColor(sf::Color::Blue);
		break;
	case 2:
		this->clickRect.setSize(sf::Vector2f(70.f, 70.f));
		this->clickRect.setFillColor(sf::Color::Green);
		break;
	default:
		this->clickRect.setSize(sf::Vector2f(70.f, 70.f));
		this->clickRect.setFillColor(sf::Color::Yellow);
		break;
	}

	//Spawn the enemy!
	this->enemies.push_back(this->clickRect);

	//Remove the enemy at the end of the screen

}

void Game::pollEvents()
{
	//Event polling
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type) {
			//Close when click "x" in the window
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			//Close when press "Esc" keyboard button
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Game::updateMousePos(){
	/*
	@return void

	Updates the mouse position
		-Mouse position relative to the window(Vector2i)	
	*/

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText(){
	std::stringstream ss;

	ss << "Points: " << this->playerScore << std::endl;
	ss << "Healt Points: " << this->health << std::endl;

	this->uiText.setString(ss.str());
}

void Game::updateEnemies(){

	/*
		@return void

		Updates the enemy spawn rate(time)
		Spawn enemies while the number is less then the maximun amout of enemies
		Moves the enemies downwards
		Removes the enemies at the edge of the screen //TODO
	*/

	//Updating the enemy spawn rate timer
	if (this->enemies.size() < this->maxEnemies) {

		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			//Spawn the enemy and reset the timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else {
			this->enemySpawnTimer += 1.f;
		}		
	}

	//Enemies movement and update
	for (int i = 0; i < this->enemies.size(); i++) {
		bool deleted = false;
		this->enemies[i].move(0.f, enemySpeed);

		//Check if the enemy got to the bottom of the screen and delete it
		if (this->enemies[i].getPosition().y > this->window->getSize().y) {
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;			
			std::cout << "Health: " << this->health << std::endl;
		}
	}
	//Check if clicked and delete the enemy upon click
	//Also checks if the left mouse button is held(if it is then enemies will not be killed)
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (this->mouseHeld == false) {
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++) {
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
					
					//Gain score points
					if (this->enemies[i].getFillColor() == sf::Color::Red) {
						this->playerScore += 10;
						std::cout << "Points: " << this->playerScore << std::endl;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Blue) {
						this->playerScore += 5;
						std::cout << "Points: " << this->playerScore << std::endl;
					}
					else {
						this->playerScore += 1;
						std::cout << "Points: " << this->playerScore << std::endl;
					}

					//Delete the enemy(KILL + POINT!)
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}		
	}
	else {
		this->mouseHeld = false;
	}
	


}

void Game::update(){
	this->pollEvents();
	//Check if the game is running and keeps updating
	if (endGame == false) {
		this->updateMousePos();

		this->updateText();

		this->updateEnemies();
	}

	//End game condition
	//Check the players health and if it reaches 0 the players loses!
	if (this->health <= 0) {
		endGame = true;
		std::cout << "YOU LOSE!";
	}
}

void Game::renderEnemies(sf::RenderTarget& target){
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::renderText(sf::RenderTarget& target){
	target.draw(this->uiText);
}

void Game::render(){
	/*
	@return void
	-Clear old frame
	-Render objects
	-Display frame in window

	Renders the game objects.
	*/
	this->window->clear(sf::Color(0, 128, 255, 255));

	//Draw game objects
	this->renderEnemies(*this->window);

	this->renderText(*this->window);

	this->window->display();
}


