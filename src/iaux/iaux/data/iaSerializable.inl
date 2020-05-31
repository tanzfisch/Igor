// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header stream

__IGOR_INLINE__ bool iaSerializable::write(std::ofstream& stream, const char* buffer, uint32 bytes)
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

__IGOR_INLINE__ bool iaSerializable::read(std::ifstream& stream, char* buffer, uint32 bytes)
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

__IGOR_INLINE__ bool iaSerializable::readUInt(std::ifstream& stream, uint64& value, uint8 bytes)
{
    con_assert(bytes <= 8, "invalid parameter");
    con_assert(stream.is_open(), "stream is not open");

    value = 0;
    if (!stream.read(reinterpret_cast<char*>(&value), bytes))
    {
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt(std::ofstream& stream, uint64 value, uint8 bytes)
{
    con_assert(bytes <= 8, "invalid parameter");
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), bytes))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::readInt(std::ifstream& stream, int64& value, uint8 bytes)
{
    con_assert(bytes <= 8, "invalid parameter");
    con_assert(stream.is_open(), "stream is not open");

    value = 0;
    if (!stream.read(reinterpret_cast<char*>(&value), bytes))
    {
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt(std::ofstream& stream, int64 value, uint8 bytes)
{
    con_assert(bytes <= 8, "invalid parameter");
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), bytes))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt8(std::ofstream& stream, uint8 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 1))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt8(std::ofstream& stream, int8 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 1))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt16(std::ofstream& stream, uint16 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 2))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt16(std::ofstream& stream, int16 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 2))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt32(std::ofstream& stream, uint32 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 4))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt32(std::ofstream& stream, int32 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 4))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt64(std::ofstream& stream, uint64 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 8))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt64(std::ofstream& stream, int64 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 8))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeFloat32(std::ofstream& stream, float32 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 4))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeFloat64(std::ofstream& stream, float64 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 8))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt8(std::ifstream& stream, uint8& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 1))
    {
        con_err("read failed");
        return false;
    }
    return true;
}


__IGOR_INLINE__ bool iaSerializable::readInt8(std::ifstream& stream, int8& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 1))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt16(std::ifstream& stream, uint16& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 2))
    {
        con_err("read failed");
        return false;
    }
    return true;
}


__IGOR_INLINE__ bool iaSerializable::readInt16(std::ifstream& stream, int16& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 2))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt32(std::ifstream& stream, uint32& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 4))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readInt64(std::ifstream& stream, int64& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 8))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt64(std::ifstream& stream, uint64& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 8))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readInt32(std::ifstream& stream, int32& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 4))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readFloat32(std::ifstream& stream, float32& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 4))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readFloat64(std::ifstream& stream, float64& value)
{
    con_assert(stream.is_open(), "stream is not open");
    
    if (!stream.read(reinterpret_cast<char*>(&value), 8))
    {
        con_err("read failed");
        return false;
    }
    return true;
}