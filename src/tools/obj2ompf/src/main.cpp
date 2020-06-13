// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "OBJ2OMPF.h"

#include <iaux/system/iaDirectory.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <igor/igor.h>
using namespace igor;

int main(int argc, char* argv[])
{
    igor::startup();

    OBJ2OMPF obj2ompf;
    obj2ompf.convert(argc, argv);

    igor::shutdown();

	return 0;
}
