#include "SpriteAnimation.h"

#include <iaConsole.h>
#include <iaString.h>
using namespace IgorAux;

#include <iMouse.h>
#include <iKeyboard.h>
#include <iSprite.h>
#include <iTextureFont.h>
#include <iTimer.h>
#include <iApplication.h>
#include <iRenderer.h>
#include <iTextureResourceFactory.h>
#include <iMaterial.h>
#include <iMaterialResourceFactory.h>
#include <iStatistics.h>
using namespace Igor;

#include <sstream>

SpriteAnimation::SpriteAnimation()
{
	init();
}

SpriteAnimation::~SpriteAnimation()
{
	deinit();
}

void SpriteAnimation::init()
{
	// some console output
	con_endl(" -- Sprite Animation Example --");
	con_endl("");

	// define the window as we want it
	_window.setTitle("Sprite Animation Example");
	_window.setClientSize(1024, 768);
	_window.setCentered();
	// register calback to window close event so we can shutdown the application propperly
	_window.registerWindowCloseDelegate(WindowCloseDelegate(this, &SpriteAnimation::onWindowClosed));
	// register callback to window resize event so we can adopt the view to resolution
	_window.registerWindowResizeDelegate(WindowResizeDelegate(this, &SpriteAnimation::onWindowResize));

	// define a view
	// set up an orthogonal projection with the dimensions of the windows client rectangle
	// the client rectangle is the size of the actual rendering area
	_view.setOrthogonal(0.0f, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0f);
	// register callback to the rendering event of this view
	_view.registerRenderDelegate(RenderDelegate(this, &SpriteAnimation::onRender));
	// set background color
	_view.setClearColor(1.0, 1.0, 1.0, 1.0);
	// add the view to the window
	_window.addView(&_view);

	// open the window after you added the view to prevent a warning message that there was no view defined. 
	// but it is also allowed to add views after the window was already opened
	_window.open();

	// load a texture font
	_font = new iTextureFont("StandardFont.png");

	// load a texture as a sprite
	// sprites are basically textures that have some additional meta data that help you to place and orientate them
	_walk = new iSprite(iTextureResourceFactory::getInstance().loadFile("walk.png", iResourceCacheMode::Free, iTextureBuildMode::Normal));
	// set up walk frames
	for (int i = 0; i < 3; ++i)
	{
		_walk->addFrame(iaVector2f(i * 50.0, 0), iaVector2f(50.0, 85), iaVector2f(25, 80));
	}
	for (int i = 0; i < 3; ++i)
	{
		_walk->addFrame(iaVector2f(i * 50.0, 82), iaVector2f(50.0, 85), iaVector2f(25, 80));
	}
	for (int i = 0; i < 3; ++i)
	{
		_walk->addFrame(iaVector2f(i * 50.0, 172), iaVector2f(50.0, 85), iaVector2f(25, 80));
	}
	for (int i = 0; i < 3; ++i)
	{
		_walk->addFrame(iaVector2f(i * 50.0, 258), iaVector2f(50.0, 85), iaVector2f(25, 80));
	}

	for (int i = 0; i < 4; ++i)
	{
		_flags[i] = false;
	}

	// create some materials
	_materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
	iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
	iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
	iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

	// load requested textures
	iTextureResourceFactory::getInstance().flush();

	// register callback to application handle event. the application handle event will be called every frame just before the rendering
	iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &SpriteAnimation::onHandle));
	// register callback to key pressed event
	iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &SpriteAnimation::onKeyDown));
	// register callback to key released event
	iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &SpriteAnimation::onKeyUp));
	// register callback to mosue moved event
	iMouse::getInstance().registerMouseMoveDelegate(iMouseMoveDelegate(this, &SpriteAnimation::onMouseMove));

	// load an other texture with the Igor Logo
	_igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
}

