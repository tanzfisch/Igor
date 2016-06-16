#include <iVoxelData.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iVoxelData::~iVoxelData()
    {
        freeMemory();
    }

    void iVoxelData::freeMemory()
    {
        if (_data != nullptr)
        {
            delete [] _data;
            _data = nullptr;
        }
    }

    void iVoxelData::initData(int64 width, int64 height, int64 depth)
    {
        con_assert(width > 1 && height > 1 && depth > 1, "voxel grid too small");

        if (_width != width ||
            _height != height ||
            _depth != depth)
        {
            freeMemory();

            _width = width;
            _height = height;
            _depth = depth;

            _data = new iVoxelPole[_width * _depth];
            for (int i = 0; i < _width * _depth; ++i)
            {
                _data[i]._density.setSize(_height, _clearValue);
                _data[i]._material.setSize(_height, _clearValue);
            }
        }
        else
        {
            for (int i = 0; i < _width * _depth; ++i)
            {
                _data[i]._density.clear(_clearValue);
                _data[i]._material.clear(_clearValue);
            }
        }
    }

    void iVoxelData::clear()
    {
        initData(_width, _depth, _height);
    }

    void iVoxelData::setClearValue(uint8 clearValue)
    {
        _clearValue = clearValue;
    }

    uint8 iVoxelData::getClearValue() const
    {
        return _clearValue;
    }

	void iVoxelData::setVoxelPole(iaVector3I pos, int64 height, uint8 density)
	{
		con_assert(height > 0, "zero height");
		con_assert(height + pos._y - 1 < _height, "zero height");
		con_assert(pos._x >= 0 && pos._x < _width, "out of range");
		con_assert(pos._y >= 0 && pos._y < _height, "out of range");
		con_assert(pos._z >= 0 && pos._z < _depth, "out of range");
		_data[pos._z * _depth + pos._x]._density.setValue(pos._y, height, density);
	}

    void iVoxelData::setVoxelLine(iaVector3I pos1, iaVector3I pos2, uint8 density)
    {
        iaVector3I u(pos1._x, pos1._y, pos1._z);
        iaVector3I delta(pos2._x - pos1._x, pos2._y - pos1._y, pos2._z - pos1._z);
        iaVector3I step;

        (delta._x > 0) ? step._x = 1 : step._x = -1;
        (delta._y > 0) ? step._y = 1 : step._y = -1;
        (delta._z > 0) ? step._z = 1 : step._z = -1;

        if (delta._x < 0) delta._x = -delta._x;
        if (delta._y < 0) delta._y = -delta._y;
        if (delta._z < 0) delta._z = -delta._z;

        int64 dist = (delta._x>delta._y) ? delta._x : delta._y;
        dist = (dist > delta._z) ? dist : delta._z;

        iaVector3I err(delta._x, delta._y, delta._z);

        for (int i = 0; i < dist; i++)
        {
            setVoxelDensity(iaVector3I(u._x, u._y, u._z), density);

            err += delta;

            if (err._x > dist)
            {
                err._x -= dist;
                u._x += step._x;
            }

            if (err._y > dist)
            {
                err._y -= dist;
                u._y += step._y;
            }

            if (err._z > dist)
            {
                err._z -= dist;
                u._z += step._z;
            }
        }

        setVoxelDensity(iaVector3I(pos2._x, pos2._y, pos2._z), density);
    }

    vector<uint8> iVoxelData::getVoxelLine(iaVector3I pos1, iaVector3I pos2)
    {
        vector<uint8> result;

        iaVector3I u(pos1._x, pos1._y, pos1._z);
        iaVector3I delta(pos2._x - pos1._x, pos2._y - pos1._y, pos2._z - pos1._z);
        iaVector3I step;

        (delta._x > 0) ? step._x = 1 : step._x = -1;
        (delta._y > 0) ? step._y = 1 : step._y = -1;
        (delta._z > 0) ? step._z = 1 : step._z = -1;

        if (delta._x < 0) delta._x = -delta._x;
        if (delta._y < 0) delta._y = -delta._y;
        if (delta._z < 0) delta._z = -delta._z;

        int64 dist = (delta._x>delta._y) ? delta._x : delta._y;
        dist = (dist > delta._z) ? dist : delta._z;

        iaVector3I err(delta._x, delta._y, delta._z);

        for (int i = 0; i < dist; i++)
        {
            result.push_back(getVoxelDensity(iaVector3I(u._x, u._y, u._z)));

            err += delta;

            if (err._x > dist)
            {
                err._x -= dist;
                u._x += step._x;
            }

            if (err._y > dist)
            {
                err._y -= dist;
                u._y += step._y;
            }

            if (err._z > dist)
            {
                err._z -= dist;
                u._z += step._z;
            }
        }

        result.push_back(getVoxelDensity(iaVector3I(u._x, u._y, u._z)));

        return result;
    }

    bool iVoxelData::getIntersection(iaVector3I pos, iaVector3I dir, iaVector3I &return_pos, uint8 &return_value)
    {
        iaVector3I u(pos);
        iaVector3I delta(dir);
        iaVector3I step;

        return_pos.set(0, 0, 0);
        return_value = _nonSolidValue;

        (delta._x > 0) ? step._x = 1 : step._x = -1;
        (delta._y > 0) ? step._y = 1 : step._y = -1;
        (delta._z > 0) ? step._z = 1 : step._z = -1;

        if (delta._x < 0) delta._x = -delta._x;
        if (delta._y < 0) delta._y = -delta._y;
        if (delta._z < 0) delta._z = -delta._z;

        int64 dist = (delta._x>delta._y) ? delta._x : delta._y;
        dist = (dist > delta._z) ? dist : delta._z;

        iaVector3I err(delta._x, delta._y, delta._z);

        uint8 temp_value;
        bool inside_solid = false;

        if (u._x >= 0 && u._y >= 0 && u._z >= 0 &&
            u._x < _width && u._y < _height && u._z < _depth)
        {
            if ((temp_value = getVoxelDensity(iaVector3I(u._x, u._y, u._z))) != _nonSolidValue)
            {
                inside_solid = true;
            }
        }

        for (int i = 0; i < dist; i++)
        {
            if (u._x >= 0 && u._y >= 0 && u._z >= 0 &&
                u._x < _width && u._y < _height && u._z < _depth)
            {
                if ((temp_value = getVoxelDensity(iaVector3I(u._x, u._y, u._z))) != _nonSolidValue)
                {
                    if (!inside_solid)
                    {
                        return_pos = u;
                        return_value = temp_value;
                        return true;
                    }
                }
                else
                {
                    inside_solid = false;
                }
            }
            else
            {
                inside_solid = false;
            }

            err += delta;

            if (err._x > dist)
            {
                err._x -= dist;
                u._x += step._x;
            }

            if (err._y > dist)
            {
                err._y -= dist;
                u._y += step._y;
            }

            if (err._z > dist)
            {
                err._z -= dist;
                u._z += step._z;
            }
        }

        if (u._x >= 0 && u._y >= 0 && u._z >= 0 &&
            u._x < _width && u._y < _height && u._z < _depth)
        {
            if ((temp_value = getVoxelDensity(iaVector3I(u._x, u._y, u._z))) != _nonSolidValue)
            {
                if (!inside_solid)
                {
                    return_pos = u;
                    return_value = temp_value;
                    return true;
                }
            }
        }

        return false;
    }

    iaRLE<uint8, uint8>& iVoxelData::getDensityBlocks(int64 xDir, int64 zDir)
    {
        return _data[zDir * _depth + xDir]._density;
    }

    iaRLE<uint8, uint8>& iVoxelData::getMaterialBlocks(int64 xDir, int64 zDir)
    {
        return _data[zDir * _depth + xDir]._material;
    }

    void iVoxelData::setVoxelDensity(iaVector3I pos, uint8 density)
    {
		con_assert(pos._x >= 0 && pos._x < _width, "out of range");
		con_assert(pos._y >= 0 && pos._y < _height, "out of range");
		con_assert(pos._z >= 0 && pos._z < _depth, "out of range");
        _data[pos._z * _depth + pos._x]._density.setValue(pos._y, density);
    }

    uint8 iVoxelData::getVoxelDensity(iaVector3I pos)
    {
        return _data[pos._z * _depth + pos._x]._density.getValue(pos._y);
    }

    void iVoxelData::setVoxelMaterial(iaVector3I pos, uint8 material)
    {
        _data[pos._z * _depth + pos._x]._material.setValue(pos._y, material);
    }

    uint8 iVoxelData::getVoxelMaterial(iaVector3I pos)
    {
        return _data[pos._z * _depth + pos._x]._material.getValue(pos._y);
    }

    int64 iVoxelData::getWidth()
    {
        return _width;
    }

    int64 iVoxelData::getDepth()
    {
        return _depth;
    }

    int64 iVoxelData::getHeight()
    {
        return _height;
    }

}
