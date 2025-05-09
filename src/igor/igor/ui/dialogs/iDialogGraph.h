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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_DIALOGGRAPH__
#define __IGOR_DIALOGGRAPH__

#include <igor/ui/dialogs/iDialog.h>

#include <map>

namespace igor
{

    class iWidgetGraph;
    class iWidgetLabel;
    class iWidgetNumberChooser;

    /*! the graph edit dialog
    */
    class IGOR_API iDialogGraph : public iDialog
    {

    public:
        /*! init members

		\param parent the parent of this widget
		*/
        iDialogGraph(const iWidgetPtr parent = nullptr);

        /*! does nothing
        */
        ~iDialogGraph() = default;

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

        /*! show/open the decision box

        call configure before open

        \param dialogCloseDelegate the closing delegate
        \param color the color to start with
        \param useAlpha if true also use the alpha channel
        */
        void open(iDialogCloseDelegate dialogCloseDelegate, const std::vector<std::vector<iaVector2f>> &graphs);

        /*! set name of axis

        \param index axis index
        \param name name of axis
        */
        void setAxisName(uint32 index, const iaString &name);

        /*! defines how many after points will be displayed

        \param afterPoint amount of digits after point
        */
        void setAfterPoint(int32 afterPoint);

        /*! \returns amount of after point
        */
        int32 getAfterPoint() const;

        /*! \returns current configured graphs
        */
        const std::vector<std::vector<iaVector2f>> &getGraphs() const;

    private:
        /*! after point digit amount
        */
        int32 _afterPoint = 2;

        /*! axis names
        */
        std::map<uint32, iaString> _axisNames;

        /*! axis number chooser
        */
        std::map<uint32, iWidgetNumberChooser *> _axisNumberChooser;

        /*! copy of initially set graphs
        */
        std::vector<std::vector<iaVector2f>> _oldGraphs;

        /*! current graphs
        */
        std::vector<std::vector<iaVector2f>> _graphs;

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
        iWidgetGraph *_graph = nullptr;

        /*! all widgets
        */
        std::vector<iWidgetPtr> _allWidgets;

        /*! all data grid entry widgets
        */
        std::vector<iWidgetPtr> _dataGridEntryWidgets;

        /*! handles ok button clicked event

        \param source the ok button it self
        */
        void onOK(const iWidgetPtr source);

        /*! triggered when value is to delete

        \param source the source widget
        */
        void onDelete(const iWidgetPtr source);

        /*! handles cancel button clicked event

        \param source the cancel button it self
        */
        void onCancel(const iWidgetPtr source);

        /*! handles reset button click event

        \param source the reset button it self
        */
        void onReset(const iWidgetPtr source);

        /*! triggered by all the number choosers

        \param source the source widget
        */
        void onValueChanged(const iWidgetPtr source);

        /*! updates gui by selected value
        */
        void updateSelection();

        /*! updates the graph
        */
        void updateGraph();

        /*! triggered when graph selection changed

        \param source the source widget
        */
        void onSelectionChanged(const iWidgetPtr source);

        /*! triggered when graph content changed

        \param source the source widget
        */
        void onGraphChanged(const iWidgetPtr source);

        /*! initializes gui elements
        */
        void initGUI();
    };

    /*! dialog graph pointer definition
    */
    typedef iDialogGraph *iDialogGraphPtr;

} // namespace igor

#endif // __IGOR_DIALOGGRAPH__