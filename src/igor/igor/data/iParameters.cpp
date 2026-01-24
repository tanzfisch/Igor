// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/data/iParameters.h>

#include <igor/data/iFrustum.h>
#include <igor/renderer/utils/iRendererDefines.h>
#include <igor/resources/shader/iShader.h>
#include <igor/data/iAny.h>

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

    iParameters::iParameters(const iParametersMap &parameters)
    {
        for (const auto &pair : parameters)
        {
            setParameter(pair.first, pair.second);
        }
    }

    void iParameters::setParameter(const iaString &name, const iAny &value)
    {
        _parameters[name] = value;
    }

    const iAny &iParameters::getParameter(const iaString &name, const iAny &defaultValue) const
    {
        auto iter = _parameters.find(name);
        if (iter == _parameters.end())
        {
            return defaultValue;
        }

        return iter->second;
    }

    bool iParameters::hasParameter(const iaString &name) const
    {
        return _parameters.find(name) != _parameters.end();
    }

    iAnyType iParameters::getParameterType(const iaString &name) const
    {
        auto iter = _parameters.find(name);
        if (iter == _parameters.end())
        {
            con_err("parameter \"" << name << "\" not found");
            return iAnyType::Unknown;
        }

        return iter->second.getType();
    }

    const iParametersMap &iParameters::getData() const
    {
        return _parameters;
    }

    std::wostream &operator<<(std::wostream &stream, const iParameters &parameters)
    {
        stream << std::endl
               << __IGOR_LOGGING_TAB__ << std::setfill(L' ');

        for (const auto &param : parameters.getData())
        {
            stream << std::right << std::setw(40) << param.first << " | " << param.second << " (" << toString(param.second.getType()) << ")" << std::endl
                   << __IGOR_LOGGING_TAB__;
        }

        return stream;
    }

}