// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/shader/iShaderFactory.h>

#include <igor/resources/iResourceManager.h>
#include <igor/resources/shader/loader/iShaderIO.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

namespace igor
{

    iShaderFactory::iShaderFactory()
        : iFactory(IGOR_RESOURCE_SHADER, IGOR_SUPPORTED_SHADER_EXTENSIONS)
    {
    }

    iResourcePtr iShaderFactory::createResource()
    {
        iShaderPtr defaultMaterial = iRenderer::getInstance().getDefaultShader();
        const iaString source = defaultMaterial->getSource();

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_SHADER},
                           {IGOR_RESOURCE_PARAM_ID, iaUUID()},
                           {IGOR_RESOURCE_PARAM_SOURCE, source}});

        iShaderPtr shader(new iShader(param));
        iShaderIO::read(iResourceManager::getInstance().resolvePath(source), shader);

        return shader;
    }

    bool iShaderFactory::saveResource(iResourcePtr resource, const iaString &filename)
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
        iShaderPtr shader = std::dynamic_pointer_cast<iShader>(resource);
        return iShaderIO::write(fullFilepath, shader);
    }

    iResourcePtr iShaderFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iShader(parameters));
    }

    bool iShaderFactory::loadResource(iResourcePtr resource)
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
        iShaderPtr shader = std::dynamic_pointer_cast<iShader>(resource);
        return iShaderIO::read(fullFilepath, shader);
    }

    void iShaderFactory::unloadResource(iResourcePtr resource)
    {
        // nothing else to do here
    }

    iaString iShaderFactory::getHashData(const iParameters &parameters) const
    {
        iaString hashData;

        // TODO if anything

        return hashData;
    }

}; // namespace igor