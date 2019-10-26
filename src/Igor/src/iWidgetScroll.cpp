// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetScroll.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iMaterialResourceFactory.h>
#include <iUserControl.h>
#include <iMaterial.h>
#include <iRenderer.h>
#include <iIntersection.h>
#include <iTextureResourceFactory.h>

#include <iaConsole.h>
#include <iaColor4.h>
#include <iaMatrix.h>
using namespace IgorAux;

namespace Igor
{

	static const int32 BORDER_WIDTH = 2;
	static const int32 BORDER_WIDTH2 = 4;

	iWidgetScroll::iWidgetScroll(const iWidgetPtr parent)
		: iWidget(parent)
	{
		setGrowingByContent(false);

		// get some resources
		_upTexture = iTextureResourceFactory::getInstance().loadFile("icons/up.png");
		_downTexture = iTextureResourceFactory::getInstance().loadFile("icons/down.png");
		_leftTexture = iTextureResourceFactory::getInstance().loadFile("icons/left.png");
		_rightTexture = iTextureResourceFactory::getInstance().loadFile("icons/right.png");
	}

	bool iWidgetScroll::handleMouseKeyUp(iKeyCode key)
	{
		if (!isActive())
		{
			return false;
		}

		if (key == iKeyCode::MouseLeft)
		{
			if (_hscrollButton._mouseOver)
			{
				_hscrollButton._appearanceState = iWidgetState::Clicked;
				_hscrollButton._mouseDown = false;
				return true;
			}

			if (_vscrollButton._mouseOver)
			{
				_vscrollButton._appearanceState = iWidgetState::Clicked;
				_vscrollButton._mouseDown = false;
				return true;
			}

			if (_leftButton._mouseOver)
			{
				_leftButton._appearanceState = iWidgetState::Clicked;
				_leftButton._mouseDown = false;
				return true;
			}

			if (_rightButton._mouseOver)
			{
				_rightButton._appearanceState = iWidgetState::Clicked;
				_rightButton._mouseDown = false;
				return true;
			}

			if (_upButton._mouseOver)
			{
				_upButton._appearanceState = iWidgetState::Clicked;
				_upButton._mouseDown = false;
				return true;
			}

			if (_downButton._mouseOver)
			{
				_downButton._appearanceState = iWidgetState::Clicked;
				_downButton._mouseDown = false;
				return true;
			}
		}

		return iWidget::handleMouseKeyUp(key);
	}

	bool iWidgetScroll::handleMouseDoubleClick(iKeyCode key)
	{
		if (!isActive())
		{
			return false;
		}

		if (key == iKeyCode::MouseLeft)
		{
			if (handleButtonClicks())
			{
				return true;
			}
		}

		return iWidget::handleMouseDoubleClick(key);
	}

	bool iWidgetScroll::handleMouseKeyDown(iKeyCode key)
	{
		if (!isActive())
		{
			return false;
		}

		if (key == iKeyCode::MouseLeft)
		{
			if (_hscrollButton._mouseOver)
			{
				_hscrollButton._appearanceState = iWidgetState::Pressed;
				_hscrollButton._mouseDown = true;
				return true;
			}

			if (_vscrollButton._mouseOver)
			{
				_vscrollButton._appearanceState = iWidgetState::Pressed;
				_vscrollButton._mouseDown = true;
				return true;
			}

			if (handleButtonClicks())
			{
				return true;
			}
		}

		return iWidget::handleMouseKeyDown(key);
	}

	bool iWidgetScroll::handleButtonClicks()
	{
		auto child = _children.front();
		if (child == nullptr)
		{
			return false;
		}

		if (_leftButton._mouseOver)
		{
			_leftButton._appearanceState = iWidgetState::Pressed;
			_leftButton._mouseDown = true;

			_hscroll -= 1.0f / (child->getActualWidth() / getActualWidth());

			if (_hscroll < 0.0f)
			{
				_hscroll = 0.0f;
			}

			return true;
		}

		if (_rightButton._mouseOver)
		{
			_rightButton._appearanceState = iWidgetState::Pressed;
			_rightButton._mouseDown = true;

			_hscroll += 1.0f / (child->getActualWidth() / getActualWidth());

			if (_hscroll > 1.0f)
			{
				_hscroll = 1.0f;
			}

			return true;
		}

		if (_upButton._mouseOver)
		{
			_upButton._appearanceState = iWidgetState::Pressed;
			_upButton._mouseDown = true;

			_vscroll -= 1.0f / (child->getActualHeight() / getActualHeight());

			if (_vscroll < 0.0f)
			{
				_vscroll = 0.0f;
			}

			return true;
		}

		if (_downButton._mouseOver)
		{
			_downButton._appearanceState = iWidgetState::Pressed;
			_downButton._mouseDown = true;

			_vscroll += 1.0f / (child->getActualHeight() / getActualHeight());

			if (_vscroll > 1.0f)
			{
				_vscroll = 1.0f;
			}

			return true;
		}

		return false;
	}

