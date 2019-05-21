// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
__IGOR_INLINE__ std::wostream& operator<<(std::wostream &ostr, const iaTransform<T> &t)
{
    ostr << "rotate " << m._rotate << "\n";
    ostr << "scale  " << m_scale << "\n";
    ostr << "shear  " << m_shear << "\n";
    ostr << "trans  " << m_translate << "\n";
    return ostr;
}

template <class T>
__IGOR_INLINE__ iaTransform<T>::iaTransform()
{
    identity();
}

template <class T>
__IGOR_INLINE__ iaTransform<T>::~iaTransform()
{
}

template <class T>
__IGOR_INLINE__ void iaTransform<T>::getMatrix(iaMatrix<T>& matrix)
{
	matrix.identity();
	matrix.rotate(_rotate);
	matrix.scale(_scale);
	matrix.shear(_shear);
	matrix.translate(_translate);
}