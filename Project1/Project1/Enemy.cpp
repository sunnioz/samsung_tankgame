#include "Enemy.h"

void Enemy::initShape()
{
	this->shape.setRadius(rand()%20+20);
	this->shape.setPointCount(rand()%20 + 3);
	this->shape.setFillColor(sf::Color(rand()%255+1,rand()%255+1,rand()%255+1));
}

void Enemy::initVariables()
{
	this->pointCount = rand() % 8 + 3; //min = 3 max = 10
	this->type		= 0;
	this->speed		= static_cast<float>(this->pointCount/3+3);
	this->hpMax		= static_cast<int>(this->pointCount);
	this->hp		= this->hpMax;
	this->damage	= this->pointCount;
	this->points	= this->pointCount;
}

Enemy::Enemy()
{

}


Enemy::Enemy(float pos_x, float pos_y)
{
	this->shape.setPosition(pos_x,pos_y);
	this->initShape();
	this->initVariables();
}


Enemy::~Enemy(void)
{
	
}

//Accessors
const sf::FloatRect Enemy::getBounds() const
{
	return this->shape.getGlobalBounds();
}

const int & Enemy::getPoints() const
{
	return this->points;
}

const int & Enemy::getDamage() const
{
	return this->damage;
}

void Enemy::update()
{
	this->shape.move(0.f, this->speed);
}

void Enemy::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
}

