#include "OpenGL2DExample.h"

#include <iaConsole.h>
#include <iaString.h>
using namespace IgorAux;

#include <iMouse.h>
#include <iSprite.h>
#include <iTextureFont.h>
#include <iTimer.h>
#include <iApplication.h>
#include <iRenderer.h>
#include <iTextureResourceFactory.h>
#include <iMaterial.h>
#include <iMaterialResourceFactory.h>
#include <iRenderStatistics.h>
using namespace Igor;

#include <sstream>
using namespace std;

OpenGL2DExample::OpenGL2DExample()
{
	init();
}

OpenGL2DExample::~OpenGL2DExample()
{
	deinit();
}

void OpenGL2DExample::init()
{
	con(" -- OpenGL 2D Test --" << endl);

	_window.setSize(1024, 768);
    _window.setPosition(100, 100);
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &OpenGL2DExample::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &OpenGL2DExample::onWindowResize));
    
    _view.setClearColor(iaColor4f(0.0, 0.5, 0.5, 1));
    _view.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
    _view.registerRenderDelegate(RenderDelegate(this, &OpenGL2DExample::render));

    _window.addView(&_view);
    _window.open();

	iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(this, &OpenGL2DExample::handle));
	iKeyboard::getInstance().registerKeyDownDelegate(KeyDownDelegateExt(this, &OpenGL2DExample::keyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(MouseMoveFullDelegate(this, &OpenGL2DExample::onMouseMove));

	_particleTexture = iTextureResourceFactory::getInstance().loadFile("simpleParticle.png");
	_dummyTexture = iTextureResourceFactory::getInstance().getDummyTexture();
	_font = new iTextureFont("StandardFont.png");	

	_sprite = new iSprite(iTextureResourceFactory::getInstance().loadFile("OpenGL-Logo.jpg"));
	_sprite->setCoi(iaVector2f(128, 64));

	// set up particle system
	_particleSystem.setParticleSize(10.0f, 50.0f);
	_particleSystem.setInitialVelocity(iaVector2f(20, 0));
	_particleSystem.setMaxParticleCount(100);
	_particleSystem.setEmitRate(1);
	_particleSystem.setSpreadFactor(0.5f);
	_particleSystem.setAirDrag(0.995f);
	_particleSystem.setGravitation(iaVector2f(0, +0.2f));

	// create some materials
	_materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

	_materialWithoutDepthTest = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithoutDepthTest)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

	_modelMatrix.translate(iaVector3f(0, 0, -30));

    _logoPosition.set(200,320);

    _spline.addSupportPoint(iaVector3f(100, 100, 0));
    _spline.addSupportPoint(iaVector3f(150, 80, 0));
    _spline.addSupportPoint(iaVector3f(900, 600, 0));
    _spline.addSupportPoint(iaVector3f(700, 500, 0));
    _spline.addSupportPoint(iaVector3f(100, 700, 0));
    _spline.addSupportPoint(iaVector3f(900, 300, 0));
    _spline.addSupportPoint(iaVector3f(50, 150, 0));
    _spline.addSupportPoint(iaVector3f(100, 100, 0));
    _spline.setResolution(10);

    _rainbow.insertColor(iaColor4f(0, 0, 0, 0.0), 0.0f);
    _rainbow.insertColor(iaColor4f(0, 1, 0, 0.25), 0.25f);
    _rainbow.insertColor(iaColor4f(1, 0, 0, 0.5), 0.5f);
    _rainbow.insertColor(iaColor4f(0, 0, 1, 0.75), 0.75f);
    _rainbow.insertColor(iaColor4f(1, 1, 0, 1.0), 1.0f);

    _renderStatistics = new iRenderStatistics();
}

void OpenGL2DExample::deinit()
{
    _view.unregisterRenderDelegate(RenderDelegate(this, &OpenGL2DExample::render));

    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTextureAndBlending);
	_materialWithTextureAndBlending = 0;
    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithoutDepthTest);
	_materialWithoutDepthTest = 0;

	_window.close();
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &OpenGL2DExample::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &OpenGL2DExample::onWindowResize));
	_window.removeView(&_view);
	 
	iApplication::getInstance().unregisterApplicationHandleDelegate(ApplicationHandleDelegate(this, &OpenGL2DExample::handle));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(MouseMoveFullDelegate(this, &OpenGL2DExample::onMouseMove));
	iKeyboard::getInstance().unregisterKeyDownDelegate(KeyDownDelegateExt(this, &OpenGL2DExample::keyPressed));

	if (_sprite)
	{
		delete _sprite;
	}

	if (_font)
	{
		delete _font;
	}

	_particleTexture = 0;
	_dummyTexture = 0;	
}

