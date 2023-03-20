// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlAction.h>

#include <igor/ui/actions/iActionManager.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetPicture.h>
#include <igor/ui/widgets/iWidgetGrid.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iUserControlAction::iUserControlAction(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlAction, parent)
    {
        init();
    }

    void iUserControlAction::init()
    {
        setIgnoreChildEventHandling();

        iWidgetGridPtr actionGrid = new iWidgetGrid(this);
        actionGrid->setHorizontalAlignment(iHorizontalAlignment::Strech);
        actionGrid->appendColumns(1);
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
        if (_action == nullptr)
        {
            return;
        }

        _action->execute(*_actionContext);
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
        if (_action == nullptr)
        {
            _textLabel->setText("");
            _picture->setTexture("");
            _picture->setSize(_fixPictureSize ? 16 : 0, 16);
            _picture->setMaxSize(_fixPictureSize ? 16 : 0, 16);
        }
        else
        {
            _textLabel->setText(_action->getDescription());
            _picture->setTexture(_action->getPicturePath());
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

    void iUserControlAction::setAction(const iActionPtr action, const iActionContextPtr context)
    {
        if (!iActionManager::getInstance().isRegistered(action))
        {
            con_err("can't use unregistered action");
            return;
        }

        _action = action;
        _actionContext = context;

        update();
    }

    iActionContextPtr iUserControlAction::getActionContext() const
    {
        return _actionContext;
    }

    iActionPtr iUserControlAction::getAction() const
    {
        return _action;
    }

} // namespace igor