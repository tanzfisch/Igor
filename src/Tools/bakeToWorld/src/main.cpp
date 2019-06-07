#include "bakeToWorld.h"

#include <iaDirectory.h>
#include <iaConsole.h>
using namespace IgorAux;

#include <Igor.h>
using namespace Igor;

int main(int argc, char* argv[])
{
    Igor::startup();

    OBJ2OMPF obj2ompf;
    obj2ompf.convert(argc, argv);

    Igor::shutdown();

	return 0;
}
