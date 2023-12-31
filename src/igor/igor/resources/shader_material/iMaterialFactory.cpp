// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/shader_material/iMaterialFactory.h>

#include <igor/resources/iResourceManager.h>
#include <igor/resources/shader_material/loader/iShaderMaterialIO.h>
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
        iShaderMaterialPtr defaultMaterial = iRenderer::getInstance().getDefaultMaterial();
        const iaString source = defaultMaterial->getSource();

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                           {IGOR_RESOURCE_PARAM_ID, iaUUID()},
                           {IGOR_RESOURCE_PARAM_SOURCE, source}});

        iShaderMaterialPtr material(new iShaderMaterial(param));
        iShaderMaterialIO::read(iResourceManager::getInstance().resolvePath(source), material);

        return material;
    }

    bool iMaterialFactory::saveResource(iResourcePtr resource, const iaString &filename)
    {
        iaString filepath;

        if (filename.isEmpty())
        {
            filepath = iResourceManager::getInstance().getFilename(resource->getID());
            if (filepath.isEmpty())
            {
                filepath = resource->getSource();
            }

            if (filepath.isEmpty())
            {
                con_err("not a valid source path " << resource->getID());
                return false;
            }
        }
        else
        {
            filepath = filename;
        }

        const iaString fullFilepath = iResourceManager::getInstance().resolvePath(filepath);
        iShaderMaterialPtr material = std::dynamic_pointer_cast<iShaderMaterial>(resource);
        return iShaderMaterialIO::write(fullFilepath, material);
    }

    iResourcePtr iMaterialFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iShaderMaterial(parameters));
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
            con_err("not a valid source path " << resource->getID());
            return false;
        }

        const iaString fullFilepath = iResourceManager::getInstance().resolvePath(filepath);
        iShaderMaterialPtr material = std::dynamic_pointer_cast<iShaderMaterial>(resource);
        return iShaderMaterialIO::read(fullFilepath, material);
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