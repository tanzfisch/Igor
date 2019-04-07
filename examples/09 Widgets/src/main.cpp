#include "WidgetsExample.h"

#include <Igor.h>
using namespace Igor;

int main()
{
	Igor::startup();

    WidgetsExample* example = new WidgetsExample();
	example->run();
	delete example;
	
	Igor::shutdown();

	return 0;
}
