// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetButton.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/iResourceManager.h>
#include <igor/ui/actions/iActionManager.h>

namespace igor
{
    iWidgetButton::iWidgetButton(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetButton, iWidgetKind::Widget, parent)
    {
        _configuredHeight = 10;
        _configuredWidth = 20;
        _reactOnMouseWheel = false;
    }

    iWidgetButton::~iWidgetButton()
    {
        _texture = nullptr;
    }

    void iWidgetButton::setAction(const iaString &actionName, const iActionContextPtr context)
    {
        setAction(iActionManager::getInstance().getAction(actionName), context);
    }

    void iWidgetButton::setAction(const iActionPtr action, const iActionContextPtr context)
    {
        if (!iActionManager::getInstance().isRegistered(action))
        {
            con_err("can't use unregistered action");
            return;
        }

        if (_action != nullptr)
        {
            unregisterOnClickEvent(iClickDelegate(this, &iWidgetButton::onInternalClick));
        }

        _action = action;
        _actionContext = context;

        if (_action == nullptr)
        {
            return;
        }

        registerOnClickEvent(iClickDelegate(this, &iWidgetButton::onInternalClick));

        setText(_action->getBrief());
        setTooltip(_action->getDescription());
        setIcon(_action->getIcon());
    }

    void iWidgetButton::onInternalClick(const iWidgetPtr source)
    {
        if (_action == nullptr)
        {
            return;
        }

        _action->execute(*_actionContext);
    }

    iActionContextPtr iWidgetButton::getActionContext() const
    {
        return _actionContext;
    }

    iActionPtr iWidgetButton::getAction() const
    {
        return _action;
    }

    void iWidgetButton::setText(const iaString &text)
    {
        _text = text;
    }

    const iaString &iWidgetButton::getText() const
    {
        return _text;
    }

    void iWidgetButton::setIcon(const iaString &iconAlias)
    {
        setIcon(iResourceManager::getInstance().loadResource<iTexture>(iconAlias));
    }

    void iWidgetButton::setIcon(iTexturePtr texture)
    {
        _iconTexture = texture;
    }    

    void iWidgetButton::setTexture(const iaString &textureAlias)
    {
        setTexture(iResourceManager::getInstance().loadResource<iTexture>(textureAlias));
    }

    void iWidgetButton::setTexture(iTexturePtr texture)
    {
        _texture = texture;
    }

    void iWidgetButton::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (isGrowingByContent())
        {
            if (!_text.isEmpty())
            {
                float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
                int32 textWidth = static_cast<int32>(iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text, fontSize));

                minWidth = static_cast<int32>(static_cast<float32>(textWidth) + fontSize * 2.5f);
                minHeight = static_cast<int32>(fontSize * 1.5f);
            }
            else if (_texture != nullptr)
            {
                // we don't actually want it to scale with the texture size since the texture is considered a background
                minWidth = 16;
                minHeight = 16;
            }
        }

        setMinSize(minWidth, minHeight);
    }

    iHorizontalAlignment iWidgetButton::getHorizontalTextAlignment() const
    {
        return _horizontalTextAlignment;
    }

    iVerticalAlignment iWidgetButton::getVerticalTextAlignment() const
    {
        return _verticalTextAlignment;
    }

    void iWidgetButton::setHorizontalTextAlignment(iHorizontalAlignment align)
    {
        _horizontalTextAlignment = align;
    }

    void iWidgetButton::setVerticalTextAlignment(iVerticalAlignment valign)
    {
        _verticalTextAlignment = valign;
    }

    void iWidgetButton::draw()
    {
        if (isVisible())
        {
            iWidgetManager::getInstance().getTheme()->drawButton(getActualRect(), _text, _horizontalTextAlignment, _verticalTextAlignment, _texture, _iconTexture, getState(), isEnabled());
        }
    }

} // namespace igor