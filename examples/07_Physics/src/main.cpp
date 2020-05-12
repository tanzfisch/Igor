#include "PhysicsExample.h"

#include <Igor.h>
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
