#include "App/App.h"
#include "Game/Game.h"

int main()
{
	Game::Init("Flocking Bees");
	Game::Get().Run();
	Game::Shutdown();

	return 0;
}