void SpriteAnimation::deinit()
{
	// unregister some callbacks. otherwhise you will be reminded of callbacks that where not released
	iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &SpriteAnimation::onHandle));
	iMouse::getInstance().unregisterMouseMoveDelegate(iMouseMoveDelegate(this, &SpriteAnimation::onMouseMove));
	iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &SpriteAnimation::onKeyDown));
	iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &SpriteAnimation::onKeyUp));

	// unregister the rendering callback. if not you will get a warning message because your shutdown was not complete
	_view.unregisterRenderDelegate(RenderDelegate(this, &SpriteAnimation::onRender));

	// release materials (optional)
	iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTextureAndBlending);
	_materialWithTextureAndBlending = 0;

	// release some textures. otherwhise you will get a reminder of possible mem leak
	if (_walk != nullptr)
	{
		delete _walk;
		_walk = nullptr;
	}

	if (_font != nullptr)
	{
		delete _font;
		_font = nullptr;
	}

	_igorLogo = nullptr;

	// tell texture manager to flush and actually release textures
	iTextureResourceFactory::getInstance().flush();

	// close the window, release callback and remove the view. if not you will get various reminders that you should
	_window.close();
	_window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &SpriteAnimation::onWindowClosed));
	_window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &SpriteAnimation::onWindowResize));
	_window.removeView(&_view);
}

void SpriteAnimation::onMouseMove(const iaVector2i& position)
{
	// save mouse position for later
	// TODO _lastMousePos.set(static_cast<float32>(position._x), static_cast<float32>(position._y));
}

void SpriteAnimation::run()
{
	// calls the applications endless loop
	iApplication::getInstance().run();
}

void SpriteAnimation::onWindowClosed()
{
	// breaks the applications endless loop
	iApplication::getInstance().stop();
}

void SpriteAnimation::onWindowResize(int32 clientWidth, int32 clientHeight)
{
	// to keep pixel coordinates
	_view.setOrthogonal(0.0f, static_cast<float32>(clientWidth), static_cast<float32>(clientHeight), 0.0f);
}

void SpriteAnimation::onKeyDown(iKeyCode key)
{
	switch (key)
	{
	case iKeyCode::ESC:
		// breaks the applications endless loop
		iApplication::getInstance().stop();
		break;

	case iKeyCode::Left:
		_flags[0] = true;
		break;

	case iKeyCode::Up:
		_flags[1] = true;
		break;

	case iKeyCode::Right:
		_flags[2] = true;
		break;

	case iKeyCode::Down:
		_flags[3] = true;
		break;
	}
}

void SpriteAnimation::onKeyUp(iKeyCode key)
{
	switch (key)
	{
	case iKeyCode::Left:
		_flags[0] = false;
		break;

	case iKeyCode::Up:
		_flags[1] = false;
		break;

	case iKeyCode::Right:
		_flags[2] = false;
		break;

	case iKeyCode::Down:
		_flags[3] = false;
		break;
	}
}

void SpriteAnimation::onHandle()
{
	// moves the logo towards the mouse position
// TODO 	_logoPosition += (_lastMousePos - _logoPosition) * 0.01f;

	iaVector2f velocity;

	if (_flags[0])
	{
		velocity._x -= 1;
	}
	if (_flags[1])
	{
		velocity._y -= 1;
	}
	if (_flags[2])
	{
		velocity._x += 1;
	}
	if (_flags[3])
	{
		velocity._y += 1;
	}

	if (velocity.length())
	{
		velocity.normalize();
	}

	_characterVelocity = velocity;

	_characterPosition += _characterVelocity;
}

void SpriteAnimation::onRender()
{
	// since the model matrix is by default an identity matrix which would cause all our 2d rendering end up at depth zero
	// and the near clipping plane of our frustum can't be zero we have to push the scene a bit away from zero (e.g. -30 just a random number with no meaning)
	iaMatrixd matrix;
	iRenderer::getInstance().setViewMatrix(matrix);
	matrix.translate(0, 0, -30);
	iRenderer::getInstance().setModelMatrix(matrix);

	// change material again to textured an draw the logo
	iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
	iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

	static uint32 index = 0;
	iRenderer::getInstance().drawSprite(_walk, index++, _characterPosition);

	if (index >= _walk->getFrameCount())
	{
		index = 0;
	}

	drawLogo();

	// draw frame rate in lower right corner
	_statisticsVisualizer.drawStatistics(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void SpriteAnimation::drawLogo()
{
	iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
	iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

	float32 width = static_cast<float32>(_igorLogo->getWidth());
	float32 height = static_cast<float32>(_igorLogo->getHeight());
	float32 x = static_cast<float32>(_window.getClientWidth()) - width;
	float32 y = static_cast<float32>(_window.getClientHeight()) - height;

	iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}


