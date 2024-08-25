#pragma once

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include <map>
#include<string>
#include<sstream>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>

class Game
{
private:
	//Window
	sf::RenderWindow* window;

	//Resources
	std::map<std::string, sf::Texture*> textures;
	std::vector<Bullet*> bullets;

	//GUI
	sf::Font font;
	sf::Text pointText;

	sf::Text gameOverText;

	//World
	sf::Texture worldBackgroundTex;
	sf::Sprite worldBackground;

	//Systems
	unsigned points;


	// Player
	Player* player;

	//PlayerGUI
	sf::RectangleShape playerHpBar;
	sf::RectangleShape playerHpBarBack;

	//Enemy
	float spawnTimer;
	float spawnTimerMax;
	std::vector<Enemy*> enemies;


	void initWindow();

	void initTextures();
	void initGUI();
	void initWorld();
	void initSystems();


	void initPlayer();
	void initEnemies();
public:
	Game(void);
	virtual ~Game(void);



	void run();

	void updatePollEvents();
	void updateInput();
	void updateGUI();
	void updateWorld();
	void updateCollision();
	void updateBullets();
	void updateEnemies();
	void updateCombat();
	void update();

	void renderGUI();
	void renderWorld();
	void render();
};

