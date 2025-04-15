// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/events/iEventFilesystem.h>

#include <sstream>

namespace igor
{

    iEventFileCreated::iEventFileCreated(const iaString &filename)
        : iEvent(nullptr), _filename(filename)
    {
    }

    const iaString iEventFileCreated::getInfo() const
    {
        std::wstringstream stream;

        stream << getName() << "[" << _filename << "]";

        return stream.str().c_str();
    }

    const iaString& iEventFileCreated::getFilename() const
    {
        return _filename;
    }

    iEventFileDeleted::iEventFileDeleted(const iaString &filename)
        : iEvent(nullptr), _filename(filename)
    {
    }

    const iaString iEventFileDeleted::getInfo() const
    {
        std::wstringstream stream;

        stream << getName() << "[" << _filename << "]";

        return stream.str().c_str();
    }

    const iaString& iEventFileDeleted::getFilename() const
    {
        return _filename;
    }

    iEventFileMovedFrom::iEventFileMovedFrom(const iaString &filename)
        : iEvent(nullptr), _filename(filename)
    {
    }

    const iaString iEventFileMovedFrom::getInfo() const
    {
        std::wstringstream stream;

        stream << getName() << "[" << _filename << "]";

        return stream.str().c_str();
    }

    const iaString& iEventFileMovedFrom::getFilename() const
    {
        return _filename;
    }

    iEventFileMovedTo::iEventFileMovedTo(const iaString &filename)
        : iEvent(nullptr), _filename(filename)
    {
    }

    const iaString iEventFileMovedTo::getInfo() const
    {
        std::wstringstream stream;

        stream << getName() << "[" << _filename << "]";

        return stream.str().c_str();
    }

    const iaString& iEventFileMovedTo::getFilename() const
    {
        return _filename;
    }

    iEventFileChanged::iEventFileChanged(const iaString &filename)
        : iEvent(nullptr), _filename(filename)
    {
    }

    const iaString iEventFileChanged::getInfo() const
    {
        std::wstringstream stream;

        stream << getName() << "[" << _filename << "]";

        return stream.str().c_str();
    }

    const iaString& iEventFileChanged::getFilename() const
    {
        return _filename;
    }    

} // namespace igor