void OpenGL2DExample::onMouseMove(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
{
    _lastMousePos._x = x2;
    _lastMousePos._y = y2;
}

void OpenGL2DExample::run()
{
	iApplication::getInstance().run();
}

void OpenGL2DExample::onWindowClosed()
{
	iApplication::getInstance().stop();
}

void OpenGL2DExample::onWindowResize(int32 clientWidth, int32 clientHeight)
{
    // to keep pixel coordinates
    _view.setOrthogonal(0, clientWidth, clientHeight, 0);
}

void OpenGL2DExample::keyPressed(iKeyCode key)
{
	if (key == iKeyCode::ESC)
	{
		iApplication::getInstance().stop();
	}

    if (key == iKeyCode::Space)
    {
        iMouse::getInstance().setCenter();
    }
}

// triggered by timer
void OpenGL2DExample::particlesHandle()
{
	iaVector2f velocity(12, 0);
	velocity.rotateXY(_emitangle + M_PI * 0.25f);
	_particleSystem.setInitialVelocity(velocity);

	_emitangle = cos(_animationvalue / 180 * M_PI) * 0.15f;
	_animationvalue += 10.0f;

    _particleSystem.handle();
}

// triggered per frame
void OpenGL2DExample::handle()
{
	_logoRotationAngle += 0.01f;
    if (_logoRotationAngle >= M_PI * 2.0)
    {
        _logoRotationAngle = 0.0f;
    }

    _logoPosition += (_lastMousePos - _logoPosition) * 0.01f;

    _spline.setSupportPoint(iaVector3f(_logoPosition._x, _logoPosition._y, 0), 3);

    particlesHandle();
}

// triggered per frame
void OpenGL2DExample::render()
{
	// scene graph for 2D elementes or just an even simpler interface?
	iRenderer::getInstance().setModelMatrix(_modelMatrix);

    iMaterialResourceFactory::getInstance().setMaterial(_materialWithoutDepthTest);

	iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 1));

	iRenderer::getInstance().drawRectangle(10, 10, 200, 150);
	iRenderer::getInstance().drawRectangle(220, 10, 200, 150);

	iRenderer::getInstance().setColor(iaColor4f(1, 1, 0, 1));
	iRenderer::getInstance().setLineWidth(3);

	iRenderer::getInstance().drawLine(20, 20, 200, 150);
	iRenderer::getInstance().drawLine(20, 150, 200, 20);
	iRenderer::getInstance().drawLine(110, 20, 110, 150);
	iRenderer::getInstance().drawLine(20, 85, 200, 85);

	for (int x = 0; x<19; ++x)
	{
		for (int y = 0; y<14; ++y)
		{
			iRenderer::getInstance().setPointSize(rand() % 5 + 1);
			iRenderer::getInstance().drawPoint(230 + x * 10, 20 + y * 10);
		}
	}

    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);

	iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
    iRenderer::getInstance().drawSprite(_sprite, _logoPosition._x, _logoPosition._y, _logoRotationAngle, 1.5f, 1.5f);

	iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
	iRenderer::getInstance().drawTexture(10, 170, 410, 300, _dummyTexture);

	iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 0.5));
	iRenderer::getInstance().bindTexture(_particleTexture, 0);
	iRenderer::getInstance().drawParticles(-10, 700, 0, _particleSystem.getParticles(), _particleSystem.getParticleCount(), &_rainbow);

	iRenderer::getInstance().setFont(_font);
	iRenderer::getInstance().setFontSize(15.0f);
	iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 1));

	iaString wikipediaOpenGL = "OpenGL (Open Graphics Library) ist eine Spezifikation fuer eine plattform- und programmiersprachenunabhaengige Programmierschnittstelle zur Entwicklung von 2D- und 3D-Computergrafik. Der OpenGL-Standard beschreibt etwa 250 Befehle, die die Darstellung komplexer 3D-Szenen in Echtzeit erlauben. Zudem koennen andere Organisationen (zumeist Hersteller von Grafikkarten) proprietaere Erweiterungen definieren. Wikipedia";
	iRenderer::getInstance().drawString(600, 100, wikipediaOpenGL, -30, 400);

    iMaterialResourceFactory::getInstance().setMaterial(_materialWithoutDepthTest);
    iRenderer::getInstance().setColor(iaColor4f(1, 0, 0.5, 1));

    auto spline = _spline.getSpline();
    iRenderer::getInstance().drawLineStrip(spline);

    iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 1));
    iRenderer::getInstance().drawRectangle(790, 10, 200, 150);

    static float32 offset = 0.0f;
    iRenderer::getInstance().setLineWidth(1);
    iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 1));

    float64 lastValue = perlinNoise.getValue(offset * 0.1, 4) * 150;
    for (int x = 1; x < 200; ++x)
    {
        float64 value = perlinNoise.getValue((offset + x) * 0.1, 4) * 150;
        iRenderer::getInstance().drawLine(790 + x-1, 10 + lastValue, 790 + x, 10 + value);
        lastValue = value;
    }

    offset += 1.0f;

    _renderStatistics->drawStatistics(&_window, _font, iaColor4f(0,1,0,1));
}


