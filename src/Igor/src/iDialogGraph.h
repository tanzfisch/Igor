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

#ifndef __iDIALOGGRAPH__
#define __iDIALOGGRAPH__

#include <iDialog.h>

#include <iaEvent.h>
#include <iaString.h>
#include <iaColor4.h>
using namespace IgorAux;

namespace Igor
{

    class iWidgetGrid;
    class iWidgetButton;
    class iWidgetGraph;
    class iWidgetLabel;

    /*! event triggered when color chooser was closed
    */
    iaEVENT(iDialogGraphCloseEvent, iDialogGraphCloseDelegate, void, (bool ok, const vector<vector<iaVector2f>>& graphs), (ok, graphs));

    /*! the color chooser dialog
    */
    class Igor_API iDialogGraph : public iDialog
    {

        friend class iWidgetManager;

    public:

        /*! show/open the decision box

        \param closeDelegate the closing delegate
        \param color the color to start with
        \param useAlpha if true also use the alpha channel
        */
        void show(iDialogGraphCloseDelegate closeDelegate, const vector<vector<iaVector2f>>& graphs);

        /*! configures x axis

        \param xMin minimum value on x axis
        \param xMax maximum value on x axis
        \param xStepping the number chooser stepping for the x axis
        */
        void configureXAxis(float32 xMin, float32 xMax, float32 xStepping);

        /*! configures y axis

        \param yMin minimum value on y axis
        \param yMax maximum value on y axis
        \param yStepping the number chooser stepping for the y axis
        */
        void configureYAxis(float32 yMin, float32 yMax, float32 yStepping);

        /*! sets dialog title

        \param title the new title
        */
        void setTitle(const iaString& title);

        /*! \returns dialog title
        */
        iaString getTitle() const;

        /*! set name of axis

        \param index axis index
        \param name name of axis
        */
        void setAxisName(uint32 index, const iaString& name);

    private:

        /*! dialog title
        */
        iaString _title;

        /*! axis names
        */
        map<uint32, iaString> _axisNames;

        map<uint32, iWidgetNumberChooser*> _axisNumberChooser;

        /*! the close event
        */
        iDialogGraphCloseEvent _closeEvent;

        /*! copy of initialy set graphs
        */
        vector<vector<iaVector2f>> _oldGraphs;

        /*! current graphs
        */
        vector<vector<iaVector2f>> _graphs;

        /*! title label
        */
        iWidgetLabel* _titleLabel = nullptr;

        /*! min value on x axis
        */
        float32 _xMin = 0;

        /*! max value on x axis
        */
        float32 _xMax = 1;

        float32 _xStepping = 0.1;

        /*! min value on y axis
        */
        float32 _yMin = 0;

        /*! max value on y axis
        */
        float32 _yMax = 1;

        float32 _yStepping = 0.1;

        /*! actual graph
        */
        iWidgetGraph* _graph = nullptr;

        /*! root widget
        */
        iWidgetGrid* _grid = nullptr;
        
        /*! all widgets
        */
        vector<iWidget*> _allWidgets;

        /*! all data grid entry widgets
        */
        vector<iWidget*> _dataGridEntryWidgets;

        map<uint32, iaVector2i> _widgetLocationMatrix;

        /*! handles ok button clicked event

        \param source the ok button it self
        */
        void onOK(iWidget* source);

        /*! handles cancel button clicked event

        \param source the cancel button it self
        */
        void onCancel(iWidget* source);

        /*! handles reset button click event

        \param source the reset button it self
        */
        void onReset(iWidget* source);

        void onGraphUpdate(iWidget* source);
        void onFullUpdate(iWidget* source);

        void updateGraph();

        void retriveGraphs(vector<vector<iaVector2f>>& graphs);

        /*! closes the dialog and sends closed event

        will be triggered by any button
        */
        void close();

        /*! deinitializes the gui elements
        */
        void deinitGUI();

        /*! initializes gui elements
        */
        void initGUI();

        /*! does nothing
        */
        iDialogGraph() = default;

        /*! deinitializes gui
        */
        ~iDialogGraph();

        /*! creates instance of this widget type
        */
        static iDialog* createInstance();

    };

}

#endif