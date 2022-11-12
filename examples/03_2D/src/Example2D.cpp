// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include "Example2D.h"

#include <sstream>

Example2D::Example2D(iWindowPtr window)
    : ExampleBase(window, "2D Interfaces")
{
}

void Example2D::onInit()
{
    // load the background tile texture
    _backgroundTexture = iTextureResourceFactory::getInstance().loadFile("ice.png");

    // let's see what happens if we want Igor to load a texture that does not exist
    con_endl("!!! next statement will fail with an error message about a missing texture !!!");
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
    // set emitter position to lover left corner of the screen
    _particleSystem.setEmitterPosition(iaVector2f(-10.0f, static_cast<float32>(getWindow()->getClientHeight() - 150)));

    // define a rainbow multi color gradient for our particles
    _rainbow.setValue(0.0f, iaColor4f(1.0f, 0.0f, 1.0f, 0.0f));
    _rainbow.setValue(0.2f, iaColor4f(0.0f, 0.0f, 1.0f, 0.2f));
    _rainbow.setValue(0.4f, iaColor4f(0.0f, 1.0f, 1.0f, 0.4f));
    _rainbow.setValue(0.6f, iaColor4f(0.0f, 1.0f, 0.0f, 0.6f));
    _rainbow.setValue(0.8f, iaColor4f(1.0f, 1.0f, 0.0f, 0.8f));
    _rainbow.setValue(1.0f, iaColor4f(1.0f, 0.0f, 0.0f, 1.0f));

    // load a texture as an atlas
    _doughnuts = iAtlas::create(iTextureResourceFactory::getInstance().loadFile("doughnuts.png"));
    // set up frame
    _doughnutsFrameIndex = _doughnuts->addFrame(iaVector2f(0.0, 0.0), iaVector2f(0.5, 0.5), iaVector2f(0.0, 0.0), false);
    _doughnuts->addFrame(iaVector2f(0.5, 0.0), iaVector2f(0.5, 0.5), iaVector2f(0.0, 0.0), false);
    _doughnuts->addFrame(iaVector2f(0.0, 0.5), iaVector2f(0.5, 0.5), iaVector2f(0.0, 0.0), false);
    _doughnuts->addFrame(iaVector2f(0.5, 0.5), iaVector2f(0.5, 0.5), iaVector2f(0.0, 0.0), false);
    // setup matrix
    _doughnutMatrix.scale(64.0f, 64.0f, 1.0f);

    _doughnutsTime = iaTime::getNow();

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

    _updateTimingHandle = new iTimerHandle(iTimerTickDelegate(this, &Example2D::onUpdate), iaTime::fromMilliseconds(10));
    _updateTimingHandle->start();

    // generate a random seed
    _rand.setSeed(static_cast<uint32>(iaTime::getNow().getMicrosenconds()));
}

void Example2D::onDeinit()
{
    // stop timer
    if (_updateTimingHandle != nullptr)
    {
        delete _updateTimingHandle;
        _updateTimingHandle = nullptr;
    }

    // release some resources
    _doughnuts = nullptr;
    _particleTexture = nullptr;
    _backgroundTexture = nullptr;
    _dummyTexture = nullptr;
}

void Example2D::onEvent(iEvent &event)
{
    // first call example base
    ExampleBase::onEvent(event);

    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(Example2D::onMouseMoveEvent));
}

bool Example2D::onMouseMoveEvent(iEventMouseMove &event)
{
    // save mouse position for later
    _lastMousePos.set(static_cast<float32>(event.getPosition()._x), static_cast<float32>(event.getPosition()._y));
    return true;
}

void Example2D::onUpdate(const iaTime &time)
{
    _doughnutMatrix.rotate(0.05, iaAxis::Z);

    // moves the logo towards the mouse position
    _doughnutMatrix._pos += (iaVector3f(_lastMousePos._x, _lastMousePos._y, 0.0) - _doughnutMatrix._pos) * 0.05f;

    // moves one of the splines support point to the logo's position
    _spline.setSupportPoint(_doughnutMatrix._pos, 3);

    // doughnut time <3
    if (time > _doughnutsTime + iaTime::fromSeconds(0.5))
    {
        _doughnutsFrameIndex++;
        _doughnutsFrameIndex = _doughnutsFrameIndex % 4;
        _doughnutsTime = time;
    }

    // update particles
    updateParticles();
}

void Example2D::updateParticles()
{
    // manipulates particles initial velocity over time
    iaVector2f velocity(12, 0);
    float32 emitangle = static_cast<float32>(_perlinNoise.getValue(_particleAnimatioValue * 0.05, 3) - 1.0);

    velocity.rotateXY(emitangle);
    _particleSystem.setInitialVelocity(velocity);
    _particleAnimatioValue += 1.0f;

    // calls particles iteration
    _particleSystem.handle();
}

