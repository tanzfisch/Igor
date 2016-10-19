#include "Example2D.h"

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
using namespace std;

Example2D::Example2D()
{
    init();
}

Example2D::~Example2D()
{
    deinit();
}

void Example2D::init()
{
    // some console output
    con_endl(" -- Rendering 2D Example --");
    con_endl("");

    // define the window as we want it
    _window.setTitle("Rendering 2D Example");
    _window.setSize(1024, 768);
    // register calback to window close event so we can shutdown the application propperly
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &Example2D::onWindowClosed));
    // register callback to window resize event so we can adopt the view to resolution
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &Example2D::onWindowResize));

    // define a view
    // switch of the clear color step because we will use a tiled background so there is no need to clear the framebuffer every frame
    _view.setClearColor(false);
    // set up an orthogonal projection with the dimensions of the windows client rectangle
    // the client rectangle is the size of the actual rendering area
    _view.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
    // register callback to the rendering event of this view
    _view.registerRenderDelegate(RenderDelegate(this, &Example2D::onRender));
    // add the view to the window
    _window.addView(&_view);

    // open the window after you added the view to prevent a warning message that there was no view defined. 
    // but it is also allowed to add views after the window was already opened
    _window.open();

    // load the background tile texture
    _backgroundTexture = iTextureResourceFactory::getInstance().loadFile("WidgetThemePattern.png");

    // let's see what happens if we want Igor to load a texture that does not exist
    con_endl("!!! next statement will fail with an error message about a missing texture !!!");
    con_endl("");
    _dummyTexture = iTextureResourceFactory::getInstance().loadFile("doesnotexist.foobar");
    // by default Igor generates a dummy texture that will be returned instead
    // it is a checker texture with some gradients in color and alpha channels

    // set up particle system
    // load a texture for our particle system
    _particleTexture = iTextureResourceFactory::getInstance().requestFile("particleDot.png");
    // set the range of particle sizes at spawn
    _particleSystem.setParticleSize(10.0f, 70.0f);
    // set how much the particles should grow each frame
    _particleSystem.setParticleSizeDelta(0.5f, 3.0f);
    // set initial velocity vector of particles at spawn
    _particleSystem.setInitialVelocity(iaVector2f(40, 0));
    // make the animation endless. this causes the particles to be reused after their lifetime is over
    _particleSystem.setLoopable(true);
    // sets particle life time in frames
    _particleSystem.setParticleLifetime(80);
    // set the emission rate per frame
    _particleSystem.setEmitRate(5);
    // set maximal particle count. since the particle system is in loop mode we need at least as 
    // much particles as the life time times the emission rate to have a constant stream of particles
    _particleSystem.setMaxParticleCount(_particleSystem.getParticleLifetime() * _particleSystem.getEmitRate());
    // set the spread factor to 5% of the distribution circle
    // you can also interpret it as opening angle in percent
    _particleSystem.setSpreadFactor(0.05f);
    // don't want air drag
    _particleSystem.setAirDrag(0.0f);
    // apply external force. in this case something like gravity but positive because the y coordinate axis goes down the screen
    _particleSystem.setExternalForce(iaVector2f(0, 0.2f));

    // define a rainbow multi color gradient for our particles
    _rainbow.insertValue(0.0f, iaColor4f(1, 0, 1, 0.0));
    _rainbow.insertValue(0.2f, iaColor4f(0, 0, 1, 0.2));
    _rainbow.insertValue(0.4f, iaColor4f(0, 1, 1, 0.4));
    _rainbow.insertValue(0.6f, iaColor4f(0, 1, 0, 0.6));
    _rainbow.insertValue(0.8f, iaColor4f(1, 1, 0, 0.8));
    _rainbow.insertValue(1.0f, iaColor4f(1, 0, 0, 1.0));

    // load a texture font
    _font = new iTextureFont("StandardFont.png");

    // load a texture as a sprite
    // sprites are basically textures that have some additional meta data that help you to place and orientate them
    _openGLLogo = new iSprite(iTextureResourceFactory::getInstance().loadFile("OpenGL-Logo.jpg"));
    // set the center as the origin of the sprite
    _openGLLogo->setOrigin(iaVector2f(_openGLLogo->getTexture()->getWidth() * 0.5, _openGLLogo->getTexture()->getHeight() * 0.5));
    
    // initalize a spline loop
    _spline.addSupportPoint(iaVector3f(100, 100, 0));
    _spline.addSupportPoint(iaVector3f(150, 80, 0));
    _spline.addSupportPoint(iaVector3f(900, 600, 0));
    _spline.addSupportPoint(iaVector3f(700, 500, 0));
    _spline.addSupportPoint(iaVector3f(100, 700, 0));
    _spline.addSupportPoint(iaVector3f(900, 300, 0));
    _spline.addSupportPoint(iaVector3f(50, 150, 0));
    // close the loop by having the end point at the same position as the start point
    _spline.addSupportPoint(iaVector3f(100, 100, 0));
    // defines the resolution of the output line strip if we later call getSpline
    _spline.setResolution(20);

    // create some materials
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _materialWithTexture = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTexture)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTexture)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _materialWithoutDepthTest = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithoutDepthTest)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    
    // load requested textures
    iTextureResourceFactory::getInstance().flush();

    // register callback to application handle event. the application handle event will be called every frame just before the rendering
    iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &Example2D::onHandle));
    // register callback to esc key pressed event
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownSpecificDelegate(this, &Example2D::onKeyESCPressed), iKeyCode::ESC);
    // register callback to mosue moved event
    iMouse::getInstance().registerMouseMoveDelegate(iMouseMoveDelegate(this, &Example2D::onMouseMove));

    // load an other texture with the Igor Logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png");

    // generate a random seed
    _rand.setSeed(static_cast<uint32>(iTimer::getInstance().getTime()));
}

