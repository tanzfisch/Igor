#include "StateMachineExample.h"

#include <Igor.h>
using namespace Igor;

int main(void)
{
	Igor::startup();

	StateMachineExample* example = new StateMachineExample();
	example->run();
	delete example;

	Igor::shutdown();

	return 0;
}
