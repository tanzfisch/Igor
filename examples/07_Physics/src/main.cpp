#include "PhysicsExample.h"

#include <igor/igor.h>
using namespace Igor;

int main()
{
	Igor::startup();

	PhysicsExample* example = new PhysicsExample();
	example->run();
	delete example;
	
	Igor::shutdown();

	return 0;
}
