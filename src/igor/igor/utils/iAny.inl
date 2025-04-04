// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header stream

template <typename T>
void iAny::add()
{
    auto typeIndex = std::type_index(typeid(T));
    _comparators[typeIndex] = [](const std::any &a, const std::any &b) -> bool
    {
        return std::any_cast<T>(a) == std::any_cast<T>(b);
    };
}