	void iWidgetScroll::handleMouseMove(const iaVector2i& pos)
	{
		if (!isActive())
		{
			return;
		}

		auto child = _children.front();
		if (child == nullptr)
		{
			return;
		}

		child->handleMouseMove(pos);

		if (_hscrollButton._mouseDown)
		{
			if (!_children.empty())
			{
				_hscroll += static_cast<float32>(iMouse::getInstance().getPosDelta()._x) / static_cast<float32>(calcHorizontalScrollSpace() - _hscrollButton._rectangle._width);
				if (_hscroll < 0.0f)
				{
					_hscroll = 0.0f;
				}

				if (_hscroll > 1.0f)
				{
					_hscroll = 1.0f;
				}
			}
		}

		if (_vscrollButton._mouseDown)
		{
			if (!_children.empty())
			{
				_vscroll += static_cast<float32>(iMouse::getInstance().getPosDelta()._y) / static_cast<float32>(calcVerticalScrollSpace() - _vscrollButton._rectangle._height);
				if (_vscroll < 0.0f)
				{
					_vscroll = 0.0f;
				}

				if (_vscroll > 1.0f)
				{
					_vscroll = 1.0f;
				}
			}
		}

		if (pos._x >= _absoluteX &&
			pos._x < _absoluteX + _actualWidth &&
			pos._y >= _absoluteY &&
			pos._y < _absoluteY + _actualHeight)
		{
			if (!_isMouseOver)
			{
				_widgetState = iWidgetState::Highlighted;
				_mouseOver(this);
			}

			_isMouseOver = true;

			if (!_hscrollButton._mouseDown)
			{
				if (iIntersection::intersects(pos, _hscrollButton._rectangle))
				{
					_hscrollButton._appearanceState = iWidgetState::Highlighted;
					_hscrollButton._mouseOver = true;
				}
				else
				{
					_hscrollButton._appearanceState = iWidgetState::Standby;
					_hscrollButton._mouseOver = false;
				}
			}

			if (!_vscrollButton._mouseDown)
			{
				if (iIntersection::intersects(pos, _vscrollButton._rectangle))
				{
					_vscrollButton._appearanceState = iWidgetState::Highlighted;
					_vscrollButton._mouseOver = true;
				}
				else
				{
					_vscrollButton._appearanceState = iWidgetState::Standby;
					_vscrollButton._mouseOver = false;
				}
			}

			if (iIntersection::intersects(pos, _leftButton._rectangle))
			{
				_leftButton._appearanceState = iWidgetState::Highlighted;
				_leftButton._mouseOver = true;
			}
			else
			{
				_leftButton._appearanceState = iWidgetState::Standby;
				_leftButton._mouseOver = false;
			}

			if (iIntersection::intersects(pos, _rightButton._rectangle))
			{
				_rightButton._appearanceState = iWidgetState::Highlighted;
				_rightButton._mouseOver = true;
			}
			else
			{
				_rightButton._appearanceState = iWidgetState::Standby;
				_rightButton._mouseOver = false;
			}

			if (iIntersection::intersects(pos, _upButton._rectangle))
			{
				_upButton._appearanceState = iWidgetState::Highlighted;
				_upButton._mouseOver = true;
			}
			else
			{
				_upButton._appearanceState = iWidgetState::Standby;
				_upButton._mouseOver = false;
			}

			if (iIntersection::intersects(pos, _downButton._rectangle))
			{
				_downButton._appearanceState = iWidgetState::Highlighted;
				_downButton._mouseOver = true;
			}
			else
			{
				_downButton._appearanceState = iWidgetState::Standby;
				_downButton._mouseOver = false;
			}
		}
		else
		{
			if (_isMouseOver)
			{
				_widgetState = iWidgetState::Standby;

				_leftButton._appearanceState = iWidgetState::Standby;
				_leftButton._mouseOver = false;
				_rightButton._appearanceState = iWidgetState::Standby;
				_rightButton._mouseOver = false;
				_upButton._appearanceState = iWidgetState::Standby;
				_upButton._mouseOver = false;
				_downButton._appearanceState = iWidgetState::Standby;
				_downButton._mouseOver = false;

				_mouseOff(this);
			}

			_isMouseOver = false;
		}
	}

