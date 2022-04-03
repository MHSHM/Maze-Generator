#include <Game.h>

#include <glew.h>
#include <glfw3.h>

int main(void)
{
	Game game;

	bool init = game.Init();

	if (init == true)
	{
		game.Run_Game(); 
	}

	game.Shutdown();
}