
// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/iResourceDictionary.h>

#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaFile.h>
using namespace iaux;

#include <tinyxml.h>

namespace igor
{

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

        // just ordering it to reduce the diff we potentially have to commit
        std::sort(_data.begin(), _data.end(), [](std::tuple<iResourceID, iaString, iaString> const &a, std::tuple<iResourceID, iaString, iaString> const &b)
                  { return std::get<0>(a) < std::get<0>(b); });

        for (auto &tuple : _data)
        {
            stream << "        <Resource id=\"" << std::get<0>(tuple) << "\"";

            if (!std::get<2>(tuple).isEmpty())
            {
                stream << " alias=\"" << std::get<2>(tuple) << "\"";
            }

            stream << " source=\"" << std::get<1>(tuple) << "\"";

            stream << " />\n";
        }

        stream << "    </ResourceDictionary>\n";
        stream << "</Igor>\n";

        con_info("written resource dictionary " << filename);
        return true;
    }

    bool iResourceDictionary::addResource(const iResourceID &uuid, const iaString &source, const iaString &alias, bool internal)
    {
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
                con_err("alias collision " << alias << " -> " << uuidAlias.toString());
                return false;
            }

            _aliasLookup[uuidAlias] = uuid;
        }

        // skip marked internal so they are excluded from export
        if (!internal)
        {
            _data.emplace_back(uuid, source, alias);
        }

        return true;
    }

    void iResourceDictionary::removeResource(iResourceID resourceID)
    {
        if (!resourceID.isValid())
        {
            return;
        }

        auto iterSource = _resourceDictionaryLookup.find(resourceID);
        if (iterSource != _resourceDictionaryLookup.end())
        {
            _resourceDictionaryLookup.erase(iterSource);
        }

        auto iterAlias = std::find_if(_aliasLookup.begin(), _aliasLookup.end(), [&resourceID](const auto &pair)
                                      { return pair.second == resourceID; });

        if (iterAlias != _aliasLookup.end())
        {
            _aliasLookup.erase(iterAlias);
        }

        auto iterData = std::find_if(_data.begin(), _data.end(), [&resourceID](const auto &tuple)
                                     { return std::get<0>(tuple) == resourceID; });

        if (iterData != _data.end())
        {
            _data.erase(iterData);
        }
    }

    bool iResourceDictionary::readResourceDictionaryElement(TiXmlElement *element, bool internal)
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
            iaUUID uuid(id);

            if (!addResource(uuid, source, alias, internal))
            {
                return false;
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
            con_err("not an igor xml file \"" << temp << "\"");
            return false;
        }

        TiXmlElement *resourceDictionary = root->FirstChildElement("ResourceDictionary");
        if (resourceDictionary == nullptr)
        {
            con_err("invalid file \"" << temp << "\"");
            return false;
        }

        iaString internal(resourceDictionary->Attribute("internal"));

        if (!readResourceDictionaryElement(resourceDictionary, (!internal.isEmpty() && internal == "true")))
        {
            con_err("can't read all resource dictionary entries from \"" << filename << "\"");
            return false;
        }

        con_info("loaded resource dictionary \"" << filename << "\"");
        return true;
    }

    void iResourceDictionary::clear()
    {
        _resourceDictionaryLookup.clear();
        _aliasLookup.clear();
        _data.clear();
    }

    void iResourceDictionary::setAlias(iResourceID id, const iaString &alias)
    {
        iResourceID resource = getResource(alias);
        if (resource.isValid())
        {
            con_err("alias \"" << alias << "\" already defined for a resource " << resource);
            return;
        }

        const iaString oldAlias = getAlias(id);
        if (oldAlias == alias)
        {
            return;
        }

        // remove old alias
        if (!oldAlias.isEmpty())
        {
            std::hash<std::wstring> hashFunc;
            std::size_t hash = hashFunc(oldAlias.getData());
            iResourceID aliasid(hash);

            _aliasLookup.erase(aliasid);
        }

        for (auto &tuple : _data)
        {
            if (std::get<0>(tuple) == id)
            {
                std::get<2>(tuple) = alias;
            }
        }

        if (alias.isEmpty())
        {
            return;
        }

        std::hash<std::wstring> hashFunc;
        std::size_t hash = hashFunc(alias.getData());
        iResourceID aliasid(hash);

        _aliasLookup[aliasid] = id;
    }

    const iaString &iResourceDictionary::getAlias(iResourceID id) const
    {
        for (const auto &tuple : _data)
        {
            if (std::get<0>(tuple) == id)
            {
                return std::get<2>(tuple);
            }
        }

        static iaString notFound;

        return notFound;
    }

    const iaString &iResourceDictionary::getFilename(iResourceID id) const
    {
        auto iter = _resourceDictionaryLookup.find(id);
        if (iter != _resourceDictionaryLookup.end())
        {
            return iter->second;
        }

        static iaString notFound;

        return notFound;
    }

    const iResourceID iResourceDictionary::addResource(const iaString &filename, const iaString &alias, bool internal)
    {
        iaUUID uuid;
        if (!addResource(uuid, filename, alias, internal))
        {
            con_crit("internal error");
        }

        return uuid;
    }

    const iResourceID iResourceDictionary::getResource(const iaString &text) const
    {
        if (text.isEmpty())
        {
            return iResourceID(IGOR_INVALID_ID);
        }

        // check if this is a known alias
        std::hash<std::wstring> hashFunc;
        std::size_t hash = hashFunc(text.getData());
        iResourceID aliasid(hash);

        auto iter = _aliasLookup.find(aliasid);
        if (iter != _aliasLookup.end())
        {
            return iter->second;
        }

        // check if this is a file path within the dictionary
        // (just take the first hit and ignore others)
        for (const auto &tuple : _data)
        {
            if (std::get<1>(tuple) == text)
            {
                return std::get<0>(tuple);
            }
        }

        return iResourceID(IGOR_INVALID_ID);
    }
}