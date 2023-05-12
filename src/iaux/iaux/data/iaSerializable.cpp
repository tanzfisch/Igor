// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header stream

#include <iaux/data/iaSerializable.h>

#include <iaux/system/iaConsole.h>
#include <iaux/data/iaString.h>

#include <cstring>

namespace iaux
{

    bool iaSerializable::write(std::ofstream &stream, const iaColor3f &value)
    {
        return iaSerializable::write(stream, reinterpret_cast<const char *>(&value._r), sizeof(iaColor3f));
    }

    bool iaSerializable::read(std::ifstream &stream, iaColor3f &value)
    {
        return iaSerializable::read(stream, reinterpret_cast<char *>(&value._r), sizeof(iaColor3f));
    }

    bool iaSerializable::write(std::ofstream &stream, const iaColor3c &value)
    {
        return iaSerializable::write(stream, reinterpret_cast<const char *>(&value._r), sizeof(iaColor3c));
    }

    bool iaSerializable::read(std::ifstream &stream, iaColor3c &value)
    {
        return iaSerializable::read(stream, reinterpret_cast<char *>(&value._r), sizeof(iaColor3c));
    }

    bool iaSerializable::write(std::ofstream &stream, const iaColor4f &value)
    {
        return iaSerializable::write(stream, reinterpret_cast<const char *>(&value._r), sizeof(iaColor4f));
    }

    bool iaSerializable::read(std::ifstream &stream, iaColor4f &value)
    {
        return iaSerializable::read(stream, reinterpret_cast<char *>(&value._r), sizeof(iaColor4f));
    }

    bool iaSerializable::write(std::ofstream &stream, const iaVector2f &value)
    {
        return iaSerializable::write(stream, reinterpret_cast<const char *>(value.getData()), sizeof(iaVector2f));
    }

    bool iaSerializable::read(std::ifstream &stream, iaVector2f &value)
    {
        return iaSerializable::read(stream, reinterpret_cast<char *>(value.getData()), sizeof(iaVector2f));
    }

    bool iaSerializable::write(std::ofstream &stream, const iaVector3f &value)
    {
        return iaSerializable::write(stream, reinterpret_cast<const char *>(value.getData()), sizeof(iaVector3f));
    }

    bool iaSerializable::read(std::ifstream &stream, iaVector3f &value)
    {
        return iaSerializable::read(stream, reinterpret_cast<char *>(value.getData()), sizeof(iaVector3f));
    }

    bool iaSerializable::write(std::ofstream &stream, const iaVector4f &value)
    {
        return iaSerializable::write(stream, reinterpret_cast<const char *>(value.getData()), sizeof(iaVector4f));
    }

    bool iaSerializable::read(std::ifstream &stream, iaVector4f &value)
    {
        return iaSerializable::read(stream, reinterpret_cast<char *>(value.getData()), sizeof(iaVector4f));
    }

    bool iaSerializable::write(std::ofstream &stream, const iaGradientf &value)
    {
        auto gradient = value.getValues();
        if (!iaSerializable::writeUInt16(stream, static_cast<uint16>(gradient.size())))
        {
            return false;
        }

        for (auto entry : gradient)
        {
            if (!iaSerializable::writeFloat32(stream, entry.first))
            {
                return false;
            }

            if (!iaSerializable::writeFloat32(stream, entry.second))
            {
                return false;
            }
        }

        return true;
    }

    bool iaSerializable::read(std::ifstream &stream, iaGradientf &value)
    {
        uint16 entryCount = 0;
        float32 gPos = 0;
        float32 gValue = 0;

        if (!iaSerializable::readUInt16(stream, entryCount))
        {
            return false;
        }

        for (int i = 0; i < entryCount; ++i)
        {
            if (!iaSerializable::readFloat32(stream, gPos))
            {
                return false;
            }

            if (!iaSerializable::readFloat32(stream, gValue))
            {
                return false;
            }

            value.setValue(gPos, gValue);
        }

        return true;
    }

    bool iaSerializable::write(std::ofstream &stream, const iaGradientui &value)
    {
        auto gradient = value.getValues();
        if (!iaSerializable::writeUInt16(stream, static_cast<uint16>(gradient.size())))
        {
            return false;
        }

        for (auto entry : gradient)
        {
            if (!iaSerializable::writeFloat32(stream, entry.first))
            {
                return false;
            }

            if (!iaSerializable::writeUInt32(stream, entry.second))
            {
                return false;
            }
        }

        return true;
    }

    bool iaSerializable::read(std::ifstream &stream, iaGradientui &value)
    {
        uint16 entryCount = 0;
        float32 gPos = 0;
        uint32 gValue = 0;

        if (!iaSerializable::readUInt16(stream, entryCount))
        {
            return false;
        }

        for (int i = 0; i < entryCount; ++i)
        {
            if (!iaSerializable::readFloat32(stream, gPos))
            {
                return false;
            }

            if (!iaSerializable::readUInt32(stream, gValue))
            {
                return false;
            }

            value.setValue(gPos, gValue);
        }

        return true;
    }

