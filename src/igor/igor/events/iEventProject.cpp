// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/events/iEventProject.h>

#include <sstream>

namespace igor
{

    iEventProjectLoaded::iEventProjectLoaded(const iaString &filename)
        : iEvent(nullptr), _filename(filename)
    {
    }

    const iaString iEventProjectLoaded::getInfo() const
    {
        std::wstringstream stream;

        stream << getName() << "[" << _filename << "]";

        return stream.str().c_str();
    }

    const iaString &iEventProjectLoaded::getFilename() const
    {
        return _filename;
    }

    iEventProjectUnloaded::iEventProjectUnloaded()
        : iEvent(nullptr)
    {
    }

    const iaString iEventProjectUnloaded::getInfo() const
    {
        std::wstringstream stream;

        stream << getName();

        return stream.str().c_str();        
    }

} // namespace igor