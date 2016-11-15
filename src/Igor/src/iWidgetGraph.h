//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2014-2016 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it   
// under the terms of the GNU Lesser General Public License as published by  
// the Free Software Foundation; either version 3 of the License, or (at   
// your option) any later version.                                           
// 
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
// 
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
// 
// contact: martinloga@gmx.de  

#ifndef __iWIDGETGRAPH__
#define __iWIDGETGRAPH__

#include <iWidget.h>

#include <iaString.h>
#include <iaColor4.h>
#include <iaVector2.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{

    class iTexture;

    /*! graph widget. to display graphs
    */
	class Igor_API iWidgetGraph : public iWidget
	{

        /*! needs to be friend because it's the factory that creates this widget
        */
        friend class iWidgetManager;

        /*! graph data
        */
        struct GraphData
        {
            /*! list of points in graph
            */
            vector<iaVector2f> _points;

            /*! the line color
            */
            iaColor4f _lineColor = {1.0, 1.0, 1.0, 1.0};

            /*! the points color
            */
            iaColor4f _pointColor = { 1.0, 1.0, 1.0, 1.0 };

            /*! line width of graph
            */
            float32 _lineWidth = 1.0;

            /*! size of points. if zero no points will be drawn
            */
            float32 _pointSize = 0.0;
        };

	public:

        /*! clears all data
        */
        void clear();

        /*! sets line color for specified graph

        \param id identifier of graph
        \param color color of graph
        */
        void setLineColor(uint64 id, const iaColor4f& color);

        /*! \returns line color of specified graph

        \param name name of graph
        */
        iaColor4f getLineColor(uint64 id);

        /*! sets points color for specified graph

        \param id identifier of graph
        \param color color of graph
        */
        void setPointColor(uint64 id, const iaColor4f& color);

        /*! \returns points color of specified graph

        \param name name of graph
        */
        iaColor4f getPointColor(uint64 id);

        /*! sets line width of specified graph

        \param id graph id
        \param lineWidth the new line width
        */
        void setLineWidth(uint64 id, float32 lineWidth);

        /*! \returns line width of specified graph

        \param id graph id
        */
        float32 getLineWidth(uint64 id);

        /*! sets points size for specified graph

        \param id id of graph
        \param pointSize the point size to set
        */
        void setPointSize(uint64 id, float32 pointSize);

        /*! \returns point size of specified graph

        \param id graph id
        */
        float32 getPointSize(uint64 id);

        /*! sets points of graph

        \param id the graphs id
        \param points list of points for the graph
        */
        void setPoints(uint64 id, vector<iaVector2f> points);

        /*! \returns points of specified graph

        \param id the graphs id
        */
        vector<iaVector2f> getPoints(uint64 id);

        /*! set user specified boudings / range of graph coordinate system

        \param boundings the boundings
        */
        void setBoundings(const iRectanglef& boundings);

        /*! \returns user specified boudings / range of graph
        */
        iRectanglef getBoundings();

        /*! activated user defined boundings / range of graph

        \param useBoundings if true user specified boundings will be used
        */
        void setUseBoundings(bool useBoundings = true);

        /*! \returns if true user specified boundings are in use
        */
        bool isUsingBoundings();

        /*! set data extend flag

        if true and a graph does not reach the bounding it will be extended up to the boundings

        \param extendData if true wrap data will be activated
        */
        void setExtrapolateData(bool extendData = true);

        /*! \returns true if data will be extended
        */
        bool getExtrapolateData();

        /*! sets the grid visibility

        \param viewGrid if true the grid is visible
        */
        void setViewGrid(bool viewGrid = true);

        /*! \returns true if the grid is visible
        */
        bool getViewGrid() const;

        /*! configures the resolution of the grid

        \param x amount of vertical grid lines (2 minimum)
        \param y amount of horizontal grid lines (2 minimum)
        */
        void setGridResolution(uint32 x, uint32 y);

        /*! \returns amount of vertical grid lines
        */
        uint32 getGridXResolution() const;

        /*! \returns amount of horizontal grid lines
        */
        uint32 getGridYResolution() const;

        /*! sets if the background frame is visible or not

        \param viewFrame if true the background frame is visible
        */
        void setViewFrame(bool viewFrame = true);

        /*! \returns true if the background frame is visible
        */
        bool getViewFrame() const;

        /*! sets if labels are visible

        \param viewLabels if true labels are visible
        */
        void setViewLabels(bool viewLabels = true);

        /*! \returns if labels are visible
        */
        bool getViewLabels() const;

        /*! \returns count of graphs
        */
        int32 getGraphCount() const;

        /*! sets the interactive feature

        \param interactive if true graph will be interactive
        */
        void setInteractive(bool interactive = true);

        /*! \returns true if graph is interactive
        */
        bool isInteractive();

	private:

        /*! flag if graph is interactive
        */
        bool _interactive = false;

        /*! dirty flag if graph data was changed
        */
        bool _dirty = true;

        /*! flag if labels are visible
        */
        bool _viewLabels = false;

        /*! calculated data boudings
        */
        iRectanglef _dataBoundings;

        /*! user defined boudings
        */
        iRectanglef _boundings = { 0, 0, 1, 1 };

        /*! if true user specified boundings will be used
        */
        bool _useUserBoudings = false;

        /*! if true graphs that are smaller then the boundings will be extended to bounding size
        */
        bool _extrapolateData = false;

        /*! maps with all graphs
        */
        map<uint64, GraphData> _graphs;

        /*! the grid resolution
        */
        iaVector2i _gridResolution = {5, 3};

        /*! flag if true we render a grid
        */
        bool _viewGrid = false;

        /*! flag if true we render a frame
        */
        bool _viewFrame = false;

        /*! makes some calculations before we can draw
        */
        void prepareDraw();

        /*! updates size based on it's content
        */
        void calcMinSize();

		/*! draws the widget
		*/
		void draw();

        /*! handles incomming mouse key down events

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseKeyDown(iKeyCode key);

        /*! ctor initializes member variables
        */
        iWidgetGraph();

        /*! does nothing
        */
		virtual ~iWidgetGraph() = default;

        /*! creates instance of this widget type
        */
        static iWidget* createInstance();

	};
}

#endif
