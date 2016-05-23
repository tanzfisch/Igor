// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetScroll.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iMaterialResourceFactory.h>
#include <iWidgetUserControl.h>
#include <iMaterial.h>
#include <iRenderer.h>

#include <iaConsole.h>
#include <iaColor4.h>
#include <iaMatrix.h>
using namespace IgorAux;

namespace Igor
{

	iWidgetScroll::iWidgetScroll()
		: iWidget(iWidgetType::Scroll)
	{
		setGrowingByContent(false);
	}

	void iWidgetScroll::handleMouseMove(int32 x, int32 y)
	{
		if (isActive())
		{
			vector<iWidget*> widgets = _children;

			for (auto widget : widgets)
			{
				widget->handleMouseMove(x, y);
			}

			if (x >= _absoluteX &&
				x < _absoluteX + _actualWidth &&
				y >= _absoluteY &&
				y < _absoluteY + _actualHeight)
			{
				if (!_isMouseOver)
				{
					_widgetAppearanceState = iWidgetAppearanceState::Highlighted;
					_mouseOver(this);
				}

				_isMouseOver = true;
			}
			else
			{
				if (_isMouseOver)
				{
					_widgetAppearanceState = iWidgetAppearanceState::Standby;
					_mouseOff(this);
				}

				_isMouseOver = false;
			}
		}
	}

	void iWidgetScroll::setHorrizontalScroll(float32 value)
	{
		con_assert(value >= 0.0f && value <= 1.0f, "out od range");
		_hscroll = value;
		update();
	}

	void iWidgetScroll::setVerticalScroll(float32 value)
	{
		con_assert(value >= 0.0f && value <= 1.0f, "out od range");
		_vscroll = value;
		update();
	}

