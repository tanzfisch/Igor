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
// (c) Copyright 2012-2018 by Martin Loga
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

#include <map>
using namespace std;

namespace Igor
{

    class iWidgetGrid;
    class iWidgetButton;
    class iWidgetGraph;
    class iWidgetLabel;

    /*! event triggered when color chooser was closed
    */
    iaEVENT(iDialogGraphCloseEvent, iDialogGraphCloseDelegate, void, (bool ok, const vector<vector<iaVector2f>>& graphs), (ok, graphs));

    /*! the graph edit dialog
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

        /*! defines how many after points will be displayed

        \param afterPoint amount of digits after point
        */
        void setAfterPoint(int32 afterPoint);

        /*! \returns amount of after point
        */
        int32 getAfterPoint() const;

    private:

        /*! after point digit amount
        */
        int32 _afterPoint = 2;

        /*! dialog title
        */
        iaString _title;

        /*! axis names
        */
        map<uint32, iaString> _axisNames;

        /*! axis number chooser
        */
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
        float32 _xMin = 0.0f;

        /*! max value on x axis
        */
        float32 _xMax = 1.0f;

        /*! up down stepping of number choosers for x axis values
        */
        float32 _xStepping = 0.1f;

        /*! min value on y axis
        */
        float32 _yMin = 0.0f;

        /*! max value on y axis
        */
        float32 _yMax = 1.0f;

        /*! up down stepping of number choosers for y axis values
        */
        float32 _yStepping = 0.1f;

        /*! index of selected value
        */
        int32 _selectedValueIndex = 0;

        /*! widget graph
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

        /*! handles ok button clicked event

        \param source the ok button it self
        */
        void onOK(iWidget* source);

        /*! triggered when value is to delete

        \param source the source widget
        */
        void onDelete(iWidget* source);

        /*! handles cancel button clicked event

        \param source the cancel button it self
        */
        void onCancel(iWidget* source);

        /*! handles reset button click event

        \param source the reset button it self
        */
        void onReset(iWidget* source);

        /*! triggered by all the number choosers

        \param source the source widget
        */
        void onValueChanged(iWidget* source);

        /*! updates gui by selected value
        */
        void updateSelection();

        /*! updates the graph
        */
        void updateGraph();

        /*! triggered when graph selection changed

        \param index the index of selection
        */
        void onSelectionChanged(int32 index);

        /*! triggered when graph content changed

        \param source the source widget
        */
        void onGraphChanged(iWidget* source);

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