void Example2D::deinit()
{
    // unregister some callbacks. otherwhise you will be reminded of callbacks that where not released
    iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &Example2D::onHandle));
    iMouse::getInstance().unregisterMouseMoveDelegate(iMouseMoveDelegate(this, &Example2D::onMouseMove));
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownSpecificDelegate(this, &Example2D::onKeyESCPressed), iKeyCode::ESC);

    // unregister the rendering callback. if not you will get a warning message because your shutdown was not complete
    _view.unregisterRenderDelegate(RenderDelegate(this, &Example2D::onRender));

    // close the window, release callback and remove the view. if not you will get various reminders that you should
    _window.close();
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &Example2D::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &Example2D::onWindowResize));
    _window.removeView(&_view);

    // release materials (optional)
    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTextureAndBlending);
    _materialWithTextureAndBlending = 0;
    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTexture);
    _materialWithTexture = 0;
    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithoutDepthTest);
    _materialWithoutDepthTest = 0;

    // release some textures. otherwhise you will get a reminder of possible mem leak
    if (_openGLLogo != nullptr)
    {
        delete _openGLLogo;
        _openGLLogo = nullptr;
    }

    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }

    _particleTexture = nullptr;
    _backgroundTexture = nullptr;
    _dummyTexture = nullptr;
}

void Example2D::onMouseMove(int32 x, int32 y)
{
    // save mouse position for later
    _lastMousePos.set(x, y);
}

void Example2D::run()
{
    // calls the applications endless loop
    iApplication::getInstance().run();
}

void Example2D::onWindowClosed()
{
    // breaks the applications endless loop
    iApplication::getInstance().stop();
}

void Example2D::onWindowResize(int32 clientWidth, int32 clientHeight)
{
    // to keep pixel coordinates
    _view.setOrthogonal(0, clientWidth, clientHeight, 0);
}

void Example2D::onKeyESCPressed()
{
    // breaks the applications endless loop
    iApplication::getInstance().stop();
}

void Example2D::onHandle()
{
    // manipulate the rotation angle of the logo
    _logoRotationAngle += 0.01f;
    if (_logoRotationAngle >= M_PI * 2.0)
    {
        _logoRotationAngle = 0.0f;
    }

    // moves the logo towards the mouse position
    _logoPosition += (_lastMousePos - _logoPosition) * 0.01f;

    // moves one of the splines support point to the logo's position
    _spline.setSupportPoint(iaVector3f(_logoPosition._x, _logoPosition._y, 0), 3);

    // update particles
    updateParticles();
}

