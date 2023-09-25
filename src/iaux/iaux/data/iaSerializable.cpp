// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header stream

#include <iaux/data/iaSerializable.h>

#include <iaux/system/iaConsole.h>
#include <iaux/data/iaString.h>

#include <cstring>

namespace iaux
{

    bool iaSerializable::write(std::ofstream &stream, const char *buffer, uint32 bytes)
    {
        con_assert(stream.is_open(), "stream is not open");
        con_assert(buffer != nullptr, "zero pointer");
        con_assert(bytes != 0, "nothing to do");

        if (!stream.write(buffer, bytes))
        {
            return false;
        }

        return true;
    }

    bool iaSerializable::read(std::ifstream &stream, char *buffer, uint32 bytes)
    {
        con_assert(stream.is_open(), "stream is not open");
        con_assert(buffer != nullptr, "zero pointer");
        con_assert(bytes != 0, "nothing to do");

        if (!stream.read(buffer, bytes))
        {
            return false;
        }
        return true;
    }

    bool iaSerializable::readUInt(std::ifstream &stream, uint64 &value, uint8 bytes)
    {
        con_assert(bytes <= 8, "invalid parameter");
        con_assert(stream.is_open(), "stream is not open");

        value = 0;
        if (!stream.read(reinterpret_cast<char *>(&value), bytes))
        {
            return false;
        }

        return true;
    }

    bool iaSerializable::writeUInt(std::ofstream &stream, uint64 value, uint8 bytes)
    {
        con_assert(bytes <= 8, "invalid parameter");
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.write(reinterpret_cast<char *>(&value), bytes))
        {
            con_err("write failed");
            return false;
        }

