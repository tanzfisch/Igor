
// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/iResourceDictionary.h>

#include <tinyxml.h>

namespace igor
{

    iResourceDictionary::iResourceDictionary()
    {
    }

    bool iResourceDictionary::write(const iaString &filename)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::wofstream stream;
        stream.open(temp);

        if (!stream.is_open())
        {
            con_err("can't open to write \"" << temp << "\"");
            return false;
        }

        stream << "<?xml version=\"1.0\"?>\n";
        stream << "<Igor>\n";
        stream << "    <ResourceDictionary>\n";

        for (auto &pair : _data)
        {
            stream << "        <Resource id=\"" << pair.first << "\""; 

            if (!pair.second.second.isEmpty())
            {
                stream << " alias=\"" << pair.second.second << "\"";
            }

            stream << " source=\"" << pair.second.first << "\"";

            stream << " />\n";
        }

        stream << "    </ResourceDictionary>\n";
        stream << "</Igor>\n";

        con_info("written resource dictionary " << filename);
        return true;
    }

    bool iResourceDictionary::readResourceDictionaryElement(TiXmlElement *element)
    {
        TiXmlElement *resource = element->FirstChildElement("Resource");
        if (resource == nullptr)
        {
            con_warn("resource dictionary is empty");
            return true;
        }

        do
        {
            // get name and value
            iaString id(resource->Attribute("id"));
            iaString source(resource->Attribute("source"));
            iaString alias(resource->Attribute("alias"));
            iaString internal(resource->Attribute("internal"));

            iaUUID uuid(id);

            auto iter = _resourceDictionaryLookup.find(uuid);
            if (iter != _resourceDictionaryLookup.end())
            {
                con_err("resource id collision " << uuid);
                return false;
            }

            _resourceDictionaryLookup[uuid] = source;

            // store alias to resource lookup
            if (!alias.isEmpty())
            {
                iaUUID uuidAlias(alias);

                auto iter = _aliasLookup.find(uuidAlias);
                if (iter != _aliasLookup.end())
                {
                    con_err("alias collision " << alias);
                    return false;
                }

                _aliasLookup[uuidAlias] = uuid;
            }

            // skip certain internal so they don't get mixed up when exporting resources
            if (internal.isEmpty() || internal == "false")
            {
                _data[uuid] = {source, alias};
            }

            resource = resource->NextSiblingElement("Resource");
        } while (resource != nullptr);

        return true;
    }

    bool iResourceDictionary::read(const iaString &filename)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        TiXmlDocument document(temp);
        if (!document.LoadFile())
        {
            con_err("can't read \"" << filename << "\". " << document.ErrorDesc());
            return false;
        }

        TiXmlElement *root = document.FirstChildElement("Igor");
        if (root == nullptr)
        {
            con_err("not an igor xml file");
            return false;
        }

        TiXmlElement *resourceDictionary = root->FirstChildElement("ResourceDictionary");
        if (resourceDictionary != nullptr)
        {
            if (!readResourceDictionaryElement(resourceDictionary))
            {
                con_err("can't read all resource dictionary entries from \"" << filename << "\"");
                return false;
            }
        }

        con_info("loaded resource dictionary \"" << filename << "\"");
        return true;
    }

    void iResourceDictionary::clear()
    {
        _resourceDictionaryLookup.clear();
    }

    const iaString &iResourceDictionary::getFilePath(iResourceID id) const
    {
        auto iter = _resourceDictionaryLookup.find(id);
        if (iter != _resourceDictionaryLookup.end())
        {
            return iter->second;
        }

        static iaString notFound;

        return notFound;
    }

    const iResourceID iResourceDictionary::getResource(const iaString &alias) const
    {
        if(alias.isEmpty())
        {
            return iResourceID(IGOR_INVALID_ID);
        }

        std::hash<std::wstring> hashFunc;
        std::size_t hash = hashFunc(alias.getData());
        iResourceID aliasid(hash);

        auto iter = _aliasLookup.find(aliasid);
        if (iter == _aliasLookup.end())
        {
            return iResourceID(IGOR_INVALID_ID);
        }

        return iter->second;
    }
}