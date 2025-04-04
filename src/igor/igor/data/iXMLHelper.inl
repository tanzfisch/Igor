// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

template <>
inline const iaColor3f iXMLHelper::getValue(TiXmlElement *element, const iaString &elementName, const iaColor3f &defaultValue)
{
    const iaString value = getValue(element, elementName);

    if (value.isEmpty())
    {
        return defaultValue;
    }

    std::vector<iaString> tokens;
    value.split(",", tokens);
    if (tokens.size() != 3)
    {
        return defaultValue;
    }

    return iaColor3f(iaString::toFloat(iaString::trim(tokens[0])),
                     iaString::toFloat(iaString::trim(tokens[1])),
                     iaString::toFloat(iaString::trim(tokens[2])));
}

template <>
inline const iaVector2f iXMLHelper::getValue(TiXmlElement *element, const iaString &elementName, const iaVector2f &defaultValue)
{
    const iaString value = getValue(element, elementName);

    if (value.isEmpty())
    {
        return defaultValue;
    }

    std::vector<iaString> tokens;
    value.split(",", tokens);
    if (tokens.size() != 2)
    {
        return defaultValue;
    }

    return iaVector2f(iaString::toFloat(iaString::trim(tokens[0])),
                      iaString::toFloat(iaString::trim(tokens[1])));
}

template <>
inline const float32 iXMLHelper::getValue(TiXmlElement *element, const iaString &elementName, const float32 &defaultValue)
{
    const iaString value = getValue(element, elementName);

    if (value.isEmpty())
    {
        return defaultValue;
    }

    return (float32)iaString::toFloat(iaString::trim(value));
}

template <>
inline const iaString iXMLHelper::getValue(TiXmlElement *element, const iaString &elementName, const iaString &defaultValue)
{
    const iaString value = getValue(element, elementName);

    if (value.isEmpty())
    {
        return defaultValue;
    }

    return value;
}