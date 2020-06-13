#include "WidgetsExample.h"

#include <igor/igor.h>
using namespace igor;

int main()
{
	igor::startup();

    WidgetsExample* example = new WidgetsExample();
	example->run();
	delete example;
	
	igor::shutdown();

	return 0;
}
