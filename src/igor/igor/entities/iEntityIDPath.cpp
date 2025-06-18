// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntityIDPath.h>

namespace igor
{

    iEntityIDPath::iEntityIDPath(const iEntityIDPath& other)
    {
        _path = other._path;
    }

    iEntityIDPath::iEntityIDPath(const iaString& path)
    {
        std::vector<iaString> tokens;
        path.split('/', tokens);

        for(const auto &token : tokens)
        {
            _path.push_back(iaUUID(token));
        }
    }

    const iaString iEntityIDPath::toString() const
    {
        iaString result;

        for(const auto &entityID : _path)
        {
            result += '/';
            result += entityID.toString();
        }

        return result;
    }

    const std::vector<iEntityID>& iEntityIDPath::getPath() const
    {
        return _path;
    }

    void iEntityIDPath::operator+=(const iEntityID &entityID)
    {
        _path.push_back(entityID);
    }

    bool iEntityIDPath::operator==(const iEntityIDPath &other) const
    {
        return _path == other._path;
    }

    bool iEntityIDPath::operator!=(const iEntityIDPath &other) const
    {
        return _path != other._path;
    }

    void iEntityIDPath::reverse()
    {
        std::reverse(_path.begin(), _path.end());
    }
}