// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iUserControlAction.h>

#include <iActionManager.h>
#include <iWidgetLabel.h>
#include <iWidgetPicture.h>
#include <iWidgetGrid.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	iUserControlAction::iUserControlAction(const iWidgetPtr parent)
        : iUserControl(parent)
	{
        init();        
	}

    void iUserControlAction::init()
    {
        setIgnoreChildEventHandling();

        iWidgetGridPtr actionGrid = new iWidgetGrid(this);
        actionGrid->setHorizontalAlignment(iHorizontalAlignment::Strech);
        actionGrid->appendCollumns(1);
        actionGrid->setStrechColumn(1);

        _picture = new iWidgetPicture();
        _picture->setSize(16, 16);
        _picture->setMaxSize(16, 16);
        _picture->setKeepAspectRatio(false);
        actionGrid->addWidget(_picture, 0, 0);

        _textLabel = new iWidgetLabel();
        _textLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
        actionGrid->addWidget(_textLabel, 1, 0);
        actionGrid->setCellSpacing(4);

        actionGrid->registerOnClickEvent(iClickDelegate(this, &iUserControlAction::onClick));
    }

    void iUserControlAction::onClick(const iWidgetPtr source)
    {
        iActionPtr action = getAction();

        if (action == nullptr)
        {
            return;
        }

        action->execute();
    }

    void iUserControlAction::setFixedPictureSize(bool value)
    {
        _fixPictureSize = value;

        update();
    }

    bool iUserControlAction::getFixedPictureSize() const
    {
        return _fixPictureSize;
    }

    void iUserControlAction::update()
    {
        iActionPtr action = iActionManager::getInstance().getAction(_actionID);

        if (action == nullptr)
        {
            _textLabel->setText("");
            _picture->setTexture("");
            _picture->setSize(_fixPictureSize ? 16 : 0, 16);
            _picture->setMaxSize(_fixPictureSize ? 16 : 0, 16);
        }
        else
        {
            _textLabel->setText(action->getText());
            _picture->setTexture(action->getPicturePath());
            if (_picture->hasTexture())
            {
                _picture->setSize(16, 16);
                _picture->setMaxSize(16, 16);
            }
            else
            {
                _picture->setSize(_fixPictureSize ? 16 : 0, 16);
                _picture->setMaxSize(_fixPictureSize ? 16 : 0, 16);
            }
        }
    }

    void iUserControlAction::setAction(const iActionPtr action)
    {
        con_assert(action != nullptr, "zero pointer");

        if (action == nullptr)
        {
            return;
        }

        _actionID = action->getID();

        update();
    }

    iActionPtr iUserControlAction::getAction() const
    {
        return iActionManager::getInstance().getAction(_actionID);
    }

}