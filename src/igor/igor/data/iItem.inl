// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

template <>
inline void iItem::setValue(const iaString &key, const iaString &value)
{
    const uint8 *data = reinterpret_cast<const uint8 *>(value.getData());
    _data.setData(key, data, value.getSize());
}

template <>
inline iaString iItem::getValue(const iaString &key) const
{
    uint8 *data = nullptr;
    uint32 dataSize = 0;
    _data.getData(key, &data, dataSize);

    if (data == nullptr)
    {
        return iaString();
    }

    return iaString(reinterpret_cast<wchar_t *>(data));
}

template <>
inline void iItem::setValue(const iaString &key, const bool& value)
{
    uint8 boolVal = value;
    _data.setData(key, &boolVal, sizeof(uint8));
}

template <>
inline bool iItem::getValue(const iaString &key) const
{
    uint8 *data = nullptr;
    uint32 dataSize = 0;
    _data.getData(key, &data, dataSize);

    if (data == nullptr)
    {
        return false;
    }

    return (bool)*data;
}

template <>
inline void iItem::setValue(const iaString &key, const uint64& value)
{
    _data.setData(key, reinterpret_cast<const uint8*>(&value), sizeof(uint64));
}

template <>
inline uint64 iItem::getValue(const iaString &key) const
{
    uint8 *data = nullptr;
    uint32 dataSize = 0;
    _data.getData(key, &data, dataSize);

    if (data == nullptr)
    {
        return 0;
    }

    return (uint64)*reinterpret_cast<uint64*>(data);
}