        return true;
    }

    bool iaSerializable::readInt(std::ifstream &stream, int64 &value, uint8 bytes)
    {
        con_assert(bytes <= 8, "invalid parameter");
        con_assert(stream.is_open(), "stream is not open");

        value = 0;
        if (!stream.read(reinterpret_cast<char *>(&value), bytes))
        {
            return false;
        }

        return true;
    }

    bool iaSerializable::writeInt(std::ofstream &stream, int64 value, uint8 bytes)
    {
        con_assert(bytes <= 8, "invalid parameter");
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.write(reinterpret_cast<char *>(&value), bytes))
        {
            con_err("write failed");
            return false;
        }

        return true;
    }

    bool iaSerializable::writeUInt8(std::ofstream &stream, uint8 value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.write(reinterpret_cast<char *>(&value), 1))
        {
            con_err("write failed");
            return false;
        }

        return true;
    }

    bool iaSerializable::writeInt8(std::ofstream &stream, int8 value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.write(reinterpret_cast<char *>(&value), 1))
        {
            con_err("write failed");
            return false;
        }

        return true;
    }

    bool iaSerializable::writeUInt16(std::ofstream &stream, uint16 value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.write(reinterpret_cast<char *>(&value), 2))
        {
            con_err("write failed");
            return false;
        }

        return true;
    }

    bool iaSerializable::writeInt16(std::ofstream &stream, int16 value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.write(reinterpret_cast<char *>(&value), 2))
        {
            con_err("write failed");
            return false;
        }

        return true;
    }

    bool iaSerializable::writeUInt32(std::ofstream &stream, uint32 value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.write(reinterpret_cast<char *>(&value), 4))
        {
            con_err("write failed");
            return false;
        }

        return true;
    }

    bool iaSerializable::writeInt32(std::ofstream &stream, int32 value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.write(reinterpret_cast<char *>(&value), 4))
        {
            con_err("write failed");
            return false;
        }

        return true;
    }

    bool iaSerializable::writeUInt64(std::ofstream &stream, uint64 value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.write(reinterpret_cast<char *>(&value), 8))
        {
            con_err("write failed");
            return false;
        }

        return true;
    }

    bool iaSerializable::writeInt64(std::ofstream &stream, int64 value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.write(reinterpret_cast<char *>(&value), 8))
        {
            con_err("write failed");
            return false;
        }

        return true;
    }

    bool iaSerializable::writeFloat32(std::ofstream &stream, float32 value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.write(reinterpret_cast<char *>(&value), 4))
        {
            con_err("write failed");
            return false;
        }

        return true;
    }

    bool iaSerializable::writeFloat64(std::ofstream &stream, float64 value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.write(reinterpret_cast<char *>(&value), 8))
        {
            con_err("write failed");
            return false;
        }

        return true;
    }

    bool iaSerializable::readUInt8(std::ifstream &stream, uint8 &value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.read(reinterpret_cast<char *>(&value), 1))
        {
            con_err("read failed");
            return false;
        }
        return true;
    }

    bool iaSerializable::readInt8(std::ifstream &stream, int8 &value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.read(reinterpret_cast<char *>(&value), 1))
        {
            con_err("read failed");
            return false;
        }
        return true;
    }

    bool iaSerializable::readUInt16(std::ifstream &stream, uint16 &value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.read(reinterpret_cast<char *>(&value), 2))
        {
            con_err("read failed");
            return false;
        }
        return true;
    }

    bool iaSerializable::readInt16(std::ifstream &stream, int16 &value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.read(reinterpret_cast<char *>(&value), 2))
        {
            con_err("read failed");
            return false;
        }
        return true;
    }

    bool iaSerializable::readUInt32(std::ifstream &stream, uint32 &value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.read(reinterpret_cast<char *>(&value), 4))
        {
            con_err("read failed");
            return false;
        }
        return true;
    }

    bool iaSerializable::readInt64(std::ifstream &stream, int64 &value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.read(reinterpret_cast<char *>(&value), 8))
        {
            con_err("read failed");
            return false;
        }
        return true;
    }

    bool iaSerializable::readUInt64(std::ifstream &stream, uint64 &value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.read(reinterpret_cast<char *>(&value), 8))
        {
            con_err("read failed");
            return false;
        }
        return true;
    }

    bool iaSerializable::readInt32(std::ifstream &stream, int32 &value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.read(reinterpret_cast<char *>(&value), 4))
        {
            con_err("read failed");
            return false;
        }
        return true;
    }

    bool iaSerializable::readFloat32(std::ifstream &stream, float32 &value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.read(reinterpret_cast<char *>(&value), 4))
        {
            con_err("read failed");
            return false;
        }
        return true;
    }

    bool iaSerializable::readFloat64(std::ifstream &stream, float64 &value)
    {
        con_assert(stream.is_open(), "stream is not open");

        if (!stream.read(reinterpret_cast<char *>(&value), 8))
        {
            con_err("read failed");
            return false;
        }
        return true;
    }

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

    bool iaSerializable::write(std::ofstream &stream, const iaKeyFrameGraphf &value)
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

    bool iaSerializable::read(std::ifstream &stream, iaKeyFrameGraphf &value)
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

    bool iaSerializable::write(std::ofstream &stream, const iaKeyFrameGraphui &value)
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

    bool iaSerializable::read(std::ifstream &stream, iaKeyFrameGraphui &value)
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

    bool iaSerializable::write(std::ofstream &stream, const iaKeyFrameGraphVector3f &value)
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

    bool iaSerializable::read(std::ifstream &stream, iaKeyFrameGraphVector3f &value)
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

    bool iaSerializable::write(std::ofstream &stream, const iaKeyFrameGraphVector2f &value)
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

    bool iaSerializable::read(std::ifstream &stream, iaKeyFrameGraphVector2f &value)
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

    bool iaSerializable::write(std::ofstream &stream, const iaKeyFrameGraphColor4f &value)
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

    bool iaSerializable::read(std::ifstream &stream, iaKeyFrameGraphColor4f &value)
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
#ifdef IGOR_DEBUG
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