#include "OpenGL3DExample.h"

#include <Igor.h>
using namespace Igor;

int main()
{
	Igor::startup();

	OpenGL3DExample* example = new OpenGL3DExample();
	example->run();
	delete example;
	
	Igor::shutdown();

	return 0;
}
