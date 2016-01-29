// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
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
        _width = 0;
        _height = 0;

        initGrid();
    }

    void iWidgetGrid::clear()
    {
        _widgetRows.clear();
        _widgets.clear();
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

        uint32 collumnCount = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());

        for (uint32 i = 0; i < count; ++i)
        {
            GridCollumn gridCollumn;
            gridCollumn._widgetCollumn.resize(collumnCount);
            _widgetRows.push_back(gridCollumn);
        }

        update();
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

        update();
    }

    void iWidgetGrid::removeCollumn(uint32 at)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");
        uint32 collumnCount = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());
        con_assert(at < collumnCount, "out of range");

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

        update();
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
            uint32 collumnCount = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());
            GridCollumn gridCollumn;
            gridCollumn._widgetCollumn.resize(collumnCount);

            auto iter = _widgetRows.begin();
            for (uint32 i = 0; i < at; ++i)
            {
                iter++;
            }

            _widgetRows.insert(iter, gridCollumn);
            update();
        }
    }

    void iWidgetGrid::insertCollumn(uint32 at)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");
        uint32 collumnCount = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());
        con_assert(at <= collumnCount, "out of range");

        if (at == collumnCount)
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

            update();
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

        update();
    }

    void iWidgetGrid::update()
    {
        recalcWidthHeight();
        updateParent();
    }

    int32 iWidgetGrid::getBorder()
    {
        return _border;
    }

    void iWidgetGrid::setBorder(int32 border)
    {
        _border = border;
        update();
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

        if (key == iKeyCode::MouseLeft)
        {
            if (_selectedCollumn != _mouseOverCollumn ||
                _selectedRow != _mouseOverRow)
            {
                _selectedCollumn = _mouseOverCollumn;
                _selectedRow = _mouseOverRow;
                _change(this);
            }

            _click(this);

            return true;
        }

        return false;
    }

    void iWidgetGrid::handleMouseMove(int32 x, int32 y)
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        int row = 0;
        int col = 0;
        _mouseOverRow = -1;
        _mouseOverCollumn = -1;

        auto iterRow = _widgetRows.begin();
        while (iterRow != _widgetRows.end())
        {
            col = 0;

            auto iterCollumn = (*iterRow)._widgetCollumn.begin();
            while (iterCollumn != (*iterRow)._widgetCollumn.end())
            {
                if ((*iterCollumn)._widgetID != iWidget::INVALID_WIDGET_ID)
                {
                    iWidget* widget = iWidgetManager::getInstance().getWidget((*iterCollumn)._widgetID);

                    if (widget != nullptr)
                    {
                        widget->handleMouseMove(x, y);
                    }
                }

                if (x >= (*iterCollumn)._x &&
                    x < (*iterCollumn)._x + (*iterCollumn)._width &&
                    y >= (*iterCollumn)._y &&
                    y < (*iterCollumn)._y + (*iterCollumn)._height)
                {
                    _mouseOverRow = row;
                    _mouseOverCollumn = col;
                }

                iterCollumn++;
                col++;
            }

            iterRow++;
            row++;
        }

        if (isActive())
        {
            if (x >= _posx &&
                x < _posx + _width &&
                y >= _posy &&
                y < _posy + _height)
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
        update();
    }

    void iWidgetGrid::draw()
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        if (!isVisible())
        {
            return;
        }
        int32 row = 0;
        int32 col = 0;

        auto iterRow = _widgetRows.begin();
        while (iterRow != _widgetRows.end())
        {
            col = 0;

            auto iterCollumn = (*iterRow)._widgetCollumn.begin();
            while (iterCollumn != (*iterRow)._widgetCollumn.end())
            {
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
                        iWidgetManager::getInstance().getTheme()->drawGridSelection(_posx + (*iterCollumn)._x, _posy + (*iterCollumn)._y, (*iterCollumn)._width, (*iterCollumn)._height);
                    }
                    else if (drawHighlight)
                    {
                        iWidgetManager::getInstance().getTheme()->drawGridHighlight(_posx + (*iterCollumn)._x, _posy + (*iterCollumn)._y, (*iterCollumn)._width, (*iterCollumn)._height);
                    }
                }

                iWidget* widget = iWidgetManager::getInstance().getWidget((*iterCollumn)._widgetID);

                if (widget != nullptr)
                {
                    widget->calcPosition(_posx + (*iterCollumn)._x, _posy + (*iterCollumn)._y, (*iterCollumn)._width, (*iterCollumn)._height);
                    widget->draw();
                }

                iterCollumn++;
                col++;
            }

            iterRow++;
            row++;
        }
    }

    uint32 iWidgetGrid::getRowCount()
    {
        return static_cast<uint32>(_widgetRows.size());
    }

    uint32 iWidgetGrid::getCollumnCount()
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        return static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());
    }


    void iWidgetGrid::recalcWidthHeight()
    {
        con_assert(!_widgetRows.empty(), "grid can't be empty");

        int32 biggestsize = 0;

        _width = 0;
        _height = 0;

        uint32 rowCount = static_cast<uint32>(_widgetRows.size());
        uint32 collumnCount = static_cast<uint32>(_widgetRows[0]._widgetCollumn.size());

        for (uint32 x = 0; x < collumnCount; ++x)
        {
            biggestsize = 0;

            for (uint32 y = 0; y < rowCount; y++)
            {
                iWidget* widget = iWidgetManager::getInstance().getWidget(_widgetRows[y]._widgetCollumn[x]._widgetID);

                if (widget != nullptr)
                {
                    if (widget->getWidth() > biggestsize)
                    {
                        biggestsize = widget->getWidth();
                    }
                }
            }

            for (uint32 y = 0; y < rowCount; ++y)
            {
                _widgetRows[y]._widgetCollumn[x]._width = biggestsize;
            }

            _width += biggestsize;
        }


        for (uint32 y = 0; y < rowCount; ++y)
        {
            biggestsize = 0;

            for (uint32 x = 0; x < collumnCount; ++x)
            {
                iWidget* widget = iWidgetManager::getInstance().getWidget(_widgetRows[y]._widgetCollumn[x]._widgetID);

                if (widget != nullptr)
                {
                    if (widget->getHeight() > biggestsize)
                    {
                        biggestsize = widget->getHeight();
                    }
                }
            }

            for (uint32 x = 0; x < collumnCount; ++x)
            {
                _widgetRows[y]._widgetCollumn[x]._height = biggestsize;
            }

            _height += biggestsize;
        }

        int32 posx = 0;
        int32 posy = 0;

        for (uint32 x = 0; x < collumnCount; ++x)
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

            for (uint32 x = 0; x < collumnCount; ++x)
            {
                _widgetRows[y]._widgetCollumn[x]._x = posx;

                posx += _widgetRows[y]._widgetCollumn[x]._width + _cellspacing;
            }
        }

        _width += collumnCount*_cellspacing - _cellspacing + _border * 2;
        _height += rowCount*_cellspacing - _cellspacing + _border * 2;
    }

    void iWidgetGrid::addWidget(iWidget* widget)
    {
        addWidget(widget, 0, 0, nullptr);
    }

    void iWidgetGrid::removeWidget(iWidget* widget)
    {
        iWidget::removeWidget(widget);
    }

    void iWidgetGrid::addWidget(iWidget* widget, int32 col, int32 row, void* userData)
    {
        con_assert(_widgetRows.size() > row, "out of range");

        uint64 widgetID = (widget != nullptr) ? widget->getID() : iWidget::INVALID_WIDGET_ID;
            
        if (_widgetRows.size() > row &&
            _widgetRows[row]._widgetCollumn.size() > col)
        {
            if (widgetID != iWidget::INVALID_WIDGET_ID)
            {
                auto iter = find(_widgets.begin(), _widgets.end(), widget);
                if (iter == _widgets.end())
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
                    update();
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