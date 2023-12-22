// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/material/iMaterialFactory.h>

#include <igor/resources/iResourceManager.h>
#include <igor/resources/material/iMaterialIO.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

namespace igor
{

    iMaterialFactory::iMaterialFactory()
        : iFactory(IGOR_RESOURCE_MATERIAL, IGOR_SUPPORTED_MATERIAL_EXTENSIONS)
    {
    }

    iResourcePtr iMaterialFactory::createResource()
    {
        iMaterialPtr defaultMaterial = iRenderer::getInstance().getDefaultMaterial();
        iaString filename = defaultMaterial->getFilename();

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                           {IGOR_RESOURCE_PARAM_ID, iaUUID()},
                           {IGOR_RESOURCE_PARAM_SOURCE, filename}});

        iMaterialPtr material(new iMaterial(param));
        iMaterialIO::read(iResourceManager::getInstance().resolvePath(filename), material);
        material->setName("New Material");

        return material;
    }

    iResourcePtr iMaterialFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iMaterial(parameters));
    }

    bool iMaterialFactory::loadResource(iResourcePtr resource)
    {
        iaString filepath = iResourceManager::getInstance().getFilename(resource->getID());
        if (filepath.isEmpty())
        {
            filepath = resource->getSource();
        }
        
        if (filepath.isEmpty())
        {
            con_err("not a valid material " << resource->getID());
            return false;
        }

        const iaString fullFilepath = iResourceManager::getInstance().resolvePath(filepath);
        iMaterialPtr material = std::dynamic_pointer_cast<iMaterial>(resource);
        return iMaterialIO::read(iResourceManager::getInstance().resolvePath(fullFilepath), material);
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

}; // namespace igor