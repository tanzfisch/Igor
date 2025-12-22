// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlTexture.h>

#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/resources/iResourceManager.h>
#include <igor/resources/texture/iThumbnailCache.h>
#include <igor/resources/texture/iTexture.h>
#include <igor/data/iMimeData.h>
#include <igor/ui/iDrag.h>

namespace igor
{

    iUserControlTexture::iUserControlTexture(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlTexture, parent)
    {
        setAcceptDrop(true);

        onInitUI();
    }

    iUserControlTexture::~iUserControlTexture()
    {
    }

    void iUserControlTexture::onInitUI()
    {
        iWidgetBoxLayoutPtr layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, this);        
        _picture = new iWidgetPicture(layout);
        _picture->setKeepAspectRatio(false);
        _picture->setMaxSize(128, 128);
        _picture->setMinSize(128, 128);

        iWidgetBoxLayoutPtr labelLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, layout);
        _labelID = new iWidgetLabel(labelLayout);
        _labelID->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelID->setVerticalAlignment(iVerticalAlignment::Top);

        _labelAlias = new iWidgetLabel(labelLayout);
        _labelAlias->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelAlias->setVerticalAlignment(iVerticalAlignment::Top);
    }

    void iUserControlTexture::setID(iResourceID textureID)
    {
        _textureID = textureID;

        if (!_textureID.isValid())
        {
            _picture->setTexture(iTexturePtr());
            _labelID->setText("");
            _labelAlias->setText("");
            return;
        }

        _picture->setTexture(iThumbnailCache::getInstance().getThumbnail(_textureID));
        _labelID->setText(textureID.toString());
        _labelAlias->setText(iResourceManager::getInstance().getAlias(_textureID));

        _change(this);
    }

    const iResourceID& iUserControlTexture::getID() const
    {
        return _textureID;
    }

    void iUserControlTexture::onDragMove(iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID())
        {
            drag.reject();
            return;
        }

        iResourceID id = mimeData.getResourceID();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType != IGOR_RESOURCE_TEXTURE)
        {
            drag.reject();
            return;
        }

        drag.accept();
    }

    void iUserControlTexture::onDrop(const iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID())
        {
            return;
        }

        iResourceID id = mimeData.getResourceID();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType != IGOR_RESOURCE_TEXTURE)
        {
            return;
        }

        setID(id);
    }

} // namespace igor