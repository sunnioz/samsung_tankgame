#include "Player.h"



void Player::initVariables()
{
	this->movementSpeed = 10.f;
	this->attackCooldownMax = 10.f;
	this->attackCooldown = this->attackCooldownMax;

	this->hpMax = 10;
	this->hp = this->hpMax;
}


void Player::initSprite()
{
	this->sprite.setTexture(this->texture);

	//Resize
	this->sprite.scale(0.15f,0.15f);

}

void Player::initTexture()
{
	if(this->texture.loadFromFile("Textures/tank_image.png") == false)
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE::Counld not";
	}

}


Player::Player(void)
{
	this->initVariables();
	this->initTexture();
	this->initSprite();
}
const sf::Vector2f & Player::getPos() const
{
	return this->sprite.getPosition();
}

const sf::FloatRect Player::getBounds() const
{
	return this->sprite.getGlobalBounds();
}

const int & Player::getHp() const
{
	return this->hp;
}

const int & Player::getHpMax() const
{
	return this->hpMax;
}

void Player::setPosition(const sf::Vector2f pos)
{
	this->sprite.setPosition(pos);
}

void Player::setPosition(const float x, const float y)
{
	this->sprite.setPosition(x, y);
}

void Player::setHp(const int hp)
{
	this->hp = hp;
}

void Player::loseHp(const int value)
{
	this->hp -= value;
	if (this->hp < 0)
		this->hp = 0;
}

Player::~Player(void)
{
}

void Player::move(const float dirX, const float dirY)
{
	this->sprite.move(this->movementSpeed* dirX, this->movementSpeed* dirY);
}



const bool Player::canAttack()
{
	if(this->attackCooldown >= this->attackCooldownMax)
	{
		this->attackCooldown = 0.f;
		return true;
	}
	return false;
}


void Player::updateAttack()
{
	if(this->attackCooldown < this->attackCooldownMax)
		this->attackCooldown += 0.75f;
}


void Player::update()
{
	this->updateAttack();
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
