// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/config/iConfig.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <tinyxml.h>

#include <json.hpp>
using json = nlohmann::json;

namespace igor
{
    iConfig::iConfig()
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

    iConfig::~iConfig()
    {
    }

    bool iConfig::write(const iaString &filename)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ofstream stream;
        stream.open(temp);

        if (!stream.is_open())
        {
            con_err("can't open to write \"" << temp << "\"");
            return false;
        }

        json configJson = {
            {"logLevel", getValue("logLevel")},
            {"minRenderContextThreads", getValue("minRenderContextThreads")},
            {"maxRenderContextThreads", getValue("maxRenderContextThreads")},
            {"minPhysicsThreads", getValue("minPhysicsThreads")},
            {"maxPhysicsThreads", getValue("maxPhysicsThreads")},
            {"minThreads", getValue("minThreads")},
            {"maxThreads", getValue("maxThreads")},
            {"loadMode", getValue("loadMode")},
            {"searchPaths", getValueAsArray("searchPaths")}};

        stream << configJson.dump(4);

        con_info("written igor configuration " << filename);
        return true;
    }

    void iConfig::read(const iaString &filename)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ifstream file(temp);
        json data = json::parse(file);

        for (const auto &element : data.items())
        {
            if (element.key() == "searchPaths")
            {
                std::vector<iaString> paths = element.value();
                set("searchPaths", paths);
                continue;
            }

            set(element.key().c_str(), element.value().get<iaString>());
        }

        con_info("loaded configuration \"" << filename << "\"");
    }

    const iaString iConfig::getValue(const iaString &setting) const
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

    int64 iConfig::getValueAsInt(const iaString &setting) const
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

    float64 iConfig::getValueAsFloat(const iaString &setting) const
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

    const std::vector<iaString> iConfig::getValueAsArray(const iaString &setting) const
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

    bool iConfig::hasSetting(const iaString &setting) const
    {
        return (_settings.find(setting) != _settings.end());
    }

    void iConfig::set(const iaString &setting, const iaString &value)
    {
        std::vector<iaString> values = {value};
        set(setting, values);
    }

    void iConfig::set(const iaString &setting, const std::vector<iaString> &values)
    {
        _settings[setting] = values;
    }

    void iConfig::reset(const iaString &setting)
    {
        auto iter = _settings.find(setting);
        if (iter != _settings.end())
        {
            _settings.erase(iter);
        }
    }

} // namespace igor
