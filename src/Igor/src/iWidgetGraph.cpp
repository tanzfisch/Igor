// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetGraph.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>
#include <iTextureResourceFactory.h>
#include <iRenderer.h>
#include <iMaterialResourceFactory.h>

namespace Igor
{

    iWidgetGraph::iWidgetGraph()
    {
        _configuredWidth = 100;
        _configuredHeight = 40;
        _reactOnMouseWheel = false;

        setHorizontalAlignment(iHorizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);
    }

    iWidget* iWidgetGraph::createInstance()
    {
        return new iWidgetGraph();
    }

    void iWidgetGraph::clear()
    {
        _graphs.clear();
    }

    void iWidgetGraph::setLineColor(uint64 id, const iaColor4f& color)
    {
        _graphs[id]._lineColor = color;
    }

    iaColor4f iWidgetGraph::getLineColor(uint64 id)
    {
        return _graphs[id]._lineColor;
    }

    void iWidgetGraph::setPointColor(uint64 id, const iaColor4f& color)
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

    void iWidgetGraph::setPoints(uint64 id, vector<iaVector2f> points)
    {
        _graphs[id]._points = points;
        _dirty = true;
    }

    vector<iaVector2f> iWidgetGraph::getPoints(uint64 id)
    {
        return _graphs[id]._points;
    }

    void iWidgetGraph::calcMinSize()
    {
        setMinSize(0, 0);
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
        if (isVisible())
        {
            prepareDraw();

            iRectanglef boundings;

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

            iRectanglef graphRenderArea;
            graphRenderArea._x = static_cast<float32>(getActualPosX());
            graphRenderArea._y = static_cast<float32>(getActualPosY());
            graphRenderArea._width = static_cast<float32>(getActualWidth());
            graphRenderArea._height = static_cast<float32>(getActualHeight());

            if (_viewFrame)
            {
                graphRenderArea._x += 4;
                graphRenderArea._y += 4;
                graphRenderArea._width -= 8;
                graphRenderArea._height -= 8;
                iWidgetManager::getInstance().getTheme()->drawBackgroundFrame(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), getAppearanceState(), isActive());
            }
            
            if (_viewGrid)
            {
                float32 stepX = graphRenderArea._width / (_gridResolution._x - 1);
                float32 stepY = graphRenderArea._height / (_gridResolution._y - 1);

                vector<float32> verticalLines;
                for (int i = 0; i < _gridResolution._x; ++i)
                {
                    verticalLines.push_back(static_cast<float32>(i) * stepX);
                }

                vector<float32> horizontalLines;
                for (int i = 0; i < _gridResolution._y; ++i)
                {
                    horizontalLines.push_back(static_cast<float32>(i) * stepY);
                }

                iWidgetManager::getInstance().getTheme()->drawGridlines(static_cast<int32>(graphRenderArea._x), static_cast<int32>(graphRenderArea._y), 
                    static_cast<int32>(graphRenderArea._width), static_cast<int32>(graphRenderArea._height), 1.0, verticalLines, horizontalLines, isActive());
            }

            float32 scaleX = graphRenderArea._width / boundings._width;
            float32 scaleY = graphRenderArea._height / boundings._height;

            for (auto graph : _graphs)
            {
                if (graph.second._points.size() > 0)
                {
                    vector<iaVector2f> points;
                    iaVector2f currentPoint;

                    if (_extrapolateData &&
                        graph.second._points[0]._x > boundings._x)
                    {
                        currentPoint._x = boundings._x;
                        currentPoint._y = graph.second._points[0]._y;
                        currentPoint._x = ((currentPoint._x - boundings._x) * scaleX);
                        currentPoint._y = boundings._height - ((currentPoint._y - boundings._y) * scaleY) + graphRenderArea._height;
                        points.push_back(currentPoint);
                    }

                    for (auto point : graph.second._points)
                    {
                        currentPoint._x = ((point._x - boundings._x) * scaleX);
                        currentPoint._y = boundings._height - ((point._y - boundings._y) * scaleY) + graphRenderArea._height;
                        points.push_back(currentPoint);
                    }

                    if (_extrapolateData &&
                        graph.second._points[graph.second._points.size() - 1]._x < boundings._x + boundings._width)
                    {
                        currentPoint._x = boundings._x + boundings._width;
                        currentPoint._y = graph.second._points[graph.second._points.size() - 1]._y;
                        currentPoint._x = ((currentPoint._x - boundings._x) * scaleX);
                        currentPoint._y = boundings._height - ((currentPoint._y - boundings._y) * scaleY) + graphRenderArea._height;
                        points.push_back(currentPoint);
                    }

                    iWidgetManager::getInstance().getTheme()->drawGraph(static_cast<int32>(graphRenderArea._x), static_cast<int32>(graphRenderArea._y), 
                        graph.second._lineColor, graph.second._pointColor, graph.second._lineWidth, graph.second._pointSize, points);
                }
            }
        }
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

    void iWidgetGraph::setViewFrame(bool viewFrame)
    {
        _viewFrame = viewFrame;
    }

    bool iWidgetGraph::getViewFrame() const
    {
        return _viewFrame;
    }

    void iWidgetGraph::setBoundings(const iRectanglef& boundings)
    {
        _boundings = boundings;
    }

    iRectanglef iWidgetGraph::getBoundings()
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

}
