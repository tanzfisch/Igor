// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "OBJ2OMPF.h"

#include <iaux/system/iaDirectory.h>
#include <iaux/system/iaConsole.h>
using namespace IgorAux;

#include <igor/igor.h>
using namespace Igor;

int main(int argc, char* argv[])
{
    Igor::startup();

    OBJ2OMPF obj2ompf;
    obj2ompf.convert(argc, argv);

    Igor::shutdown();

	return 0;
}
