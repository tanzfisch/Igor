// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header stream

#include <igor/data/iAny.h>

#include <unordered_map>
#include <typeindex>

namespace igor
{
    iaString toString(const iAnyType &value)
    {
        const static iaString text[] = {
            L"Unknown",
            L"Bool",
            L"uint8",
            L"int8",
            L"uint16",
            L"int16",
            L"uint32",
            L"int32",
            L"uint64",
            L"int64",
            L"float32",
            L"float64",
            L"std_string",
            L"std_wstring",
            L"iaString",
            L"iaVector2f",
            L"iaVector2d",
            L"iaVector2i",
            L"iaVector2I",
            L"iaVector3f",
            L"iaVector3d",
            L"iaVector3i",
            L"iaVector3I",
            L"iaVector4f",
            L"iaVector4d",
            L"iaVector4i",
            L"iaVector4I"};

        return text[static_cast<int>(value)];
    }

    static std::unordered_map<std::type_index, iAnyType> s_typeIndexToAnyType = {
        {typeid(bool), iAnyType::Bool},
        {typeid(std::string), iAnyType::std_string},
        {typeid(std::wstring), iAnyType::std_wstring},
        {typeid(iaString), iAnyType::iaString},
        {typeid(int8), iAnyType::int8},
        {typeid(uint8), iAnyType::uint8},
        {typeid(int16), iAnyType::int16},
        {typeid(uint16), iAnyType::uint16},
        {typeid(int32), iAnyType::int32},
        {typeid(uint32), iAnyType::uint32},
        {typeid(int64), iAnyType::int64},
        {typeid(uint64), iAnyType::uint64},
        {typeid(float32), iAnyType::float32},
        {typeid(float64), iAnyType::float64},
        {typeid(iaVector2f), iAnyType::iaVector2f},
        {typeid(iaVector2d), iAnyType::iaVector2d},
        {typeid(iaVector2i), iAnyType::iaVector2i},
        {typeid(iaVector2I), iAnyType::iaVector2I},
        {typeid(iaVector3f), iAnyType::iaVector3f},
        {typeid(iaVector3d), iAnyType::iaVector3d},
        {typeid(iaVector3i), iAnyType::iaVector3i},
        {typeid(iaVector3I), iAnyType::iaVector3I},
        {typeid(iaVector4f), iAnyType::iaVector4f},
        {typeid(iaVector4d), iAnyType::iaVector4d},
        {typeid(iaVector4i), iAnyType::iaVector4i},
        {typeid(iaVector4I), iAnyType::iaVector4I}};

    iAny::iAny(const iAny &value)
    {
        _data = value._data;
    }

    std::any iAny::getAny(const iAnyType &type)
    {
        con_assert(type != iAnyType::Unknown, "unknown type");

        switch (type)
        {
        case iAnyType::Bool:
            return false;
        case iAnyType::uint8:
            return uint8();
        case iAnyType::int8:
            return int8();
        case iAnyType::uint16:
            return uint16();
        case iAnyType::int16:
            return int16();
        case iAnyType::uint32:
            return uint32();
        case iAnyType::int32:
            return int32();
        case iAnyType::uint64:
            return uint64();
        case iAnyType::int64:
            return int64();
        case iAnyType::float32:
            return float32();
        case iAnyType::float64:
            return float64();
        case iAnyType::std_string:
            return std::string();
        case iAnyType::std_wstring:
            return std::wstring();
        case iAnyType::iaString:
            return iaString();
        case iAnyType::iaVector2f:
            return iaVector2f();
        case iAnyType::iaVector2d:
            return iaVector2d();
        case iAnyType::iaVector2i:
            return iaVector2i();
        case iAnyType::iaVector2I:
            return iaVector2I();
        case iAnyType::iaVector3f:
            return iaVector3f();
        case iAnyType::iaVector3d:
            return iaVector3d();
        case iAnyType::iaVector3i:
            return iaVector3i();
        case iAnyType::iaVector3I:
            return iaVector3I();
        case iAnyType::iaVector4f:
            return iaVector4f();
        case iAnyType::iaVector4d:
            return iaVector4d();
        case iAnyType::iaVector4i:
            return iaVector4i();
        case iAnyType::iaVector4I:
            return iaVector4I();
        }
    }

    iaString iAny::toString() const
    {
        switch (getType())
        {
        case iAnyType::Unknown:
        default:
            return "unknown type";
        case iAnyType::Bool:
            return (std::any_cast<bool>(_data) ? "true" : "false");
        case iAnyType::uint8:
            return iaString::toString(std::any_cast<uint8>(_data));
        case iAnyType::int8:
            return iaString::toString(std::any_cast<int8>(_data));
        case iAnyType::uint16:
            return iaString::toString(std::any_cast<uint16>(_data));
        case iAnyType::int16:
            return iaString::toString(std::any_cast<int16>(_data));
        case iAnyType::uint32:
            return iaString::toString(std::any_cast<uint32>(_data));
        case iAnyType::int32:
            return iaString::toString(std::any_cast<int32>(_data));
        case iAnyType::uint64:
            return iaString::toString(std::any_cast<uint64>(_data));
        case iAnyType::int64:
            return iaString::toString(std::any_cast<int64>(_data));
        case iAnyType::float32:
            return iaString::toString(std::any_cast<float32>(_data));
        case iAnyType::float64:
            return iaString::toString(std::any_cast<float64>(_data));
        case iAnyType::std_string:
            return iaString(std::any_cast<std::string>(_data).c_str());
        case iAnyType::std_wstring:
            return iaString(std::any_cast<std::wstring>(_data).c_str());
        case iAnyType::iaString:
            return std::any_cast<iaString>(_data);
        case iAnyType::iaVector2f:
            return iaString::toString(std::any_cast<iaVector2f>(_data));
        case iAnyType::iaVector2d:
            return iaString::toString(std::any_cast<iaVector2d>(_data));
        case iAnyType::iaVector2i:
            return iaString::toString(std::any_cast<iaVector2i>(_data));
        case iAnyType::iaVector2I:
            return iaString::toString(std::any_cast<iaVector2I>(_data));
        case iAnyType::iaVector3f:
            return iaString::toString(std::any_cast<iaVector3f>(_data));
        case iAnyType::iaVector3d:
            return iaString::toString(std::any_cast<iaVector3d>(_data));
        case iAnyType::iaVector3i:
            return iaString::toString(std::any_cast<iaVector3i>(_data));
        case iAnyType::iaVector3I:
            return iaString::toString(std::any_cast<iaVector3I>(_data));
        case iAnyType::iaVector4f:
            return iaString::toString(std::any_cast<iaVector4f>(_data));
        case iAnyType::iaVector4d:
            return iaString::toString(std::any_cast<iaVector4d>(_data));
        case iAnyType::iaVector4i:
            return iaString::toString(std::any_cast<iaVector4i>(_data));
        case iAnyType::iaVector4I:
            return iaString::toString(std::any_cast<iaVector4I>(_data));
        }
    }

