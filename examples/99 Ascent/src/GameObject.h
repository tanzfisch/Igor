#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include <iEntity.h>
#include <iNode.h>
using namespace Igor;

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{
    class iScene;
    class iPhysicsBody;
    class iPhysicsJoint;
    class iView;
    class iWindow;
}

enum class Fraction
{
    Red,
    Blue,
    Green,
    Yellow,
    None
};

enum class GameObjectKind
{
    Vehicle,
    Weapon,
    None
};

/*! Acent specific game object
*/
class GameObject : public iEntity
{

    friend class Ascent;

public:

    /*! \returns the kind of game object
    */
    GameObjectKind getKind() const;

    /*! \returns the fraction this game object belongs to
    */
    Fraction getFraction() const;

    /*! \returns shield value
    */
    float32 getShield() const;

    /*! sets shield value

    \param value the new shield value
    */
    void setShield(float32 value);

    /*! \returns the health value
    */
    float32 getHealth() const;

    /*! sets the health value

    \param value the new health value
    */
    void setHealth(float32 value);

    /*! \returns damage value
    */
    float32 getDamage() const;

    /*! sets damage value

    \param value the new damage value
    */
    void setDamage(float32 value);

    /*! \returns shield damage value
    */
    float32 getShieldDamage() const;

    /*! sets shield damage value

    \param value the new shield damage value
    */
    void setShieldDamage(float32 value);

    /*! sets shield and health to zero and kills the game object
    */
    void kill();

protected:    
    
    /*! todo this is a workaround
    */
    static iScene* _scene;

    virtual void hitBy(uint64 entityID) = 0;

    /*! initialize entity
    */
    virtual void init() = 0;

    /*! deinitialize entity
    */
    virtual void deinit() = 0;

    /*! handle entity
    */
    virtual void handle() = 0;

    /*! nothing todo
    */
    GameObject(Fraction fraction, GameObjectKind kind);

    /*! nothing to do
    */
    virtual ~GameObject() = default;

private:

    /*! the shield value
    */
    float32 _shield = 0;

    /*! the health value
    */
    float32 _health = 0;

    /*! shield damage value
    */
    float32 _shieldDamage = 0;

    /*! damage value
    */
    float32 _damage = 0;

    /*! fraction the game object belongs to
    */
    Fraction _fraction = Fraction::None;

    /*! kind type of this game object
    */
    GameObjectKind _kind = GameObjectKind::None;


};

#endif