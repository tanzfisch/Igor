// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/shader/iShaderMaterialFactory.h>

#include <igor/resources/iResourceManager.h>
#include <igor/resources/shader/loader/iShaderMaterialIO.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

namespace igor
{

    iShaderMaterialFactory::iShaderMaterialFactory()
        : iFactory(IGOR_RESOURCE_SHADER, IGOR_SUPPORTED_SHADER_EXTENSIONS)
    {
    }

    iResourcePtr iShaderMaterialFactory::createResource()
    {
        iShaderPtr defaultMaterial = iRenderer::getInstance().getDefaultShader();
        const iaString source = defaultMaterial->getSource();

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_SHADER},
                           {IGOR_RESOURCE_PARAM_ID, iaUUID()},
                           {IGOR_RESOURCE_PARAM_SOURCE, source}});

        iShaderPtr shaderMaterial(new iShader(param));
        iShaderMaterialIO::read(iResourceManager::getInstance().resolvePath(source), shaderMaterial);

        return shaderMaterial;
    }

    bool iShaderMaterialFactory::saveResource(iResourcePtr resource, const iaString &filename)
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
                con_err("not a valid source path \"" << filepath << "\" for ID " << resource->getID());
                return false;
            }
        }
        else
        {
            filepath = filename;
        }

        const iaString fullFilepath = iResourceManager::getInstance().resolvePath(filepath);
        iShaderPtr shaderMaterial = std::dynamic_pointer_cast<iShader>(resource);
        return iShaderMaterialIO::write(fullFilepath, shaderMaterial);
    }

    iResourcePtr iShaderMaterialFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iShader(parameters));
    }

    bool iShaderMaterialFactory::loadResource(iResourcePtr resource)
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
        iShaderPtr shaderMaterial = std::dynamic_pointer_cast<iShader>(resource);
        return iShaderMaterialIO::read(fullFilepath, shaderMaterial);
    }

    void iShaderMaterialFactory::unloadResource(iResourcePtr resource)
    {
        // nothing else to do here
    }

    iaString iShaderMaterialFactory::getHashData(const iParameters &parameters) const
    {
        iaString hashData;

        // TODO if anything

        return hashData;
    }

}; // namespace igor