// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iBoneFactory.h>

#include <iBone.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    
    iBoneFactory::iBoneFactory()
    {
    }

    uint64 iBoneFactory::createBone()
    {
        iBone* bone = new iBone();
        _bones[bone->getID()] = bone;
        return bone->getID();
    }

    void iBoneFactory::destroyBone(uint64 boneID)
    {
        auto iter = _bones.find(boneID);
        if (iter != _bones.end())
        {
            delete (*iter).second;
            _bones.erase(iter);
        }
        else
        {
            con_err("bone with id " << boneID << " not found");
        }
    }

    iBone* iBoneFactory::getBone(uint64 boneID)
    {
        iBone* result = nullptr;

        auto iter = _bones.find(boneID);
        if (iter != _bones.end())
        {
            delete (*iter).second;
            _bones.erase(iter);
        }
        else
        {
            con_err("bone with id " << boneID << " not found");
        }
    }
}