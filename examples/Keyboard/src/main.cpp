#include "KeyboardExample.h"

#include <Igor.h>
using namespace Igor;

int main(void)
{
	Igor::startup();

	KeyboardExample* example = new KeyboardExample();
	example->run();
	delete example;

	Igor::shutdown();

	return 0;
}
