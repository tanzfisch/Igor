// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetGraph.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>

#include <igor/data/iIntersection.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iWidgetGraph::iWidgetGraph(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetGraph, iWidgetKind::Widget, parent)
    {
        _configuredMinWidth = 100;
        _configuredMinHeight = 40;
        _reactOnMouseWheel = false;

        setHorizontalAlignment(iHorizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);
    }

    void iWidgetGraph::setInteractive(bool interactive)
    {
        _interactive = interactive;
    }

    bool iWidgetGraph::isInteractive()
    {
        return _interactive;
    }

    void iWidgetGraph::clearPoints()
    {
        for (auto &graph : _graphs)
        {
            graph.second._points.clear();
        }
        _dirty = true;
    }

    void iWidgetGraph::setLineColor(uint64 id, const iaColor4f &color)
    {
        _graphs[id]._lineColor = color;
    }

    iaColor4f iWidgetGraph::getLineColor(uint64 id)
    {
        return _graphs[id]._lineColor;
    }

    void iWidgetGraph::setPointColor(uint64 id, const iaColor4f &color)
    {
        _graphs[id]._pointColor = color;
    }

    iaColor4f iWidgetGraph::getPointColor(uint64 id)
    {
        return _graphs[id]._pointColor;
    }

    void iWidgetGraph::setLineWidth(uint64 id, float32 lineWidth)
    {
        _graphs[id]._lineWidth = lineWidth;
    }

    float32 iWidgetGraph::getLineWidth(uint64 id)
    {
        return _graphs[id]._lineWidth;
    }

    void iWidgetGraph::setPointSize(uint64 id, float32 pointSize)
    {
        _graphs[id]._pointSize = pointSize;
    }

    float32 iWidgetGraph::getPointSize(uint64 id)
    {
        return _graphs[id]._pointSize;
    }

    void iWidgetGraph::setPoints(uint64 id, std::vector<iaVector2f> points)
    {
        _graphs[id]._points = points;
        _dirty = true;
    }

    std::vector<iaVector2f> iWidgetGraph::getPoints(uint64 id)
    {
        return _graphs[id]._points;
    }

    void iWidgetGraph::calcMinSize()
    {
        updateMinSize(0, 0);
    }

    void iWidgetGraph::prepareDraw()
    {
        if (_dirty)
        {
            float32 minX = 999999;
            float32 maxX = -999999;
            float32 minY = 999999;
            float32 maxY = -999999;

            for (auto graph : _graphs)
            {
                for (auto point : graph.second._points)
                {
                    if (point._x < minX)
                    {
                        minX = point._x;
                    }

                    if (point._x > maxX)
                    {
                        maxX = point._x;
                    }

                    if (point._y < minY)
                    {
                        minY = point._y;
                    }

                    if (point._y > maxY)
                    {
                        maxY = point._y;
                    }
                }
            }

            _dataBoundings.setX(minX);
            _dataBoundings.setY(minY);
            _dataBoundings.setWidth(maxX - minX);
            _dataBoundings.setHeight(maxY - minY);

            if (_dataBoundings.getWidth() <= 0)
            {
                _dataBoundings.setWidth(1.0);
            }

            if (_dataBoundings.getHeight() <= 0)
            {
                _dataBoundings.setHeight(1.0);
            }

            _dirty = false;
        }
    }

    void iWidgetGraph::setViewLabels(bool viewLabels)
    {
        _viewLabels = viewLabels;
    }

    bool iWidgetGraph::getViewLabels() const
    {
        return _viewLabels;
    }

    void iWidgetGraph::draw()
    {
        if (!isVisible())
        {
            return;
        }

        prepareDraw();

        iaRectanglef boundings;
        calcBoundings(boundings);

        iaRectanglef graphRenderArea;
        calcRenderArea(graphRenderArea);

        iWidgetManager::getInstance().getTheme()->drawGraphFrame(getActualRect(), getState(), isEnabled());

        if (_viewGrid)
        {
            float32 stepX = graphRenderArea._width / (_gridResolution._x - 1);
            float32 stepY = graphRenderArea._height / (_gridResolution._y - 1);

            float32 stepXData = boundings._width / (_gridResolution._x - 1);
            float32 stepYData = boundings._height / (_gridResolution._y - 1);

            std::vector<iaVector2f> verticalLines;
            for (int i = 0; i < _gridResolution._x; ++i)
            {
                verticalLines.push_back(iaVector2f(static_cast<float32>(i) * stepX, static_cast<float32>(i) * stepXData + boundings._x));
            }

            std::vector<iaVector2f> horizontalLines;
            for (int i = 0; i < _gridResolution._y; ++i)
            {
                horizontalLines.push_back(iaVector2f(static_cast<float32>(i) * stepY, (boundings._y + boundings._height) - static_cast<float32>(i) * stepYData));
            }

            iWidgetManager::getInstance().getTheme()->drawGraphGridlines(iaRectanglef(static_cast<int32>(graphRenderArea._x), static_cast<int32>(graphRenderArea._y),
                                                                                      static_cast<int32>(graphRenderArea._width), static_cast<int32>(graphRenderArea._height)),
                                                                         1.0, verticalLines, horizontalLines, isEnabled());

            if (_viewLabels)
            {
                iWidgetManager::getInstance().getTheme()->drawGraphLabels(iaRectanglef(static_cast<int32>(graphRenderArea._x), static_cast<int32>(graphRenderArea._y),
                                                                                       static_cast<int32>(graphRenderArea._width), static_cast<int32>(graphRenderArea._height)),
                                                                          verticalLines, horizontalLines, isEnabled());
            }
        }

        float32 scaleX = graphRenderArea._width / boundings._width;
        float32 scaleY = graphRenderArea._height / boundings._height;

        for (auto graph : _graphs)
        {
            if (graph.second._points.size() > 0)
            {
                std::vector<iaVector2f> points;
                iaVector2f currentPoint;

                if (_extrapolateData &&
                    graph.second._points[0]._x > boundings._x)
                {
                    currentPoint._x = boundings._x;
                    currentPoint._y = graph.second._points[0]._y;
                    currentPoint._x = ((currentPoint._x - boundings._x) * scaleX);
                    currentPoint._y = graphRenderArea._height - ((currentPoint._y - boundings._y) * scaleY);
                    points.push_back(currentPoint);
                }

                for (auto point : graph.second._points)
                {
                    currentPoint._x = ((point._x - boundings._x) * scaleX);
                    currentPoint._y = graphRenderArea._height - ((point._y - boundings._y) * scaleY);
                    points.push_back(currentPoint);
                }

                if (_extrapolateData &&
                    graph.second._points[graph.second._points.size() - 1]._x < boundings._x + boundings._width)
                {
                    currentPoint._x = boundings._x + boundings._width;
                    currentPoint._y = graph.second._points[graph.second._points.size() - 1]._y;
                    currentPoint._x = ((currentPoint._x - boundings._x) * scaleX);
                    currentPoint._y = graphRenderArea._height - ((currentPoint._y - boundings._y) * scaleY);
                    points.push_back(currentPoint);
                }

                iWidgetManager::getInstance().getTheme()->drawGraph(iaRectanglef(static_cast<int32>(graphRenderArea._x), static_cast<int32>(graphRenderArea._y)),
                                                                    graph.second._lineColor, graph.second._pointColor, graph.second._lineWidth, graph.second._pointSize, points);
            }
        }

        if (_interactive)
        {
            std::vector<iaVector2f> points = _graphs[0]._points;

            iaRectanglef buttonRect;
            buttonRect._height = _buttonHeight;
            buttonRect._width = 9;
            buttonRect._y = static_cast<int32>(graphRenderArea._height + graphRenderArea._y + 1.0f);

            for (auto point : points)
            {
                buttonRect._x = static_cast<int32>((((point._x - boundings._x) / boundings._width) * graphRenderArea._width) + graphRenderArea._x - 4.0f);
                iWidgetManager::getInstance().getTheme()->drawButton(buttonRect, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, nullptr, iWidgetState::Standby, isEnabled());
            }
        }
    }

    void iWidgetGraph::calcBoundings(iaRectanglef &boundings)
    {
        if (_useUserBoudings)
        {
            boundings = _boundings;
        }
        else
        {
            boundings = _dataBoundings;
        }

        if (boundings._width < _dataBoundings._width)
        {
            boundings._width = _dataBoundings._width;
        }

        if (boundings._height < _dataBoundings._height)
        {
            boundings._height = _dataBoundings._height;
        }
    }

    void iWidgetGraph::calcRenderArea(iaRectanglef &graphRenderArea)
    {
        graphRenderArea._x = static_cast<float32>(getActualPosX());
        graphRenderArea._y = static_cast<float32>(getActualPosY());
        graphRenderArea._width = static_cast<float32>(getActualWidth());
        graphRenderArea._height = static_cast<float32>(getActualHeight());

        graphRenderArea._x += 4;
        graphRenderArea._y += 4;
        graphRenderArea._width -= 8;
        graphRenderArea._height -= 8;

        if (_interactive)
        {
            graphRenderArea._x += 5;
            graphRenderArea._width -= 10;
            graphRenderArea._height -= (_buttonHeight + 2);
        }
    }

    bool iWidgetGraph::onMouseKeyDown(iEventMouseKeyDown &event)
    {
        iaVector2f mousePos(static_cast<float32>(getLastMousePos()._x), static_cast<float32>(getLastMousePos()._y));

        if (_interactive)
        {
            iaRectanglef graphRenderArea;
            calcRenderArea(graphRenderArea);

            iaRectanglef boundings;
            calcBoundings(boundings);

            std::vector<iaVector2f> points = _graphs[0]._points;

            iaRectanglef buttonRect;
            buttonRect._height = static_cast<float32>(_buttonHeight);
            buttonRect._width = 9;
            buttonRect._y = graphRenderArea._height + graphRenderArea._y + 1;

            int index = 0;
            for (auto point : points)
            {
                buttonRect._x = (((point._x - boundings._x) / boundings._width) * graphRenderArea._width) + graphRenderArea._x - 4;

                if (iIntersection::intersects(mousePos, buttonRect))
                {
                    _selectionChanged(index);
                    _change(this);
                }

                index++;
            }

            if (iIntersection::intersects(mousePos, graphRenderArea))
            {
                float32 value = ((mousePos._x - graphRenderArea._x) / graphRenderArea._width) * boundings._width;

                for (auto &graph : _graphs)
                {
                    if (graph.second._points.size() == 1)
                    {
                        auto iter = graph.second._points.begin();
                        if ((*iter)._x > value)
                        {
                            graph.second._points.insert(iter, iaVector2f(value, (*iter)._y));
                        }
                        else
                        {
                            graph.second._points.push_back(iaVector2f(value, (*iter)._y));
                        }
                    }
                    else
                    {
                        auto iter = graph.second._points.begin();
                        while (iter != graph.second._points.end())
                        {
                            if ((*iter)._x > value)
                            {
                                auto lastIter = iter - 1;
                                graph.second._points.insert(iter, iaVector2f(value, ((*lastIter)._y + (*iter)._y) * 0.5f));
                                break;
                            }

                            iter++;
                        }
                    }
                }

                _change(this);
            }
        }

        return iWidget::onMouseKeyDown(event);
    }

    int32 iWidgetGraph::getGraphCount() const
    {
        return static_cast<int32>(_graphs.size());
    }

    void iWidgetGraph::setGridResolution(uint32 x, uint32 y)
    {
        con_assert(x >= 2 && y >= 2, "invalid parameters");

        if (x >= 2 && y >= 2)
        {
            _gridResolution.set(static_cast<int32>(x), static_cast<int32>(y));
        }
        else
        {
            con_err("invalid parameters");
        }
    }

    uint32 iWidgetGraph::getGridXResolution() const
    {
        return static_cast<uint32>(_gridResolution._x);
    }

    uint32 iWidgetGraph::getGridYResolution() const
    {
        return static_cast<uint32>(_gridResolution._y);
    }

    void iWidgetGraph::setViewGrid(bool viewGrid)
    {
        _viewGrid = viewGrid;
    }

    bool iWidgetGraph::getViewGrid() const
    {
        return _viewGrid;
    }

    void iWidgetGraph::setBoundings(const iaRectanglef &boundings)
    {
        _boundings = boundings;
    }

    iaRectanglef iWidgetGraph::getBoundings()
    {
        return _boundings;
    }

    void iWidgetGraph::setUseBoundings(bool useBoundings)
    {
        _useUserBoudings = useBoundings;
    }

    bool iWidgetGraph::isUsingBoundings()
    {
        return _useUserBoudings;
    }

    void iWidgetGraph::setExtrapolateData(bool wrapData)
    {
        _extrapolateData = wrapData;
    }

    bool iWidgetGraph::getExtrapolateData()
    {
        return _extrapolateData;
    }

} // namespace igor
