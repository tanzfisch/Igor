// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

template <typename T>
void iRenderer::drawPoint(const iaVector2<T> &v, const iaColor4f &color)
{
    drawPointInternal(iaVector3f(static_cast<float32>(v._x), static_cast<float32>(v._y), 0.0), color);
}

template <typename T>
void iRenderer::drawPoint(T x, T y, const iaColor4f &color)
{
    drawPointInternal(iaVector3f(static_cast<float32>(x), static_cast<float32>(y), 0.0), color);
}

template <typename T>
void iRenderer::drawPoint(const iaVector3<T> &v, const iaColor4f &color)
{
    drawPointInternal(iaVector3f(static_cast<float32>(v._x), static_cast<float32>(v._y), static_cast<float32>(v._z)), color);
}

template <typename T>
void iRenderer::drawLine(T x1, T y1, T x2, T y2, const iaColor4f &color)
{
    drawLineInternal(iaVector3f(static_cast<float32>(x1), static_cast<float32>(y1), 0.0),
                     iaVector3f(static_cast<float32>(x2), static_cast<float32>(y2), 0.0), color);
}

template <typename T>
void iRenderer::drawLine(const iaVector2<T> &v1, const iaVector2<T> &v2, const iaColor4f &color)
{
    drawLineInternal(iaVector3f(static_cast<float32>(v1._x), static_cast<float32>(v1._y), 0.0),
                     iaVector3f(static_cast<float32>(v2._x), static_cast<float32>(v2._y), 0.0), color);
}