void Example2D::onRenderOrtho()
{
    // since the model matrix is by default an identity matrix which would cause all our 2d rendering end up at depth zero
    // and the near clipping plane of our frustum can't be zero we have to push the scene a bit away from zero
    iaMatrixd matrix;
    matrix.translate(0, 0, -1);
    iRenderer2::getInstance().setModelMatrix(matrix);

    // draw some background
    iRenderer2::getInstance().setBlendingActive(false);
    const float32 width = getWindow()->getClientWidth();
    const float32 height = getWindow()->getClientHeight();
    iaVector2f tiling(width / _backgroundTexture->getWidth(),
                      height / _backgroundTexture->getHeight());
    iRenderer2::getInstance().drawTexturedRectangle(0.0f, 0.0f, width, height, _backgroundTexture, iaColor4f::white, tiling);

    iRenderer2::getInstance().drawFilledRectangle(10, 10, 200, 150, iaColor4f(0, 0, 0, 1));
    iRenderer2::getInstance().drawFilledRectangle(220, 10, 200, 150, iaColor4f(0, 0, 0, 1));

    iRenderer2::getInstance().setLineWidth(3);

    iRenderer2::getInstance().drawLine(20, 20, 200, 150, iaColor4f(1, 1, 0, 1));
    iRenderer2::getInstance().drawLine(20, 150, 200, 20, iaColor4f(1, 1, 0, 1));
    iRenderer2::getInstance().drawLine(110, 20, 110, 150, iaColor4f(1, 1, 0, 1));
    iRenderer2::getInstance().drawLine(20, 85, 200, 85, iaColor4f(1, 1, 0, 1));

    iRenderer2::getInstance().setPointSize(static_cast<float32>(_rand.getNext() % 5 + 1));

    for (int x = 0; x < 19; ++x)
    {
        for (int y = 0; y < 14; ++y)
        {
            iRenderer2::getInstance().drawPoint(static_cast<float32>(230 + x * 10), static_cast<float32>(20 + y * 10), iaColor4f(1, 1, 0, 1));
        }
    }
   
    iRenderer2::getInstance().setBlendingActive(true);

    // draw the texture that we could not have loaded at startup
    iRenderer2::getInstance().drawTexturedRectangle(10, 170, 410, 150, _dummyTexture);

    // draw the particles
    iRenderer2::getInstance().drawParticles(_particleSystem.getParticles(), _particleSystem.getParticleCount(), _particleTexture, _rainbow);

    // draw some text from wikipedia
    iaString wikipediaOpenGL = "OpenGL (Open Graphics Library) ist eine Spezifikation fuer eine plattform- und programmiersprachenunabhaengige "
                               "Programmierschnittstelle zur Entwicklung von 2D- und 3D-Computergrafik. Der OpenGL-Standard beschreibt etwa 250 "
                               "Befehle, die die Darstellung komplexer 3D-Szenen in Echtzeit erlauben. Zudem koennen andere Organisationen "
                               "(zumeist Hersteller von Grafikkarten) proprietaere Erweiterungen definieren.\n\nWikipedia";

    iRenderer2::getInstance().setFont(getFont());
    iRenderer2::getInstance().setFontSize(15.0f);
    iRenderer2::getInstance().drawString(350, 350, wikipediaOpenGL, iaColor4f(0, 0, 0, 1), 400);

    // draw spline
    iRenderer2::getInstance().setBlendingActive(false);
    std::vector<iaVector3f> points;
    _spline.getPoints(points, 100);
    iRenderer2::getInstance().drawLineStrip(points, iaColor4f(1, 0, 0.5, 1));

    // draw perlin noise based graph in the upper right corner
    iRenderer2::getInstance().drawFilledRectangle(static_cast<float32>(getWindow()->getClientWidth() - 260), 10.0f, 250.0f, 150.0f, iaColor4f(0, 0, 0, 1));
    iRenderer2::getInstance().setLineWidth(3);

    // just to make that graph move a bit
    static float32 offset = 0.0f;
    offset += 1.0f;

    float64 lastValue = _perlinNoise.getValue(offset * 0.01, 6) * 150;
    for (int x = 1; x < 250; ++x)
    {
        float64 value = _perlinNoise.getValue((offset + x) * 0.01, 6) * 150;
        iRenderer2::getInstance().drawLine(static_cast<float32>(getWindow()->getClientWidth() - 260 + x - 1),
                                           static_cast<float32>(10 + lastValue),
                                           static_cast<float32>(getWindow()->getClientWidth() - 260 + x),
                                           static_cast<float32>(10 + value), iaColor4f(0, 1, 0, 1));
        lastValue = value;
    }

    ExampleBase::onRenderOrtho();

    // doughnuts <3
    iRenderer2::getInstance().setBlendingActive(true);
    iRenderer2::getInstance().drawFrame(_doughnutMatrix, _doughnuts, _doughnutsFrameIndex);    
}
