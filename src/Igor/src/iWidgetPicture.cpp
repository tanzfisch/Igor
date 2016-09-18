// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetPicture.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>
#include <iTextureResourceFactory.h>

namespace Igor
{

    iWidgetPicture::iWidgetPicture()
        : iWidget(iWidgetType::Label)
    {
        setHorrizontalAlignment(iHorrizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);
        _reactOnMouseWheel = false;
    }

    iWidgetPicture::~iWidgetPicture()
    {
        _texture = nullptr;
    }

    void iWidgetPicture::setMaxSize(int32 width, int32 height)
    {
        _maxWidth = width;
        _maxHeight = height;
    }

    int32 iWidgetPicture::getMaxWidth()
    {
        return _maxWidth;
    }

    int32 iWidgetPicture::getMaxHeight()
    {
        return _maxHeight;
    }

    void iWidgetPicture::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (isGrowingByContent())
        {
            if (_texture != nullptr)
            {
                minWidth = _texture->getWidth();
                minHeight = _texture->getHeight();
            }
        }

        setMinSize(minWidth, minHeight);

        if (_minWidth > _maxWidth)
        {
            _minWidth = _maxWidth;
        }

        if (_minHeight > _maxHeight)
        {
            _minHeight = _maxHeight;
        }
    }

    void iWidgetPicture::draw()
    {
        if (isVisible() &&
            _texture != nullptr)
        {
            iWidgetManager::getInstance().getTheme()->drawPicture(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), _texture, _widgetAppearanceState, isActive());
        }
    }

    const iaString& iWidgetPicture::getTexture() const
    {
        return _texturePath;
    }

    void iWidgetPicture::setTexture(const iaString& texturePath)
    {
        if (_texturePath != texturePath)
        {
            _texturePath = texturePath;
            _texture = iTextureResourceFactory::getInstance().loadFile(_texturePath);
        }
    }

}
