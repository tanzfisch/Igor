#ifndef __ENTITY__
#define __ENTITY__

#include <iDefines.h>
#include <iSphere.h>
using namespace Igor;

class Entity
{

public:

    uint64 getID();
    uint32 getFraction();
    const iSpheref& getSphere() const;

    void syncPosition();

    virtual void handle() = 0;

    Entity(uint32 fraction = 0);
    virtual ~Entity();

protected:

    iSpheref _sphere;

private:

    static uint64 _nextID;

    uint64 _id = 0;

    uint32 _fraction = 0;

    

};

#endif