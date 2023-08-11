// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/config/iConfigReader.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <tinyxml.h>

namespace igor
{
    iConfigReader::iConfigReader()
    {
    }

    iConfigReader::~iConfigReader()
    {
    }

    /*void iConfigReader::readThreadingConfig(TiXmlElement *threading)
    {
        TiXmlElement *threads = threading->FirstChildElement("Threads");
        iaString minThreads(threads->Attribute("min"));
        iaString maxThreads(threads->Attribute("max"));

        TiXmlElement *renderContextThreads = threading->FirstChildElement("RenderContextThreads");
        iaString minRenderContextThreads(renderContextThreads->Attribute("min"));
        iaString maxRenderContextThreads(renderContextThreads->Attribute("max"));

        TiXmlElement *physicsThreads = threading->FirstChildElement("PhysicsThreads");
        iaString minPhysicsThreads(physicsThreads->Attribute("min"));
        iaString maxPhysicsThreads(physicsThreads->Attribute("max"));
    }*/

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