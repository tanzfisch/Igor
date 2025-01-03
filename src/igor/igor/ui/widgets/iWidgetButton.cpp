// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
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
        _configuredMinHeight = 10;
        _configuredMinWidth = 20;
        _reactOnMouseWheel = false;

        setBackground(iaColor4f::white);
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
        if (iconAlias.isEmpty())
        {
            _iconTexture = nullptr;
        }
        else
        {
            setIcon(iResourceManager::getInstance().loadResource<iTexture>(iconAlias));
        }
    }

    void iWidgetButton::setIcon(iTexturePtr texture)
    {
        _iconTexture = texture;
    }

    iTexturePtr iWidgetButton::getIcon() const
    {
        return _iconTexture;
    }

    iTexturePtr iWidgetButton::getBackgroundTexture() const
    {
        return _texture;
    }

    void iWidgetButton::setBackgroundTexture(const iaString &textureAlias)
    {
        setBackgroundTexture(iResourceManager::getInstance().loadResource<iTexture>(textureAlias));
    }

    void iWidgetButton::setBackgroundTexture(iTexturePtr texture)
    {
        _texture = texture;
    }

    void iWidgetButton::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (isGrowingByContent())
        {
            if (_texture != nullptr)
            {
                // we don't actually want it to scale with the texture size since the texture is considered a background
                minWidth = 16;
                minHeight = 16;
            }
            else if (!_text.isEmpty())
            {
                const float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
                const int32 textWidth = static_cast<int32>(iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text, fontSize));

                if (_iconTexture != nullptr)
                {
                    minWidth = static_cast<int32>(static_cast<float32>(textWidth) + fontSize * 2.5f);
                }
                else
                {
                    minWidth = static_cast<int32>(static_cast<float32>(textWidth) + fontSize);
                }

                minHeight = static_cast<int32>(fontSize * 1.5f);
            }

            minWidth = std::max(minWidth, _configuredMinWidth);
            minHeight = std::max(minHeight, _configuredMinHeight);
        }

        updateMinSize(minWidth, minHeight);
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
        if (!isVisible())
        {
            return;
        }

        iWidgetManager::getInstance().getTheme()->drawWidgetButton(this);
    }

    void iWidgetButton::setCheckable(bool checkable)
    {
        _checkable = checkable;
    }

    bool iWidgetButton::isCheckable() const
    {
        return _checkable;
    }

    void iWidgetButton::setChecked(bool check)
    {
        _checked = check;
    }

    bool iWidgetButton::isChecked() const
    {
        return _checked;
    }

} // namespace igor