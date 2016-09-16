// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetGrid.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iWidgetUserControl.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    iWidgetGrid::iWidgetGrid()
        : iWidget(iWidgetType::Grid)
    {
        _configuredWidth = 0;
        _configuredHeight = 0;
        initGrid();
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
        _selectedCollumn = -1;
        _selectedRow = -1;
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
                _widgetRows[y]._widgetCollumn[x]._width = biggestsize;
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
                _widgetRows[y]._widgetCollumn[x]._height = biggestsize;
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

                posy += _widgetRows[y]._widgetCollumn[x]._height + _cellspacing;
            }
        }

        for (uint32 y = 0; y < rowCount; ++y)
        {
            posx = _border;

            for (uint32 x = 0; x < columnCount; ++x)
            {
                _widgetRows[y]._widgetCollumn[x]._x = posx;

                posx += _widgetRows[y]._widgetCollumn[x]._width + _cellspacing;
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

        setMinSize(minWidth, minHeight);
    }


    void iWidgetGrid::calcChildOffsets(vector<iaVector2i>& offsets)
    {
        offsets.clear();
        offsets.resize(_children.size());

        iaVector2i pos;

        auto iterRow = _widgetRows.begin();
        while (iterRow != _widgetRows.end())
        {
            auto iterCollumn = (*iterRow)._widgetCollumn.begin();
            while (iterCollumn != (*iterRow)._widgetCollumn.end())
            {
                int index = 0;
                int foundIndex = -1;

                auto iter = _children.begin();
                while (iter != _children.end())
                {
                    if ((*iter)->getID() == (*iterCollumn)._widgetID)
                    {
                        foundIndex = index;
                        break;
                    }
                    index++;
                    iter++;
                }

                if (foundIndex != -1)
                {
                    pos._x = _marginLeft + (*iterCollumn)._x;
                    pos._y = _marginTop + (*iterCollumn)._y;

                    offsets[foundIndex] = pos;
                }

                iterCollumn++;
            }

            iterRow++;
        }
    }

    void iWidgetGrid::updateAlignment()
    {
        iWidget::updateAlignment();

        int32 minWidth = _minWidth;
        int32 minHeight = _minHeight;
        uint32 rowCount = static_cast<uint32>(_widgetRows.size());
        uint32 columnCount = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());

        if (_strechRow > -1 &&
            getVerticalAlignment() == iVerticalAlignment::Strech &&
            _strechRow < rowCount)
        {
            int32 diff = _actualHeight - minHeight;

            for (uint32 x = 0; x < columnCount; ++x)
            {
                _widgetRows[_strechRow]._widgetCollumn[x]._height += diff;
            }

            if (_strechRow + 1 < rowCount)
            {
                for (uint32 x = 0; x < columnCount; ++x)
                {
                    for (uint32 y = _strechRow + 1; y < rowCount; ++y)
                    {
                        _widgetRows[y]._widgetCollumn[x]._y += diff;
                    }
                }
            }

            minHeight += diff;
        }

        if (_strechCol > -1 &&
            getHorrizontalAlignment() == iHorrizontalAlignment::Strech &&
            _strechCol < columnCount)
        {
            int32 diff = _actualWidth - minWidth;

            for (uint32 y = 0; y < rowCount; ++y)
            {
                _widgetRows[y]._widgetCollumn[_strechCol]._width += diff;
            }

            if (_strechCol + 1 < columnCount)
            {
                for (uint32 y = 0; y < rowCount; ++y)
                {
                    for (uint32 x = _strechCol + 1; x < columnCount; ++x)
                    {
                        _widgetRows[y]._widgetCollumn[x]._x += diff;
                    }
                }
            }

            minWidth += diff;
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
            if (_selectedCollumn != _mouseOverCollumn ||
                _selectedRow != _mouseOverRow)
            {
                _selectedCollumn = _mouseOverCollumn;
                _selectedRow = _mouseOverRow;
                _change(this);
            }

            _click(this);

            if (key == iKeyCode::MouseRight)
            {
                _contextMenu(this);
            }

            return true;
        }

        return false;
    }

    void iWidgetGrid::handleMouseMove(int32 x, int32 y)
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
                        widget->handleMouseMove(x, y);
                    }
                }

                if (x >= col._absoluteX &&
                    x < col._absoluteX + col._width &&
                    y >= col._absoluteY  &&
                    y < col._absoluteY + col._height)
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
            if (x >= getActualPosX() &&
                x < getActualPosX() + getActualWidth() &&
                y >= getActualPosY() &&
                y < getActualPosY() + getActualHeight())
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

    void iWidgetGrid::draw(int32 parentPosX, int32 parentPosY)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (isVisible())
        {
            int32 row = 0;
            int32 col = 0;

            int32 realX = _absoluteX;
            int32 realY = _absoluteY;
            int32 realWidth = _actualWidth;
            int32 realHeight = _actualHeight;

            auto iterRow = _widgetRows.begin();
            while (iterRow != _widgetRows.end())
            {
                col = 0;

                auto iterCollumn = (*iterRow)._widgetCollumn.begin();
                while (iterCollumn != (*iterRow)._widgetCollumn.end())
                {
                    (*iterCollumn)._absoluteX = realX + (*iterCollumn)._x;
                    (*iterCollumn)._absoluteY = realY + (*iterCollumn)._y;
                    _absoluteX = realX + (*iterCollumn)._x;
                    _absoluteY = realY + (*iterCollumn)._y;
                    _actualWidth = (*iterCollumn)._width;
                    _actualHeight = (*iterCollumn)._height;

                    iWidgetManager::getInstance().getTheme()->drawGridField(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), getAppearanceState());

                    if (_selectMode != iSelectionMode::NoSelection)
                    {
                        bool drawSelected = false;
                        bool drawHighlight = false;

                        switch (_selectMode)
                        {
                        case iSelectionMode::Collumn:
                            if (_selectedCollumn == col)
                            {
                                drawSelected = true;
                            }

                            if (_mouseOverCollumn == col)
                            {
                                drawHighlight = true;
                            }
                            break;

                        case iSelectionMode::Row:
                            if (_selectedRow == row)
                            {
                                drawSelected = true;
                            }

                            if (_mouseOverRow == row)
                            {
                                drawHighlight = true;
                            }
                            break;

                        case iSelectionMode::Field:
                            if (_selectedCollumn == col &&
                                _selectedRow == row)
                            {
                                drawSelected = true;
                            }

                            if (_mouseOverCollumn == col &&
                                _mouseOverRow == row)
                            {
                                drawHighlight = true;
                            }
                            break;
                        }

                        if (drawSelected)
                        {
                            iWidgetManager::getInstance().getTheme()->drawGridSelection(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight());
                        }
                        else if (drawHighlight)
                        {
                            iWidgetManager::getInstance().getTheme()->drawGridHighlight(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight());
                        }
                    }

                    iWidget* widget = iWidgetManager::getInstance().getWidget((*iterCollumn)._widgetID);

                    if (widget != nullptr)
                    {
                        // updating childrens alignment once more but this time with fake parent boundaries
                        widget->updateAlignment();
                        widget->draw(getActualPosX(), getActualPosY());
                        // TODO widget->updatePosition(_absoluteX, _absoluteY);
                    }

                    iterCollumn++;
                    col++;
                }

                iterRow++;
                row++;
            }

            _absoluteX = realX;
            _absoluteY = realY;
            _actualWidth = realWidth;
            _actualHeight = realHeight;
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
        for (uint32 x = 0; x < columnCount, removed == false; ++x)
        {
            for (uint32 y = 0; y < rowCount, removed == false; y++)
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