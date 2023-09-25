// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

template <typename T>
bool iIntersection::intersects(const iaSphere<T> &sphereA, const iaSphere<T> &sphereB)
{
    T distance = sphereB._center.distance(sphereA._center);
    distance -= sphereA._radius;
    distance -= sphereB._radius;
    return distance <= 0;
}

template <typename T>
bool iIntersection::contains(const iaSphere<T> &sphereA, const iaSphere<T> &sphereB)
{
    const T distance = sphereB._center.distance(sphereA._center) + sphereB._radius;
    return distance <= sphereA._radius;
}

template <typename T>
bool iIntersection::inFrontOf(const iaSphere<T> &sphere, const iPlane<T> &plane)
{
    const T distancePlanePoint = (plane._normal * sphere._center) - plane._distance;
    return distancePlanePoint > -sphere._radius;
}

template <typename T>
bool iIntersection::intersects(const iaSphere<T> &sphere, const iFrustum<T> &frustum)
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
    const T distancePlanePoint = (plane._normal * cube._center) - plane._distance;
    return distancePlanePoint > (-cube._halfEdgeLength * 3);
}

template <typename T>
bool iIntersection::intersects(const iAACube<T> &cube, const iaSphere<T> &sphere)
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
bool iIntersection::intersects(const iaVector2<T> &point, const iaRectangle<T> &rectangle)
{
    if (point._x < rectangle._x)
    {
        return false;
    }

    if (point._y < rectangle._y)
    {
        return false;
    }

    if (point._x > rectangle._x + rectangle._width)
    {
        return false;
    }

    if (point._y > rectangle._y + rectangle._height)
    {
        return false;
    }

    return true;
}

template <typename T>
bool iIntersection::intersects(const iaRectangle<T> &rectangleA, const iaRectangle<T> &rectangleB)
{
    if(rectangleA.getRight() < rectangleB.getLeft())
    {
        return false;
    }

    if(rectangleA.getLeft() > rectangleB.getRight())
    {
        return false;
    }

    if(rectangleA.getBottom() < rectangleB.getTop())
    {
        return false;
    }

    if(rectangleA.getTop() > rectangleB.getBottom())
    {
        return false;
    }

    return true;
}

template <typename T>
bool iIntersection::intersects(const iaCircle<T> &circle, const iaRectangle<T> &rectangle)
{
    const T right = rectangle._x + rectangle._width;
    const T bottom = rectangle._y + rectangle._height;
    const T closestX = (circle._center._x < rectangle._x ? rectangle._x : (circle._center._x > right ? right : circle._center._x));
    const T closestY = (circle._center._y < rectangle._y ? rectangle._y : (circle._center._y > bottom ? bottom : circle._center._y));
    const T dx = closestX - circle._center._x;
    const T dy = closestY - circle._center._y;

    return (dx * dx + dy * dy) <= circle._radius * circle._radius;
}

template <typename T>
bool iIntersection::contains(const iaRectangle<T> &rectangle, const iaCircle<T> &circle)
{
    if(circle._center._x - circle._radius < rectangle._x)
    {
        return false;
    }

    if(circle._center._y - circle._radius < rectangle._y)
    {
        return false;
    }

    if(circle._center._x + circle._radius > rectangle.getRight())
    {
        return false;
    }

    if(circle._center._y + circle._radius > rectangle.getBottom())
    {
        return false;
    }

    return true;
}

template <typename T>
bool iIntersection::intersects(const iaCircle<T> &circleA, const iaCircle<T> &circleB)
{
    const T diffSq = circleA._center.distance2(circleB._center);
    return diffSq <= circleA._radius * circleA._radius + circleB._radius * circleB._radius;
}

template <typename T>
bool iIntersection::intersects(const iaVector2<T> &point, const iaCircle<T> &circle)
{
    const T diffSq = point.distance2(circle._center);
    return diffSq <= circle._radius * circle._radius;
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
bool iIntersection::intersects(const iPlane<T> &plane, const iRay<T> &ray, iaVector3<T> &intersection)
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
