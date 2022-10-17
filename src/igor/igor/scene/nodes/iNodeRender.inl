// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

__IGOR_INLINE__ bool iNodeRender::isVisible() const
{
    return _visible;
}

__IGOR_INLINE__ uint64 iNodeRender::getMaterial() const
{
    return _materialID;
}

__IGOR_INLINE__ const iaMatrixd &iNodeRender::getWorldMatrix() const
{
    return _worldMatrix;
}