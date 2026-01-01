
#include "board.h"


int main()
{

	GameComponents game;

	while (!glfwWindowShouldClose(game.window))
	{
		game.render();
	}
	game.end();
	return 0;
}