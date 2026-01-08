// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

template <>
inline void iUserDataComponent::setValue(const iaString &key, const iaString &value)
{
    const uint8 *data = reinterpret_cast<const uint8 *>(value.getData());
    _data.setData(key, data, value.getSize());
}

template <>
inline iaString iUserDataComponent::getValue(const iaString &key) const
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
inline void iUserDataComponent::setValue(const iaString &key, const iaUUID &value)
{
    const iaString string = value.toString();
    const uint8 *data = reinterpret_cast<const uint8 *>(string.getData());
    _data.setData(key, data, string.getSize());
}

template <>
inline iaUUID iUserDataComponent::getValue(const iaString &key) const
{
    uint8 *data = nullptr;
    uint32 dataSize = 0;
    _data.getData(key, &data, dataSize);

    if (data == nullptr)
    {
        return iaString();
    }

    return iaUUID(iaString(reinterpret_cast<wchar_t *>(data)));
}

template <>
inline void iUserDataComponent::setValue(const iaString &key, const bool& value)
{
    uint8 boolVal = value;
    _data.setData(key, &boolVal, sizeof(uint8));
}

template <>
inline bool iUserDataComponent::getValue(const iaString &key) const
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
inline void iUserDataComponent::setValue(const iaString &key, const int64& value)
{
    _data.setData(key, reinterpret_cast<const uint8*>(&value), sizeof(int64));
}

template <>
inline int64 iUserDataComponent::getValue(const iaString &key) const
{
    uint8 *data = nullptr;
    uint32 dataSize = 0;
    _data.getData(key, &data, dataSize);

    if (data == nullptr)
    {
        return 0;
    }

    return (int64)*reinterpret_cast<int64*>(data);
}

template <>
inline void iUserDataComponent::setValue(const iaString &key, const float64& value)
{
    _data.setData(key, reinterpret_cast<const uint8*>(&value), sizeof(float64));
}

template <>
inline float64 iUserDataComponent::getValue(const iaString &key) const
{
    uint8 *data = nullptr;
    uint32 dataSize = 0;
    _data.getData(key, &data, dataSize);

    if (data == nullptr)
    {
        return 0;
    }

    return (float64)*reinterpret_cast<float64*>(data);
}