// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/data/iParameters.h>

#include <igor/data/iFrustum.h>
#include <igor/renderer/utils/iRendererDefines.h>
#include <igor/resources/shader/iShader.h>
#include <igor/utils/iAnyUtil.h>

#include <iaux/data/iaUUID.h>
#include <iaux/data/iaColor3.h>
#include <iaux/data/iaColor4.h>
#include <iaux/data/iaCircle.h>
#include <iaux/data/iaRectangle.h>
#include <iaux/math/iaMatrix.h>
#include <iaux/math/iaQuaternion.h>
#include <iaux/system/iaFile.h>
#include <iaux/system/iaTime.h>

namespace igor
{

    iParameters::iParameters(const std::unordered_map<iaString, std::any> &parameters)
    {
        for (const auto &pair : parameters)
        {
            setParameter(pair.first, pair.second);
        }
    }

    bool iParameters::hasParameter(const iaString &name) const
    {
        return _parameters.find(name) != _parameters.end();
    }

    void iParameters::setParameter(const iaString &name, const std::any value)
    {
        if (value.type() == typeid(const char *))
        {
            _parameters[name] = iaString(std::any_cast<const char *>(value));
        }
        else if (value.type() == typeid(const wchar_t *))
        {
            _parameters[name] = iaString(std::any_cast<const wchar_t *>(value));
        }
        else if (value.type() == typeid(std::string))
        {
            _parameters[name] = iaString(std::any_cast<std::string>(value).c_str());
        }
        else if (value.type() == typeid(std::wstring))
        {
            _parameters[name] = iaString(std::any_cast<std::wstring>(value).c_str());
        }
        else
        {
            _parameters[name] = value;
        }
    }

    const std::unordered_map<iaString, std::any> &iParameters::getData() const
    {
        return _parameters;
    }

    IAUX_API std::wostream &operator<<(std::wostream &stream, const std::any &any)
    {
        stream << iAnyUtil::toString(any);
        return stream;
    }

    std::wostream &operator<<(std::wostream &stream, const iParameters &parameters)
    {
        stream << std::endl
               << __IGOR_LOGGING_TAB__ << std::setfill(L' ');

        for (const auto &param : parameters.getData())
        {
            stream << std::right << std::setw(40) << param.first << " | " << param.second << " (" << iAnyUtil::toString(param.second.type()) << ")" << std::endl
                   << __IGOR_LOGGING_TAB__;
        }

        return stream;
    }

}