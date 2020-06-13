#include "MouseExample.h"

#include <igor/igor.h>

int main(void)
{
    // first thing to call before any other igor functionality
	igor::startup();

	MouseExample* example = new MouseExample();
	example->run();
	delete example;

    // last thing to call to shut down igor propperly
	igor::shutdown();

	return 0;
}
