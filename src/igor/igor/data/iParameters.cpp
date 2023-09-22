// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/data/iParameters.h>

#include <igor/data/iFrustum.h>
#include <igor/renderer/utils/iRendererDefines.h>
#include <igor/resources/material/iMaterial.h>

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

    iParameters::iParameters(const std::map<iaString, std::any> &parameters)
    {
        _parameters = parameters;
    }

    bool iParameters::hasParameter(const iaString &name) const
    {
        return _parameters.find(name) != _parameters.end();
    }

    void iParameters::setParameter(const iaString &name, const std::any value)
    {
        _parameters[name] = value;
    }

    const std::map<iaString, std::any> &iParameters::getParameters() const
    {
        return _parameters;
    }

    IAUX_API std::wostream &operator<<(std::wostream &stream, const std::any &any)
    {
        if (any.type() == typeid(iaString))
        {
            stream << std::any_cast<iaString>(any);
        }
        else if (any.type() == typeid(int8))
        {
            stream << iaString::toString(std::any_cast<int8>(any));
        }
        else if (any.type() == typeid(uint8))
        {
            stream << iaString::toString(std::any_cast<uint8>(any));
        }
        else if (any.type() == typeid(int16))
        {
            stream << iaString::toString(std::any_cast<int16>(any));
        }
        else if (any.type() == typeid(uint16))
        {
            stream << iaString::toString(std::any_cast<uint16>(any));
        }
        else if (any.type() == typeid(int32))
        {
            stream << iaString::toString(std::any_cast<int32>(any));
        }
        else if (any.type() == typeid(uint32))
        {
            stream << iaString::toString(std::any_cast<uint32>(any));
        }
        else if (any.type() == typeid(int64))
        {
            stream << iaString::toString(std::any_cast<int64>(any));
        }
        else if (any.type() == typeid(uint64))
        {
            stream << iaString::toString(std::any_cast<uint64>(any));
        }
        else if (any.type() == typeid(float32))
        {
            stream << iaString::toString(std::any_cast<float32>(any));
        }
        else if (any.type() == typeid(float64))
        {
            stream << iaString::toString(std::any_cast<float64>(any));
        }
        else if (any.type() == typeid(bool))
        {
            stream << (std::any_cast<bool>(any) ? "true" : "false");
        }
        else if (any.type() == typeid(iaVector2f))
        {
            stream << std::any_cast<iaVector2f>(any);
        }
        else if (any.type() == typeid(iaVector2d))
        {
            stream << std::any_cast<iaVector2d>(any);
        }
        else if (any.type() == typeid(iaVector2i))
        {
            stream << std::any_cast<iaVector2i>(any);
        }
        else if (any.type() == typeid(iaVector2I))
        {
            stream << std::any_cast<iaVector2I>(any);
        }
        else if (any.type() == typeid(iaVector3f))
        {
            stream << std::any_cast<iaVector3f>(any);
        }
        else if (any.type() == typeid(iaVector3d))
        {
            stream << std::any_cast<iaVector3d>(any);
        }
        else if (any.type() == typeid(iaVector3i))
        {
            stream << std::any_cast<iaVector3i>(any);
        }
        else if (any.type() == typeid(iaVector3I))
        {
            stream << std::any_cast<iaVector3I>(any);
        }
        else if (any.type() == typeid(iaVector4f))
        {
            stream << std::any_cast<iaVector4f>(any);
        }
        else if (any.type() == typeid(iaVector4d))
        {
            stream << std::any_cast<iaVector4d>(any);
        }
        else if (any.type() == typeid(iaVector4i))
        {
            stream << std::any_cast<iaVector4i>(any);
        }
        else if (any.type() == typeid(iaVector4I))
        {
            stream << std::any_cast<iaVector4I>(any);
        }
        else if (any.type() == typeid(iaColor3c))
        {
            stream << std::any_cast<iaColor3c>(any);
        }
        else if (any.type() == typeid(iaColor3f))
        {
            stream << std::any_cast<iaColor3f>(any);
        }
        else if (any.type() == typeid(iaColor4c))
        {
            stream << std::any_cast<iaColor4c>(any);
        }
        else if (any.type() == typeid(iaColor4f))
        {
            stream << std::any_cast<iaColor4f>(any);
        }
        else if (any.type() == typeid(iaCirclef))
        {
            stream << std::any_cast<iaCirclef>(any);
        }
        else if (any.type() == typeid(iaCircled))
        {
            stream << std::any_cast<iaCircled>(any);
        }
        else if (any.type() == typeid(iaCircleI))
        {
            stream << std::any_cast<iaCircleI>(any);
        }
        else if (any.type() == typeid(iaCirclei))
        {
            stream << std::any_cast<iaCirclei>(any);
        }
        else if (any.type() == typeid(iaRectanglef))
        {
            stream << std::any_cast<iaRectanglef>(any);
        }
        else if (any.type() == typeid(iaRectangled))
        {
            stream << std::any_cast<iaRectangled>(any);
        }
        else if (any.type() == typeid(iaRectangleI))
        {
            stream << std::any_cast<iaRectangleI>(any);
        }
        else if (any.type() == typeid(iaRectanglei))
        {
            stream << std::any_cast<iaRectanglei>(any);
        }
        else if (any.type() == typeid(iaMatrixf))
        {
            stream << std::any_cast<iaMatrixf>(any);
        }
        else if (any.type() == typeid(iaMatrixd))
        {
            stream << std::any_cast<iaMatrixd>(any);
        }
        else if (any.type() == typeid(iaQuaternionf))
        {
            stream << std::any_cast<iaQuaternionf>(any);
        }
        else if (any.type() == typeid(iaQuaterniond))
        {
            stream << std::any_cast<iaQuaterniond>(any);
        }
        else if (any.type() == typeid(iaUUID))
        {
            stream << std::any_cast<iaUUID>(any);
        }
        else if (any.type() == typeid(iaTime))
        {
            stream << std::any_cast<iaTime>(any);
        }
        else if (any.type() == typeid(iFrustumf))
        {
            stream << std::any_cast<iFrustumf>(any);
        }
        else if (any.type() == typeid(iFrustumd))
        {
            stream << std::any_cast<iFrustumd>(any);
        }
        else if (any.type() == typeid(iResourceCacheMode))
        {
            stream << (iResourceCacheMode)std::any_cast<iResourceCacheMode>(any);
        }
        else if (any.type() == typeid(iMaterialVisibility))
        {
            stream << (iMaterialVisibility)std::any_cast<iMaterialVisibility>(any);
        }
        else if (any.type() == typeid(iColorFormat))
        {
            stream << (iColorFormat)std::any_cast<iColorFormat>(any);
        }
        else if (any.type() == typeid(iTextureBuildMode))
        {
            stream << (iTextureBuildMode)std::any_cast<iTextureBuildMode>(any);
        }
        else if (any.type() == typeid(iTextureWrapMode))
        {
            stream << (iTextureWrapMode)std::any_cast<iTextureWrapMode>(any);
        }     
        else if (any.type() == typeid(iResourceManagerLoadMode))
        {
            stream << (iResourceManagerLoadMode)std::any_cast<iResourceManagerLoadMode>(any);
        }                              
        else
        {
            stream << "duh";
        }

        return stream;
    }

    std::wostream &operator<<(std::wostream &stream, const iParameters &parameters)
    {
        stream << std::endl
               << __IGOR_LOGGING_TAB__ << std::setfill(L' ');

        for (const auto &param : parameters.getParameters())
        {
            stream << std::right << std::setw(20) << param.first << " | " << std::left << std::setw(50) << param.second << std::endl
                   << __IGOR_LOGGING_TAB__;
        }

        return stream;
    }

}