template <typename T>
void iRenderer::drawLine(const iaVector3<T> &v1, const iaVector3<T> &v2, const iaColor4f &color)
{
    drawLineInternal(iaVector3f(static_cast<float32>(v1._x), static_cast<float32>(v1._y), static_cast<float32>(v1._z)),
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

        drawLineInternal(iaVector3f(static_cast<float32>(start._x), static_cast<float32>(start._y), 0.0f),
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

        drawLineInternal(iaVector3f(static_cast<float32>(start._x), static_cast<float32>(start._y), static_cast<float32>(start._z)),
                         iaVector3f(static_cast<float32>(stop._x), static_cast<float32>(stop._y), static_cast<float32>(stop._z)), color);
    }
}

template <typename T>
void iRenderer::drawLineLoop(const std::vector<iaVector2<T>> &points, const iaColor4f &color)
{
    drawLineStrip(points, color);
    drawLineInternal(points.back(), points.front(), color);
}

template <typename T>
void iRenderer::drawLineLoop(const std::vector<iaVector3<T>> &points, const iaColor4f &color)
{
    drawLineStrip(points, color);
    drawLineInternal(points.back(), points.front(), color);
}

template <typename T>
void iRenderer::drawRectangle(T x, T y, T width, T height, const iaColor4f &color)
{
    const iaVector3f v0(static_cast<float32>(x), static_cast<float32>(y), 0.0);
    const iaVector3f v1(static_cast<float32>(x), static_cast<float32>(y + height), 0.0);
    const iaVector3f v2(static_cast<float32>(x + width), static_cast<float32>(y + height), 0.0);
    const iaVector3f v3(static_cast<float32>(x + width), static_cast<float32>(y), 0.0);

    drawLineInternal(v0, v1, color);
    drawLineInternal(v1, v2, color);
    drawLineInternal(v2, v3, color);
    drawLineInternal(v3, v0, color);
}

template <typename T>
void iRenderer::drawRectangle(const iaRectangle<T> &rect, const iaColor4f &color)
{
    const iaVector3f v0(static_cast<float32>(rect._x), static_cast<float32>(rect._y), 0.0);
    const iaVector3f v1(static_cast<float32>(rect._x), static_cast<float32>(rect._y + rect._height), 0.0);
    const iaVector3f v2(static_cast<float32>(rect._x + rect._width), static_cast<float32>(rect._y + rect._height), 0.0);
    const iaVector3f v3(static_cast<float32>(rect._x + rect._width), static_cast<float32>(rect._y), 0.0);

    drawLineInternal(v0, v1, color);
    drawLineInternal(v1, v2, color);
    drawLineInternal(v2, v3, color);
    drawLineInternal(v3, v0, color);
}

template <typename T>
void iRenderer::drawQuad(const iaVector3<T> &o, const iaVector3<T> &u, const iaVector3<T> &v, const iaColor4f &color)
{
    const iaVector3f of = iaVector3f(static_cast<float32>(o._x), static_cast<float32>(o._y), static_cast<float32>(o._z));
    const iaVector3f uf = iaVector3f(static_cast<float32>(u._x), static_cast<float32>(u._y), static_cast<float32>(u._z));
    const iaVector3f vf = iaVector3f(static_cast<float32>(v._x), static_cast<float32>(v._y), static_cast<float32>(v._z));

    drawQuadInternal(of + vf - uf,
                     of - vf - uf,
                     of - vf + uf,
                     of + vf + uf,
                     color);
}

template <typename T>
void iRenderer::drawQuad(const iaMatrix<T> &matrix, const iaColor4f &color)
{
    const iaVector3<T> v0(-0.5f, -0.5f, 0.0f);
    const iaVector3<T> v1(-0.5f, 0.5f, 0.0f);
    const iaVector3<T> v2(0.5f, 0.5f, 0.0f);
    const iaVector3<T> v3(0.5f, -0.5f, 0.0f);

    iaMatrixf matrixf;
    for (int i = 0; i < 16; ++i)
    {
        matrixf[i] = static_cast<float32>(matrix[i]);
    }

    drawQuadInternal(matrixf * v0,
                     matrixf * v1,
                     matrixf * v2,
                     matrixf * v3,
                     color);
}

template <typename T>
void iRenderer::drawQuad(const iaVector3<T> &v1, const iaVector3<T> &v2, const iaVector3<T> &v3, const iaVector3<T> &v4, const iaColor4f &color)
{
    drawQuadInternal(v1, v2, v3, v4, color);
}

template <typename T>
void iRenderer::drawFilledRectangle(const iaRectangle<T> &rect, const iaColor4f &color)
{
    drawQuadInternal(iaVector3f(static_cast<float32>(rect._x), static_cast<float32>(rect._y), 0.0),
                     iaVector3f(static_cast<float32>(rect._x), static_cast<float32>(rect._y + rect._height), 0.0),
                     iaVector3f(static_cast<float32>(rect._x + rect._width), static_cast<float32>(rect._y + rect._height), 0.0),
                     iaVector3f(static_cast<float32>(rect._x + rect._width), static_cast<float32>(rect._y), 0.0),
                     color);
}

template <typename T>
void iRenderer::drawFilledRectangle(T x, T y, T width, T height, const iaColor4f &color)
{
    drawQuadInternal(iaVector3f(static_cast<float32>(x), static_cast<float32>(y), 0.0),
                     iaVector3f(static_cast<float32>(x), static_cast<float32>(y + height), 0.0),
                     iaVector3f(static_cast<float32>(x + width), static_cast<float32>(y + height), 0.0),
                     iaVector3f(static_cast<float32>(x + width), static_cast<float32>(y), 0.0), color);
}

template <typename T>
void iRenderer::drawTexturedRectangle(T x, T y, T width, T height, const iTexturePtr &texture, const iaColor4f &color, bool blend, const iaVector2<T> &tiling)
{
    drawTexturedQuadInternal(iaVector3f(static_cast<float32>(x), static_cast<float32>(y), 0.0),
                             iaVector3f(static_cast<float32>(x), static_cast<float32>(y + height), 0.0),
                             iaVector3f(static_cast<float32>(x + width), static_cast<float32>(y + height), 0.0),
                             iaVector3f(static_cast<float32>(x + width), static_cast<float32>(y), 0.0),
                             texture, color, blend, iaVector2f(static_cast<float32>(tiling._x), static_cast<float32>(tiling._y)));
}

template <typename T>
void iRenderer::drawTexturedRectangle(const iaRectangle<T> &rect, const iTexturePtr &texture, const iaColor4f &color, bool blend, const iaVector2<T> &tiling)
{
    drawTexturedQuadInternal(iaVector3f(static_cast<float32>(rect._x), static_cast<float32>(rect._y), 0.0),
                             iaVector3f(static_cast<float32>(rect._x), static_cast<float32>(rect._y + rect._height), 0.0),
                             iaVector3f(static_cast<float32>(rect._x + rect._width), static_cast<float32>(rect._y + rect._height), 0.0),
                             iaVector3f(static_cast<float32>(rect._x + rect._width), static_cast<float32>(rect._y), 0.0),
                             texture, color, blend, iaVector2f(static_cast<float32>(tiling._x), static_cast<float32>(tiling._y)));
}

template <typename T>
void iRenderer::drawTexturedQuad(const iaVector3<T> &o, const iaVector3<T> &u, const iaVector3<T> &v, iTexturePtr texture, const iaColor4f &color, bool blend, const iaVector2<T> &tiling)
{
    const iaVector3f of = iaVector3f(static_cast<float32>(o._x), static_cast<float32>(o._y), static_cast<float32>(o._z));
    const iaVector3f uf = iaVector3f(static_cast<float32>(u._x), static_cast<float32>(u._y), static_cast<float32>(u._z));
    const iaVector3f vf = iaVector3f(static_cast<float32>(v._x), static_cast<float32>(v._y), static_cast<float32>(v._z));
    const iaVector2f t(static_cast<float32>(tiling._x), static_cast<float32>(tiling._y));

    drawTexturedQuadInternal(of + vf - uf,
                             of - vf - uf,
                             of - vf + uf,
                             of + vf + uf,
                             texture, color, blend, t);
}

template <typename T>
void iRenderer::drawTexturedQuad(const iaMatrix<T> &matrix, const iTexturePtr &texture, const iaColor4f &color, bool blend, const iaVector2<T> &tiling)
{
    const iaVector3<T> v0(-0.5f, -0.5f, 0.0f);
    const iaVector3<T> v1(-0.5f, 0.5f, 0.0f);
    const iaVector3<T> v2(0.5f, 0.5f, 0.0f);
    const iaVector3<T> v3(0.5f, -0.5f, 0.0f);
    const iaVector2f t(static_cast<float32>(tiling._x), static_cast<float32>(tiling._y));

    drawTexturedQuadInternal(matrix * v0,
                             matrix * v1,
                             matrix * v2,
                             matrix * v3,
                             texture, color, blend, t);
}

template <typename T>
void iRenderer::drawTexturedQuad(const iaVector3<T> &v1, const iaVector3<T> &v2, const iaVector3<T> &v3, const iaVector3<T> &v4, const iTexturePtr &texture, const iaColor4f &color, bool blend, const iaVector2<T> &tiling)
{
    drawTexturedQuadInternal(iaVector3f(static_cast<float32>(v1._x), static_cast<float32>(v1._y), static_cast<float32>(v1._z)),
                             iaVector3f(static_cast<float32>(v2._x), static_cast<float32>(v2._y), static_cast<float32>(v2._z)),
                             iaVector3f(static_cast<float32>(v3._x), static_cast<float32>(v3._y), static_cast<float32>(v3._z)),
                             iaVector3f(static_cast<float32>(v4._x), static_cast<float32>(v4._y), static_cast<float32>(v4._z)),
                             texture, color, blend, iaVector2f(tiling._x, tiling._y));
}

template <typename T>
void iRenderer::drawSprite(const iaMatrix<T> &matrix, const iSpritePtr &atlas, uint32 frameIndex, const iaVector2<T> &size, const iaColor4f &color, bool blend)
{
    iaMatrixf matrixf;
    for (int i = 0; i < 16; ++i)
    {
        matrixf[i] = static_cast<float32>(matrix[i]);
    }
    drawSpriteInternal(matrixf, atlas, frameIndex, iaVector2f(size._x, size._y), color, blend);
}

template <typename T>
void iRenderer::drawCircle(const iaCircle<T> &circle, int segments, const iaColor4f &color)
{
    drawCircle(circle._center._x, circle._center._y, circle._radius, segments, color);
}

template <typename T>
void iRenderer::drawCircle(const iaVector2<T> &pos, T radius, int segments, const iaColor4f &color)
{
    drawCircle(pos._x, pos._y, radius, segments, color);
}

template <typename T>
void iRenderer::drawCircle(T x, T y, T radius, int segments, const iaColor4f &color)
{
    con_assert(segments >= 3, "minimum segments is 3");

    const float32 step = 2 * M_PI / static_cast<float32>(segments);
    float32 angleA = 0;
    float32 angleB = step;

    for (int i = 0; i < segments; ++i)
    {
        drawLineInternal(iaVector3f(x + radius * cosf(angleA), y + radius * sinf(angleA), 0.0f),
                         iaVector3f(x + radius * cosf(angleB), y + radius * sinf(angleB), 0.0f), color);
        angleA += step;
        angleB += step;
    }
}

template <typename T>
void iRenderer::drawFilledCircle(const iaCircle<T> &circle, int segments, const iaColor4f &color)
{
    drawFilledCircleInternal(static_cast<float32>(circle._center._x), static_cast<float32>(circle._center._y), static_cast<float32>(circle._radius), segments, color);
}

template <typename T>
void iRenderer::drawFilledCircle(const iaVector2<T> &pos, T radius, int segments, const iaColor4f &color)
{
    drawFilledCircleInternal(static_cast<float32>(pos._x), static_cast<float32>(pos._y), static_cast<float32>(radius), segments, color);
}

template <typename T>
void iRenderer::drawFilledCircle(T x, T y, T radius, int segments, const iaColor4f &color)
{
    drawFilledCircleInternal(static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(radius), segments, color);
}

template <typename T>
void iRenderer::drawBox(const iAACube<T> &cube, const iaColor4f &color)
{
    drawBoxInternal(iAABoxf(iaVector3f(cube._center._x, cube._center._y, cube._center._z),
                    iaVector3f(cube._halfEdgeLength, cube._halfEdgeLength, cube._halfEdgeLength)),
            color);
}

template <typename T>
void iRenderer::drawBox(const iAABox<T> &box, const iaColor4f &color)
{
    drawBoxInternal(iAABoxf(iaVector3f(box._center._x, box._center._y, box._center._z),
                    iaVector3f(box._halfWidths._x, box._halfWidths._y, box._halfWidths._z)),
            color);
}