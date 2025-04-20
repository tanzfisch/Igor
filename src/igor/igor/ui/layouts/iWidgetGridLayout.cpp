// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/layouts/iWidgetGridLayout.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/ui/user_controls/iUserControl.h>
#include <igor/data/iIntersection.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{
    iWidgetGridLayout::iWidgetGridLayout(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetGridLayout, iWidgetKind::Layout, parent)
    {
        _configuredMinWidth = 0;
        _configuredMinHeight = 0;
        _ignoreChildEventConsumption = true;
        initGrid();
    }

    void iWidgetGridLayout::clear()
    {
        clearChildren();
        _widgetRows.clear();
        _children.clear();
        initGrid();
    }

    void iWidgetGridLayout::setHighlightMode(iSelectionMode highlightMode)
    {
        _highlightMode = highlightMode;
    }

    void iWidgetGridLayout::setSelectMode(iSelectionMode selectMode)
    {
        _selectMode = selectMode;
    }

    iSelectionMode iWidgetGridLayout::getSelectMode() const
    {
        return _selectMode;
    }

    iSelectionMode iWidgetGridLayout::getHighlightMode() const
    {
        return _highlightMode;
    }

    const std::any iWidgetGridLayout::getSelectedUserData()
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

    std::any iWidgetGridLayout::getUserData(int32 col, int32 row)
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

    int32 iWidgetGridLayout::getSelectedRow() const
    {
        return _selectedRow;
    }

    bool iWidgetGridLayout::isSelected()
    {
        return ((_selectedColumn != -1) && (_selectedRow != -1)) ? true : false;
    }

    void iWidgetGridLayout::unselect()
    {
        if (_selectedColumn != -1 ||
            _selectedRow != -1)
        {
            _selectedColumn = -1;
            _selectedRow = -1;

            _change(this);
        }
    }

    void iWidgetGridLayout::select(int32 column, int32 row)
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

    int32 iWidgetGridLayout::getSelectedColumn() const
    {
        return _selectedColumn;
    }

    void iWidgetGridLayout::initGrid()
    {
        GridColumn gridColumn;
        gridColumn._widgetColumn.resize(1);
        _widgetRows.push_back(gridColumn);
        unselect();
    }

    void iWidgetGridLayout::appendRows(uint32 count)
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

    void iWidgetGridLayout::removeRow(uint32 at)
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

    void iWidgetGridLayout::removeColumn(uint32 at)
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

    void iWidgetGridLayout::insertRow(uint32 at)
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

    void iWidgetGridLayout::insertColumn(uint32 at)
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

    void iWidgetGridLayout::appendColumns(uint32 count)
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

    void iWidgetGridLayout::calcMinSize()
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
                _widgetRows[y]._widgetColumn[x]._configuredMinWidth = biggestsize;
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
                _widgetRows[y]._widgetColumn[x]._configuredMinHeight = biggestsize;
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

                posy += _widgetRows[y]._widgetColumn[x]._configuredMinHeight + _cellSpacing;
            }
        }

        for (uint32 y = 0; y < rowCount; ++y)
        {
            posx = _border;

            for (uint32 x = 0; x < columnCount; ++x)
            {
                _widgetRows[y]._widgetColumn[x]._x = posx;

                posx += _widgetRows[y]._widgetColumn[x]._configuredMinWidth + _cellSpacing;
            }
        }

        minWidth += columnCount * _cellSpacing - _cellSpacing + _border * 2;
        minHeight += rowCount * _cellSpacing - _cellSpacing + _border * 2;

        if (getConfiguredMinWidth() > minWidth)
        {
            minWidth = getConfiguredMinWidth();
        }

        if (getConfiguredMinHeight() > minHeight)
        {
            minHeight = getConfiguredMinHeight();
        }

        // no client area definition needed because every child has it's individual field
        setClientArea(0, 0, 0, 0);

        updateMinSize(minWidth, minHeight);
    }

    void iWidgetGridLayout::calcChildOffsets(std::vector<iaRectanglef> &offsets)
    {
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

    void iWidgetGridLayout::updateAlignment(int32 clientWidth, int32 clientHeight)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        iWidget::updateAlignment(clientWidth, clientHeight);

        int32 rowCount = static_cast<int32>(_widgetRows.size());
        int32 columnCount = static_cast<int32>(_widgetRows[0]._widgetColumn.size());

        if (getVerticalAlignment() == iVerticalAlignment::Stretch)
        {
            if (_stretchRow < rowCount)
            {
                int32 diff = _actualHeight - _minHeight;

                for (int32 x = 0; x < columnCount; ++x)
                {
                    _widgetRows[_stretchRow]._widgetColumn[x]._actualHeight = _widgetRows[_stretchRow]._widgetColumn[x]._configuredMinHeight + diff;
                }

                if (_stretchRow + 1 < rowCount)
                {
                    for (int32 x = 0; x < columnCount; ++x)
                    {
                        for (int32 y = _stretchRow + 1; y < rowCount; ++y)
                        {
                            _widgetRows[y]._widgetColumn[x]._y += diff;
                        }
                    }
                }
            }
            else
            {
                con_warn("vertical stretch is configured but an invalid stretch row index was defined");
            }
        }

        if (getHorizontalAlignment() == iHorizontalAlignment::Stretch)
        {
            if (_stretchCol < columnCount)
            {
                int32 diff = _actualWidth - _minWidth;

                for (int32 y = 0; y < rowCount; ++y)
                {
                    _widgetRows[y]._widgetColumn[_stretchCol]._actualWidth = _widgetRows[y]._widgetColumn[_stretchCol]._configuredMinWidth + diff;
                }

                if (_stretchCol + 1 < columnCount)
                {
                    for (int32 y = 0; y < rowCount; ++y)
                    {
                        for (int32 x = _stretchCol + 1; x < columnCount; ++x)
                        {
                            _widgetRows[y]._widgetColumn[x]._x += diff;
                        }
                    }
                }
            }
            else
            {
                con_warn("horizontal stretch is configured but an invalid stretch column index was defined");
            }
        }
    }

    int32 iWidgetGridLayout::getBorder()
    {
        return _border;
    }

    void iWidgetGridLayout::setBorder(int32 border)
    {
        _border = border;
    }

    void iWidgetGridLayout::setStretchRow(int32 row)
    {
        _stretchRow = row;
    }

    int32 iWidgetGridLayout::getStretchRow() const
    {
        return _stretchRow;
    }

    void iWidgetGridLayout::setStretchColumn(int32 col)
    {
        _stretchCol = col;
    }

    int32 iWidgetGridLayout::getStretchColumn() const
    {
        return _stretchCol;
    }

    int32 iWidgetGridLayout::getMouseOverRow() const
    {
        return _mouseOverRow;
    }

    int32 iWidgetGridLayout::getMouseOverColumn() const
    {
        return _mouseOverColumn;
    }

    bool iWidgetGridLayout::onMouseDoubleClick(iEventMouseKeyDoubleClick &event)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (!isEnabled() ||
            !isMouseOver())
        {
            return false;
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();
        bool result = false;

        for (auto widget : widgets)
        {
            if (widget->onMouseDoubleClick(event))
            {
                result = true;
            }
        }

        if (!_ignoreChildEventConsumption && result)
        {
            return true;
        }

        if (_selectMode == iSelectionMode::NoSelection)
        {
            return result;
        }

        if (event.getKey() == iKeyCode::MouseLeft)
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

    bool iWidgetGridLayout::onMouseKeyDown(iEventMouseKeyDown &event)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (!isEnabled())
        {
            return false;
        }

        bool result = iWidget::onMouseKeyDown(event);

        if (_selectMode == iSelectionMode::NoSelection)
        {
            return result;
        }

        return false;
    }

    void iWidgetGridLayout::setEmptyCellsSelectable(bool emptyCellsSelectable)
    {
        _emptyCellsSelectable = emptyCellsSelectable;
    }

    bool iWidgetGridLayout::getEmptyCellsSelectable() const
    {
        return _emptyCellsSelectable;
    }

    bool iWidgetGridLayout::onMouseKeyUp(iEventMouseKeyUp &event)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (!isEnabled() ||
            (!isMouseOver() &&
             !_acceptOutOfBoundsClicks))
        {
            return false;
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();
        bool result = false;

        for (auto widget : widgets)
        {
            if (widget->onMouseKeyUp(event))
            {
                result = true;
            }
        }

        if (!_ignoreChildEventConsumption && result)
        {
            return true;
        }
        else
        {
            if (event.getKey() == iKeyCode::MouseLeft ||
                event.getKey() == iKeyCode::MouseRight)
            {
                if (event.getKey() == iKeyCode::MouseLeft)
                {
                    _click(this);
                }

                if (_selectMode != iSelectionMode::NoSelection)
                {
                    if (_selectedColumn != _mouseOverColumn ||
                        _selectedRow != _mouseOverRow)
                    {
                        if (_selectMode == iSelectionMode::Cell &&
                            !_emptyCellsSelectable &&
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

                if (event.getKey() == iKeyCode::MouseRight)
                {
                    _contextMenu(this);
                }

                return true;
            }
        }

        return false;
    }

    void iWidgetGridLayout::onMouseMove(iEventMouseMove &event)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        int rowNum = 0;
        int colNum = 0;
        _mouseOverRow = -1;
        _mouseOverColumn = -1;

        const auto &pos = event.getPosition();

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
                        widget->onMouseMove(event);
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

        if (isEnabled())
        {
            auto rect = getActualRect();
            if (iIntersection::intersects(pos, rect) &&
                !event.isConsumed())
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

    bool iWidgetGridLayout::onMouseWheel(iEventMouseWheel &event)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (!isEnabled() ||
            !isMouseOver())
        {
            return false;
        }

        auto iterRow = _widgetRows.begin();
        while (iterRow != _widgetRows.end())
        {
            auto iterColumn = iterRow->_widgetColumn.begin();
            while (iterColumn != iterRow->_widgetColumn.end())
            {
                if (iterColumn->_widgetID != iWidget::INVALID_WIDGET_ID)
                {
                    iWidgetPtr widget = iWidgetManager::getInstance().getWidget(iterColumn->_widgetID);

                    if (widget != nullptr &&
                        widget->onMouseWheel(event))
                    {
                        return true;
                    }
                }
                iterColumn++;
            }

            iterRow++;
        }

        return false;
    }

    int32 iWidgetGridLayout::getCellSpacing()
    {
        return _cellSpacing;
    }

    void iWidgetGridLayout::setCellSpacing(int32 cellSpacing)
    {
        _cellSpacing = cellSpacing;
    }

    void iWidgetGridLayout::draw()
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (!isVisible())
        {
            return;
        }

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

    uint32 iWidgetGridLayout::getRowCount()
    {
        return static_cast<uint32>(_widgetRows.size());
    }

    uint32 iWidgetGridLayout::getColumnCount()
    {
        if (_widgetRows.empty())
        {
            return 0;
        }

        return static_cast<uint32>(_widgetRows[0]._widgetColumn.size());
    }

    void iWidgetGridLayout::addWidget(iWidgetPtr widget)
    {
        addWidget(widget, 0, 0, nullptr);
    }

    bool iWidgetGridLayout::isCellEmpty(int32 col, int32 row)
    {
        if (_widgetRows.size() < row || _widgetRows[row]._widgetColumn.size() < col)
        {
            return false;
        }

        return (_widgetRows[row]._widgetColumn[col]._widgetID == iWidget::INVALID_WIDGET_ID) &&
               (!_widgetRows[row]._widgetColumn[col]._userData.has_value());
    }

    void iWidgetGridLayout::removeWidget(iWidgetPtr widget)
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

    void iWidgetGridLayout::addWidget(iWidgetPtr widget, int32 col, int32 row, const std::any &userData)
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
            con_err("can't add invalid widget id. use iWidgetGridLayout::removeWidget");
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