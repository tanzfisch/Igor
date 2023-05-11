// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

template <typename T>
void iRenderer::drawPoint(const iaVector2<T> &v, const iaColor4f &color)
{
    drawPoint(iaVector3f(static_cast<float32>(v._x), static_cast<float32>(v._y), 0.0), color);
}

template <typename T>
void iRenderer::drawPoint(T x, T y, const iaColor4f &color)
{
    drawPoint(iaVector3f(static_cast<float32>(x), static_cast<float32>(y), 0.0), color);
}

template <typename T>
void iRenderer::drawPoint(const iaVector3<T> &v, const iaColor4f &color)
{
    drawPoint(iaVector3f(static_cast<float32>(v._x), static_cast<float32>(v._y), static_cast<float32>(v._z)), color);
}

template <typename T>
void iRenderer::drawLine(T x1, T y1, T x2, T y2, const iaColor4f &color)
{
    drawLine(iaVector3f(static_cast<float32>(x1), static_cast<float32>(y1), 0.0),
             iaVector3f(static_cast<float32>(x2), static_cast<float32>(y2), 0.0), color);
}

template <typename T>
void iRenderer::drawLine(const iaVector2<T> &v1, const iaVector2<T> &v2, const iaColor4f &color)
{
    drawLine(iaVector3f(static_cast<float32>(v1._x), static_cast<float32>(v1._y), 0.0),
             iaVector3f(static_cast<float32>(v2._x), static_cast<float32>(v2._y), 0.0), color);
}

template <typename T>
void iRenderer::drawLine(const iaVector3<T> &v1, const iaVector3<T> &v2, const iaColor4f &color)
{
    drawLine(iaVector3f(static_cast<float32>(v1._x), static_cast<float32>(v1._y), static_cast<float32>(v1._z)),
             iaVector3f(static_cast<float32>(v2._x), static_cast<float32>(v2._y), static_cast<float32>(v2._z)), color);
}

template <typename T>
void iRenderer::drawLineStrip(const std::vector<iaVector2<T>> &points, const iaColor4f &color)
{
    con_assert(points.size() > 1, "too few points");

    for (int i = 1; i < points.size(); ++i)
    {
        const auto &start = points[i - 1];
        const auto &stop = points[i];

        drawLine(iaVector3f(static_cast<float32>(start._x), static_cast<float32>(start._y), 0.0f),
                 iaVector3f(static_cast<float32>(stop._x), static_cast<float32>(stop._y), 0.0f), color);
    }
}

template <typename T>
void iRenderer::drawLineStrip(const std::vector<iaVector3<T>> &points, const iaColor4f &color)
{
    con_assert(points.size() > 1, "too few points");

    for (int i = 1; i < points.size(); ++i)
    {
        const auto &start = points[i - 1];
        const auto &stop = points[i];

        drawLine(iaVector3f(static_cast<float32>(start._x), static_cast<float32>(start._y), static_cast<float32>(start._z)),
                 iaVector3f(static_cast<float32>(stop._x), static_cast<float32>(stop._y), static_cast<float32>(stop._z)), color);
    }
}

template <typename T>
void iRenderer::drawLineLoop(const std::vector<iaVector2<T>> &points, const iaColor4f &color)
{
    con_assert(points.size() > 1, "too few points");

    drawLineStrip(points, color);
    drawLine(points.back(), points.front(), color);
}

template <typename T>
void iRenderer::drawLineLoop(const std::vector<iaVector3<T>> &points, const iaColor4f &color)
{
    con_assert(points.size() > 1, "too few points");

    drawLineStrip(points, color);
    drawLine(points.back(), points.front(), color);
}

template <typename T>
void iRenderer::drawRectangle(T x, T y, T width, T height, const iaColor4f &color)
{
    const iaVector3f v0(static_cast<float32>(x), static_cast<float32>(y), 0.0);
    const iaVector3f v1(static_cast<float32>(x), static_cast<float32>(y + height), 0.0);
    const iaVector3f v2(static_cast<float32>(x + width), static_cast<float32>(y + height), 0.0);
    const iaVector3f v3(static_cast<float32>(x + width), static_cast<float32>(y), 0.0);

    drawLine(v0, v1, color);
    drawLine(v1, v2, color);
    drawLine(v2, v3, color);
    drawLine(v3, v0, color);
}

template <typename T>
void iRenderer::drawRectangle(const iaRectangle<T> &rect, const iaColor4f &color)
{
    const iaVector3f v0(static_cast<float32>(rect._x), static_cast<float32>(rect._y), 0.0);
    const iaVector3f v1(static_cast<float32>(rect._x), static_cast<float32>(rect._y + rect._height), 0.0);
    const iaVector3f v2(static_cast<float32>(rect._x + rect._width), static_cast<float32>(rect._y + rect._height), 0.0);
    const iaVector3f v3(static_cast<float32>(rect._x + rect._width), static_cast<float32>(rect._y), 0.0);

    drawLine(v0, v1, color);
    drawLine(v1, v2, color);
    drawLine(v2, v3, color);
    drawLine(v3, v0, color);
}

template <typename T>
void iRenderer::drawFilledRectangle(const iaRectangle<T> &rect, const iaColor4f &color)
{
    drawFilledRectangle(static_cast<float32>(rect._x), static_cast<float32>(rect._y), static_cast<float32>(rect._width), static_cast<float32>(rect._height), color);
}

template <typename T>
void iRenderer::drawFilledRectangle(T x, T y, T width, T height, const iaColor4f &color)
{
    drawFilledRectangle(static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(width), static_cast<float32>(height), color);
}

template <typename T>
void iRenderer::drawTexturedRectangle(T x, T y, T width, T height, const iTexturePtr &texture, const iaColor4f &color, bool blend, const iaVector2<T> &tiling)
{
    drawTexturedQuad(iaVector3f(static_cast<float32>(x), static_cast<float32>(y), 0.0),
                     iaVector3f(static_cast<float32>(x), static_cast<float32>(y + height), 0.0),
                     iaVector3f(static_cast<float32>(x + width), static_cast<float32>(y + height), 0.0),
                     iaVector3f(static_cast<float32>(x + width), static_cast<float32>(y), 0.0),
                     texture, color, blend, iaVector2f(static_cast<float32>(tiling._x), static_cast<float32>(tiling._y)));
}

template <typename T>
void iRenderer::drawTexturedRectangle(const iaRectangle<T> &rect, const iTexturePtr &texture, const iaColor4f &color, bool blend, const iaVector2<T> &tiling)
{
    drawTexturedQuad(iaVector3f(static_cast<float32>(rect._x), static_cast<float32>(rect._y), 0.0),
                     iaVector3f(static_cast<float32>(rect._x), static_cast<float32>(rect._y + rect._height), 0.0),
                     iaVector3f(static_cast<float32>(rect._x + rect._width), static_cast<float32>(rect._y + rect._height), 0.0),
                     iaVector3f(static_cast<float32>(rect._x + rect._width), static_cast<float32>(rect._y), 0.0),
                     texture, color, blend, iaVector2f(static_cast<float32>(tiling._x), static_cast<float32>(tiling._y)));
}