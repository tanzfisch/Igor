// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga

#include "Mica.h"

#include <igor/igor.h>
using namespace igor;

#include <iaux/system/iaDirectory.h>
using namespace iaux;

int main(int argc, char* argv[])
{
	// start the engine
    igor::startup();

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
    igor::shutdown();

	return 0;
}