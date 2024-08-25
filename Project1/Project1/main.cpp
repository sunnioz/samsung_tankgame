#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Player.h"
#include <time.h>
#include<cstdlib>
#include<ctime>
#include<iostream>

int main()
{
	srand(time(static_cast<unsigned>(0)));

    Game game;

    game.run();

    return 0;
}