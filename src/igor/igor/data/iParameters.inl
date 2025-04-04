// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

template <typename T>
T iParameters::getParameter(const iaString &name, const T &defaultValue) const
{
    auto iter = _parameters.find(name);
    if (iter == _parameters.end())
    {
        return defaultValue;
    }

    try
    {
        return std::any_cast<T>(iter->second);
    }
    catch (const std::exception &e)
    {
        // common mistake is to use a string but not make it an iaString
        con_crit("invalid any cast for parameter " << name);
    }

    return T();
}