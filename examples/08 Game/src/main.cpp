#include "GameMenu.h"

#include <Igor.h>
using namespace Igor;

int main(void)
{
	Igor::startup();

	GameMenu* game = new GameMenu();
    game->run();
    delete game;

	Igor::shutdown();

	return 0;
}
