// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

__IGOR_INLINE__ iRenderStateValue iMaterial::getRenderState(const iRenderState state) const
{
    return _renderStateSet.getRenderState(state);
}