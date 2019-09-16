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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __iWIDGETGRID__
#define __iWIDGETGRID__

#include <iWidget.h>
#include <vector>


namespace Igor
{

    class iUserControl;

    /*! grid selection mode
    */
    enum class iSelectionMode
    {
        Field,
        Row,
        Collumn,
        NoSelection
    };

    /*!
    \todo cols und rows dynamisch anpassen
    */
    class Igor_API iWidgetGrid : public iWidget
    {
		
        /*! internal helper struct that represents a child widget and it's position
        */
        struct Field
        {
        public:

            /*! pointer to child widget
            */
            uint64 _widgetID = iWidget::INVALID_WIDGET_ID;

            /*! width of child
            */
            int32 _actualWidth = 10;

            /*! height of child
            */
            int32 _actualHeight = 10;

            /*! configured width of child
            */
            int32 _configuredWidth = 10;

            /*! configured height of child
            */
            int32 _configuredHeight = 10;

            /*! horizontal position of child
            */
            int32 _x = 0;

            /*! vertical position of child
            */
            int32 _y = 0;

            /*! user data that can be attached to a grid field
            */
            void* _userData = nullptr;
        };

        /*! internal struct that represents a collumn of widgets
        */
        struct GridCollumn
        {
            /*! list of widgets within one collumn
            */
            std::vector<Field> _widgetCollumn;
        };

    public:

		/*! ctor initializes member variables

		\param parent optional parent
		*/
		iWidgetGrid(iWidgetPtr parent = nullptr);

		/*! does nothing
		*/
		~iWidgetGrid() = default;

        /*! appends rows at the bottom of the grid

        \param count the amount of rows to be added
        */
        void appendRows(uint32 count);

        /*! appends collumns on the right of the grid

        \param count the amount of collumns to be added
        */
        void appendCollumns(uint32 count);

        /*! insert a single row before given position

        \param at the given position
        */
        void insertRow(uint32 at);

        /*! insert a single collumn before given position

        \param at the given position
        */
        void insertCollumn(uint32 at);

        /*! removes row at given position

        \param at the given position
        */
        void removeRow(uint32 at);

        /*! removed collumn at given position

        \param at the given position
        */
        void removeCollumn(uint32 at);

        /*! clears the whole grid
        */
        void clear();

        /*! \returns row count
        */
        uint32 getRowCount();

        /*! \returns collumn count
        */
        uint32 getColumnCount();

        /*! \returns cell spacing
        */
        int32 getCellSpacing();

        /*! sets cell spacing
        */
        void setCellSpacing(int32 cellSpacing);

        /*! \returns border
        */
        int32 getBorder();

        /*! sets border
        */
        void setBorder(int32 border);

        /*! adds a child widget to this widget at position 0, 0

        if there is already a child at 0, 0 it will be replaced

        you might want to use addWidget(iWidgetPtr widget, int32 col, int32 row, void* userData = nullptr); instead

        \param widget the child widget to be added
        */
        void addWidget(iWidgetPtr widget);

        /*! removes a child widget frmo this widget regardless of it's position

        the former position will be cleared

        \param widget the child widget to be removed
        */
        void removeWidget(iWidgetPtr widget);

        /*! add widget and set it at given position

        \param widget the widget pointer
        \param col column index
        \param row row index
        */
        void addWidget(iWidgetPtr widget, int32 col, int32 row, void* userData = nullptr);

        /*! sets selection mode of grid

        \param mode the mode of selection
        */
        void setSelectMode(iSelectionMode mode);

        /*! \returns selection mode
        */
        iSelectionMode getSelectMode() const;

        void select(int32 collumn, int32 row);

        /*! unselects the grid
        */
        void unSelect();

        /*! \returns true: if grid is selected
        */
        bool isSelected();

        /*! \returns selected row

        if not selected it will return -1
        */
        int32 getSelectedRow() const;

        /*! \returns selected collumn

        if not selected it will return -1
        */
        int32 getSelectedCollumn() const;

        /*! \returns pointer to user Data of selected field
        */
        void* getSelectedUserData();

        /*! \returns pointer to user Data of specified field

        \param col specfied collumn 
        \param row specfied row
        */
        void* getUserData(int32 col, int32 row);

        /*! defines which row will be streched if the grid is vertically streched

        \param row row number to be streched
        */
		void setStrechRow(int32 row);

        /*! \returns row number to be streched
        */
        int32 getStrechRow() const;

        /*! defines which column will be streched if the grid is horrizontally streched

        \param col column number to be streched
        */
		void setStrechColumn(int32 col);

        /*! \retruns column number to be streched
        */
		int32 getStrechColumn() const;

    private:

        /*! row number to be streched
        */
		int32 _strechRow = 0;

        /*! column number to be streched
        */
		int32 _strechCol = 0;

        /*! the child widgets
        */
        std::vector<GridCollumn> _widgetRows;

        /*! cellspacing within the grid
        */
        int32 _cellspacing = 4;

        /*! border size
        */
        int32 _border = 0;

        /*! saves over wich row the mouse last was
        */
        int32 _mouseOverRow = -1;

        /*! saves over wich collumn the mouse last was
        */
        int32 _mouseOverCollumn = -1;

        /*! saves over wich row the mouse last was
        */
        int32 _selectedRow = -1;

        /*! saves over wich collumn the mouse last was
        */
        int32 _selectedCollumn = -1;

        /*! mode of selection
        */
        iSelectionMode _selectMode = iSelectionMode::NoSelection;
		
        /*! handles incomming mouse wheel event

        \param d mouse wheel delta
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseWheel(int32 d);

        /*! handles incomming mouse key down events

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseKeyDown(iKeyCode key);

        /*! handles incomming double click

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseDoubleClick(iKeyCode key);

        /*! handles mouse key up events

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseKeyUp(iKeyCode key);

        /*! handles incomming mouse move events

        \param position mouse position
        */
        void handleMouseMove(const iaVector2i& pos);

        /*! updates size based on it's content
        */
        void calcMinSize();

		/*! draws the widget
		*/
		void draw();

        /*! initializes an empty grid with default size of one row and collumn
        */
        void initGrid();

        /*! updates widget's alignment

        \param clientWidth maximum width this widget can align to
        \param clientHeight maximum height this widget can align to
        */
        void updateAlignment(int32 clientWidth, int32 clientHeight);

        /*! calculates childrens ofsets relative to thair parent

        \param offsets vector to be filled with childrens offsets
        */
        void calcChildOffsets(std::vector<iRectanglei>& offsets);

    };

	/*! widget grid pointer definition
	*/
	typedef iWidgetGrid* iWidgetGridPtr;

}

#endif
