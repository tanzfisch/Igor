// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga

#include "Mica.h"

#include <Igor.h>
using namespace Igor;

#include <iaDirectory.h>
using namespace IgorAux;

int main(int argc, char* argv[])
{
	// start the engine
    Igor::startup();

	// create and run mica
    Mica* mica = new Mica();
    if (argc >= 2)
    {
		mica->run(argv[1]);
    }
    else
    {
		mica->run("");
    }

	// kill mica
    delete mica;

	// stop the engine
    Igor::shutdown();

	return 0;
}
