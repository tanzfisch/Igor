// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/prefab/loader/iPrefabIO.h>

#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaFile.h>
#include <iaux/utils/iaJson.h>

#include <fstream>
#include <iostream>

namespace igor
{
    bool iPrefabIO::read(const iaString &filename, const iPrefabPtr &prefab)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ifstream file(temp);
        json data = json::parse(file);

        // TODO

        return true;
    }

    bool iPrefabIO::write(const iaString &filename, const iPrefabPtr &prefab)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ofstream stream;
        stream.open(temp);

        // TODO stream << dataJson.dump(4);

        return true;
    }

} // namespace igor
