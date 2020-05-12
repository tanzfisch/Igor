#include "SpriteAnimation.h"

#include <Igor.h>
using namespace Igor;

int main(void)
{
    // first thing to do
	Igor::startup();

    // create and run example
	SpriteAnimation*example = new SpriteAnimation();
	example->run();
	delete example;
	
    // last thing to do
	Igor::shutdown();

	return 0;
}
