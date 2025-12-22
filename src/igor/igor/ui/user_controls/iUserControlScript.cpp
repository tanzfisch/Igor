// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlScript.h>

#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/resources/iResourceManager.h>
#include <igor/data/iMimeData.h>
#include <igor/ui/iDrag.h>

namespace igor
{

    iUserControlScript::iUserControlScript(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlScript, parent)
    {
        setAcceptDrop(true);

        onInitUI();
    }

    iUserControlScript::~iUserControlScript()
    {
    }

    void iUserControlScript::onInitUI()
    {
        iWidgetBoxLayoutPtr mainLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);

        iWidgetBoxLayoutPtr spriteLabelLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, mainLayout);
        auto labelSprite = new iWidgetLabel(spriteLabelLayout);
        labelSprite->setText("Sprite");
        labelSprite->setHorizontalAlignment(iHorizontalAlignment::Left);
        labelSprite->setVerticalAlignment(iVerticalAlignment::Top);

        _labelID = new iWidgetLabel(spriteLabelLayout);
        _labelID->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelID->setVerticalAlignment(iVerticalAlignment::Top);

        _labelAlias = new iWidgetLabel(spriteLabelLayout);
        _labelAlias->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelAlias->setVerticalAlignment(iVerticalAlignment::Top);

        _labelSrcFile = new iWidgetLabel(spriteLabelLayout);
        _labelSrcFile->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelSrcFile->setVerticalAlignment(iVerticalAlignment::Top);

        // TODO add edit button?
    }

    void iUserControlScript::setID(iResourceID scriptID)
    {
        _scriptID = scriptID;
        _labelID->setText("");
        _labelAlias->setText("");
        _labelSrcFile->setText("");

        if (!_scriptID.isValid())
        {
            return;
        }

        auto script = iResourceManager::getInstance().loadResource<iSprite>(_scriptID);
        if (script == nullptr)
        {
            return;
        }

        _labelID->setText(_scriptID.toString());
        _labelAlias->setText(iResourceManager::getInstance().getAlias(_scriptID));
        _labelSrcFile->setText(script->getSource());

        _change(this);
    }

    const iResourceID& iUserControlScript::getID() const
    {
        return _scriptID;
    }

    void iUserControlScript::onDragMove(iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID())
        {
            drag.reject();
            return;
        }

        iResourceID id = mimeData.getResourceID();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType != IGOR_RESOURCE_SCRIPT)
        {
            drag.reject();
            return;
        }

        drag.accept();
    }

    void iUserControlScript::onDrop(const iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID())
        {
            return;
        }

        iResourceID id = mimeData.getResourceID();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType == IGOR_RESOURCE_SCRIPT)
        {
            setID(id);
        }
    }

} // namespace igor