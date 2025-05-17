// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

template <class T>
void iaString::toVector(const iaString &text, iaVector2<T> &vector)
{
    std::vector<iaString> tokens;
    text.split(',', tokens);
    con_assert(tokens.size() == 2, "invalid format");

    float64 x = iaString::toFloat(iaString::trim(tokens[0]));
    float64 y = iaString::toFloat(iaString::trim(tokens[1]));

    vector.set(static_cast<T>(x), static_cast<T>(y));
}

template <class T>
void iaString::toVector(const iaString &text, iaVector3<T> &vector)
{
    std::vector<iaString> tokens;
    text.split(',', tokens);
    con_assert(tokens.size() == 3, "invalid format");

    float64 x = iaString::toFloat(iaString::trim(tokens[0]));
    float64 y = iaString::toFloat(iaString::trim(tokens[1]));
    float64 z = iaString::toFloat(iaString::trim(tokens[2]));

    vector.set(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z));
}

template <class T>
void iaString::toVector(const iaString &text, iaVector4<T> &vector)
{
    std::vector<iaString> tokens;
    text.split(',', tokens);
    con_assert(tokens.size() == 4, "invalid format");

    float64 x = iaString::toFloat(iaString::trim(tokens[0]));
    float64 y = iaString::toFloat(iaString::trim(tokens[1]));
    float64 z = iaString::toFloat(iaString::trim(tokens[2]));
    float64 w = iaString::toFloat(iaString::trim(tokens[4]));

    vector.set(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z), static_cast<T>(w));
}

template <class T>
iaString iaString::toString(const iaVector2<T> &vec, int afterPoint)
{
    iaString result;

    result += iaString::toString(vec._x, afterPoint);
    result += ", ";
    result += iaString::toString(vec._y, afterPoint);

    return result;
}

template <class T>
iaString iaString::toString(const iaVector3<T> &vec, int afterPoint)
{
    iaString result;

    result += iaString::toString(vec._x, afterPoint);
    result += ", ";
    result += iaString::toString(vec._y, afterPoint);
    result += ", ";
    result += iaString::toString(vec._z, afterPoint);

    return result;
}

template <class T>
iaString iaString::toString(const iaQuaternion<T> &q, int afterPoint)
{
    iaString result;

    result += iaString::toString(q._w, afterPoint);
    result += ", ";
    result += iaString::toString(q._x, afterPoint);
    result += ", ";
    result += iaString::toString(q._y, afterPoint);
    result += ", ";
    result += iaString::toString(q._z, afterPoint);

    return result;
}

template <class T>
iaString iaString::toString(const iaVector4<T> &vec, int afterPoint)
{
    iaString result;

    result += iaString::toString(vec._x, afterPoint);
    result += ", ";
    result += iaString::toString(vec._y, afterPoint);
    result += ", ";
    result += iaString::toString(vec._z, afterPoint);
    result += ", ";
    result += iaString::toString(vec._w, afterPoint);

    return result;
}