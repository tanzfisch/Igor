// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/script/iScriptFactory.h>

#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

namespace igor
{

    iScriptFactory::iScriptFactory() : iFactory(IGOR_RESOURCE_SCRIPT, IGOR_SUPPORTED_SCRIPT_EXTENSIONS) {}

    iResourcePtr iScriptFactory::createResource()
    {
        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_SCRIPT},
                           {IGOR_RESOURCE_PARAM_SCRIPT, L"print(\"hello Igor\")"},
                           {IGOR_RESOURCE_PARAM_ID, iaUUID()}});

        return createResource(param);
    }

    iResourcePtr iScriptFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iScript(parameters));
    }

    bool iScriptFactory::loadResource(iResourcePtr resource)
    {
        iaString filepath = iResourceManager::getInstance().getFilename(resource->getID());
        if (filepath.isEmpty())
        {
            filepath = resource->getSource();
        }

        const iaString fullFilepath = iResourceManager::getInstance().resolvePath(filepath);
        iScriptPtr script = std::dynamic_pointer_cast<iScript>(resource);

        char temp[1024];
        fullFilepath.getData(temp, 1024);
        std::ifstream file(temp);
        if (!file.is_open())
        {
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        script->setScript(iaString(buffer.str().c_str()));

        file.close();

        return true;
    }

    void iScriptFactory::unloadResource(iResourcePtr resource)
    {
        // nothing else to do here
    }

    static std::string wstringToUtf8(const std::wstring &wstr)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }

    bool iScriptFactory::saveResource(iResourcePtr resource, const iaString &filename)
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
        iScriptPtr script = std::dynamic_pointer_cast<iScript>(resource);

        char temp[1024];
        fullFilepath.getData(temp, 1024);
        std::ofstream file(temp);

        if (!file.is_open())
        {
            return false;
        }

        file << script->getScript();

        file.close();

        return true;
    }
}; // namespace igor
