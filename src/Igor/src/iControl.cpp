#include <iControl.h>

#include <iNodeTransform.h>
#include <iNodeFactory.h>

#include <iMouse.h>
#include <iKeyboard.h>
#include <iApplication.h>

namespace Igor
{

    iControl::iControl(bool initDefaultLayout)
    {
        _activeDir[iTransformDirection::Foreward] = false;
        _activeDir[iTransformDirection::Backward] = false;
        _activeDir[iTransformDirection::Left] = false;
        _activeDir[iTransformDirection::Right] = false;
        _activeDir[iTransformDirection::Up] = false;
        _activeDir[iTransformDirection::Down] = false;
        _activeDir[iTransformDirection::RotateLeft] = false;
        _activeDir[iTransformDirection::RotateRight] = false;
        _activeDir[iTransformDirection::RotateUp] = false;
        _activeDir[iTransformDirection::RotateDown] = false;
        _activeDir[iTransformDirection::RollLeft] = false;
        _activeDir[iTransformDirection::RollRight] = false;
        _activeDir[iTransformDirection::MouseHV] = false;

        _factorDir[iTransformDirection::Foreward] = 1.0f;
        _factorDir[iTransformDirection::Backward] = 1.0f;
        _factorDir[iTransformDirection::Left] = 1.0f;
        _factorDir[iTransformDirection::Right] = 1.0f;
        _factorDir[iTransformDirection::Up] = 1.0f;
        _factorDir[iTransformDirection::Down] = 1.0f;
        _factorDir[iTransformDirection::RotateLeft] = 0.01f;
        _factorDir[iTransformDirection::RotateRight] = 0.01f;
        _factorDir[iTransformDirection::RotateUp] = 0.01f;
        _factorDir[iTransformDirection::RotateDown] = 0.01f;
        _factorDir[iTransformDirection::RollLeft] = 0.01f;
        _factorDir[iTransformDirection::RollRight] = 0.01f;
        _factorDir[iTransformDirection::MouseHV] = 0.005f;

        if (initDefaultLayout)
        {
            setKeyBinding(iKeyCode::MouseLeft, iTransformDirection::MouseHV);

            setKeyBinding(iKeyCode::Q, iTransformDirection::Up);
            setKeyBinding(iKeyCode::W, iTransformDirection::Foreward);
            setKeyBinding(iKeyCode::E, iTransformDirection::Down);
            setKeyBinding(iKeyCode::A, iTransformDirection::Left);
            setKeyBinding(iKeyCode::S, iTransformDirection::Backward);
            setKeyBinding(iKeyCode::D, iTransformDirection::Right);

            setKeyBinding(iKeyCode::Left, iTransformDirection::RotateLeft);
            setKeyBinding(iKeyCode::Right, iTransformDirection::RotateRight);
            setKeyBinding(iKeyCode::Up, iTransformDirection::RotateUp);
            setKeyBinding(iKeyCode::Down, iTransformDirection::RotateDown);
        }

        iMouse::getInstance().registerMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &iControl::onKeyDown));
        iMouse::getInstance().registerMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &iControl::onKeyUp));
        iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &iControl::onKeyDown));
        iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &iControl::onKeyUp));

        iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &iControl::onHandle));
    }

    iControl::~iControl()
    {
        iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &iControl::onHandle));

        iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &iControl::onKeyDown));
        iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &iControl::onKeyUp));
        iMouse::getInstance().unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &iControl::onKeyDown));
        iMouse::getInstance().unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &iControl::onKeyUp));
    }

    void iControl::setKeyBinding(iKeyCode key, iTransformDirection dir)
    {
        _keyMapping[key] = dir;
    }

    void iControl::removeKeyBinding(iKeyCode key)
    {
        auto iter = _keyMapping.find(key);
        if (iter != _keyMapping.end())
        {
            _keyMapping.erase(iter);
        }
    }

    void iControl::start(iTransformDirection direction)
    {
        _activeDir[direction] = true;
    }

    void iControl::stop(iTransformDirection direction)
    {
        _activeDir[direction] = false;
    }

    void iControl::setFactor(iTransformDirection direction, float32 speed)
    {
        _factorDir[direction] = speed;
    }

    float32 iControl::getFactor(iTransformDirection direction)
    {
        return _factorDir[direction];
    }

    void iControl::onKeyDown(iKeyCode key)
    {
        auto iterDir = _keyMapping.find(key);
        if (iterDir != _keyMapping.end())
        {
            auto iterActive = _activeDir.find((*iterDir).second);
            if (iterActive != _activeDir.end())
            {
                (*iterActive).second = true;
            }
        }
    }

    void iControl::onKeyUp(iKeyCode key)
    {
        auto iterDir = _keyMapping.find(key);
        if (iterDir != _keyMapping.end())
        {
            auto iterActive = _activeDir.find((*iterDir).second);
            if (iterActive != _activeDir.end())
            {
                (*iterActive).second = false;
            }
        }
    }

}