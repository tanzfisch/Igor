// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iSystemPosition.h>
#include <iOctree.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    iSystemPosition::iSystemPosition()
    {
        _octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 40000.0), 50.0, 8, 4);
    }

    iSystemPosition::~iSystemPosition()
    {
        delete _octree;
        _octree = nullptr;
    }

    void iSystemPosition::handle()
    {
        // nothing to do
    }

    void iSystemPosition::registerEntity(uint64 entityID)
    {
        con_assert(_spheres.find(entityID) == _spheres.end(), "already registered");

        if (_spheres.find(entityID) == _spheres.end())
        {
            iSpheref sphere;
            _spheres[entityID] = sphere;
            _octree->insert(entityID, sphere);
        }
    }

    void iSystemPosition::unregisterEntity(uint64 entityID)
    {
        con_assert(_spheres.find(entityID) != _spheres.end(), "entity id not found");

        auto iter = _spheres.find(entityID);
        if (iter != _spheres.end())
        {
            _spheres.erase(iter);
            _octree->remove(entityID);
        }
    }

    bool iSystemPosition::hasEntity(uint64 entityID)
    {
        auto iter = _spheres.find(entityID);
        if (iter != _spheres.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void iSystemPosition::setPosition(uint64 entityID, const iaVector3f& position)
    {
        auto iter = _spheres.find(entityID);
        if (iter != _spheres.end())
        {
            (*iter).second._center = position;
            _octree->update(entityID, (*iter).second);
        }
    }

    const iaVector3f& iSystemPosition::getPosition(uint64 entityID)
    {
        iaVector3f result;

        auto iter = _spheres.find(entityID);
        if (iter != _spheres.end())
        {
            result = (*iter).second._center;
        }

        return result;
    }

    void iSystemPosition::query(const iSphered& sphere, vector<uint64>& data)
    {
        _octree->resetFilter();
        _octree->filter(sphere);
        _octree->getResult(data);
    }

}