	void iWidgetScroll::setHorizontalScroll(float32 value)
	{
		con_assert(value >= 0.0f && value <= 1.0f, "out od range");
		_hscroll = value;
	}

	void iWidgetScroll::setVerticalScroll(float32 value)
	{
		con_assert(value >= 0.0f && value <= 1.0f, "out od range");
		_vscroll = value;
	}

	bool iWidgetScroll::handleMouseWheel(int32 d)
	{
		if (!isActive())
		{
			return false;
		}

		const auto child = _children.front();
		if (child == nullptr)
		{
			return false;
		}

		if (child->handleMouseWheel(d))
		{
			return true;
		}

		if (_vscrollActive)
		{
			_vscroll -= d * (1.0f / (child->getActualHeight() / getActualHeight()));
			if (_vscroll < 0.0f)
			{
				_vscroll = 0.0f;
			}

			if (_vscroll > 1.0f)
			{
				_vscroll = 1.0f;
			}

			return true;
		}
		else if (_hscrollActive)
		{
			_hscroll -= d * (1.0f / (child->getActualWidth() / getActualWidth()));
			if (_hscroll < 0.0f)
			{
				_hscroll = 0.0f;
			}

			if (_hscroll > 1.0f)
			{
				_hscroll = 1.0f;
			}

			return true;
		}

		return false;
	}

	int32 iWidgetScroll::getScollbarWidth()
	{
		return _scrollbarWidth;
	}

	void iWidgetScroll::setScollbarWidth(int32 width)
	{
		_scrollbarWidth = width;
	}

	void iWidgetScroll::calcMinSize()
	{
		setMinSize(0, 0);
	}

	void iWidgetScroll::calcButtons()
	{
		const auto child = _children.front();
		if (child == nullptr)
		{
			return;
		}

		int32 childWidth = child->getActualWidth();
		int32 childHeight = child->getActualHeight();

		if (_vscrollActive) // v scrollbar
		{
			int32 scrollspace = calcVerticalScrollSpace();

			_vscrollButton._rectangle.setHeight(static_cast<int32>((static_cast<float32>(getActualHeight()) / static_cast<float32>(childHeight)) * static_cast<float32>(scrollspace)));
			_vscrollButton._rectangle.setX(getActualPosX() + getActualWidth() - _scrollbarWidth - 2);
			_vscrollButton._rectangle.setY(getActualPosY() + static_cast<int32>(static_cast<float32>(scrollspace - _vscrollButton._rectangle.getHeight()) * _vscroll + 3.0f + static_cast<float32>(_scrollbarWidth)));
			_vscrollButton._rectangle.setWidth(_scrollbarWidth);
		}

		if (_hscrollActive) // h scrollbar
		{
			int32 scrollspace = calcHorizontalScrollSpace();

			_hscrollButton._rectangle.setWidth(static_cast<int32>((static_cast<float32>(getActualWidth()) / static_cast<float32>(childWidth)) * static_cast<float32>(scrollspace)));
			_hscrollButton._rectangle.setX(getActualPosX() + static_cast<int32>(static_cast<float32>(scrollspace - _hscrollButton._rectangle.getWidth()) * _hscroll + 4.0f + static_cast<float32>(_scrollbarWidth)));
			_hscrollButton._rectangle.setY(getActualPosY() + getActualHeight() - _scrollbarWidth - 2);
			_hscrollButton._rectangle.setHeight(_scrollbarWidth);
		}

		_leftButton._rectangle.set(getActualPosX() + 2, getActualPosY() + getActualHeight() - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth);
		_upButton._rectangle.set(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + 2, _scrollbarWidth, _scrollbarWidth);

		if (_vscrollActive && _hscrollActive)
		{
			_rightButton._rectangle.set(getActualPosX() + getActualWidth() - _scrollbarWidth * 2 - 2, getActualPosY() + getActualHeight() - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth);
			_downButton._rectangle.set(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + getActualHeight() - 2 - _scrollbarWidth * 2, _scrollbarWidth, _scrollbarWidth);
		}
		else if (_hscrollActive)
		{
			_rightButton._rectangle.set(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + getActualHeight() - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth);
		}
		else if (_vscrollActive)
		{
			_downButton._rectangle.set(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + getActualHeight() - 2 - _scrollbarWidth, _scrollbarWidth, _scrollbarWidth);
		}
	}

