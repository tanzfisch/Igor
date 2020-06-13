#include "PhysicsExample.h"

#include <igor/igor.h>
using namespace igor;

int main()
{
	igor::startup();

	PhysicsExample* example = new PhysicsExample();
	example->run();
	delete example;
	
	igor::shutdown();

	return 0;
}
