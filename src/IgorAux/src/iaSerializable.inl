// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header stream

__IGOR_INLINE__ bool iaSerializable::write(ofstream& stream, const char* buffer, uint32 bytes)
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

__IGOR_INLINE__ bool iaSerializable::read(ifstream& stream, char* buffer, uint32 bytes)
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

__IGOR_INLINE__ bool iaSerializable::readUInt(ifstream& stream, uint64& value, uint8 bytes)
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

__IGOR_INLINE__ bool iaSerializable::writeUInt(ofstream& stream, uint64 value, uint8 bytes)
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

__IGOR_INLINE__ bool iaSerializable::readInt(ifstream& stream, int64& value, uint8 bytes)
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

__IGOR_INLINE__ bool iaSerializable::writeInt(ofstream& stream, int64 value, uint8 bytes)
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

__IGOR_INLINE__ bool iaSerializable::writeUInt8(ofstream& stream, uint8 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 1))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt8(ofstream& stream, int8 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 1))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt16(ofstream& stream, uint16 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 2))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt16(ofstream& stream, int16 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 2))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt32(ofstream& stream, uint32 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 4))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt32(ofstream& stream, int32 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 4))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt64(ofstream& stream, uint64 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 8))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt64(ofstream& stream, int64 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 8))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeFloat32(ofstream& stream, float32 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 4))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeFloat64(ofstream& stream, float64 value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.write(reinterpret_cast<char*>(&value), 8))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt8(ifstream& stream, uint8& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 1))
    {
        con_err("read failed");
        return false;
    }
    return true;
}


__IGOR_INLINE__ bool iaSerializable::readInt8(ifstream& stream, int8& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 1))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt16(ifstream& stream, uint16& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 2))
    {
        con_err("read failed");
        return false;
    }
    return true;
}


__IGOR_INLINE__ bool iaSerializable::readInt16(ifstream& stream, int16& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 2))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt32(ifstream& stream, uint32& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 4))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readInt64(ifstream& stream, int64& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 8))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt64(ifstream& stream, uint64& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 8))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readInt32(ifstream& stream, int32& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 4))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readFloat32(ifstream& stream, float32& value)
{
    con_assert(stream.is_open(), "stream is not open");

    if (!stream.read(reinterpret_cast<char*>(&value), 4))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readFloat64(ifstream& stream, float64& value)
{
    con_assert(stream.is_open(), "stream is not open");
    
    if (!stream.read(reinterpret_cast<char*>(&value), 8))
    {
        con_err("read failed");
        return false;
    }
    return true;
}