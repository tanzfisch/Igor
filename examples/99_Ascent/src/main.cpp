#include "Ascent.h"

#include <igor/igor.h>
using namespace Igor;

int main()
{
	Igor::startup();

	Ascent *ascent = new Ascent();
	ascent->run();
	delete ascent;

	Igor::shutdown();

	return 0;
}
