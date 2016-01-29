#include <iEntityTransformControl.h>

#include <iEntityFactory.h>
#include <iPhysics.h>

#include <iMouse.h>
#include <iKeyboard.h>
#include <iApplication.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iEntityTransformControl::iEntityTransformControl(bool initDefaultLayout)
        : iControl(initDefaultLayout)
    {
    }

    iEntityTransformControl::~iEntityTransformControl()
    {
    }

    uint32 iEntityTransformControl::getEntity()
    {
        return _entityID;
    }

    void iEntityTransformControl::setEntity(uint32 entityID)
    {
        _entityID = entityID;
    }

    void iEntityTransformControl::onHandle()
    {
        iEntity* entity = iEntityFactory::getInstance().getEntity(_entityID);
        if (entity != nullptr)
        {
            iaMatrixf matrix;
            entity->getMatrix(matrix);

            iaVector3f resultingForce;
            iaVector3f resultingAngularForce;

            if (_activeDir[iTransformDirection::RotateLeft])
            {
                resultingAngularForce._y += 1.0f * _factorDir[iTransformDirection::RotateLeft];
            }

            if (_activeDir[iTransformDirection::RotateRight])
            {
                resultingAngularForce._y -= 1.0f * _factorDir[iTransformDirection::RotateRight];
            }

            if (_activeDir[iTransformDirection::RotateUp])
            {
                resultingAngularForce._x += 1.0f * _factorDir[iTransformDirection::RotateUp];
            }

            if (_activeDir[iTransformDirection::RotateDown])
            {
                resultingAngularForce._x -= 1.0f * _factorDir[iTransformDirection::RotateDown];
            }

            if (_activeDir[iTransformDirection::RollLeft])
            {
                resultingAngularForce._z += 1.0f * _factorDir[iTransformDirection::RollLeft];
            }

            if (_activeDir[iTransformDirection::RollRight])
            {
                resultingAngularForce._z -= 1.0f * _factorDir[iTransformDirection::RollRight];
            }

            if (_activeDir[iTransformDirection::Foreward])
            {
                iaVector3f foreward = matrix._depth;
                foreward.negate();
                foreward *= _factorDir[iTransformDirection::Foreward];
                resultingForce += foreward;
            }

            if (_activeDir[iTransformDirection::Backward])
            {
                iaVector3f backward = matrix._depth;
                backward *= _factorDir[iTransformDirection::Backward];
                resultingForce += backward;
            }

            if (_activeDir[iTransformDirection::Left])
            {
                iaVector3f left = matrix._right;
                left.negate();
                left *= _factorDir[iTransformDirection::Left];
                resultingForce += left;
            }

            if (_activeDir[iTransformDirection::Right])
            {
                iaVector3f right = matrix._right;
                right *= _factorDir[iTransformDirection::Right];
                resultingForce += right;
            }

            if (_activeDir[iTransformDirection::Up])
            {
                iaVector3f up = matrix._top;
                up *= _factorDir[iTransformDirection::Up];
                resultingForce += up;
            }

            if (_activeDir[iTransformDirection::Down])
            {
                iaVector3f down = matrix._top;
                down.negate();
                down *= _factorDir[iTransformDirection::Down];
                resultingForce += down;
            }

            entity->setForce(resultingForce);
            entity->setAngularForce(resultingAngularForce);
        }
    }

}