#include "Game.h"


void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800,600),"Sapce game",sf::Style::Close|sf::Style::Titlebar);
	this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(false);
}

void Game::initGUI()
{
	//Load font
	if (!this->font.loadFromFile("Fonts/PixellettersFull.ttf"))
		std::cout << "ERROR::GAME::Failed to load font" << "\n";

	//Init point text
	this->pointText.setPosition(700.f, 25.f);
	this->pointText.setFont(this->font);
	this->pointText.setCharacterSize(20);
	this->pointText.setColor(sf::Color::White);
	this->pointText.setString("test");

	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(60);
	this->gameOverText.setColor(sf::Color::Red);
	this->gameOverText.setString("Game Over!");
	this->gameOverText.setPosition(
		this->window->getSize().x / 2.f - this->gameOverText.getGlobalBounds().width / 2.f, 
		this->window->getSize().y / 2.f - this->gameOverText.getGlobalBounds().height / 2.f);

	//Init player GUI
	this->playerHpBar.setSize(sf::Vector2f(300.f, 25.f));
	this->playerHpBar.setFillColor(sf::Color::Red);
	this->playerHpBar.setPosition(sf::Vector2f(20.f, 20.f));

	this->playerHpBarBack = this->playerHpBar;
	this->playerHpBarBack.setFillColor(sf::Color(25, 25, 25, 200));
}

void Game::initWorld()
{
	if (!this->worldBackgroundTex.loadFromFile("Textures/background.png"))
	{
		std::cout << "ERROR::GAME::COULD NOT LOAD BACKGROUND TEXTURE" << "\n";
	}

	this->worldBackground.setTexture(this->worldBackgroundTex);
}

void Game::initSystems()
{
	this->points = 0;
}

void Game::initPlayer()
{
	this->player = new Player();

}
void Game::initEnemies()
{
	this->spawnTimerMax = 50.f;
	this->spawnTimer = this->spawnTimerMax;
}


void Game::initTextures()
{
	this->textures["BULLET"] = new sf::Texture();
	this->textures["BULLET"]->loadFromFile("Textures/bullet.png");
}


Game::Game(void)
{
	this->initWindow();
	this->initTextures();
	this->initGUI();
	this->initWorld();
	this->initSystems();

	this->initPlayer();
	this->initEnemies();
}


Game::~Game(void)
{
	delete this->window;
	delete this->player;
	for(auto &i: this->textures)
	{
		delete i.second;
	}
	for(auto *i : this->bullets)
	{
		delete i;
	}
	for(auto *i : this->enemies)
	{
		delete i;
	}
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->updatePollEvents();

		if(this->player->getHp() > 0)
			this->update();

		this->render();
	}
}

void Game::updatePollEvents()
{
	sf::Event e;
	while(this->window->pollEvent(e))
	{
		if(e.Event::type == sf::Event::Closed)
			this->window->close();
		if(e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape)
			this->window->close();
	}
}

void Game::updateInput()
{
		// Move player
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->player->move(-1.f,0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->player->move(1.f,0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->player->move(0.f,-1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->player->move(0.f,1.f);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->player->canAttack())
	{
		this->bullets.push_back(
			new Bullet(
			this->textures["BULLET"],
			this->player->getPos().x + this->player->getBounds().width/2.f,
			this->player->getPos().y-32.f,
			0.f,
			-1.f,
			5.f));
	}

}

void Game::updateGUI()
{
	std::stringstream ss;

	ss << "Points: " << this->points;

	this->pointText.setString(ss.str());

	//Update player GUI
	float hpPercent = static_cast<float>(this->player->getHp()) / this->player->getHpMax();
	this->playerHpBar.setSize(sf::Vector2f(300.f * hpPercent, this->playerHpBar.getSize().y));
}

void Game::updateWorld()
{

}

void Game::updateCollision()
{
	//Left world collision
	if (this->player->getBounds().left < 0.f)
	{
		this->player->setPosition(0.f, this->player->getBounds().top);
	}
	//Right world collison
	else if (this->player->getBounds().left + this->player->getBounds().width >= this->window->getSize().x)
	{
		this->player->setPosition(this->window->getSize().x - this->player->getBounds().width, this->player->getBounds().top);
	}

	//Top world collision
	if (this->player->getBounds().top < 0.f)
	{
		this->player->setPosition(this->player->getBounds().left, 0.f);
	}
	//Bottom world collision
	else if (this->player->getBounds().top + this->player->getBounds().height >= this->window->getSize().y)
	{
		this->player->setPosition(this->player->getBounds().left, this->window->getSize().y - this->player->getBounds().height);
	}
}


void Game::updateBullets()
{
    for (auto it = this->bullets.begin(); it != this->bullets.end(); )
    {
        (*it)->update();

        // Bullet culling (top of screen)
        if ((*it)->getBounds().top + (*it)->getBounds().height < 0.f)
        {
            // Delete bullet and remove from vector
            delete *it;
            it = this->bullets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::updateEnemies()
{
	this->spawnTimer += 0.75f;
	if (this->spawnTimer >= this->spawnTimerMax)
	{
		this->enemies.push_back(new Enemy(rand() % this->window->getSize().x-20.f, -100.f));
		this->spawnTimer = 0.f;
	}
	 for (auto it = this->enemies.begin(); it != this->enemies.end(); )
    {
        (*it)->update();

        // Enemy culling (bottom of screen)
        if ((*it)->getBounds().top > this->window->getSize().y)
        {
            // Delete enemy and remove from vector
            delete *it;
            it = this->enemies.erase(it);
        }
        // Enemy-player collision
        else if ((*it)->getBounds().intersects(this->player->getBounds()))
        {
            // Handle collision
            this->player->loseHp((*it)->getDamage());
            delete *it;
            it = this->enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::updateCombat()
{
	for (int i = 0; i < this->enemies.size(); ++i)
	{
		bool enemy_deleted = false;
		for (size_t k = 0; k < this->bullets.size() && enemy_deleted == false; k++)
		{
			if (this->enemies[i]->getBounds().intersects(this->bullets[k]->getBounds()))
			{
				this->points += this->enemies[i]->getPoints();

				delete this->enemies[i];
				this->enemies.erase(this->enemies.begin() + i);

				delete this->bullets[k];
				this->bullets.erase(this->bullets.begin() + k);

				enemy_deleted = true;
			}
		}
	}
}

void Game::update()
{
	this->updateInput();

	this->player->update();

	this->updateCollision();

	this->updateBullets();

	this->updateEnemies();

	this->updateCombat();

	this->updateGUI();

	this->updateWorld();
}

void Game::renderGUI()
{
	this->window->draw(this->pointText);
	this->window->draw(this->playerHpBarBack);
	this->window->draw(this->playerHpBar);
}

void Game::renderWorld()
{
	this->window->draw(this->worldBackground);
}


void Game::render()
{
	this->window->clear();

	//Draw world
	this->renderWorld();

	//Draw all the stuffs
	this->player->render(*this->window);

	for (auto *bullet : this->bullets)
	{
		bullet->render(this->window);
	}

	for (auto *enemy : this->enemies)
	{
		enemy->render(this->window);
	}

	this->renderGUI();

	//Game over screen
	if (this->player->getHp() <= 0)
		this->window->draw(this->gameOverText);

	this->window->display();
}