	int32 iWidgetScroll::calcHorizontalScrollSpace() const
	{
		if (_vscrollActive)
		{
			return getActualWidth() - 6 - _scrollbarWidth * 3;
		}
		else
		{
			return getActualWidth() - 6 - _scrollbarWidth * 2;
		}
	}

	int32 iWidgetScroll::calcVerticalScrollSpace() const
	{
		if (_hscrollActive)
		{
			return getActualHeight() - 6 - _scrollbarWidth * 3;
		}
		else
		{
			return getActualHeight() - 6 - _scrollbarWidth * 2;
		}
	}

	void iWidgetScroll::calcChildOffsets(std::vector<iRectanglei>& offsets)
	{
		offsets.clear();
		offsets.resize(_children.size());

		if (_children.empty())
		{
			return;
		}

		const auto child = _children.front();
		if (child == nullptr)
		{
			return;
		}

		iRectanglei clientRect;

		float32 offsetX = 0;
		float32 offsetY = 0;

		if (_hscrollActive)
		{
			offsetX = _hscroll * (child->getConfiguredWidth() - getActualWidth() + 4);
		}

		if (_vscrollActive)
		{
			offsetY = _vscroll * (child->getConfiguredHeight() - getActualHeight() + 4);
		}

		clientRect.setX(offsetX);
		clientRect.setY(offsetY);
		clientRect.setWidth(getActualWidth());
		clientRect.setHeight(getActualHeight());

		offsets[0] = clientRect;
	}

	void iWidgetScroll::updateAlignment(int32 clientWidth, int32 clientHeight)
	{
		iWidget::updateAlignment(clientWidth, clientHeight);
	}

	void iWidgetScroll::calcChildFrame()
	{
		if (_vscrollActive && _hscrollActive) // hv scrollbars
		{
			_childFrame.setX(2);
			_childFrame.setY(2);
			_childFrame.setWidth(getActualWidth() - BORDER_WIDTH2 - _scrollbarWidth);
			_childFrame.setHeight(getActualHeight() - BORDER_WIDTH2 - _scrollbarWidth);
		}
		else if (_hscrollActive)
		{
			_childFrame.setX(2);
			_childFrame.setY(2);
			_childFrame.setWidth(getActualWidth() - BORDER_WIDTH2);
			_childFrame.setHeight(getActualHeight() - BORDER_WIDTH2 - _scrollbarWidth);
		}
		else if (_vscrollActive)
		{
			_childFrame.setX(2);
			_childFrame.setY(2);
			_childFrame.setWidth(getActualWidth() - BORDER_WIDTH2 - _scrollbarWidth);
			_childFrame.setHeight(getActualHeight() - BORDER_WIDTH2);
		}
		else // no scrollbars
		{
			_childFrame.setX(2);
			_childFrame.setY(2);
			_childFrame.setWidth(getActualWidth() - BORDER_WIDTH2);
			_childFrame.setHeight(getActualHeight() - BORDER_WIDTH2);
		}
	}

