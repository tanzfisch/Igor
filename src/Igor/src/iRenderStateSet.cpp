// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iRenderStateSet.h>

namespace Igor
{

	iRenderStateSet::iRenderStateSet()
	{
		// initialize default settings
		_renderStates[static_cast<unsigned int>(iRenderState::DepthTest)] = iRenderStateValue::On;
		_renderStates[static_cast<unsigned int>(iRenderState::DepthMask)] = iRenderStateValue::On;
		_renderStates[static_cast<unsigned int>(iRenderState::Blend)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::CullFace)] = iRenderStateValue::On;
		_renderStates[static_cast<unsigned int>(iRenderState::Fog)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Lighting)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Light0)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Light1)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Light2)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Light3)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Light4)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Light5)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Light6)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Light7)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture2D0)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture2D1)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture2D2)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture2D3)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture2D4)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture2D5)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture2D6)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture2D7)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture1D0)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture1D1)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture1D2)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture1D3)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture1D4)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture1D5)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture1D6)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::Texture1D7)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::ColorMaskRed)] = iRenderStateValue::On;
		_renderStates[static_cast<unsigned int>(iRenderState::ColorMaskGreen)] = iRenderStateValue::On;
		_renderStates[static_cast<unsigned int>(iRenderState::ColorMaskBlue)] = iRenderStateValue::On;
		_renderStates[static_cast<unsigned int>(iRenderState::ColorMaskAlpha)] = iRenderStateValue::On;
		_renderStates[static_cast<unsigned int>(iRenderState::Wireframe)] = iRenderStateValue::Off;
		_renderStates[static_cast<unsigned int>(iRenderState::DepthFunc)] = iRenderStateValue::Less;
		_renderStates[static_cast<unsigned int>(iRenderState::CullFaceFunc)] = iRenderStateValue::Back;
		_renderStates[static_cast<unsigned int>(iRenderState::BlendFuncSource)] = iRenderStateValue::SourceAlpha;
		_renderStates[static_cast<unsigned int>(iRenderState::BlendFuncDestination)] = iRenderStateValue::OneMinusSourceAlpha;
        _renderStates[static_cast<unsigned int>(iRenderState::Instanced)] = iRenderStateValue::Off;
        _renderStates[static_cast<unsigned int>(iRenderState::InstancedFunc)] = iRenderStateValue::PositionOrientation;
	}

	void iRenderStateSet::setRenderState(const iRenderState state, const iRenderStateValue value)
	{
		if (value != iRenderStateValue::Invalid ||
			value != iRenderStateValue::RenderStateValueCount ||
			state != iRenderState::RenderStateCount)
		{
			_renderStates[static_cast<unsigned int>(state)] = value;
		}
		else
		{
			con_err("invalid parameter");
		}
	}

	iRenderStateValue iRenderStateSet::getRenderStateValue(const iRenderState state)
	{
        if (state != iRenderState::RenderStateCount)
        {
            return _renderStates[static_cast<unsigned int>(state)];
        }
        else
        {
            con_err("invalid parameter");
        }

        return iRenderStateValue::Invalid;
	}

};