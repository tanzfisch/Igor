// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/prefab/loader/iPrefabIO.h>

#include <igor/resources/iResourceManager.h>
#include <igor/entities/iEntitySystemModule.h>

#include <iaux/utils/iaJson.h>

#include <fstream>

namespace igor
{
    bool iPrefabIO::read(const iaString &filename, const iPrefabPtr &prefab)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ifstream file(temp);
        json data = json::parse(file);

        // prefab->_sceneID = iEntitySystemModule::getInstance().createScene()->getID();

        // auto scene = data["entityScene"].get<iEntityScene>();

        return true;
    }

    bool iPrefabIO::write(const iaString &filename, const iPrefabPtr &prefab)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ofstream stream;
        stream.open(temp);

        auto scene = iEntitySystemModule::getInstance().getScene(prefab->getSceneID());
        json dataJson;
        dataJson["entityScene"] = *scene;
        stream << dataJson.dump(4);

        return true;
    }

} // namespace igor