void Example2D::updateParticles()
{
    // manipulates particles initial velocity over time
    iaVector2f velocity(12, 0);
    float32 emitangle = _perlinNoise.getValue(_particleAnimatioValue * 0.05, 3) - 1.0;
    
    velocity.rotateXY(emitangle);
    _particleSystem.setInitialVelocity(velocity);
    _particleAnimatioValue += 1.0f;

    // calls particles iteration
    _particleSystem.handle();
}

void Example2D::onRender()
{
    // since the model matrix is by default an identity matrix which would cause all our 2d rendering end up at depth zero
    // and the near clipping plane of our frustum can't be zero we have to push the scene a bit away from zero (e.g. -30 just a random number with no meaning)
    iaMatrixf matrix;
    iRenderer::getInstance().setViewMatrix(matrix);
    matrix.translate(iaVector3f(0, 0, -30));
    iRenderer::getInstance().setModelMatrix(matrix);

    // set a textured material and draw the tiles texture as background
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTexture);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
    iRenderer::getInstance().drawTextureTiled(0, 0, _window.getClientWidth(), _window.getClientHeight(), _backgroundTexture);

    // set non textured material and draw some primitves
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

    for (int x = 0; x < 19; ++x)
    {
        for (int y = 0; y < 14; ++y)
        {
            iRenderer::getInstance().setPointSize(_rand.getNext() % 5 + 1);
            iRenderer::getInstance().drawPoint(230 + x * 10, 20 + y * 10);
        }
    }

    // change material again to textured an draw the logo
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
    iRenderer::getInstance().drawSprite(_openGLLogo, _logoPosition._x, _logoPosition._y, _logoRotationAngle, 1.5f, 1.5f);

    // draw the texture that we could not load at startup
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
    iRenderer::getInstance().drawTexture(10, 170, 410, 150, _dummyTexture);

    // draw the particles
    iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 0.5));
    iRenderer::getInstance().bindTexture(_particleTexture, 0);
    iRenderer::getInstance().drawParticles(-10, _window.getClientHeight() - 150, 0, _particleSystem.getParticles(), _particleSystem.getParticleCount(), &_rainbow);

    // draw some text from wikipedia
    iaString wikipediaOpenGL = "OpenGL (Open Graphics Library) ist eine Spezifikation fuer eine plattform- und programmiersprachenunabhaengige Programmierschnittstelle zur Entwicklung von 2D- und 3D-Computergrafik. Der OpenGL-Standard beschreibt etwa 250 Befehle, die die Darstellung komplexer 3D-Szenen in Echtzeit erlauben. Zudem koennen andere Organisationen (zumeist Hersteller von Grafikkarten) proprietaere Erweiterungen definieren. Wikipedia";
    iRenderer::getInstance().setFont(_font);
    iRenderer::getInstance().setFontSize(15.0f);
    iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 1));
    iRenderer::getInstance().drawString(600, 100, wikipediaOpenGL, -30, 400);

    // draw spline
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithoutDepthTest);
    iRenderer::getInstance().setColor(iaColor4f(1, 0, 0.5, 1));
    iRenderer::getInstance().drawLineStrip(_spline.getSpline());

    // draw random graph in the upper right corner
    iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 1));
    iRenderer::getInstance().drawRectangle(_window.getClientWidth() - 260, 10, 250, 150);

    static float32 offset = 0.0f;
    iRenderer::getInstance().setLineWidth(1);
    iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 1));

    float64 lastValue = _perlinNoise.getValue(offset * 0.01, 6) * 150;
    for (int x = 1; x < 250; ++x)
    {
        float64 value = _perlinNoise.getValue((offset + x) * 0.01, 6) * 150;
        iRenderer::getInstance().drawLine(_window.getClientWidth() - 260 + x - 1, 10 + lastValue, _window.getClientWidth() - 260 + x, 10 + value);
        lastValue = value;
    }

    offset += 1.0f;

    drawLogo();

    // draw frame rate in lower right corner
    iStatistics::getInstance().drawStatistics(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void Example2D::drawLogo()
{
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = _igorLogo->getWidth() * 0.6;
    float32 height = _igorLogo->getHeight() * 0.6;
    float32 x = _window.getClientWidth() - width;
    float32 y = _window.getClientHeight() - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}


