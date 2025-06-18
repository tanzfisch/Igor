// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga

#include "Mica.h"

int main(int argc, char *argv[])
{
    // start the engine
    igor::startup();

    // create and run mica
    Mica *mica = new Mica();
    mica->run();

    // kill mica
    delete mica;

    // stop the engine
    igor::shutdown();

    return 0;
}
