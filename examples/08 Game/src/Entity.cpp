#include "Entity.h"

uint64 Entity::_nextEntityID = 1;

Entity::Entity()
{
    _id = _nextEntityID++;
}

Entity::~Entity()
{

}

uint64 Entity::getID()
{
    return _id;
}

const iSpheref& Entity::getSphere()
{
    return _sphere;
}