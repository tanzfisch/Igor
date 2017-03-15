// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

template <typename T>
bool iIntersection::intersects(const iAACube<T> &cube, const iaVector3<T> &vec)
{
    if (abs(cube._center._x - vec._x) < cube._halfEdgeLength)
    {
        if (abs(cube._center._y - vec._y) < cube._halfEdgeLength)
        {
            if (abs(cube._center._z - vec._z) < cube._halfEdgeLength)
            {
                return true;
            }
        }
    }
    return false;
}

template <typename T>
bool iIntersection::intersects(const iAACube<T> &cubeA, const iAACube<T> &cubeB)
{
    T sum = cubeA._halfEdgeLength + cubeB._halfEdgeLength;

    if (abs(cubeA._center._x - cubeB._center._x) < sum)
    {
        if (abs(cubeA._center._y - cubeB._center._y) < sum)
        {
            if (abs(cubeA._center._z - cubeB._center._z) < sum)
            {
                return true;
            }
        }
    }
    return false;
}

template <typename T>
bool iIntersection::intersects(const iAABox<T> &boxA, const iAABox<T> &boxB)
{
    iaVector3<T> sum = boxA._halfWidths;
    sum += boxB._halfWidths;

    if (abs(boxA._center._x - boxB._center._x) < sum._x)
    {
        if (abs(boxA._center._y - boxB._center._y) < sum._y)
        {
            if (abs(boxA._center._z - boxB._center._z) < sum._z)
            {
                return true;
            }
        }
    }
    return false;
}

template <typename T>
bool iIntersection::intersects(iRectangle<T> rectangle, iaVector2<T> point)
{
    if (point._x < rectangle._x)
    {
        return false;
    }

    if (point._y < rectangle._y)
    {
        return false;
    }

    if (point._x > rectangle._x + rectangle._width - 1)
    {
        return false;
    }

    if (point._y > rectangle._y + rectangle._height - 1)
    {
        return false;
    }

    return true;
}

template <typename T>
bool iIntersection::intersects(const iAABox<T> &box, const iaVector3<T> &vec)
{
    if (abs(box._center._x - vec._x) < box._halfWidths._x)
    {
        if (abs(box._center._y - vec._y) < box._halfWidths._y)
        {
            if (abs(box._center._z - vec._z) < box._halfWidths._z)
            {
                return true;
            }
        }
    }
    return false;
}



/* todo maybe we can use this later

bool intersection(Point<T> point, Shape<T> shape)
{
int i, j;
bool result = false;
for (i = 0, j = shape.positions.size()-1; i < shape.positions.size(); j = i++)
{
if ((((shape.positions[i].y <= point.pos.y) && (point.pos.y < shape.positions[j].y)) ||
((shape.positions[j].y <= point.pos.y) && (point.pos.y < shape.positions[i].y))) &&
(point.pos.x < (shape.positions[j].x - shape.positions[i].x) * (point.pos.y - shape.positions[i].y) / (shape.positions[j].y - shape.positions[i].y) + shape.positions[i].x))
result = !result;
}
return result;
}*/