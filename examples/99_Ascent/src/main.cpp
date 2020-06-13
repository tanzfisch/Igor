#include "Ascent.h"

#include <igor/igor.h>
using namespace igor;

int main()
{
	igor::startup();

	Ascent *ascent = new Ascent();
	ascent->run();
	delete ascent;

	igor::shutdown();

	return 0;
}