    iAnyType iAny::getType() const
    {
        const auto iter = s_typeIndexToAnyType.find(_data.type());
        if (iter == s_typeIndexToAnyType.end())
        {
            return iAnyType::Unknown;
        }

        return iter->second;
    }

    bool iAny::compare(const iAny &any1, const iAny &any2)
    {
        std::any a = any1.getData();
        std::any b = any2.getData();

        if (a.type() != b.type())
        {
            return false;
        }

        if (a.type() == typeid(bool))
        {
            return std::any_cast<bool>(a) == std::any_cast<bool>(b);
        }
        else if (a.type() == typeid(uint8))
        {
            return std::any_cast<uint8>(a) == std::any_cast<uint8>(b);
        }
        else if (a.type() == typeid(int8))
        {
            return std::any_cast<int8>(a) == std::any_cast<int8>(b);
        }
        else if (a.type() == typeid(uint16))
        {
            return std::any_cast<uint16>(a) == std::any_cast<uint16>(b);
        }
        else if (a.type() == typeid(int16))
        {
            return std::any_cast<int16>(a) == std::any_cast<int16>(b);
        }
        else if (a.type() == typeid(uint32))
        {
            return std::any_cast<uint32>(a) == std::any_cast<uint32>(b);
        }
        else if (a.type() == typeid(int32))
        {
            return std::any_cast<int32>(a) == std::any_cast<int32>(b);
        }
        else if (a.type() == typeid(uint64))
        {
            return std::any_cast<uint64>(a) == std::any_cast<uint64>(b);
        }
        else if (a.type() == typeid(int64))
        {
            return std::any_cast<int64>(a) == std::any_cast<int64>(b);
        }
        else if (a.type() == typeid(float32))
        {
            return std::any_cast<float32>(a) == std::any_cast<float32>(b);
        }
        else if (a.type() == typeid(float64))
        {
            return std::any_cast<float64>(a) == std::any_cast<float64>(b);
        }
        else if (a.type() == typeid(std::string))
        {
            return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
        }
        else if (a.type() == typeid(std::wstring))
        {
            return std::any_cast<std::wstring>(a) == std::any_cast<std::wstring>(b);
        }
        else if (a.type() == typeid(iaString))
        {
            return std::any_cast<iaString>(a) == std::any_cast<iaString>(b);
        }
        else if (a.type() == typeid(iaVector2f))
        {
            return std::any_cast<iaVector2f>(a) == std::any_cast<iaVector2f>(b);
        }
        else if (a.type() == typeid(iaVector2d))
        {
            return std::any_cast<iaVector2d>(a) == std::any_cast<iaVector2d>(b);
        }
        else if (a.type() == typeid(iaVector2i))
        {
            return std::any_cast<iaVector2i>(a) == std::any_cast<iaVector2i>(b);
        }
        else if (a.type() == typeid(iaVector2I))
        {
            return std::any_cast<iaVector2I>(a) == std::any_cast<iaVector2I>(b);
        }
        else if (a.type() == typeid(iaVector3f))
        {
            return std::any_cast<iaVector3f>(a) == std::any_cast<iaVector3f>(b);
        }
        else if (a.type() == typeid(iaVector3d))
        {
            return std::any_cast<iaVector3d>(a) == std::any_cast<iaVector3d>(b);
        }
        else if (a.type() == typeid(iaVector3i))
        {
            return std::any_cast<iaVector3i>(a) == std::any_cast<iaVector3i>(b);
        }
        else if (a.type() == typeid(iaVector3I))
        {
            return std::any_cast<iaVector3I>(a) == std::any_cast<iaVector3I>(b);
        }
        else if (a.type() == typeid(iaVector4f))
        {
            return std::any_cast<iaVector4f>(a) == std::any_cast<iaVector4f>(b);
        }
        else if (a.type() == typeid(iaVector4d))
        {
            return std::any_cast<iaVector4d>(a) == std::any_cast<iaVector4d>(b);
        }
        else if (a.type() == typeid(iaVector4i))
        {
            return std::any_cast<iaVector4i>(a) == std::any_cast<iaVector4i>(b);
        }
        else if (a.type() == typeid(iaVector4I))
        {
            return std::any_cast<iaVector4I>(a) == std::any_cast<iaVector4I>(b);
        }
        return false;
    }

    std::wostream &operator<<(std::wostream &stream, const iAny &any)
    {
        stream << any.toString();
        return stream;
    }

    const std::any &iAny::getData() const
    {
        return _data;
    }

}