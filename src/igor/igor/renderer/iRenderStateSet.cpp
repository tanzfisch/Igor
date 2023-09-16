// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iRenderStateSet.h>

namespace igor
{

    iRenderStateSet::iRenderStateSet()
    {
        // initialize default settings
        _renderStates[static_cast<unsigned int>(iRenderState::DepthTest)] = iRenderStateValue::On;
        _renderStates[static_cast<unsigned int>(iRenderState::DepthMask)] = iRenderStateValue::On;
        _renderStates[static_cast<unsigned int>(iRenderState::Blend)] = iRenderStateValue::Off;
        _renderStates[static_cast<unsigned int>(iRenderState::CullFace)] = iRenderStateValue::On;
        _renderStates[static_cast<unsigned int>(iRenderState::Wireframe)] = iRenderStateValue::Off;
        _renderStates[static_cast<unsigned int>(iRenderState::DepthFunc)] = iRenderStateValue::Less;
        _renderStates[static_cast<unsigned int>(iRenderState::CullFaceFunc)] = iRenderStateValue::Back;
        _renderStates[static_cast<unsigned int>(iRenderState::Instanced)] = iRenderStateValue::Off;
        _renderStates[static_cast<unsigned int>(iRenderState::InstancedFunc)] = iRenderStateValue::PositionOrientationInstancing;
    }

    void iRenderStateSet::setRenderState(const iRenderState state, const iRenderStateValue value)
    {
        _renderStates[static_cast<unsigned int>(state)] = value;
    }

    iRenderStateValue iRenderStateSet::getRenderState(const iRenderState state) const
    {
        return _renderStates[static_cast<unsigned int>(state)];
    }

}; // namespace igor
