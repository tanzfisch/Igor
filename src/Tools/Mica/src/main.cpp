// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga

#include "Mica.h"

#include <iaDirectory.h>
using namespace IgorAux;

#include <Igor.h>
using namespace Igor;

int main(int argc, char* argv[])
{

    Igor::startup();

    Mica* mica = new Mica();
    if (argc >= 2)
    {
		mica->run(argv[1]);
    }
    else
    {
		mica->run("");
    }

    delete mica;

    Igor::shutdown();

	return 0;
}
