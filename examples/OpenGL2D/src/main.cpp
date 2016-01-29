#include "OpenGL2DExample.h"

#include <Igor.h>
using namespace Igor;


int main(void)
{
	Igor::startup();

	OpenGL2DExample *example = new OpenGL2DExample();
	example->run();
	delete example;
	
	Igor::shutdown();

	return 0;
}
