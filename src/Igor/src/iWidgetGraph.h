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

            /*! line width of graph
            */
            float32 _lineWidth = 1.0;
        };

	public:

        /*! clears all data
        */
        void clear();

        /*! sets color for specified graph

        \param id identifier of graph
        \param color color of graph
        */
        void setColor(uint64 id, const iaColor4f& color);

        /*! \returns color of specified graph

        \param name name of graph
        */
        iaColor4f getColor(uint64 id);

        /*! sets line width of specified graph

        \param id graph id
        \param lineWidth the new line width
        */
        void setLineWidth(uint64 id, float32 lineWidth);

        /*! \returns line width of specified graph

        \param id graph id
        */
        float32 getLineWidth(uint64 id);

        /*! sets points of graph

        \param id the graphs id
        \param points list of points for the graph
        */
        void setPoints(uint64 id, vector<iaVector2f> points);

        /*! \returns points of specified graph

        \param id the graphs id
        */
        vector<iaVector2f> getPoints(uint64 id);

        void setBoundings(const iRectanglef& boundings);

        iRectanglef getBoundings();

        void setUseBoundings(bool useBoundings = true);

        bool isUsingBoundings();

	private:

        /*! dirty flag if graph data was changed
        */
        bool _dirty = true;

        /*! calculated data boudings
        */
        iRectanglef _dataBoundings;

        /*! user defined boudings
        */
        iRectanglef _boundings = { 0, 0, 1, 1 };

        bool useUserBoudings = false;

        /*! maps with all graphs
        */
        map<uint64, GraphData> _graphs;

        /*! makes some calculations before we can draw
        */
        void prepareDraw();

        /*! updates size based on it's content
        */
        void calcMinSize();

		/*! draws the widget
		*/
		void draw();

        /*! ctor initializes member variables
        */
        iWidgetGraph();

        /*! release texture
        */
		virtual ~iWidgetGraph();
	};
}

#endif
