// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <class T>
void iaBSpline<T>::addSupportPoint(const iaVector3<T> &point)
{
	_supportpoints.push_back(point);
	_recalc = true;
}

template <class T>
void iaBSpline<T>::clear()
{
	_supportpoints.clear();
	_recalc = true;
}

template <class T>
void iaBSpline<T>::setSupportPoint(const iaVector3<T> &point, uint32 index)
{
	_supportpoints[index] = point;
	_recalc = true;
}

template <class T>
__IGOR_INLINE__ const iaVector3<T> &iaBSpline<T>::getSupportPoint(uint32 index) const
{
	return _supportpoints[index];
}

template <class T>
const uint32 iaBSpline<T>::getRank() const
{
	return _rank;
}

template <class T>
void iaBSpline<T>::setRank(const uint32 rank)
{
	_rank = rank;
	_recalc = true;
}

template <class T>
void iaBSpline<T>::prepareU()
{
	_U.clear(); // TODO do not reallocate every time here

	for (uint32 i = 0; i < _rank + _supportpoints.size() + 1; ++i)
	{
		if (i < _rank)
		{
			_U.push_back(0);
		}
		else if (i < _supportpoints.size())
		{
			_U.push_back(i - _rank + 1);
		}
		else
		{
			_U.push_back(static_cast<uint32>(_supportpoints.size()) - _rank + 2);
		}
	}
}

template <class T>
T iaBSpline<T>::calc(int32 k, T t, int32 i)
{
	T left = (T)0.0;
	T right = (T)0.0;

	if (t < (T)(_supportpoints.size() - 1))
	{
		if ((t < _U[i] ||
			 t >= _U[i + k]))
		{
			return (T)0.0;
		}
	}
	else
	{
		if ((t < _U[i] ||
			 t > _U[i + k]))
		{
			return (T)0.0;
		}
	}

	if (k == 1)
	{
		return (T)1.0;
	}

	if (t - _U[i] != (T)0.0 &&
		_U[i + k - 1] - _U[i] != (T)0.0)
	{
		left = (t - _U[i]) / (_U[i + k - 1] - _U[i]) * calc(k - 1, t, i);
	}

	if (_U[i + k] - t != (T)0.0 &&
		_U[i + k] - _U[i + 1] != (T)0.0)
	{
		right = (_U[i + k] - t) / (_U[i + k] - _U[i + 1]) * calc(k - 1, t, i + 1);
	}

	return left + right;
}

template <class T>
iaVector3<T> iaBSpline<T>::getPointOnSpline(T t)
{
	iaVector3f result;

	if (t < (T)0.0 ||
		t > (T)1.0 ||
		_supportpoints.size() < 2)
	{
		return result;
	}

	if (_recalc)
	{
		prepareU();
		_recalc = false;
	}

	T realT = t * (_supportpoints.size() - 1);

	for (uint32 i = 0; i < _supportpoints.size(); ++i)
	{
		result += _supportpoints[i] * calc(_rank, realT, i);
	}

	return result;
}

template <class T>
const std::vector<iaVector3<T>> &iaBSpline<T>::getSupportPoints() const
{
	return _supportpoints;
}

template <class T>
void iaBSpline<T>::getPoints(std::vector<iaVector3<T>> &points, int32 pointCount)
{
	points.clear();

	if (_supportpoints.size() < 2 ||
		pointCount < 2)
	{
		return;
	}

	if (_recalc)
	{
		prepareU();
		_recalc = false;
	}

	for (int32 i = 0; i <= pointCount; ++i)
	{
		points.push_back(getPointOnSpline(static_cast<T>(i) / static_cast<T>(pointCount)));
	}
}
