#ifndef __ENTITY__
#define __ENTITY__

#include <iDefines.h>
#include <iSphere.h>
using namespace Igor;

enum class Fraction
{
    Red,
    Blue,
    Green,
    Yellow,
    None
};

enum class EntityType
{
    Vehicle,
    Weapon,
    None
};

class Entity
{

    friend class EntityManager;

public:

    uint64 getID();
        EntityType getType() const;

        const iSphered& getSphere() const;

    Fraction getFraction() const;

    float32 getShield() const;
    void setShield(float32 value);

    float32 getHealth() const;
    void setHealth(float32 value);

    float32 getDamage() const;
    void setDamage(float32 value);

    float32 getShieldDamage() const;
    void setShieldDamage(float32 value);

    void kill();

    Entity(Fraction fraction, EntityType type);
    virtual ~Entity();

protected:

    uint64 _id = 0;

    virtual void handle() = 0;
    virtual iaVector3d updatePos() = 0;
    virtual void hitBy(uint64 entityID) = 0;

private:

    float32 _shield = 0;
    float32 _health = 0;
    float32 _shieldDamage = 0;
    float32 _damage = 0;

    static uint64 _nextID;

    iSphered _sphere;
    
    Fraction _fraction = Fraction::None;
    EntityType _type = EntityType::None;

};

#endif