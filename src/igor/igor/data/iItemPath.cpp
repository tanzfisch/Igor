// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/data/iItemPath.h>

namespace igor
{

    iItemPath::iItemPath(const iaString &path)
    {
        path.split('/', _path);
    }

    iItemPath::iItemPath(const std::vector<iaString> &path)
    {
        _path = path;
    }

    const std::vector<iaString> &iItemPath::getPath() const
    {
        return _path;
    }

    void iItemPath::operator+=(const iaString &path)
    {
        _path.push_back(path);
    }

    const iaString iItemPath::toString() const
    {
        iaString result;

        for (const auto &path : _path)
        {
            result += '/';
            result += path;
        }

        return result;
    }

    bool iItemPath::operator==(const iItemPath &other) const
    {
        return _path == other._path;
    }

    bool iItemPath::operator!=(const iItemPath &other) const
    {
        return _path != other._path;
    }
}