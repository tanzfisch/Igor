// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header stream

#include <igor/utils/iAny.h>

namespace igor
{

    iAny &iAny::getInstance()
    {
        static iAny _instance;
        return _instance;
    }

    iAny::iAny()
    {
        add<iLightType>();
        add<iaAxis>();
    }

    bool iAny::compare(const std::any &a, const std::any &b) const
    {
        if (a.type() != b.type())
        {
            return false;
        }

        auto it = _comparators.find(std::type_index(a.type()));
        if (it != _comparators.end())
        {
            return it->second(a, b);
        }

        con_err("unregistered type " << a.type().name());
        return false;
    }
}