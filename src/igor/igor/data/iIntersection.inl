// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <typename T>
bool iIntersection::intersects(const iSphere<T> &sphereA, const iSphere<T> &sphereB)
{
    T distance = sphereB._center.distance(sphereA._center);
    distance -= sphereA._radius;
    distance -= sphereB._radius;
    return distance <= 0;
}

template <typename T>
bool iIntersection::contains(const iSphere<T> &sphereA, const iSphere<T> &sphereB)
{
    const T distance = sphereB._center.distance(sphereA._center) + sphereB._radius;
    return distance <= sphereA._radius;
}

template <typename T>
bool iIntersection::inFrontOf(const iSphere<T> &sphere, const iPlane<T> &plane)
{
    const T distancePlanePoint = (plane._normal * sphere._center) - plane._distance;
    return distancePlanePoint > -sphere._radius;
}

template <typename T>
bool iIntersection::intersects(const iSphere<T> &sphere, const iFrustum<T> &frustum)
{
    if (!inFrontOf(sphere, frustum._nearPlane))
    {
        return false;
    }
    if (!inFrontOf(sphere, frustum._leftPlane))
    {
        return false;
    }
    if (!inFrontOf(sphere, frustum._rightPlane))
    {
        return false;
    }
    if (!inFrontOf(sphere, frustum._bottomPlane))
    {
        return false;
    }
    if (!inFrontOf(sphere, frustum._topPlane))
    {
        return false;
    }
    if (!inFrontOf(sphere, frustum._farPlane))
    {
        return false;
    }
    return true;
}

template <typename T>
bool iIntersection::inFrontOf(const iAACube<T> &cube, const iPlane<T> &plane)
{
    //! \todo this calculations are crap. basically pretending the cube is a sphere
    const T distancePlanePoint = (plane._normal * cube._center) - plane._distance;
    return distancePlanePoint > (-cube._halfEdgeLength * 3);
}

template <typename T>
bool iIntersection::intersects(const iAACube<T> &cube, const iSphere<T> &sphere)
{
    T maxDistance = cube._halfEdgeLength + sphere._radius;

    if (abs(cube._center._x - sphere._center._x) < maxDistance)
    {
        if (abs(cube._center._y - sphere._center._y) < maxDistance)
        {
            if (abs(cube._center._z - sphere._center._z) < maxDistance)
            {
                return true;
            }
        }
    }
    return false;
}

template <typename T>
bool iIntersection::intersects(const iAACube<T> &cube, const iFrustum<T> &frustum)
{
    if (!inFrontOf(cube, frustum._nearPlane))
    {
        return false;
    }

    if (!inFrontOf(cube, frustum._leftPlane))
    {
        return false;
    }

    if (!inFrontOf(cube, frustum._rightPlane))
    {
        return false;
    }

    if (!inFrontOf(cube, frustum._bottomPlane))
    {
        return false;
    }

    if (!inFrontOf(cube, frustum._topPlane))
    {
        return false;
    }

    if (!inFrontOf(cube, frustum._farPlane))
    {
        return false;
    }

    return true;
}

template <typename T>
bool iIntersection::intersects(const iaVector3<T> &vec, const iAACube<T> &cube)
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
bool iIntersection::intersects(iaVector2<T> point, iRectangle<T> rectangle)
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
bool iIntersection::intersects(const iaVector3<T> &vec, const iAABox<T> &box)
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

template <typename T>
bool iIntersection::intersects(iPlane<T> plane, iRay<T> ray, iaVector3<T> &intersection)
{
    T denom = plane._normal * ray.m_dir;
    if (std::abs(denom) < 0.00001)
    {
        return false;
    }

    T t = ((plane._normal * (plane._normal * plane._distance)) - (plane._normal * ray.m_pos)) / (plane._normal * ray.m_dir);
    intersection = ray.m_dir;
    intersection *= t;
    intersection += ray.m_pos;

    return true;
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