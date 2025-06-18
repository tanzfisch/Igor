// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlSprite.h>

#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/resources/iResourceManager.h>
#include <igor/resources/texture/iThumbnailCache.h>
#include <igor/data/iMimeData.h>
#include <igor/ui/iDrag.h>

namespace igor
{

    iUserControlSprite::iUserControlSprite(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlSprite, parent)
    {
        setAcceptDrop(true);

        initGUI();
    }

    iUserControlSprite::~iUserControlSprite()
    {
    }

    void iUserControlSprite::initGUI()
    {
        iWidgetBoxLayoutPtr mainLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);

        iWidgetBoxLayoutPtr pictureLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, mainLayout);
        _picture = new iWidgetPicture(pictureLayout);
        _picture->setKeepAspectRatio(false);
        _picture->setMaxSize(64, 64);
        _picture->setMinSize(64, 64);

        iWidgetBoxLayoutPtr spriteLabelLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, pictureLayout);
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

        auto labelTexture = new iWidgetLabel(spriteLabelLayout);
        labelTexture->setText("Texture");
        labelTexture->setHorizontalAlignment(iHorizontalAlignment::Left);
        labelTexture->setVerticalAlignment(iVerticalAlignment::Top);

        _labelTextureID = new iWidgetLabel(spriteLabelLayout);
        _labelTextureID->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelTextureID->setVerticalAlignment(iVerticalAlignment::Top);

        _labelTextureAlias = new iWidgetLabel(spriteLabelLayout);
        _labelTextureAlias->setHorizontalAlignment(iHorizontalAlignment::Left);
        _labelTextureAlias->setVerticalAlignment(iVerticalAlignment::Top);
    }

    void iUserControlSprite::setID(iResourceID spriteID)
    {
        _spriteID = spriteID;
        _picture->setTexture(iTexturePtr());
        _labelID->setText("");
        _labelAlias->setText("");

        if (!_spriteID.isValid())
        {
            return;
        }

        auto sprite = iResourceManager::getInstance().loadResource<iSprite>(_spriteID);
        if (sprite == nullptr)
        {
            return;
        }

        auto texture = sprite->getTexture();
        iResourceID textureID;
        if (texture != nullptr)
        {
            textureID = texture->getID();
            _picture->setTexture(iThumbnailCache::getInstance().getThumbnail(textureID));
        }
        else
        {
            _picture->setTexture(texture);
        }

        _labelID->setText(_spriteID.toString());
        _labelAlias->setText(iResourceManager::getInstance().getAlias(_spriteID));
        _labelTextureID->setText(textureID.toString());
        _labelTextureAlias->setText(iResourceManager::getInstance().getAlias(textureID));

        _change(this);
    }

    iResourceID iUserControlSprite::getID() const
    {
        return _spriteID;
    }

    void iUserControlSprite::onDragMove(iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID())
        {
            drag.reject();
            return;
        }

        iResourceID id = mimeData.getResourceID();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType != IGOR_RESOURCE_SPRITE &&
            resourceType != IGOR_RESOURCE_TEXTURE)
        {
            drag.reject();
            return;
        }

        drag.accept();
    }

    void iUserControlSprite::onDrop(const iDrag &drag, const iaVector2f &mousePos)
    {
        const iMimeData &mimeData = drag.getMimeData();
        if (!mimeData.hasResourceID())
        {
            return;
        }

        iResourceID id = mimeData.getResourceID();

        const iaString resourceType = iResourceManager::getInstance().getType(id);
        if (resourceType == IGOR_RESOURCE_SPRITE)
        {
            setID(id);
        }
    }

} // namespace igor