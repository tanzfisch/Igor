// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
iRay<T>::iRay(const iaVector3<T>& pos, const iaVector3<T>& dir)
{
    m_pos = pos;
    m_dir = dir;
}
