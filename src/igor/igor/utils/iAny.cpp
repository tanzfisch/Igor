// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header stream

#include <igor/utils/iAny.h>

namespace igor
{

    iAny &iAny::getInstance()
    {
        static iAny _instance;
        return _instance;
    }

    iAny::iAny()
    {
        add<iLightType>();
        add<iaAxis>();
    }

    bool iAny::compare(const std::any &a, const std::any &b) const
    {
        if (a.type() != b.type())
        {
            return false;
        }

        auto it = _comparators.find(std::type_index(a.type()));
        if (it != _comparators.end())
        {
            return it->second(a, b);
        }

        con_err("unregistered type " << a.type().name());
        return false;
    }

    iaString iAny::toString(const std::type_info &typeInfo)
    {
        static std::unordered_map<std::type_index, iaString> typeToString = {
            {typeid(void), "void"},
            {typeid(bool), "bool"},
            {typeid(char), "char"},
            {typeid(signed char), "signed char"},
            {typeid(unsigned char), "unsigned char"},
            {typeid(short), "short"},
            {typeid(unsigned short), "unsigned short"},
            {typeid(int), "int"},
            {typeid(unsigned int), "unsigned"},
            {typeid(long), "long"},
            {typeid(unsigned long), "unsigned long"},
            {typeid(long long), "long long"},
            {typeid(unsigned long long), "unsigned long long"},
            {typeid(float), "float"},
            {typeid(double), "double"},
            {typeid(long double), "long double"},
            {typeid(std::string), "std::string"},
            {typeid(iaString), "iaString"},
            {typeid(int8), "int8"},
            {typeid(uint8), "uint8"},
            {typeid(int16), "int16"},
            {typeid(uint16), "uint16"},
            {typeid(int32), "int32"},
            {typeid(uint32), "uint32"},
            {typeid(int64), "int64"},
            {typeid(uint64), "uint64"},
            {typeid(float32), "float32"},
            {typeid(float64), "float64"},
            {typeid(iaVector2f), "iaVector2f"},
            {typeid(iaVector2d), "iaVector2d"},
            {typeid(iaVector2i), "iaVector2i"},
            {typeid(iaVector2I), "iaVector2I"},
            {typeid(iaVector3f), "iaVector3f"},
            {typeid(iaVector3d), "iaVector3d"},
            {typeid(iaVector3i), "iaVector3i"},
            {typeid(iaVector3I), "iaVector3I"},
            {typeid(iaVector4f), "iaVector4f"},
            {typeid(iaVector4d), "iaVector4d"},
            {typeid(iaVector4i), "iaVector4i"},
            {typeid(iaVector4I), "iaVector4I"}
        };

        return typeToString[typeInfo];
    }

    iaString iAny::toString(const std::any &value)
    {
        if (value.type() == typeid(iaString))
        {
            return std::any_cast<iaString>(value);
        }
        else if (value.type() == typeid(int8))
        {
            return iaString::toString(std::any_cast<int8>(value));
        }
        else if (value.type() == typeid(uint8))
        {
            return iaString::toString(std::any_cast<uint8>(value));
        }
        else if (value.type() == typeid(int16))
        {
            return iaString::toString(std::any_cast<int16>(value));
        }
        else if (value.type() == typeid(uint16))
        {
            return iaString::toString(std::any_cast<uint16>(value));
        }
        else if (value.type() == typeid(int32))
        {
            return iaString::toString(std::any_cast<int32>(value));
        }
        else if (value.type() == typeid(uint32))
        {
            return iaString::toString(std::any_cast<uint32>(value));
        }
        else if (value.type() == typeid(int64))
        {
            return iaString::toString(std::any_cast<int64>(value));
        }
        else if (value.type() == typeid(uint64))
        {
            return iaString::toString(std::any_cast<uint64>(value));
        }
        else if (value.type() == typeid(float32))
        {
            return iaString::toString(std::any_cast<float32>(value));
        }
        else if (value.type() == typeid(float64))
        {
            return iaString::toString(std::any_cast<float64>(value));
        }
        else if (value.type() == typeid(bool))
        {
            return (std::any_cast<bool>(value) ? "true" : "false");
        }
        else if (value.type() == typeid(iaVector2f))
        {
            return iaString::toString(std::any_cast<iaVector2f>(value));
        }
        else if (value.type() == typeid(iaVector2d))
        {
            return iaString::toString(std::any_cast<iaVector2d>(value));
        }
        else if (value.type() == typeid(iaVector2i))
        {
            return iaString::toString(std::any_cast<iaVector2i>(value));
        }
        else if (value.type() == typeid(iaVector2I))
        {
            return iaString::toString(std::any_cast<iaVector2I>(value));
        }
        else if (value.type() == typeid(iaVector3f))
        {
            return iaString::toString(std::any_cast<iaVector3f>(value));
        }
        else if (value.type() == typeid(iaVector3d))
        {
            return iaString::toString(std::any_cast<iaVector3d>(value));
        }
        else if (value.type() == typeid(iaVector3i))
        {
            return iaString::toString(std::any_cast<iaVector3i>(value));
        }
        else if (value.type() == typeid(iaVector3I))
        {
            return iaString::toString(std::any_cast<iaVector3I>(value));
        }
        else if (value.type() == typeid(iaVector4f))
        {
            return iaString::toString(std::any_cast<iaVector4f>(value));
        }
        else if (value.type() == typeid(iaVector4d))
        {
            return iaString::toString(std::any_cast<iaVector4d>(value));
        }
        else if (value.type() == typeid(iaVector4i))
        {
            return iaString::toString(std::any_cast<iaVector4i>(value));
        }
        else if (value.type() == typeid(iaVector4I))
        {
            return iaString::toString(std::any_cast<iaVector4I>(value));
        }
        else
        {
            return "unknown type";
        }
    }
}