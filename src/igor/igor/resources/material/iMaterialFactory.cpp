// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterialFactory.h>

#include <igor/resources/iResourceManager.h>
#include <igor/resources/material/iMaterialIO.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

namespace igor
{

    const iaString &iMaterialFactory::getType() const
    {
        const static iaString typeName(L"material");
        return typeName;
    }

    iResourcePtr iMaterialFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iMaterial(parameters));
    }

    bool iMaterialFactory::loadResource(iResourcePtr resource)
    {
        iMaterialPtr material = std::dynamic_pointer_cast<iMaterial>(resource);

        /*const auto &parameters = resource->getParameters();
        const bool generate = parameters.getParameter<bool>("generate", false);
        if (generate)
        {
            // TODO make copy of default and return
            // or what ever you have in mind
        }*/

        const iaString filepath = iResourceManager::getInstance().getFilePath(resource->getID());
        return iMaterialIO::read(iResourceManager::getInstance().resolvePath(filepath), material);
    }

    void iMaterialFactory::unloadResource(iResourcePtr resource)
    {
        // nothing else to do here
    }

    iaString iMaterialFactory::getHashData(const iParameters &parameters) const
    {
        iaString hashData;

        // TODO if anything

        return hashData;
    }

    bool iMaterialFactory::matchingType(const iParameters &parameters) const
    {
        if (parameters.getParameter<iaString>("type") == getType())
        {
            return true;
        }

        iaFile file(parameters.getParameter<iaString>("name"));
        const iaString &fileExtension = file.getExtension();
        static const std::vector<iaString> supportedExtensions = {L"mat"};

        for (const auto &extension : supportedExtensions)
        {
            if (fileExtension == extension)
            {
                return true;
            }
        }

        return false;
    }

}; // namespace igor