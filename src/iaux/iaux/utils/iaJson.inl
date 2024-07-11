// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header stream

template <typename T>
const T iaJson::getValue(const json &jsonElement, const iaString &elementName, const T &defaultValue)
{
    char temp[128];
    elementName.getData(temp, 128);

    if(!jsonElement.contains(temp))
    {
        return defaultValue;
    }

    return jsonElement[temp].get<T>();
}
