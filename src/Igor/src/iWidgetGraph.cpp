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
        : iWidget(iWidgetType::Graph)
    {
        _configuredWidth = 100;
        _configuredHeight = 50;
        _reactOnMouseWheel = false;

        setHorizontalAlignment(iHorizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);
    }

    iWidgetGraph::~iWidgetGraph()
    {
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

            _dirty = false;
        }
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

            float32 scaleX = getActualWidth() / boundings._width;
            float32 scaleY = getActualHeight() / boundings._height;

            for (auto graph : _graphs)
            {
                if (graph.second._points.size() > 0)
                {
                    vector<iaVector2f> points;
                    iaVector2f currentPoint;

                    if (_extendData &&
                        graph.second._points[0]._x > boundings._x)
                    {
                        currentPoint._x = boundings._x;
                        currentPoint._y = graph.second._points[0]._y;
                        currentPoint._x = ((currentPoint._x - boundings._x) * scaleX);
                        currentPoint._y = boundings._height - ((currentPoint._y - boundings._y) * scaleY) + getActualHeight();
                        points.push_back(currentPoint);
                    }

                    for (auto point : graph.second._points)
                    {
                        currentPoint._x = ((point._x - boundings._x) * scaleX);
                        currentPoint._y = boundings._height - ((point._y - boundings._y) * scaleY) + getActualHeight();
                        points.push_back(currentPoint);
                    }

                    if (_extendData &&
                        graph.second._points[graph.second._points.size() - 1]._x < boundings._x + boundings._width)
                    {
                        currentPoint._x = boundings._x + boundings._width;
                        currentPoint._y = graph.second._points[graph.second._points.size() - 1]._y;
                        currentPoint._x = ((currentPoint._x - boundings._x) * scaleX);
                        currentPoint._y = boundings._height - ((currentPoint._y - boundings._y) * scaleY) + getActualHeight();
                        points.push_back(currentPoint);
                    }

                    iWidgetManager::getInstance().getTheme()->drawGraph(getActualPosX(), getActualPosY(), graph.second._lineColor, graph.second._pointColor, graph.second._lineWidth, graph.second._pointSize, points);
                }
            }

        }
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

    void iWidgetGraph::setExtendData(bool wrapData)
    {
        _extendData = wrapData;
    }

    bool iWidgetGraph::getExtendData()
    {
        return _extendData;
    }

}
