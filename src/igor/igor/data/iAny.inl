// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

template <class T>
iAny::iAny(T &&value)
    : _data(std::forward<T>(value))
{
}

template <typename T>
T iAny::getValue() const
{
    try
    {
        return std::any_cast<T>(_data);
    }
    catch (const std::exception &e)
    {
        con_crit("invalid any cast");
    }

    return T();
}