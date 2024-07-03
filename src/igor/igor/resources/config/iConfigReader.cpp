// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/config/iConfigReader.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <tinyxml.h>

namespace igor
{
    iConfigReader::iConfigReader()
    {
        // setting defaults
        set("logLevel", "User");
        set("minRenderContextThreads", "1");
        set("maxRenderContextThreads", "Max");
        set("minPhysicsThreads", "1");
        set("maxPhysicsThreads", "Max");
        set("minThreads", "0");
        set("maxThreads", "0");
        set("loadMode", "App");
        set("searchPaths", {"../../../data", "../../data", "../data", "data"}); // TODO finish #396
    }

    iConfigReader::~iConfigReader()
    {
    }

    bool iConfigReader::writeConfiguration(const iaString &filename)
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
        stream << "    <Config>\n";

        stream << "        <!-- loglevel: Assert, Error, Warning, Info, User (default), Debug, Trace -->\n";
        stream << "        <Setting name=\"logLevel\" value=\"" << getValue("logLevel") << "\" />\n";

        stream << "        <!-- min max for threads. Options are a numbers or \"Max\" which represents the host's max thread count -->\n";
        stream << "        <Setting name=\"minRenderContextThreads\" value=\"" << getValue("minRenderContextThreads") << "\" />\n";
        stream << "        <Setting name=\"maxRenderContextThreads\" value=\"" << getValue("maxRenderContextThreads") << "\" />\n";
        stream << "        <Setting name=\"minPhysicsThreads\" value=\"" << getValue("minPhysicsThreads") << "\" />\n";
        stream << "        <Setting name=\"maxPhysicsThreads\" value=\"" << getValue("maxPhysicsThreads") << "\" />\n";
        stream << "        <!-- only useful if there is no render context -->\n";
        stream << "        <Setting name=\"minThreads\" value=\"" << getValue("minThreads") << "\" />\n";
        stream << "        <Setting name=\"maxThreads\" value=\"" << getValue("maxThreads") << "\" />\n";

        stream << "        <!-- load mode of resource manager. loadMode: App (application decides), Sync (all synchronous) -->\n";
        stream << "        <Setting name=\"loadMode\" value=\"" << getValue("loadMode") << "\" />\n";

        stream << "        <!-- searchPaths: search paths for resources relative to current directory. In this example relative to the bin folder -->\n";
        stream << "        <Setting name=\"searchPaths\">\n";
        const auto searchPaths = getValueAsArray("searchPaths");
        for(const auto &path : searchPaths)
        {
            stream << "            <Value>" << path << "</Value>\n";
        }
        stream << "        </Setting>\n";

        stream << "    </Config>\n";
        stream << "</Igor>\n";

        con_info("written igor configuration " << filename);
        return true;
    }

    void iConfigReader::readConfigElement(TiXmlElement *config)
    {
        TiXmlElement *setting = config->FirstChildElement("Setting");
        if (setting == nullptr)
        {
            con_info("configuration has no settings");
            return;
        }

        do
        {
            // get name and value
            iaString name(setting->Attribute("name"));
            std::vector<iaString> values;

            iaString value(setting->Attribute("value"));
            if (!value.isEmpty())
            {
                values.push_back(value);
            }

            // check if there is more values
            TiXmlElement *valueElement = setting->FirstChildElement("Value");
            if (valueElement != nullptr)
            {
                do
                {
                    TiXmlNode *text = valueElement->FirstChild();
                    if (text->Type() == TiXmlNode::NodeType::TINYXML_TEXT)
                    {
                        values.push_back(text->ValueStr().data());
                    }

                    valueElement = valueElement->NextSiblingElement("Value");
                } while (valueElement != nullptr);
            }

            set(name, values);

            setting = setting->NextSiblingElement("Setting");
        } while (setting != nullptr);
    }

    void iConfigReader::readConfiguration(const iaString &filename)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        TiXmlDocument document(temp);
        if (!document.LoadFile())
        {
            con_err("can't read \"" << filename << "\". " << document.ErrorDesc());
            return;
        }

        TiXmlElement *root = document.FirstChildElement("Igor");
        if (!root)
        {
            con_err("not an igor xml file");
            return;
        }

        TiXmlElement *config = root->FirstChildElement("Config");
        if (config)
        {
            readConfigElement(config);
        }

        con_info("loaded configuration \"" << filename << "\"");
    }

    const iaString iConfigReader::getValue(const iaString &setting) const
    {
        auto iter = _settings.find(setting);
        if (iter == _settings.end())
        {
            con_err("setting \"" << setting << "\" not found ");
            return "";
        }

        con_assert(!iter->second.empty(), "invalid data");

        return iter->second[0];
    }

    int64 iConfigReader::getValueAsInt(const iaString &setting) const
    {
        auto iter = _settings.find(setting);
        if (iter == _settings.end())
        {
            con_err("setting \"" << setting << "\" not found ");
            return 0;
        }

        con_assert(!iter->second.empty(), "invalid data");

        return iaString::toInt(iter->second[0]);
    }

    float64 iConfigReader::getValueAsFloat(const iaString &setting) const
    {
        auto iter = _settings.find(setting);
        if (iter == _settings.end())
        {
            con_err("setting \"" << setting << "\" not found ");
            return 0.0;
        }

        con_assert(!iter->second.empty(), "invalid data");

        return iaString::toFloat(iter->second[0]);
    }

    const std::vector<iaString> iConfigReader::getValueAsArray(const iaString &setting) const
    {
        auto iter = _settings.find(setting);
        if (iter == _settings.end())
        {
            con_err("setting \"" << setting << "\" not found ");
            return std::vector<iaString>();
        }

        con_assert(!iter->second.empty(), "invalid data");

        return iter->second;
    }

    bool iConfigReader::hasSetting(const iaString &setting) const
    {
        return (_settings.find(setting) != _settings.end());
    }

    void iConfigReader::set(const iaString &setting, const iaString &value)
    {
        std::vector<iaString> values = {value};
        set(setting, values);
    }

    void iConfigReader::set(const iaString &setting, const std::vector<iaString> &values)
    {
        _settings[setting] = values;
    }

    void iConfigReader::reset(const iaString &setting)
    {
        auto iter = _settings.find(setting);
        if (iter != _settings.end())
        {
            _settings.erase(iter);
        }
    }

} // namespace igor