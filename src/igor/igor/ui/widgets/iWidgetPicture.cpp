// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetPicture.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetBaseTheme.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/texture/iTextureResourceFactory.h>

namespace igor
{

	iWidgetPicture::iWidgetPicture(const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetPicture, iWidgetKind::Widget, parent)
	{
		setHorizontalAlignment(iHorizontalAlignment::Center);
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
		int32 minWidth = _configuredWidth;
		int32 minHeight = _configuredHeight;

		if (isGrowingByContent())
		{
			if (_texture != nullptr)
			{
				minWidth = _texture->getWidth();
				minHeight = _texture->getHeight();
			}
		}

		setMinSize(minWidth, minHeight);

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
		if (isVisible() &&
			_texture != nullptr)
		{
			iWidgetManager::getInstance().getTheme()->drawPicture(getActualRect(), _texture, _widgetState, isActive());
		}
	}

	const iaString &iWidgetPicture::getTexture() const
	{
		return _texturePath;
	}

	void iWidgetPicture::setTexture(const iaString &texturePath)
	{
		if (_texturePath != texturePath)
		{
			_texturePath = texturePath;
			_texture = iTextureResourceFactory::getInstance().loadFile(_texturePath);
		}
	}

} // namespace igor