	bool iWidgetScroll::handleMouseWheel(int32 d)
	{
		if (!isActive())
		{
			return false;
		}

		if (!_children.empty())
		{
			iWidget* widget = _children[0];
			if (widget->handleMouseWheel(d))
			{
				return true;
			}
		}

		if (_vscrollActive)
		{
			_vscroll -= d / 10.0f;
			if (_vscroll < 0.0f)
			{
				_vscroll = 0.0f;
			}

			if (_vscroll > 1.0f)
			{
				_vscroll = 1.0f;
			}

			update();
			return true;
		}
		else if (_hscrollActive)
		{
			_hscroll -= d / 10.0f;
			if (_hscroll < 0.0f)
			{
				_hscroll = 0.0f;
			}

			if (_hscroll > 1.0f)
			{
				_hscroll = 1.0f;
			}

			update();
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

	void iWidgetScroll::update()
	{
		iWidget::update(getConfiguredWidth(), getConfiguredHeight());

		if (!_children.empty())
		{
			iWidget* widget = _children[0];

			con_assert(widget->getVerticalAlignment() == iVerticalAlignment::Top && widget->getHorrizontalAlignment() == iHorrizontalAlignment::Left, "only top left alignment is supported for children of iWidgetScroll");

			int32 childWidth = widget->getActualWidth();
			int32 childHeight = widget->getActualHeight();

			if (getActualWidth() - 4 < childWidth) // h scrollbar
			{
				_hscrollActive = true;
			}
			else
			{
				_hscrollActive = false;
			}

			if (getActualHeight() - 4 < childHeight) // v scrollbar
			{
				_vscrollActive = true;
			}
			else
			{
				_vscrollActive = false;
			}

			calcChildFrame();
			calcScrollButtons();
			calcTranslation();

			/* TODO
			up_button;
			down_button;
			left_button;
			right_button;*/
		}
	}

	void iWidgetScroll::calcScrollButtons()
	{
		int32 scrollspace;

		iWidget* widget = _children[0];

		int32 childWidth = widget->getActualWidth();
		int32 childHeight = widget->getActualHeight();

		if (_vscrollActive) // v scrollbar
		{
			// v scroll button
			if (_hscrollActive)
			{
				scrollspace = getActualHeight() - 6 - _scrollbarWidth * 3;
			}
			else
			{
				scrollspace = getActualHeight() - 6 - _scrollbarWidth * 2;
			}

			_vscrollButton.setHeight(((float32)getActualHeight() / childHeight) * scrollspace);
			_vscrollButton.setY((scrollspace - _vscrollButton.getHeight()) * _vscroll + 3 + _scrollbarWidth);
			_vscrollButton.setX(getActualWidth() - _scrollbarWidth - 2);
			_vscrollButton.setWidth(_scrollbarWidth);
		}

		if (_hscrollActive) // h scrollbar
		{
			// h scroll button
			if (_vscrollActive)
			{
				scrollspace = getActualWidth() - 6 - _scrollbarWidth * 3;
			}
			else
			{
				scrollspace = getActualWidth() - 6 - _scrollbarWidth * 2;
			}

			_hscrollButton.setWidth(((float32)getActualWidth() / childWidth) * scrollspace);
			_hscrollButton.setX((scrollspace - _hscrollButton.getWidth()) * _hscroll + 4 + _scrollbarWidth);
			_hscrollButton.setY(getActualHeight() - _scrollbarWidth - 2);
			_hscrollButton.setHeight(_scrollbarWidth);
		}
	}

	void iWidgetScroll::calcTranslation()
	{
		_translate.identity();

		iWidget* widget = _children[0];

		if (_hscrollActive)
		{
			_translate.translate(-_hscroll * (widget->getActualWidth() - getActualWidth() + 4), 0, 0);
		}

		if (_vscrollActive)
		{
			_translate.translate(0, -_vscroll * (widget->getActualHeight() - getActualHeight() + 4), 0);
		}
	}

	void iWidgetScroll::calcChildFrame()
	{
		if (_vscrollActive && _hscrollActive) // hv scrollbars
		{
			_childFrame.setX(2);
			_childFrame.setY(2);
			_childFrame.setWidth(getActualWidth() - 4 - _scrollbarWidth);
			_childFrame.setHeight(getActualHeight() - 4 - _scrollbarWidth);
		}
		else if (_hscrollActive)
		{
			_childFrame.setX(2);
			_childFrame.setY(2);
			_childFrame.setWidth(getActualWidth() - 4);
			_childFrame.setHeight(getActualHeight() - 4 - _scrollbarWidth);
		}
		else if (_vscrollActive)
		{
			_childFrame.setX(2);
			_childFrame.setY(2);
			_childFrame.setWidth(getActualWidth() - 4 - _scrollbarWidth);
			_childFrame.setHeight(getActualHeight() - 4);
		}
		else // no scrollbars
		{
			_childFrame.setX(2);
			_childFrame.setY(2);
			_childFrame.setWidth(getActualWidth() - 4);
			_childFrame.setHeight(getActualHeight() - 4);
		}
	}

	void iWidgetScroll::draw(int32 parentPosX, int32 parentPosY)
	{
		updatePosition(parentPosX, parentPosY);

		if (isVisible())
		{
			// begin rendering
			iWidgetManager::getInstance().getTheme()->drawBackgroundFrame(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), _widgetAppearanceState, isActive());

			iaColor4f dark(0.3f, 0.3f, 0.3f, 1.0f);

			// render scrollbars
			if (_vscrollActive && _hscrollActive) // hv scrollbars
			{
				iRenderer::getInstance().setColor(dark);
				iRenderer::getInstance().drawRectangle(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + 2, _scrollbarWidth, getActualHeight() - 4);
				iRenderer::getInstance().drawRectangle(getActualPosX() + 1, getActualPosY() + getActualHeight() - _scrollbarWidth - 2, getActualWidth() - 3, _scrollbarWidth); // don't ask about the coordinates -.-

				// left button
				iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX() + 2, getActualPosY() + getActualHeight() - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
				// TODO pic

				// right button
				iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX() + getActualWidth() - _scrollbarWidth * 2 - 2, getActualPosY() + getActualHeight() - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
				// TODO pic

				// h scroll button
				iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX() + _hscrollButton.getX(), getActualPosY() + _hscrollButton.getY(),
					_hscrollButton.getWidth(), _hscrollButton.getHeight(), "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());

				// top button
				iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + 2, _scrollbarWidth, _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
				// TODO pic

				// bottom button
				iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + getActualHeight() - 2 - _scrollbarWidth * 2, _scrollbarWidth,
					_scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
				// TODO pic

				// v scroll button
				iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX() + _vscrollButton.getX(), getActualPosY() + _vscrollButton.getY(), _vscrollButton.getWidth(),
					_vscrollButton.getHeight(), "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
			}
			else if (_hscrollActive) // h scrollbar
			{
				iRenderer::getInstance().setColor(dark);
				iRenderer::getInstance().drawRectangle(getActualPosX() + 1, getActualPosY() + getActualHeight() - _scrollbarWidth - 2, getActualWidth() - 3, _scrollbarWidth); // don't ask about the coordinates -.-

				// left button
				iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX() + 2, getActualPosY() + getActualHeight() - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
				// TODO pic

				// right button
				iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + getActualHeight() - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
				// TODO pic

				// h scroll button
				iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX() + _hscrollButton.getX(), getActualPosY() + _hscrollButton.getY(),
					_hscrollButton.getWidth(), _hscrollButton.getHeight(), "", iHorrizontalAlignment::Center, iVerticalAlignment::Center,
					0, iWidgetAppearanceState::Standby, isActive());
			}
			else if (_vscrollActive) // v scrollbar
			{
				iRenderer::getInstance().setColor(dark);
				iRenderer::getInstance().drawRectangle(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + 2, _scrollbarWidth, getActualHeight() - 4);

				// top button
				iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + 2, _scrollbarWidth, _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
				// TODO pic

				// bottom button
				iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + getActualHeight() - 2 - _scrollbarWidth, _scrollbarWidth,
					_scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
				// TODO pic

				// v scroll button
				iWidgetManager::getInstance().getTheme()->drawButton(getActualPosX() + _vscrollButton.getX(), getActualPosY() + _vscrollButton.getY(), _vscrollButton.getWidth(),
					_vscrollButton.getHeight(), "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
			}

			//render child
			iRectanglei absoluteFramePos(getActualPosX() + _childFrame.getX(), getActualPosY() + _childFrame.getY(), _childFrame.getWidth(), _childFrame.getHeight());

			if (!_children.empty())
			{
				iWidget* widget = _children[0];

				if (widget != nullptr)
				{
					iRectanglei viewport;
					iaMatrixf projectionMatrix;
					iaMatrixf modelMatrix;

					iRenderer::getInstance().getViewport(viewport);
					iRenderer::getInstance().getProjectionMatrix(projectionMatrix);
					iRenderer::getInstance().getModelMatrix(modelMatrix);

					iRenderer::getInstance().setViewport(absoluteFramePos.getX(), iWidgetManager::getInstance().getDesktopHeight() - absoluteFramePos.getY() - absoluteFramePos.getHeight(), absoluteFramePos.getWidth(), absoluteFramePos.getHeight());
					iRenderer::getInstance().setOrtho(0, absoluteFramePos.getWidth(), absoluteFramePos.getHeight(), 0, 1, 40);

					iaMatrixf matrix;
					matrix.translate(-getActualPosX(), -getActualPosY(), -30);
					//matrix._pos += _translate._pos;

					iRenderer::getInstance().setModelMatrix(matrix);

					widget->draw(getActualPosX() + _translate._pos._x, getActualPosY() + _translate._pos._y);

					iRenderer::getInstance().setModelMatrix(modelMatrix);
					iRenderer::getInstance().setProjectionMatrix(projectionMatrix);
					iRenderer::getInstance().setViewport(viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight());
				}
			}
		}
	}

}