	void iWidgetScroll::draw()
	{
		if (!isVisible())
		{
			return;
		}
		if (_children.empty())
		{
			return;
		}

		const auto child = _children.front();
		if (child == nullptr)
		{
			return;
		}

		con_assert(child->getVerticalAlignment() == iVerticalAlignment::Top && child->getHorizontalAlignment() == iHorizontalAlignment::Left, "only top left alignment is supported for children of iWidgetScroll");

		int32 childWidth = child->getMinWidth();
		int32 childHeight = child->getMinHeight();

		if (getActualWidth() - BORDER_WIDTH2 < childWidth) // h scrollbar
		{
			_hscrollActive = true;
		}
		else
		{
			_hscrollActive = false;
		}

		if (getActualHeight() - BORDER_WIDTH2 < childHeight) // v scrollbar
		{
			_vscrollActive = true;
		}
		else
		{
			_vscrollActive = false;
		}

		calcChildFrame();
		calcButtons();

		// begin rendering
		iWidgetManager::getInstance().getTheme()->drawBackgroundFrame(getActualRect(), _widgetState, isActive());

		iaColor4f dark(0.3f, 0.3f, 0.3f, 1.0f);

		// render scrollbars
		if (_vscrollActive && _hscrollActive) // hv scrollbars
		{
			iRenderer::getInstance().setColor(dark);
			iRenderer::getInstance().drawRectangle(static_cast<float32>(getActualPosX() + getActualWidth() - _scrollbarWidth - 2), static_cast<float32>(getActualPosY() + 2), static_cast<float32>(_scrollbarWidth), static_cast<float32>(getActualHeight() - 4));
			iRenderer::getInstance().drawRectangle(static_cast<float32>(getActualPosX() + 1), static_cast<float32>(getActualPosY() + getActualHeight() - _scrollbarWidth - 2), static_cast<float32>(getActualWidth() - 3), static_cast<float32>(_scrollbarWidth));

			// left button
			iWidgetManager::getInstance().getTheme()->drawButton(_leftButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _leftTexture, _leftButton._appearanceState, isActive());

			// right button
			iWidgetManager::getInstance().getTheme()->drawButton(_rightButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _rightTexture, _rightButton._appearanceState, isActive());

			// h scroll button
			iWidgetManager::getInstance().getTheme()->drawButton(_hscrollButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, _hscrollButton._appearanceState, isActive());

			// up button
			iWidgetManager::getInstance().getTheme()->drawButton(_upButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _upTexture, _upButton._appearanceState, isActive());

			// down button
			iWidgetManager::getInstance().getTheme()->drawButton(_downButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _downTexture, _downButton._appearanceState, isActive());

			// v scroll button
			iWidgetManager::getInstance().getTheme()->drawButton(_vscrollButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, _vscrollButton._appearanceState, isActive());
		}
		else if (_hscrollActive) // h scrollbar
		{
			iRenderer::getInstance().setColor(dark);
			iRenderer::getInstance().drawRectangle(static_cast<float32>(getActualPosX() + 1), static_cast<float32>(getActualPosY() + getActualHeight() - _scrollbarWidth - 2), static_cast<float32>(getActualWidth() - 3), static_cast<float32>(_scrollbarWidth));

			// left button				
			iWidgetManager::getInstance().getTheme()->drawButton(_leftButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _leftTexture, _leftButton._appearanceState, isActive());

			// right button
			iWidgetManager::getInstance().getTheme()->drawButton(_rightButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _rightTexture, _rightButton._appearanceState, isActive());

			// h scroll button
			iWidgetManager::getInstance().getTheme()->drawButton(_hscrollButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, _hscrollButton._appearanceState, isActive());
		}
		else if (_vscrollActive) // v scrollbar
		{
			iRenderer::getInstance().setColor(dark);
			iRenderer::getInstance().drawRectangle(static_cast<float32>(getActualPosX() + getActualWidth() - _scrollbarWidth - 2), static_cast<float32>(getActualPosY() + 2), static_cast<float32>(_scrollbarWidth), static_cast<float32>(getActualHeight() - 4));

			// up button
			iWidgetManager::getInstance().getTheme()->drawButton(_upButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _upTexture, _upButton._appearanceState, isActive());

			// down button
			iWidgetManager::getInstance().getTheme()->drawButton(_downButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _downTexture, _downButton._appearanceState, isActive());

			// v scroll button
			iWidgetManager::getInstance().getTheme()->drawButton(_vscrollButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, _vscrollButton._appearanceState, isActive());
		}

		//render child
		iRectanglei absoluteFramePos(getActualPosX() + _childFrame.getX(), getActualPosY() + _childFrame.getY(), _childFrame.getWidth(), _childFrame.getHeight());

		// do some clipping using the viewport
		iRectanglei viewport;
		iaMatrixd projectionMatrix;
		iaMatrixd modelMatrix;

		// store current situation
		iRenderer::getInstance().getViewport(viewport);
		iRenderer::getInstance().getProjectionMatrix(projectionMatrix);
		iRenderer::getInstance().getModelMatrix(modelMatrix);

		iRenderer::getInstance().setViewport(absoluteFramePos.getX(), iWidgetManager::getInstance().getDesktopHeight() - absoluteFramePos.getY() - absoluteFramePos.getHeight(), absoluteFramePos.getWidth(), absoluteFramePos.getHeight());
		iRenderer::getInstance().setOrtho(static_cast<float32>(getActualPosX()),
			static_cast<float32>(getActualPosX() + absoluteFramePos.getWidth()),
			static_cast<float32>(getActualPosY() + absoluteFramePos.getHeight()),
			static_cast<float32>(getActualPosY()), 1.0f, 40.0f);

		iaMatrixd matrix;
		matrix._pos._z = -30;
		iRenderer::getInstance().setModelMatrix(matrix);

		child->draw();

		// restore everything
		iRenderer::getInstance().setModelMatrix(modelMatrix);
		iRenderer::getInstance().setProjectionMatrix(projectionMatrix);
		iRenderer::getInstance().setViewport(viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight());
	}
}

