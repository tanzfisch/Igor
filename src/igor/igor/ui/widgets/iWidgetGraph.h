//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_WIDGETGRAPH__
#define __IGOR_WIDGETGRAPH__

#include <igor/ui/widgets/iWidget.h>

#include <iaux/data/iaString.h>
#include <iaux/data/iaColor4.h>
#include <iaux/math/iaVector2.h>
using namespace iaux;

#include <vector>
#include <map>

namespace igor
{

    class iTextureOld;

    /*! graph widget. to display graphs

    \todo highlight selection
    */
    class IGOR_API iWidgetGraph : public iWidget
    {

        /*! graph data
        */
        struct GraphData
        {
            /*! list of points in graph
            */
            std::vector<iaVector2f> _points;

            /*! the line color
            */
            iaColor4f _lineColor = {1.0, 1.0, 1.0, 1.0};

            /*! the points color
            */
            iaColor4f _pointColor = {1.0, 1.0, 1.0, 1.0};

            /*! line width of graph
            */
            float32 _lineWidth = 1.0;

            /*! size of points. if zero no points will be drawn
            */
            float32 _pointSize = 0.0;
        };

    public:
        /*! ctor initializes member variables

		\param parent optional parent
		*/
        iWidgetGraph(const iWidgetPtr parent = nullptr);

        /*! does nothing
		*/
        virtual ~iWidgetGraph() = default;

        /*! clears all point data
        */
        void clearPoints();

        /*! sets line color for specified graph

        \param id identifier of graph
        \param color color of graph
        */
        void setLineColor(uint64 id, const iaColor4f &color);

        /*! \returns line color of specified graph

        \param name name of graph
        */
        iaColor4f getLineColor(uint64 id);

        /*! sets points color for specified graph

        \param id identifier of graph
        \param color color of graph
        */
        void setPointColor(uint64 id, const iaColor4f &color);

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
        void setPoints(uint64 id, std::vector<iaVector2f> points);

        /*! \returns points of specified graph

        \param id the graphs id
        */
        std::vector<iaVector2f> getPoints(uint64 id);

        /*! set user specified boudings / range of graph coordinate system

        \param boundings the boundings
        */
        void setBoundings(const iaRectanglef &boundings);

        /*! \returns user specified boudings / range of graph
        */
        iaRectanglef getBoundings();

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

        /*! \returns selected value index
         */
        int32 getSelectedIndex() const;

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

        /*! selected index
         */
        int32 _selectedIndex = -1;

        /*! calculated data boudings
        */
        iaRectanglef _dataBoundings;

        /*! user defined boudings
        */
        iaRectanglef _boundings = {0, 0, 1, 1};

        /*! if true user specified boundings will be used
        */
        bool _useUserBoudings = false;

        /*! if true graphs that are smaller then the boundings will be extended to bounding size
        */
        bool _extrapolateData = false;

        /*! maps with all graphs
        */
        std::map<uint64, GraphData> _graphs;

        /*! the grid resolution
        */
        iaVector2i _gridResolution = {5, 3};

        /*! flag if true we render a grid
        */
        bool _viewGrid = false;

        /*! button height definition
        */
        const int32 _buttonHeight = 20;

        /*! makes some calculations before we can draw
        */
        void prepareDraw();

        /*! updates size based on it's content
        */
        void calcMinSize() override;

        /*! draws the widget
         */
        void draw() override;

        /*! calculates the render area of the graph

        \param[out] graphRenderArea the render area
        */
        void calcRenderArea(iaRectanglef &graphRenderArea);

        /*! calculates data boundings

        \param[out] boundings the calculates data boundings
        */
        void calcBoundings(iaRectanglef &boundings);

        /*! handles incoming mouse key down events

        \param event mouse key down event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseKeyDown(iEventMouseKeyDown &event) override;
    };

    /*! widget graph pointer definition
    */
    typedef iWidgetGraph* iWidgetGraphPtr;

} // namespace igor

#endif // __IGOR_WIDGETGRAPH__
