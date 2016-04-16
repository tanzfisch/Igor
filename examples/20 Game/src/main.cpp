#include "Game.h"

#include <Igor.h>
using namespace Igor;

int main(void)
{
	Igor::startup();

    Game* game = new Game();
    game->run();
    delete game;

	Igor::shutdown();

	return 0;
}
