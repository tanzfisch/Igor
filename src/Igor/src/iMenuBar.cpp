// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iMenuBar.h>

#include <iWidgetLabel.h>
#include <iMenu.h>
#include <iAction.h>
#include <iActionManager.h>
#include <iWidgetManager.h>

namespace Igor
{
    iMenuBar::iMenuBar(iWidgetPtr parent)
        :iWidget(parent)
    {
        init();
    }

    iMenuBar::~iMenuBar()
    {
    }

    void iMenuBar::init()
    {
        _grid = new iWidgetGrid(this);
        _grid->setSelectMode(iSelectionMode::NoSelection);
        _grid->registerOnClickEvent(iClickDelegate(this, &iMenuBar::onClick));
        _grid->registerOnMouseOverEvent(iMouseOverDelegate(this, &iMenuBar::onMouseOver));
    }

    void iMenuBar::onMouseOver(iWidgetPtr source)
    {
        int32 col = static_cast<iWidgetGridPtr>(source)->getMouseOverCollumn();

        con_endl("col " << col);
    }

    void iMenuBar::onClick(iWidgetPtr source)
    {
        int32 col = static_cast<iWidgetGridPtr>(source)->getSelectedCollumn();
        con_endl("click col " << col);
        
        iActionPtr action = iActionManager::getInstance().getAction(_actions[col]);
        action->execute();

        _grid->block(true);
        _grid->unSelect();
        _grid->block(false);
    }

    void iMenuBar::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (isGrowingByContent() &&
            !_children.empty())
        {
            iWidgetPtr widget = iWidgetManager::getInstance().getWidget(*_children.begin());
            minWidth = widget->getMinWidth();
            minHeight = widget->getMinHeight();
        }

        setMinSize(minWidth, minHeight);
    }

    void iMenuBar::addAction(const iActionPtr action)
    {
        if (!iActionManager::getInstance().isRegistered(action))
        {
            con_err("can't add unregistered actions");
            return;
        }

        if (!_actions.empty())
        {
            _grid->appendCollumns(1);
        }

        _actions.push_back(action->getID());

        iWidgetGridPtr actionGrid = new iWidgetGrid();
        _grid->addWidget(actionGrid, _grid->getColumnCount() - 1, 0);

        iWidgetLabelPtr textLabel = new iWidgetLabel();
        textLabel->setText(action->getText());
        actionGrid->addWidget(textLabel, 0, 0);
        actionGrid->setBorder(4);
    }

    void iMenuBar::addAction(const iaString& actionName)
    {
        addAction(iActionManager::getInstance().getAction(actionName));
    }

}