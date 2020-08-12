// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

__IGOR_INLINE__ iRenderStateValue iRenderStateSet::getRenderState(const iRenderState state) const
{
    con_assert(state < iRenderState::RenderStateCount, "enum out of bounds");
    return _renderStates[static_cast<unsigned int>(state)];
}