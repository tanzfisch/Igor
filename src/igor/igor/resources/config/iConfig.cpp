// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/config/iConfig.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <igor/utils/iJson.h>

namespace igor
{
    iConfig::iConfig()
    {
        // setting defaults
        setValue("igor.logLevel", "User");
        setValue("igor.minRenderContextThreads", "1");
        setValue("igor.maxRenderContextThreads", "Max");
        setValue("igor.minPhysicsThreads", "1");
        setValue("igor.maxPhysicsThreads", "Max");
        setValue("igor.minThreads", "0");
        setValue("igor.maxThreads", "0");
        setValue("igor.loadMode", "App");
        setValue("igor.searchPaths", {"../../../data", "../../data", "../data", "data"}); // TODO finish #396
    }

    iConfig::~iConfig()
    {
    }

    bool iConfig::write(const iaString &filename)
    {
        if(!filename.isEmpty())
        {
            _filename = filename;
        }

        if(_filename.isEmpty())
        {
            con_err("no filename specified");
            return false;
        }

        char temp[2048];
        _filename.getData(temp, 2048);

        std::ofstream stream;
        stream.open(temp);

        if (!stream.is_open())
        {
            con_err("can't open to write \"" << temp << "\"");
            return false;
        }

        json configJson;

        for(const auto &setting : _settings)
        {
            char temp[2048];
            setting.first.getData(temp, 2048);

            configJson[temp] = setting.second;
        }

        stream << configJson.dump(4);

        con_info("written igor configuration " << _filename);
        return true;
    }

    void iConfig::read(const iaString &filename)
    {
        _filename = filename;

        json data = iJson::parse(_filename);

        for (const auto &element : data.items())
        {
            if (element.value().is_array())
            {
                std::vector<iaString> paths = element.value();
                setValue(element.key().c_str(), paths);
                continue;
            }

            setValue(element.key().c_str(), element.value().get<iaString>());
        }

        con_info("loaded configuration \"" << _filename << "\"");
    }

    const iaString iConfig::getValue(const iaString &setting) const
    {
        auto iter = _settings.find(setting);
        if (iter == _settings.end())
        {
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
            return std::vector<iaString>();
        }

        con_assert(!iter->second.empty(), "invalid data");

        return iter->second;
    }

    bool iConfig::hasValue(const iaString &setting) const
    {
        return (_settings.find(setting) != _settings.end());
    }

    void iConfig::setValue(const iaString &setting, const iaString &value)
    {
        std::vector<iaString> values = {value};
        setValue(setting, values);
    }

    void iConfig::setValue(const iaString &setting, const std::vector<iaString> &values)
    {
        _settings[setting] = values;
    }

} // namespace igor
