// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

__IGOR_INLINE__ bool iaSerializable::write(ofstream& file, const char* buffer, uint32 bytes)
{
    con_assert(file.is_open(), "file is not open");
    con_assert(buffer != nullptr, "zero pointer");
    con_assert(bytes != 0, "nothing to do");

    if (!file.write(buffer, bytes))
    {
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::read(ifstream& file, char* buffer, uint32 bytes)
{
    con_assert(file.is_open(), "file is not open");
    con_assert(buffer != nullptr, "zero pointer");
    con_assert(bytes != 0, "nothing to do");

    if (!file.read(buffer, bytes))
    {
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt(ifstream& file, uint64& value, uint8 bytes)
{
    con_assert(bytes <= 8, "invalid parameter");
    con_assert(file.is_open(), "file is not open");

    value = 0;
    if (!file.read(reinterpret_cast<char*>(&value), bytes))
    {
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt(ofstream& file, uint64 value, uint8 bytes)
{
    con_assert(bytes <= 8, "invalid parameter");
    con_assert(file.is_open(), "file is not open");

    if (!file.write(reinterpret_cast<char*>(&value), bytes))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::readInt(ifstream& file, int64& value, uint8 bytes)
{
    con_assert(bytes <= 8, "invalid parameter");
    con_assert(file.is_open(), "file is not open");

    value = 0;
    if (!file.read(reinterpret_cast<char*>(&value), bytes))
    {
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt(ofstream& file, int64 value, uint8 bytes)
{
    con_assert(bytes <= 8, "invalid parameter");
    con_assert(file.is_open(), "file is not open");

    if (!file.write(reinterpret_cast<char*>(&value), bytes))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt8(ofstream& file, uint8 value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.write(reinterpret_cast<char*>(&value), 1))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt8(ofstream& file, int8 value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.write(reinterpret_cast<char*>(&value), 1))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt16(ofstream& file, uint16 value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.write(reinterpret_cast<char*>(&value), 2))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt16(ofstream& file, int16 value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.write(reinterpret_cast<char*>(&value), 2))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt32(ofstream& file, uint32 value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.write(reinterpret_cast<char*>(&value), 4))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt32(ofstream& file, int32 value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.write(reinterpret_cast<char*>(&value), 4))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeUInt64(ofstream& file, uint64 value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.write(reinterpret_cast<char*>(&value), 8))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeInt64(ofstream& file, int64 value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.write(reinterpret_cast<char*>(&value), 8))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeFloat32(ofstream& file, float32 value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.write(reinterpret_cast<char*>(&value), 4))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::writeFloat64(ofstream& file, float64 value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.write(reinterpret_cast<char*>(&value), 8))
    {
        con_err("write failed");
        return false;
    }

    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt8(ifstream& file, uint8& value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.read(reinterpret_cast<char*>(&value), 1))
    {
        con_err("read failed");
        return false;
    }
    return true;
}


__IGOR_INLINE__ bool iaSerializable::readInt8(ifstream& file, int8& value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.read(reinterpret_cast<char*>(&value), 1))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt16(ifstream& file, uint16& value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.read(reinterpret_cast<char*>(&value), 2))
    {
        con_err("read failed");
        return false;
    }
    return true;
}


__IGOR_INLINE__ bool iaSerializable::readInt16(ifstream& file, int16& value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.read(reinterpret_cast<char*>(&value), 2))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt32(ifstream& file, uint32& value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.read(reinterpret_cast<char*>(&value), 4))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readInt64(ifstream& file, int64& value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.read(reinterpret_cast<char*>(&value), 8))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readUInt64(ifstream& file, uint64& value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.read(reinterpret_cast<char*>(&value), 8))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readInt32(ifstream& file, int32& value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.read(reinterpret_cast<char*>(&value), 4))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readFloat32(ifstream& file, float32& value)
{
    con_assert(file.is_open(), "file is not open");

    if (!file.read(reinterpret_cast<char*>(&value), 4))
    {
        con_err("read failed");
        return false;
    }
    return true;
}

__IGOR_INLINE__ bool iaSerializable::readFloat64(ifstream& file, float64& value)
{
    con_assert(file.is_open(), "file is not open");
    
    if (!file.read(reinterpret_cast<char*>(&value), 8))
    {
        con_err("read failed");
        return false;
    }
    return true;
}