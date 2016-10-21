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

    void iWidgetGraph::setColor(uint64 id, const iaColor4f& color)
    {
        _graphs[id]._lineColor = color;
    }

    iaColor4f iWidgetGraph::getColor(uint64 id)
    {
        return _graphs[id]._lineColor;
    }

    void iWidgetGraph::setPoints(uint64 id, vector<iaVector2f> points)
    {
        _graphs[id]._points = points;
    }

    vector<iaVector2f> iWidgetGraph::getPoints(uint64 id)
    {
        return _graphs[id]._points;
    }

	void iWidgetGraph::calcMinSize()
	{
        setMinSize(0, 0);
	}

	void iWidgetGraph::draw()
	{
		if (isVisible())
		{
            // TODO
		}
	}

}
