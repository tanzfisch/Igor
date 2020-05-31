#include "MouseExample.h"

#include <igor/igor.h>

int main(void)
{
    // first thing to call before any other igor functionality
	Igor::startup();

	MouseExample* example = new MouseExample();
	example->run();
	delete example;

    // last thing to call to shut down igor propperly
	Igor::shutdown();

	return 0;
}
