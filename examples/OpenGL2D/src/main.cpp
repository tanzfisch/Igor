#include "OpenGL2DExample.h"

#include <Igor.h>
using namespace Igor;

int main(void)
{
    // first thing to do
	Igor::startup();

    // create and run example
	OpenGL2DExample *example = new OpenGL2DExample();
	example->run();
	delete example;
	
    // last thing to do
	Igor::shutdown();

	return 0;
}
