// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga

#include "ModelViewer.h"

#include <iaDirectory.h>
using namespace IgorAux;

#include <Igor.h>
using namespace Igor;

int main(int argc, char* argv[])
{

    Igor::startup();

    ModelViewer* example = new ModelViewer();
    if (argc >= 2)
    {
        example->run(argv[1]);
    }
    else
    {
        example->run("");
    }

    delete example;

    Igor::shutdown();

	return 0;
}
