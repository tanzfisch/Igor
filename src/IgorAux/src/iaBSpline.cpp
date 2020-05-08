// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iaBSpline.h>

#include <iaConsole.h>

namespace IgorAux
{
    
	void iaBSpline::addSupportPoint(iaVector3f point)
	{
		_supportpoints.push_back(point);
		_recalc = true;
	}

	void iaBSpline::clear()
	{
		_supportpoints.clear();
		_recalc = true;
	}

	const uint32 iaBSpline::getSupportPointsCount() const
	{
		return static_cast<uint32>(_supportpoints.size());
	}

	void iaBSpline::setSupportPoint(iaVector3f &point, uint32 index)
	{
		_supportpoints[index] = point;
		_recalc = true;
	}

	iaVector3f& iaBSpline::getSupportPoint(uint32 index)
	{
		return _supportpoints[index];
	}

    const uint32 iaBSpline::getRank() const
    {
        return _rank;
    }

	void iaBSpline::setRank(const uint32 rank)
	{
		_rank = rank;
		_recalc = true;
	}

	void iaBSpline::prepareU()
	{
		_U.clear();

		for(uint32 i=0;i<_rank+_supportpoints.size()+1;++i)
		{
			if(i<_rank)
			{
				_U.push_back(0);
			}
			else if(i<_supportpoints.size())
			{
				_U.push_back(i - _rank + 1);
			}
			else
			{	
				_U.push_back(static_cast<uint32>(_supportpoints.size()) - _rank + 2);
			}
		}
	}

	float32 iaBSpline::calc(int32 k, float32 t, int32 i)
	{
		float32 left=0.0, right=0.0;

		if(t<(float32)(_supportpoints.size()-1))
		{
            if ((t < _U[i] || t >= _U[i + k]))
            {
                return(0.0);
            }
		}
		else
		{
            if ((t<_U[i] || t>_U[i + k]))
            {
                return(0.0);
            }
		}

		if(k==1) return(1.0);

        if (t - _U[i] != 0.0 && _U[i + k - 1] - _U[i] != 0.0)
        {
            left = (t - _U[i]) / (_U[i + k - 1] - _U[i])*calc(k - 1, t, i);
        }

        if (_U[i + k] - t != 0.0 && _U[i + k] - _U[i + 1] != 0.0)
        {
            right = (_U[i + k] - t) / (_U[i + k] - _U[i + 1])*calc(k - 1, t, i + 1);
        }

		return(left+right);
	}

	iaVector3f iaBSpline::getPointOnSpline(float32 t)
	{
        con_assert(0.0 <= t && t <= 1.0, "t out of range");
        con_assert(_supportpoints.size() >= 2, "too few support points");

		iaVector3f temp;
		if(_recalc)
		{
			prepareU();
			_recalc = false;
		}

        float32 realT = t * (_supportpoints.size()-1);

		for(uint32 i=0;i<_supportpoints.size();++i)
		{
            temp += _supportpoints[i] * calc(_rank, realT, i);
		}

		return temp;
	}

	const std::vector<iaVector3f>& iaBSpline::getSupportPoints() const
	{
		return _supportpoints;
	}

	void iaBSpline::getPoints(std::vector<iaVector3f> &points, int32 pointCount)
	{
		con_assert(pointCount >= 2, "invalid param");

		points.clear();

		if (pointCount < 2)
		{
			return;
		}

        if (_recalc)
        {
            prepareU();
            _recalc = false;
        }

		for(int32 i=0;i<= pointCount;++i)
		{
			points.push_back(getPointOnSpline(static_cast<float32>(i) / static_cast<float32>(pointCount)));
		}	
	}	

}
