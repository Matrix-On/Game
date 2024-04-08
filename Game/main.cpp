#include "Game.h"

int main()
{
	Game game;
	if (!game.Init())
	{
		printf("ERROR INIT!!!!!");
		return -1;
	}

	game.Run();

	return 0;
}