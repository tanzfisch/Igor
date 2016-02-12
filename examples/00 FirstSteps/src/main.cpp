#include "FirstSteps.h"

#include <Igor.h>
using namespace Igor;

int main(void)
{
    /*! call this before you call anything else of Igor
    */
	Igor::startup();

    // create and run first steps class
    FirstSteps* firstSteps = new FirstSteps();
    firstSteps->run();
	delete firstSteps;

    /*! call this after you are done with using Igor
    */
    Igor::shutdown();

	return 0;
}
