// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

template< typename T>
bool iIntersection::isIntersecting(iRectangle<T> rectangle, iaVector2<T> point)
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