#include <iNodeTransformControl.h>

#include <iNodeTransform.h>
#include <iNodeFactory.h>

#include <iMouse.h>
#include <iKeyboard.h>
#include <iApplication.h>

namespace Igor
{

    iNodeTransformControl::iNodeTransformControl(bool initDefaultLayout)
        : iControl(initDefaultLayout)
    {
        iMouse::getInstance().registerMouseMoveFullDelegate(MouseMoveFullDelegate(this, &iNodeTransformControl::onMouseMove));
    }

    iNodeTransformControl::~iNodeTransformControl()
    {
        iMouse::getInstance().unregisterMouseMoveFullDelegate(MouseMoveFullDelegate(this, &iNodeTransformControl::onMouseMove));
    }

    uint32 iNodeTransformControl::getNode()
    {
        return _nodeID;
    }

    void iNodeTransformControl::setNode(uint32 nodeID)
    {
        _nodeID = nodeID;
    }

    void iNodeTransformControl::onHandle()
    {
        iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_nodeID));
        if (transformNode != nullptr)
        {
            if (_activeDir[iTransformDirection::RotateLeft])
            {
                _heading.rotate(_factorDir[iTransformDirection::RotateLeft], iaAxis::Y);
            }

            if (_activeDir[iTransformDirection::RotateRight])
            {
                _heading.rotate(-_factorDir[iTransformDirection::RotateRight], iaAxis::Y);
            }

            if (_activeDir[iTransformDirection::RotateUp])
            {
                _pitch.rotate(_factorDir[iTransformDirection::RotateUp], iaAxis::X);
            }

            if (_activeDir[iTransformDirection::RotateDown])
            {
                _pitch.rotate(-_factorDir[iTransformDirection::RotateDown], iaAxis::X);
            }

            if (_activeDir[iTransformDirection::RollLeft])
            {
                _roll.rotate(_factorDir[iTransformDirection::RollLeft], iaAxis::Z);
            }

            if (_activeDir[iTransformDirection::RollRight])
            {
                _roll.rotate(-_factorDir[iTransformDirection::RollRight], iaAxis::Z);
            }

            iaMatrixf orientation = _heading;
            orientation *= _pitch;
            orientation *= _roll;

            iaMatrixf matrix;
            transformNode->getMatrix(matrix);

            iaMatrixf translation;
            translation._pos = matrix._pos;

            if (_activeDir[iTransformDirection::Foreward])
            {
                translation._pos -= orientation._depth * _factorDir[iTransformDirection::Foreward];
            }

            if (_activeDir[iTransformDirection::Backward])
            {
                translation._pos += orientation._depth * _factorDir[iTransformDirection::Backward];
            }

            if (_activeDir[iTransformDirection::Left])
            {
                translation._pos -= orientation._right * _factorDir[iTransformDirection::Left];
            }

            if (_activeDir[iTransformDirection::Right])
            {
                translation._pos += orientation._right * _factorDir[iTransformDirection::Right];
            }

            if (_activeDir[iTransformDirection::Up])
            {
                translation._pos += orientation._top * _factorDir[iTransformDirection::Up];
            }

            if (_activeDir[iTransformDirection::Down])
            {
                translation._pos -= orientation._top * _factorDir[iTransformDirection::Down];
            }

            translation *= orientation;

            transformNode->setMatrix(translation);
        }
    }

    void iNodeTransformControl::onMouseMove(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window)
    {
        if (_activeDir[iTransformDirection::MouseHV])
        {
            _pitch.rotate((y1 - y2) * _factorDir[iTransformDirection::MouseHV], iaAxis::X);
            _heading.rotate((x1 - x2) * _factorDir[iTransformDirection::MouseHV], iaAxis::Y);

            iMouse::getInstance().setCenter(true);
        }
    }

}