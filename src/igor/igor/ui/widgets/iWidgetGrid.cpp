// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetGrid.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/ui/user_controls/iUserControl.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{
    iWidgetGrid::iWidgetGrid(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetGrid, iWidgetKind::Widget, parent)
    {
        _configuredWidth = 0;
        _configuredHeight = 0;
        _ignoreChildEventHandling = true;
        initGrid();
    }

    void iWidgetGrid::clear()
    {
        clearChildren();
        _widgetRows.clear();
        _children.clear();
        initGrid();
    }

    void iWidgetGrid::setHighlightMode(iSelectionMode highlightMode)
    {
        _highlightMode = highlightMode;
    }

    void iWidgetGrid::setSelectMode(iSelectionMode selectMode)
    {
        _selectMode = selectMode;
    }

    iSelectionMode iWidgetGrid::getSelectMode() const
    {
        return _selectMode;
    }

    iSelectionMode iWidgetGrid::getHighlightMode() const
    {
        return _highlightMode;
    }

    const std::any iWidgetGrid::getSelectedUserData()
    {
        if (!isSelected())
        {
            return std::any();
        }
        else
        {
            return _widgetRows[_selectedRow]._widgetColumn[_selectedColumn]._userData;
        }
    }

    std::any iWidgetGrid::getUserData(int32 col, int32 row)
    {
        con_assert(_widgetRows.size() > row, "out of range");
        con_assert(_widgetRows[row]._widgetColumn.size() > col, "out of range");

        if (_widgetRows.size() > row &&
            _widgetRows[row]._widgetColumn.size() > col)
        {
            return _widgetRows[row]._widgetColumn[col]._userData;
        }
        else
        {
            con_err("out of range");
        }

        return std::any();
    }

    int32 iWidgetGrid::getSelectedRow() const
    {
        return _selectedRow;
    }

    bool iWidgetGrid::isSelected()
    {
        return ((_selectedColumn != -1) && (_selectedRow != -1)) ? true : false;
    }

    void iWidgetGrid::unselect()
    {
        if (_selectedColumn != -1 ||
            _selectedRow != -1)
        {
            _selectedColumn = -1;
            _selectedRow = -1;

            _change(this);
        }
    }

    void iWidgetGrid::select(int32 column, int32 row)
    {
        if (row >= 0 &&
            row < getRowCount() &&
            column >= 0 &&
            column < getColumnCount())
        {
            if (_selectedColumn != column ||
                _selectedRow != row)
            {
                _selectedColumn = column;
                _selectedRow = row;

                _change(this);
            }
        }
    }

    int32 iWidgetGrid::getSelectedColumn() const
    {
        return _selectedColumn;
    }

    void iWidgetGrid::initGrid()
    {
        GridColumn gridColumn;
        gridColumn._widgetColumn.resize(1);
        _widgetRows.push_back(gridColumn);
        unselect();
    }

    void iWidgetGrid::appendRows(uint32 count)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (count == 0)
        {
            return;
        }

        uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetColumn.size());

        for (uint32 i = 0; i < count; ++i)
        {
            GridColumn gridColumn;
            gridColumn._widgetColumn.resize(columnCount);
            _widgetRows.push_back(gridColumn);
        }
    }

    void iWidgetGrid::removeRow(uint32 at)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");
        con_assert(at < _widgetRows.size(), "out of range");

        auto iter = _widgetRows.begin();
        for (uint32 i = 0; i < at; ++i)
        {
            iter++;
        }

        auto iterColumn = (*iter)._widgetColumn.begin();
        while (iterColumn != (*iter)._widgetColumn.end())
        {
            if ((*iterColumn)._widgetID != iWidget::INVALID_WIDGET_ID)
            {
                iWidgetPtr widget = iWidgetManager::getInstance().getWidget((*iterColumn)._widgetID);

                if (widget != nullptr)
                {
                    removeWidget(widget);
                }
            }
            iterColumn++;
        }

        _widgetRows.erase(iter);

        if (_widgetRows.empty())
        {
            initGrid();
        }
    }

    void iWidgetGrid::removeColumn(uint32 at)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");
        uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetColumn.size());
        con_assert(at < columnCount, "out of range");

        auto iterRow = _widgetRows.begin();
        while (iterRow != _widgetRows.end())
        {
            auto iterColumn = (*iterRow)._widgetColumn.begin();
            for (uint32 i = 0; i < at; ++i)
            {
                iterColumn++;
            }

            if ((*iterColumn)._widgetID != iWidget::INVALID_WIDGET_ID)
            {
                iWidgetPtr widget = iWidgetManager::getInstance().getWidget((*iterColumn)._widgetID);

                if (widget != nullptr)
                {
                    removeWidget(widget);
                }
            }

            (*iterRow)._widgetColumn.erase(iterColumn);

            iterRow++;
        }

        if (_widgetRows.empty())
        {
            initGrid();
        }
    }

    void iWidgetGrid::insertRow(uint32 at)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");
        con_assert(at <= _widgetRows.size(), "out of range");

        if (at == _widgetRows.size())
        {
            appendRows(1);
            // no additional update needed here
        }
        else
        {
            uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetColumn.size());
            GridColumn gridColumn;
            gridColumn._widgetColumn.resize(columnCount);

            auto iter = _widgetRows.begin();
            for (uint32 i = 0; i < at; ++i)
            {
                iter++;
            }

            _widgetRows.insert(iter, gridColumn);
        }
    }

    void iWidgetGrid::insertColumn(uint32 at)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");
        uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetColumn.size());
        con_assert(at <= columnCount, "out of range");

        if (at == columnCount)
        {
            appendColumns(1);
            // no additional update needed here
        }
        else
        {
            auto iterRow = _widgetRows.begin();
            while (iterRow != _widgetRows.end())
            {
                auto iterColumn = (*iterRow)._widgetColumn.begin();
                for (uint32 i = 0; i < at; ++i)
                {
                    iterColumn++;
                }

                Cell gridWidget;
                (*iterRow)._widgetColumn.insert(iterColumn, gridWidget);

                iterRow++;
            }
        }
    }

    void iWidgetGrid::appendColumns(uint32 count)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (count == 0)
        {
            return;
        }

        int32 currentColumnSize = static_cast<uint32>(_widgetRows[0]._widgetColumn.size());

        auto iter = _widgetRows.begin();
        while (iter != _widgetRows.end())
        {
            (*iter)._widgetColumn.resize(currentColumnSize + count);
            iter++;
        }
    }

    void iWidgetGrid::calcMinSize()
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        int32 biggestsize = 0;

        int32 minWidth = 0;
        int32 minHeight = 0;

        uint32 rowCount = static_cast<uint32>(_widgetRows.size());
        uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetColumn.size());

        for (uint32 x = 0; x < columnCount; ++x)
        {
            biggestsize = 0;

            for (uint32 y = 0; y < rowCount; y++)
            {
                iWidgetPtr widget = iWidgetManager::getInstance().getWidget(_widgetRows[y]._widgetColumn[x]._widgetID);

                if (widget != nullptr)
                {
                    if (widget->getMinWidth() > biggestsize)
                    {
                        biggestsize = widget->getMinWidth();
                    }
                }
            }

            for (uint32 y = 0; y < rowCount; ++y)
            {
                _widgetRows[y]._widgetColumn[x]._configuredWidth = biggestsize;
                _widgetRows[y]._widgetColumn[x]._actualWidth = biggestsize;
            }

            minWidth += biggestsize;
        }

        for (uint32 y = 0; y < rowCount; ++y)
        {
            biggestsize = 0;

            for (uint32 x = 0; x < columnCount; ++x)
            {
                iWidgetPtr widget = iWidgetManager::getInstance().getWidget(_widgetRows[y]._widgetColumn[x]._widgetID);

                if (widget != nullptr)
                {
                    if (widget->getMinHeight() > biggestsize)
                    {
                        biggestsize = widget->getMinHeight();
                    }
                }
            }

            for (uint32 x = 0; x < columnCount; ++x)
            {
                _widgetRows[y]._widgetColumn[x]._configuredHeight = biggestsize;
                _widgetRows[y]._widgetColumn[x]._actualHeight = biggestsize;
            }

            minHeight += biggestsize;
        }

        int32 posx = 0;
        int32 posy = 0;

        for (uint32 x = 0; x < columnCount; ++x)
        {
            posy = _border;

            for (uint32 y = 0; y < rowCount; ++y)
            {
                _widgetRows[y]._widgetColumn[x]._y = posy;

                posy += _widgetRows[y]._widgetColumn[x]._configuredHeight + _cellspacing;
            }
        }

        for (uint32 y = 0; y < rowCount; ++y)
        {
            posx = _border;

            for (uint32 x = 0; x < columnCount; ++x)
            {
                _widgetRows[y]._widgetColumn[x]._x = posx;

                posx += _widgetRows[y]._widgetColumn[x]._configuredWidth + _cellspacing;
            }
        }

        minWidth += columnCount * _cellspacing - _cellspacing + _border * 2;
        minHeight += rowCount * _cellspacing - _cellspacing + _border * 2;

        if (getConfiguredWidth() > minWidth)
        {
            minWidth = getConfiguredWidth();
        }

        if (getConfiguredHeight() > minHeight)
        {
            minHeight = getConfiguredHeight();
        }

        // no client area definition needed becasue every child has it's individual field
        setClientArea(0, 0, 0, 0);

        setMinSize(minWidth, minHeight);
    }

    void iWidgetGrid::calcChildOffsets(std::vector<iaRectanglef> &offsets)
    {
        offsets.clear();
        offsets.resize(_children.size());

        iaRectanglef clientRect;

        for (auto row : _widgetRows)
        {
            for (auto column : row._widgetColumn)
            {
                int index = 0;
                int foundIndex = -1;

                for (const auto child : _children)
                {
                    if (child->getID() == column._widgetID)
                    {
                        foundIndex = index;
                        break;
                    }
                    index++;
                }

                if (foundIndex != -1)
                {
                    clientRect.setX(column._x);
                    clientRect.setY(column._y);
                    clientRect.setWidth(column._actualWidth);
                    clientRect.setHeight(column._actualHeight);

                    offsets[foundIndex] = clientRect;
                }
            }
        }
    }

    void iWidgetGrid::updateAlignment(int32 clientWidth, int32 clientHeight)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        iWidget::updateAlignment(clientWidth, clientHeight);

        int32 rowCount = static_cast<int32>(_widgetRows.size());
        int32 columnCount = static_cast<int32>(_widgetRows[0]._widgetColumn.size());

        if (getVerticalAlignment() == iVerticalAlignment::Strech)
        {
            if (_strechRow < rowCount)
            {
                int32 diff = _actualHeight - _minHeight;

                for (int32 x = 0; x < columnCount; ++x)
                {
                    _widgetRows[_strechRow]._widgetColumn[x]._actualHeight = _widgetRows[_strechRow]._widgetColumn[x]._configuredHeight + diff;
                }

                if (_strechRow + 1 < rowCount)
                {
                    for (int32 x = 0; x < columnCount; ++x)
                    {
                        for (int32 y = _strechRow + 1; y < rowCount; ++y)
                        {
                            _widgetRows[y]._widgetColumn[x]._y += diff;
                        }
                    }
                }
            }
            else
            {
                con_warn("vertical strech is configured but an invalid strech row index was defined");
            }
        }

        if (getHorizontalAlignment() == iHorizontalAlignment::Strech)
        {
            if (_strechCol < columnCount)
            {
                int32 diff = _actualWidth - _minWidth;

                for (int32 y = 0; y < rowCount; ++y)
                {
                    _widgetRows[y]._widgetColumn[_strechCol]._actualWidth = _widgetRows[y]._widgetColumn[_strechCol]._configuredWidth + diff;
                }

                if (_strechCol + 1 < columnCount)
                {
                    for (int32 y = 0; y < rowCount; ++y)
                    {
                        for (int32 x = _strechCol + 1; x < columnCount; ++x)
                        {
                            _widgetRows[y]._widgetColumn[x]._x += diff;
                        }
                    }
                }
            }
            else
            {
                con_warn("horizontal strech is configured but an invalid strech column index was defined");
            }
        }
    }

    int32 iWidgetGrid::getBorder()
    {
        return _border;
    }

    void iWidgetGrid::setBorder(int32 border)
    {
        _border = border;
    }

    void iWidgetGrid::setStrechRow(int32 row)
    {
        _strechRow = row;
    }

    int32 iWidgetGrid::getStrechRow() const
    {
        return _strechRow;
    }

    void iWidgetGrid::setStrechColumn(int32 col)
    {
        _strechCol = col;
    }

    int32 iWidgetGrid::getStrechColumn() const
    {
        return _strechCol;
    }

    int32 iWidgetGrid::getMouseOverRow() const
    {
        return _mouseOverRow;
    }

    int32 iWidgetGrid::getMouseOverColumn() const
    {
        return _mouseOverColumn;
    }

    bool iWidgetGrid::handleMouseDoubleClick(iKeyCode key)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (!isActive() || !_isMouseOver)
        {
            return false;
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets;
        getChildren(widgets);
        bool result = false;

        for (auto widget : widgets)
        {
            if (widget->handleMouseDoubleClick(key))
            {
                result = true;
            }
        }

        if (!_ignoreChildEventHandling && result)
        {
            return true;
        }

        if (_selectMode == iSelectionMode::NoSelection)
        {
            return result;
        }

        if (key == iKeyCode::MouseLeft)
        {
            if (_selectedColumn != _mouseOverColumn ||
                _selectedRow != _mouseOverRow)
            {
                _selectedColumn = _mouseOverColumn;
                _selectedRow = _mouseOverRow;
                _change(this);
            }

            setKeyboardFocus();
            _doubleClick(this);

            return true;
        }

        return false;
    }

    bool iWidgetGrid::handleMouseKeyDown(iKeyCode key)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (!isActive())
        {
            return false;
        }

        bool result = iWidget::handleMouseKeyDown(key);

        if (_selectMode == iSelectionMode::NoSelection)
        {
            return result;
        }

        return false;
    }

    void iWidgetGrid::setEmptyCellsSelecable(bool emptyCellsSelecable)
    {
        _emptyCellsSelecable = emptyCellsSelecable;
    }

    bool iWidgetGrid::getEmptyCellsSelecable() const
    {
        return _emptyCellsSelecable;
    }

    bool iWidgetGrid::handleMouseKeyUp(iKeyCode key)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (isActive())
        {
            if (_isMouseOver || _acceptOutOfBoundsClicks)
            {
                // get copy of children
                std::vector<iWidgetPtr> widgets;
                getChildren(widgets);
                bool result = false;

                for (auto widget : widgets)
                {
                    if (widget->handleMouseKeyUp(key))
                    {
                        result = true;
                    }
                }

                if (!_ignoreChildEventHandling && result)
                {
                    return true;
                }
                else
                {
                    if (key == iKeyCode::MouseLeft ||
                        key == iKeyCode::MouseRight)
                    {
                        _click(this);

                        if (_selectMode != iSelectionMode::NoSelection)
                        {
                            if (_selectedColumn != _mouseOverColumn ||
                                _selectedRow != _mouseOverRow)
                            {
                                if (_selectMode == iSelectionMode::Cell &&
                                    !_emptyCellsSelecable &&
                                    isCellEmpty(_mouseOverColumn, _mouseOverRow))
                                {
                                    unselect();
                                }
                                else
                                {
                                    _selectedColumn = _mouseOverColumn;
                                    _selectedRow = _mouseOverRow;
                                    _change(this);
                                }
                            }
                        }

                        if (key == iKeyCode::MouseRight)
                        {
                            _contextMenu(this);
                        }

                        return true;
                    }
                }
            }
        }

        return false;
    }

    void iWidgetGrid::handleMouseMove(const iaVector2f &pos)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        int rowNum = 0;
        int colNum = 0;
        _mouseOverRow = -1;
        _mouseOverColumn = -1;

        for (auto row : _widgetRows)
        {
            colNum = 0;

            for (auto col : row._widgetColumn)
            {
                if (col._widgetID != iWidget::INVALID_WIDGET_ID)
                {
                    iWidgetPtr widget = iWidgetManager::getInstance().getWidget(col._widgetID);

                    if (widget != nullptr)
                    {
                        widget->handleMouseMove(pos);
                    }
                }

                int32 colPosX = col._x + getActualPosX();
                int32 colPosY = col._y + getActualPosY();

                if (pos._x >= colPosX &&
                    pos._x < colPosX + col._actualWidth &&
                    pos._y >= colPosY &&
                    pos._y < colPosY + col._actualHeight)
                {
                    _mouseOverRow = rowNum;
                    _mouseOverColumn = colNum;
                }

                colNum++;
            }

            rowNum++;
        }

        if (isActive())
        {
            if (pos._x >= getActualPosX() &&
                pos._x < getActualPosX() + getActualWidth() &&
                pos._y >= getActualPosY() &&
                pos._y < getActualPosY() + getActualHeight())
            {
                if (!_isMouseOver)
                {
                    _widgetState = iWidgetState::Highlighted;
                    _mouseOver(this);
                }

                _isMouseOver = true;
            }
            else
            {
                if (_isMouseOver)
                {
                    _widgetState = iWidgetState::Standby;
                    _mouseOff(this);
                }

                _isMouseOver = false;
            }
        }
    }

    bool iWidgetGrid::handleMouseWheel(int32 d)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (!isActive())
        {
            return false;
        }

        if (isMouseOver())
        {
            auto iterRow = _widgetRows.begin();
            while (iterRow != _widgetRows.end())
            {
                auto iterColumn = (*iterRow)._widgetColumn.begin();
                while (iterColumn != (*iterRow)._widgetColumn.end())
                {
                    if ((*iterColumn)._widgetID != iWidget::INVALID_WIDGET_ID)
                    {
                        iWidgetPtr widget = iWidgetManager::getInstance().getWidget((*iterColumn)._widgetID);

                        if (widget != nullptr &&
                            widget->handleMouseWheel(d))
                        {
                            return true;
                        }
                    }
                    iterColumn++;
                }

                iterRow++;
            }
        }

        return false;
    }

    int32 iWidgetGrid::getCellSpacing()
    {
        return _cellspacing;
    }

    void iWidgetGrid::setCellSpacing(int32 cellSpacing)
    {
        _cellspacing = cellSpacing;
    }

    void iWidgetGrid::draw()
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (isVisible())
        {
            int32 rowIndex = 0;
            int32 colIndex = 0;

            iWidgetManager::getInstance().getTheme()->drawGridCell(getActualRect(), getState());

            for (auto row : _widgetRows)
            {
                colIndex = 0;

                for (auto col : row._widgetColumn)
                {
                    int32 colPosX = col._x + getActualPosX();
                    int32 colPosY = col._y + getActualPosY();

                    iWidgetManager::getInstance().getTheme()->drawGridCell(iaRectanglef(colPosX, colPosY, col._actualWidth, col._actualHeight), getState());

                    bool drawSelected = false;
                    bool drawHighlight = false;

                    if (_selectMode != iSelectionMode::NoSelection)
                    {
                        switch (_selectMode)
                        {
                        case iSelectionMode::Column:
                            drawSelected = _selectedColumn == colIndex;
                            break;

                        case iSelectionMode::Row:
                            drawSelected = _selectedRow == rowIndex;
                            break;

                        case iSelectionMode::Cell:
                            drawSelected = _selectedColumn == colIndex &&
                                           _selectedRow == rowIndex;
                            break;
                        }
                    }

                    if (drawSelected)
                    {
                        iWidgetManager::getInstance().getTheme()->drawGridSelection(
                            iaRectanglef(colPosX, colPosY, col._actualWidth, col._actualHeight));
                    }
                    else
                    {
                        if (_highlightMode != iSelectionMode::NoSelection)
                        {
                            switch (_highlightMode)
                            {
                            case iSelectionMode::Column:
                                drawHighlight = _mouseOverColumn == colIndex;
                                break;

                            case iSelectionMode::Row:
                                drawHighlight = _mouseOverRow == rowIndex;
                                break;

                            case iSelectionMode::Cell:
                                drawHighlight = _mouseOverColumn == colIndex &&
                                                _mouseOverRow == rowIndex;
                                break;
                            }
                        }

                        if (drawHighlight)
                        {
                            iWidgetManager::getInstance().getTheme()->drawGridHighlight(
                                iaRectanglef(colPosX, colPosY, col._actualWidth, col._actualHeight));
                        }
                    }

                    iWidgetPtr widget = iWidgetManager::getInstance().getWidget(col._widgetID);

                    if (widget != nullptr)
                    {
                        widget->draw();
                    }

                    colIndex++;
                }

                rowIndex++;
            }
        }
    }

    uint32 iWidgetGrid::getRowCount()
    {
        return static_cast<uint32>(_widgetRows.size());
    }

    uint32 iWidgetGrid::getColumnCount()
    {
        if (_widgetRows.empty())
        {
            return 0;
        }

        return static_cast<uint32>(_widgetRows[0]._widgetColumn.size());
    }

    void iWidgetGrid::addWidget(iWidgetPtr widget)
    {
        addWidget(widget, 0, 0, nullptr);
    }

    bool iWidgetGrid::isCellEmpty(int32 col, int32 row)
    {
        con_assert(_widgetRows.size() > row && _widgetRows[row]._widgetColumn.size() > col, "out of range " << col << "," << row);

        return (_widgetRows[row]._widgetColumn[col]._widgetID == iWidget::INVALID_WIDGET_ID) &&
               (!_widgetRows[row]._widgetColumn[col]._userData.has_value());
    }

    void iWidgetGrid::removeWidget(iWidgetPtr widget)
    {
        uint32 rowCount = static_cast<uint32>(_widgetRows.size());
        uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetColumn.size());
        bool removed = false;
        for (uint32 x = 0; x < columnCount && removed == false; ++x)
        {
            for (uint32 y = 0; y < rowCount && removed == false; y++)
            {
                iWidgetPtr temp = iWidgetManager::getInstance().getWidget(_widgetRows[y]._widgetColumn[x]._widgetID);

                if (widget == temp)
                {
                    _widgetRows[y]._widgetColumn[x]._widgetID = iWidget::INVALID_WIDGET_ID;
                    _widgetRows[y]._widgetColumn[x]._userData.reset();
                    removed = true;
                }
            }
        }

        // con_assert(removed == true, "inconsistant data");
        iWidget::removeWidget(widget);
    }

    void iWidgetGrid::addWidget(iWidgetPtr widget, int32 col, int32 row, const std::any &userData)
    {
        con_assert(_widgetRows.size() > row && _widgetRows[row]._widgetColumn.size() > col, "out of range " << col << "," << row);

        if (row >= _widgetRows.size() ||
            col >= _widgetRows[row]._widgetColumn.size())
        {
            con_err("out of range");
            return;
        }

        uint64 widgetID = (widget != nullptr) ? widget->getID() : iWidget::INVALID_WIDGET_ID;

        if (widgetID == iWidget::INVALID_WIDGET_ID)
        {
            con_err("can't add invalid widget id. use iWidgetGrid::removeWidget");
            return;
        }

        auto iter = find(_children.begin(), _children.end(), widget);
        if (iter != _children.end())
        {
            con_err("widget " << widgetID << " already added. remove first to set it at different position");
            return;
        }

        // remove if cell contains a widget already
        if (_widgetRows[row]._widgetColumn[col]._widgetID != iWidget::INVALID_WIDGET_ID)
        {
            iWidgetPtr remove = iWidgetManager::getInstance().getWidget(_widgetRows[row]._widgetColumn[col]._widgetID);
            if (remove != nullptr)
            {
                iWidget::removeWidget(remove);
            }
        }

        // now add the widget
        iWidget::addWidget(widget);
        _widgetRows[row]._widgetColumn[col]._userData = userData;
        _widgetRows[row]._widgetColumn[col]._widgetID = widgetID;
    }

} // namespace igor