#include "MouseExample.h"

#include <Igor.h>

int main(void)
{
	Igor::startup();

	MouseExample* example = new MouseExample();
	example->run();
	delete example;

	Igor::shutdown();

	return 0;
}