    bool iaSerializable::write(std::ofstream &stream, const iaGradientVector3f &value)
    {
        auto gradient = value.getValues();
        if (!iaSerializable::writeUInt16(stream, static_cast<uint16>(gradient.size())))
        {
            return false;
        }

        for (auto entry : gradient)
        {
            if (!iaSerializable::writeFloat32(stream, entry.first))
            {
                return false;
            }

            if (!iaSerializable::write(stream, reinterpret_cast<const char *>(entry.second.getData()), sizeof(float32) * 3))
            {
                return false;
            }
        }

        return true;
    }

    bool iaSerializable::read(std::ifstream &stream, iaGradientVector3f &value)
    {
        uint16 entryCount = 0;
        float32 gPos = 0;
        iaVector3f gValue;

        if (!iaSerializable::readUInt16(stream, entryCount))
        {
            return false;
        }

        for (int i = 0; i < entryCount; ++i)
        {
            if (!iaSerializable::readFloat32(stream, gPos))
            {
                return false;
            }

            if (!iaSerializable::read(stream, reinterpret_cast<char *>(gValue.getData()), sizeof(float32) * 3))
            {
                return false;
            }

            value.setValue(gPos, gValue);
        }

        return true;
    }

    bool iaSerializable::write(std::ofstream &stream, const iaGradientVector2f &value)
    {
        auto gradient = value.getValues();
        if (!iaSerializable::writeUInt16(stream, static_cast<uint16>(gradient.size())))
        {
            return false;
        }

        for (auto entry : gradient)
        {
            if (!iaSerializable::writeFloat32(stream, entry.first))
            {
                return false;
            }

            if (!iaSerializable::write(stream, reinterpret_cast<const char *>(entry.second.getData()), sizeof(float32) * 2))
            {
                return false;
            }
        }

        return true;
    }

    bool iaSerializable::read(std::ifstream &stream, iaGradientVector2f &value)
    {
        uint16 entryCount = 0;
        float32 gPos = 0;
        iaVector2f gValue;

        if (!iaSerializable::readUInt16(stream, entryCount))
        {
            return false;
        }

        for (int i = 0; i < entryCount; ++i)
        {
            if (!iaSerializable::readFloat32(stream, gPos))
            {
                return false;
            }

            if (!iaSerializable::read(stream, reinterpret_cast<char *>(gValue.getData()), sizeof(float32) * 2))
            {
                return false;
            }

            value.setValue(gPos, gValue);
        }

        return true;
    }

    bool iaSerializable::write(std::ofstream &stream, const iaGradientColor4f &value)
    {
        auto gradient = value.getValues();
        if (!iaSerializable::writeUInt16(stream, static_cast<uint16>(gradient.size())))
        {
            return false;
        }

        for (auto entry : gradient)
        {
            if (!iaSerializable::writeFloat32(stream, entry.first))
            {
                return false;
            }

            if (!iaSerializable::write(stream, reinterpret_cast<const char *>(entry.second.getData()), sizeof(float32) * 4))
            {
                return false;
            }
        }

        return true;
    }

    bool iaSerializable::read(std::ifstream &stream, iaGradientColor4f &value)
    {
        uint16 entryCount = 0;
        float32 gPos = 0;
        iaColor4f gValue;

        if (!iaSerializable::readUInt16(stream, entryCount))
        {
            return false;
        }

        for (int i = 0; i < entryCount; ++i)
        {
            if (!iaSerializable::readFloat32(stream, gPos))
            {
                return false;
            }

            if (!iaSerializable::read(stream, reinterpret_cast<char *>(gValue.getData()), sizeof(float32) * 4))
            {
                return false;
            }

            value.setValue(gPos, gValue);
        }

        return true;
    }

    bool iaSerializable::writeUTF8(std::ofstream &stream, const iaString &value)
    {
        con_assert(value.getUTF8Size() <= 0xffff, "string size out of range");

        uint64 utf8Size = value.getUTF8Size();
        if (utf8Size <= 0xffff)
        {
            if (iaSerializable::writeUInt16(stream, static_cast<uint16>(utf8Size)))
            {
                if (utf8Size != 0)
                {
                    char *buffer = new char[utf8Size];
#ifdef __IGOR_DEBUG__
                    memset(buffer, '#', utf8Size);
#endif
                    value.getUTF8(buffer, utf8Size);

                    bool result = iaSerializable::write(stream, buffer, static_cast<uint16>(utf8Size));
                    delete[] buffer;
                    return result;
                }
                else
                {
                    // nothing to write
                    return true;
                }
            }
            else
            {
                con_err("failed to write buffer length");
                return false;
            }
        }
        else
        {
            con_err("string size out of range");
            return false;
        }
    }

    bool iaSerializable::readUTF8(std::ifstream &stream, iaString &value)
    {
        con_assert(stream.is_open(), "stream not open");

        uint16 utf8Size = 0;
        if (!iaSerializable::readUInt16(stream, utf8Size))
        {
            return false;
        }

        if (utf8Size != 0)
        {
            char *buffer = new char[utf8Size];
            bool result = iaSerializable::read(stream, buffer, utf8Size);
            if (result)
            {
                value.setUTF8(buffer, utf8Size);
            }
            delete[] buffer;

            return result;
        }

        return true;
    }

} // namespace iaux