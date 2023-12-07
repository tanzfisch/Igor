// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetPicture.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/iResourceManager.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	iWidgetPicture::iWidgetPicture(const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetPicture, iWidgetKind::Widget, parent)
	{
		setHorizontalAlignment(iHorizontalAlignment::Center);
		setVerticalAlignment(iVerticalAlignment::Center);
		setForeground(iaColor4f::white);
		_reactOnMouseWheel = false;
	}

	void iWidgetPicture::setMaxSize(int32 width, int32 height)
	{
		_maxWidth = width;
		_maxHeight = height;
	}

	bool iWidgetPicture::hasTexture() const
	{
		return _texture != nullptr;
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
		int32 minWidth = _configuredMinWidth;
		int32 minHeight = _configuredMinHeight;

		if (isGrowingByContent())
		{
			if (_texture != nullptr)
			{
				minWidth = _texture->getWidth();
				minHeight = _texture->getHeight();
			}
		}

		updateMinSize(minWidth, minHeight);

		float32 aspect = static_cast<float32>(minHeight) / static_cast<float32>(minWidth);

		if (_minWidth > _maxWidth)
		{
			_minWidth = _maxWidth;

			if (_keepAspectRatio)
			{
				_minHeight = static_cast<int32>(static_cast<float32>(_minWidth) * aspect);
			}
		}

		if (_minHeight > _maxHeight)
		{
			_minHeight = _maxHeight;

			if (_keepAspectRatio)
			{
				_minWidth = static_cast<int32>(static_cast<float32>(_minHeight) * (1.0f / aspect));
			}
		}
	}

	void iWidgetPicture::setKeepAspectRatio(bool keep)
	{
		_keepAspectRatio = keep;
	}

	bool iWidgetPicture::getKeepAspectRatio() const
	{
		return _keepAspectRatio;
	}

	void iWidgetPicture::draw()
	{
		if (!isVisible() ||
			_texture == nullptr)
		{
			return;
		}

		iWidgetManager::getInstance().getTheme()->drawWidgetPicture(this);

        for (const auto child : _children)
        {
            child->draw();
        }		
	}

	iTexturePtr iWidgetPicture::getTexture() const
	{
		return _texture;
	}

	void iWidgetPicture::setTexture(iTexturePtr texture)
	{
		_texture = texture;
	}

    void iWidgetPicture::setTexture(const iaString &textureAlias)
    {
        setTexture(iResourceManager::getInstance().loadResource<iTexture>(textureAlias));
    }

} // namespace igor
