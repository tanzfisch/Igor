#include "KeyboardExample.h"

#include <Igor.h>
using namespace Igor;

int main(void)
{
    /*! call this before you call anything else of Igor
    */
	Igor::startup();

    // create and run example class
	KeyboardExample* example = new KeyboardExample();
	example->run();
	delete example;

    /*! call this after you are done with using Igor
    */
    Igor::shutdown();

	return 0;
}
