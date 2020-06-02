#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include <igor/entities/iEntityLocatable.h>
using namespace Igor;

enum class Fraction
{
    Red,
    Blue,
    Green,
    Yellow,
    None
};

enum class GameObjectType
{
    Vehicle,
    Weapon,
    None
};

class GameObject : public iEntityLocatable
{

    friend class Ascent;

public:
    Fraction getFraction() const;
    GameObjectType getType() const;

    float32 getShield() const;
    void setShield(float32 value);

    float32 getHealth() const;
    void setHealth(float32 value);

    float32 getDamage() const;
    void setDamage(float32 value);

    float32 getShieldDamage() const;
    void setShieldDamage(float32 value);

    void kill();
    virtual iaVector3d getCurrentPos() = 0;

    GameObject(Fraction fraction, GameObjectType type);
    virtual ~GameObject();

protected:
    virtual void handle() = 0;
    virtual void hitBy(uint64 entityID) = 0;

private:
    float32 _shield = 0;
    float32 _health = 0;
    float32 _shieldDamage = 0;
    float32 _damage = 0;

    Fraction _fraction = Fraction::None;
    GameObjectType _type = GameObjectType::None;
};

#endif