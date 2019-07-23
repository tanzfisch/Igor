// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetGrid.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iUserControl.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    iWidgetGrid::iWidgetGrid()
    {
        _configuredWidth = 0;
        _configuredHeight = 0;
        initGrid();
    }

    iWidget* iWidgetGrid::createInstance()
    {
        return new iWidgetGrid();
    }

    void iWidgetGrid::clear()
    {
        _widgetRows.clear();
        _children.clear();
        initGrid();
    }

    void iWidgetGrid::setSelectMode(iSelectionMode mode)
    {
        _selectMode = mode;
    }

    iSelectionMode iWidgetGrid::getSelectMode() const
    {
        return _selectMode;
    }

    void* iWidgetGrid::getSelectedUserData()
    {
        if (!isSelected())
        {
            return nullptr;
        }
        else
        {
            return _widgetRows[_selectedRow]._widgetCollumn[_selectedCollumn]._userData;
        }
    }

    void* iWidgetGrid::getUserData(int32 col, int32 row)
    {
        con_assert(_widgetRows.size() > row, "out of range");
        con_assert(_widgetRows[row]._widgetCollumn.size() > col, "out of range");

        if (_widgetRows.size() > row &&
            _widgetRows[row]._widgetCollumn.size() > col)
        {
            return _widgetRows[row]._widgetCollumn[col]._userData;
        }
        else
        {
            con_err("out of range");
        }

        return nullptr;
    }

    int32 iWidgetGrid::getSelectedRow() const
    {
        return _selectedRow;
    }

    bool iWidgetGrid::isSelected()
    {
        return ((_selectedCollumn != -1) && (_selectedRow != -1)) ? true : false;
    }

    void iWidgetGrid::unSelect()
    {
        if (_selectedCollumn != -1 ||
            _selectedRow != -1)
        {
            _selectedCollumn = -1;
            _selectedRow = -1;

            _change(this);
        }
    }

    void iWidgetGrid::select(int32 collumn, int32 row)
    {
        if (row >= 0 &&
            row < getRowCount() &&
            collumn >= 0 &&
            collumn < getColumnCount())
        {
            if (_selectedCollumn != collumn ||
                _selectedRow != row)
            {
                _selectedCollumn = collumn;
                _selectedRow = row;

                _change(this);
            }
        }
    }

    int32 iWidgetGrid::getSelectedCollumn() const
    {
        return _selectedCollumn;
    }

    void iWidgetGrid::initGrid()
    {
        GridCollumn gridCollumn;
        gridCollumn._widgetCollumn.resize(1);
        _widgetRows.push_back(gridCollumn);
        unSelect();
    }

    void iWidgetGrid::appendRows(uint32 count)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (count == 0)
        {
            return;
        }

        uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());

        for (uint32 i = 0; i < count; ++i)
        {
            GridCollumn gridCollumn;
            gridCollumn._widgetCollumn.resize(columnCount);
            _widgetRows.push_back(gridCollumn);
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

        auto iterCollumn = (*iter)._widgetCollumn.begin();
        while (iterCollumn != (*iter)._widgetCollumn.end())
        {
            if ((*iterCollumn)._widgetID != iWidget::INVALID_WIDGET_ID)
            {
                iWidget* widget = iWidgetManager::getInstance().getWidget((*iterCollumn)._widgetID);

                if (widget != nullptr)
                {
                    removeWidget(widget);
                }
            }
            iterCollumn++;
        }

        _widgetRows.erase(iter);

        if (_widgetRows.empty())
        {
            initGrid();
        }
    }

    void iWidgetGrid::removeCollumn(uint32 at)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");
        uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());
        con_assert(at < columnCount, "out of range");

        auto iterRow = _widgetRows.begin();
        while (iterRow != _widgetRows.end())
        {
            auto iterCollumn = (*iterRow)._widgetCollumn.begin();
            for (uint32 i = 0; i < at; ++i)
            {
                iterCollumn++;
            }

            if ((*iterCollumn)._widgetID != iWidget::INVALID_WIDGET_ID)
            {
                iWidget* widget = iWidgetManager::getInstance().getWidget((*iterCollumn)._widgetID);

                if (widget != nullptr)
                {
                    removeWidget(widget);
                }
            }

            (*iterRow)._widgetCollumn.erase(iterCollumn);

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
            uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());
            GridCollumn gridCollumn;
            gridCollumn._widgetCollumn.resize(columnCount);

            auto iter = _widgetRows.begin();
            for (uint32 i = 0; i < at; ++i)
            {
                iter++;
            }

            _widgetRows.insert(iter, gridCollumn);
        }
    }

    void iWidgetGrid::insertCollumn(uint32 at)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");
        uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());
        con_assert(at <= columnCount, "out of range");

        if (at == columnCount)
        {
            appendCollumns(1);
            // no additional update needed here
        }
        else
        {
            auto iterRow = _widgetRows.begin();
            while (iterRow != _widgetRows.end())
            {
                auto iterCollumn = (*iterRow)._widgetCollumn.begin();
                for (uint32 i = 0; i < at; ++i)
                {
                    iterCollumn++;
                }

                Field gridWidget;
                (*iterRow)._widgetCollumn.insert(iterCollumn, gridWidget);

                iterRow++;
            }
        }
    }

    void iWidgetGrid::appendCollumns(uint32 count)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (count == 0)
        {
            return;
        }

        int32 currentCollumnSize = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());

        auto iter = _widgetRows.begin();
        while (iter != _widgetRows.end())
        {
            (*iter)._widgetCollumn.resize(currentCollumnSize + count);
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
        uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());

        for (uint32 x = 0; x < columnCount; ++x)
        {
            biggestsize = 0;

            for (uint32 y = 0; y < rowCount; y++)
            {
                iWidget* widget = iWidgetManager::getInstance().getWidget(_widgetRows[y]._widgetCollumn[x]._widgetID);

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
                _widgetRows[y]._widgetCollumn[x]._configuredWidth = biggestsize;
                _widgetRows[y]._widgetCollumn[x]._actualWidth = biggestsize;
            }

            minWidth += biggestsize;
        }

        for (uint32 y = 0; y < rowCount; ++y)
        {
            biggestsize = 0;

            for (uint32 x = 0; x < columnCount; ++x)
            {
                iWidget* widget = iWidgetManager::getInstance().getWidget(_widgetRows[y]._widgetCollumn[x]._widgetID);

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
                _widgetRows[y]._widgetCollumn[x]._configuredHeight = biggestsize;
                _widgetRows[y]._widgetCollumn[x]._actualHeight = biggestsize;
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
                _widgetRows[y]._widgetCollumn[x]._y = posy;

                posy += _widgetRows[y]._widgetCollumn[x]._configuredHeight + _cellspacing;
            }
        }

        for (uint32 y = 0; y < rowCount; ++y)
        {
            posx = _border;

            for (uint32 x = 0; x < columnCount; ++x)
            {
                _widgetRows[y]._widgetCollumn[x]._x = posx;

                posx += _widgetRows[y]._widgetCollumn[x]._configuredWidth + _cellspacing;
            }
        }

        minWidth += columnCount*_cellspacing - _cellspacing + _border * 2;
        minHeight += rowCount*_cellspacing - _cellspacing + _border * 2;

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

    void iWidgetGrid::calcChildOffsets(std::vector<iRectanglei>& offsets)
    {
        offsets.clear();
        offsets.resize(_children.size());

        iRectanglei clientRect;

        for (auto row : _widgetRows)
        {
            for (auto collumn : row._widgetCollumn)
            {
                int index = 0;
                int foundIndex = -1;

                for (auto child : _children)
                {
                    if (child->getID() == collumn._widgetID)
                    {
                        foundIndex = index;
                        break;
                    }
                    index++;
                }

                if (foundIndex != -1)
                {
                    clientRect.setX(collumn._x);
                    clientRect.setY(collumn._y);
                    clientRect.setWidth(collumn._actualWidth);
                    clientRect.setHeight(collumn._actualHeight);

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
        int32 columnCount = static_cast<int32>(_widgetRows[0]._widgetCollumn.size());

        if (getVerticalAlignment() == iVerticalAlignment::Strech)
        {
            if (_strechRow < rowCount)
            {
                int32 diff = _actualHeight - _minHeight;

                for (int32 x = 0; x < columnCount; ++x)
                {
                    _widgetRows[_strechRow]._widgetCollumn[x]._actualHeight = _widgetRows[_strechRow]._widgetCollumn[x]._configuredHeight + diff;
                }

                if (_strechRow + 1 < rowCount)
                {
                    for (int32 x = 0; x < columnCount; ++x)
                    {
                        for (int32 y = _strechRow + 1; y < rowCount; ++y)
                        {
                            _widgetRows[y]._widgetCollumn[x]._y += diff;
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
                    _widgetRows[y]._widgetCollumn[_strechCol]._actualWidth = _widgetRows[y]._widgetCollumn[_strechCol]._configuredWidth + diff;
                }

                if (_strechCol + 1 < columnCount)
                {
                    for (int32 y = 0; y < rowCount; ++y)
                    {
                        for (int32 x = _strechCol + 1; x < columnCount; ++x)
                        {
                            _widgetRows[y]._widgetCollumn[x]._x += diff;
                        }
                    }
                }
            }
            else
            {
                con_warn("horizontal strech is configured but an invalid strech collumn index was defined");
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

    bool iWidgetGrid::handleMouseDoubleClick(iKeyCode key)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (!isActive())
        {
            return false;
        }

        bool result = iWidget::handleMouseDoubleClick(key);

        if (_selectMode == iSelectionMode::NoSelection)
        {
            return result;
        }

        if (key == iKeyCode::MouseLeft)
        {
            if (_selectedCollumn != _mouseOverCollumn ||
                _selectedRow != _mouseOverRow)
            {
                _selectedCollumn = _mouseOverCollumn;
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

		if (key == iKeyCode::MouseLeft)
		{
			if (_selectedCollumn != _mouseOverCollumn ||
				_selectedRow != _mouseOverRow)
			{
				_selectedCollumn = _mouseOverCollumn;
				_selectedRow = _mouseOverRow;
				_change(this);
			}

			return true;
		}

        return false;
    }

    bool iWidgetGrid::handleMouseKeyUp(iKeyCode key)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (!isActive())
        {
            return false;
        }

        bool result = iWidget::handleMouseKeyUp(key);

        if (_selectMode == iSelectionMode::NoSelection)
        {
            return result;
        }

        if (key == iKeyCode::MouseLeft ||
            key == iKeyCode::MouseRight)
        {
            _click(this);

            if (key == iKeyCode::MouseRight)
            {
                _contextMenu(this);
            }

            return true;
        }

        return false;
    }

    void iWidgetGrid::handleMouseMove(const iaVector2i& pos)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        int rowNum = 0;
        int colNum = 0;
        _mouseOverRow = -1;
        _mouseOverCollumn = -1;

        for (auto row : _widgetRows)
        {
            colNum = 0;

            for (auto col : row._widgetCollumn)
            {
                if (col._widgetID != iWidget::INVALID_WIDGET_ID)
                {
                    iWidget* widget = iWidgetManager::getInstance().getWidget(col._widgetID);

                    if (widget != nullptr)
                    {
                        widget->handleMouseMove(pos);
                    }
                }

                int32 colPosX = col._x + getActualPosX();
                int32 colPosY = col._y + getActualPosY();

                if (pos._x >= colPosX &&
                    pos._x < colPosX + col._actualWidth &&
                    pos._y >= colPosY  &&
                    pos._y < colPosY + col._actualHeight)
                {
                    _mouseOverRow = rowNum;
                    _mouseOverCollumn = colNum;
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
                    _widgetAppearanceState = iWidgetAppearanceState::Highlighted;
                    _mouseOver(this);
                }

                _isMouseOver = true;
            }
            else
            {
                if (_isMouseOver)
                {
                    _widgetAppearanceState = iWidgetAppearanceState::Standby;
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
                auto iterCollumn = (*iterRow)._widgetCollumn.begin();
                while (iterCollumn != (*iterRow)._widgetCollumn.end())
                {
                    if ((*iterCollumn)._widgetID != iWidget::INVALID_WIDGET_ID)
                    {
                        iWidget* widget = iWidgetManager::getInstance().getWidget((*iterCollumn)._widgetID);

                        if (widget != nullptr &&
                            widget->handleMouseWheel(d))
                        {
                            return true;
                        }
                    }
                    iterCollumn++;
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

            iWidgetManager::getInstance().getTheme()->drawGridField(getActualRect(), getAppearanceState());

            for (auto row : _widgetRows)
            {
                colIndex = 0;

                for (auto col : row._widgetCollumn)
                {
                    int32 colPosX = col._x + getActualPosX();
                    int32 colPosY = col._y + getActualPosY();

                    iWidgetManager::getInstance().getTheme()->drawGridField(iRectanglei(colPosX, colPosY,
                        col._actualWidth, col._actualHeight), getAppearanceState());

                    if (_selectMode != iSelectionMode::NoSelection)
                    {
                        bool drawSelected = false;
                        bool drawHighlight = false;

                        switch (_selectMode)
                        {
                        case iSelectionMode::Collumn:
                            if (_selectedCollumn == colIndex)
                            {
                                drawSelected = true;
                            }

                            if (_mouseOverCollumn == colIndex)
                            {
                                drawHighlight = true;
                            }
                            break;

                        case iSelectionMode::Row:
                            if (_selectedRow == rowIndex)
                            {
                                drawSelected = true;
                            }

                            if (_mouseOverRow == rowIndex)
                            {
                                drawHighlight = true;
                            }
                            break;

                        case iSelectionMode::Field:
                            if (_selectedCollumn == colIndex &&
                                _selectedRow == rowIndex)
                            {
                                drawSelected = true;
                            }

                            if (_mouseOverCollumn == colIndex &&
                                _mouseOverRow == rowIndex)
                            {
                                drawHighlight = true;
                            }
                            break;
                        }

                        if (drawSelected)
                        {
                            iWidgetManager::getInstance().getTheme()->drawGridSelection(
                                iRectanglei(colPosX, colPosY, col._actualWidth, col._actualHeight));
                        }
                        else if (drawHighlight)
                        {
                            iWidgetManager::getInstance().getTheme()->drawGridHighlight(
                                iRectanglei(colPosX, colPosY, col._actualWidth, col._actualHeight));
                        }
                    }

                    iWidget* widget = iWidgetManager::getInstance().getWidget(col._widgetID);

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
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        return static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());
    }

    void iWidgetGrid::addWidget(iWidget* widget)
    {
        addWidget(widget, 0, 0, nullptr);
    }

    void iWidgetGrid::removeWidget(iWidget* widget)
    {
        uint32 rowCount = static_cast<uint32>(_widgetRows.size());
        uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());
        bool removed = false;
        for (uint32 x = 0; x < columnCount && removed == false; ++x)
        {
            for (uint32 y = 0; y < rowCount && removed == false; y++)
            {
                iWidget* temp = iWidgetManager::getInstance().getWidget(_widgetRows[y]._widgetCollumn[x]._widgetID);

                if (widget == temp)
                {
                    _widgetRows[y]._widgetCollumn[x]._widgetID = iWidget::INVALID_WIDGET_ID;
                    _widgetRows[y]._widgetCollumn[x]._userData = nullptr;
                    removed = true;
                }
            }
        }

        con_assert(removed == true, "inconsistant data");
        iWidget::removeWidget(widget);
    }

    void iWidgetGrid::addWidget(iWidget* widget, int32 col, int32 row, void* userData)
    {
        con_assert(_widgetRows.size() > row && _widgetRows[row]._widgetCollumn.size() > col, "out of range " << col << "," << row);

        uint64 widgetID = (widget != nullptr) ? widget->getID() : iWidget::INVALID_WIDGET_ID;

        if (_widgetRows.size() > row &&
            _widgetRows[row]._widgetCollumn.size() > col)
        {
            if (widgetID != iWidget::INVALID_WIDGET_ID)
            {
                auto iter = find(_children.begin(), _children.end(), widget);
                if (iter == _children.end())
                {
                    if (_widgetRows[row]._widgetCollumn[col]._widgetID != iWidget::INVALID_WIDGET_ID)
                    {
                        iWidget* remove = iWidgetManager::getInstance().getWidget(_widgetRows[row]._widgetCollumn[col]._widgetID);
                        if (remove != nullptr)
                        {
                            iWidget::removeWidget(remove);
                        }
                    }

                    iWidget::addWidget(widget);

                    _widgetRows[row]._widgetCollumn[col]._userData = userData;
                    _widgetRows[row]._widgetCollumn[col]._widgetID = widgetID;
                }
                else
                {
                    con_err("widget " << widgetID << " already added. remove first to set it at different position");
                }
            }
            else
            {
                con_err("can't add invalid widget id. use iWidgetGrid::removeWidget");
            }
        }
        else
        {
            con_err("out of range");
        }
    }

}