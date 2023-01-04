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

    initParticleSystem();
}

void Example2D::initParticleSystem()
{
    iaGradientColor4f colors;
    colors.setValue(0.0f, iaColor4f(0.0f, 0.2f, 1.0f, 1.0f));
    colors.setValue(0.5f, iaColor4f(0.0f, 0.8f, 1.0f, 1.0f));
    colors.setValue(1.0f, iaColor4f(0.0f, 0.8f, 1.0f, 0.0f));

    iaGradientVector2f visibility;
    visibility.setValue(0.0f, iaVector2f(4.5f, 5.0f));
    visibility.setValue(1.0f, iaVector2f(1.5f, 2.0f));
    visibility.setValue(2.0f, iaVector2f(4.5f, 5.0f));

    iaGradientf emission;
    emission.setValue(0.0, 3);

    iaGradientVector2f velocity;
    velocity.setValue(0.0f, iaVector2f(0.4f, 0.5f));
    velocity.setValue(1.0f, iaVector2f(0.6f, 0.6f));
    velocity.setValue(2.0f, iaVector2f(0.4f, 0.5f));

    // using negative lift here so it's basically weight
    iaGradientVector2f lift;
    lift.setValue(0.0f, iaVector2f(-0.002f, -0.002f));

    iaGradientVector2f startSize;
    startSize.setValue(0.0f, iaVector2f(100.0f, 100.0f));

    _particleSystem.setMaxParticleCount(800);
    _particleSystem.setColorGradient(colors);
    _particleSystem.setEmissionGradient(emission);
    _particleSystem.setStartVisibleTimeGradient(visibility);
    _particleSystem.setVelocityOriented();
    _particleSystem.setStartSizeGradient(startSize);
    _particleSystem.setStartVelocityGradient(velocity);
    _particleSystem.setStartLiftGradient(lift);
    _particleSystem.setAirDrag(0.985f);
    _particleSystem.start();
    
    _particleEmitter.setSize(100.0);
    _particleEmitter.setType(iEmitterType::Sphere);
    iaMatrixd matrix;
    matrix.translate(300,300,-10);
    _particleEmitter.setWorldMatrix(matrix);

    _particleTargetMaterial = iTargetMaterial::create();
    // _particleTargetMaterial->setVelocityOriented(true);

    _particlesMaterial = iMaterialResourceFactory::getInstance().loadMaterial("igor/particles_ortho_projection.mat");
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
    _backgroundTexture = nullptr;
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
    // rotate the doughnut
    _doughnutMatrix.rotate(0.05, iaAxis::Z);

    // moves the doughnut towards the mouse position
    _doughnutMatrix._pos += (iaVector3f(_lastMousePos._x, _lastMousePos._y, 0.0) - _doughnutMatrix._pos) * 0.05f;
  
    // doughnut time <3
    if (time > _doughnutsTime + iaTime::fromSeconds(0.5))
    {
        _doughnutsFrameIndex++;
        _doughnutsFrameIndex = _doughnutsFrameIndex % 4;
        _doughnutsTime = time;
    }

    // moves one of the splines support point to the logo's position
    _spline.setSupportPoint(iaVector3f(_lastMousePos._x, _lastMousePos._y, 0.0), 3);    

    _particleSystem.onUpdate(_particleEmitter);
}

void Example2D::onRenderOrtho()
{
    // initialize view matrix with identity matrix
    iaMatrixd identity;
    iRenderer::getInstance().setViewMatrix(identity);

    // since the model matrix is by default an identity matrix which would cause all our 2d rendering end up at depth zero
    // and the near clipping plane of our frustum can't be zero we have to push the scene a bit away from zero
    iaMatrixd matrix;
    matrix.translate(0, 0, -100);
    iRenderer::getInstance().setModelMatrix(matrix);

    // draw some background
/*    const float32 width = getWindow()->getClientWidth();
    const float32 height = getWindow()->getClientHeight();
    iaVector2f tiling(width / _backgroundTexture->getWidth(),
                      height / _backgroundTexture->getHeight());
    iRenderer::getInstance().drawTexturedRectangle(0.0f, 0.0f, width, height, _backgroundTexture, iaColor4f::white, false, tiling);

    iRenderer::getInstance().drawFilledRectangle(10, 10, 200, 150, iaColor4f(0, 0, 0, 1));
    iRenderer::getInstance().drawFilledRectangle(220, 10, 200, 150, iaColor4f(0, 0, 0, 1));

    iRenderer::getInstance().setLineWidth(3);

    iRenderer::getInstance().drawLine(20, 20, 200, 150, iaColor4f(1, 1, 0, 1));
    iRenderer::getInstance().drawLine(20, 150, 200, 20, iaColor4f(1, 1, 0, 1));
    iRenderer::getInstance().drawLine(110, 20, 110, 150, iaColor4f(1, 1, 0, 1));
    iRenderer::getInstance().drawLine(20, 85, 200, 85, iaColor4f(1, 1, 0, 1));

    iRenderer::getInstance().setLineWidth(1);

    for (int x = 0; x < 10; ++x)
    {
        for (int y = 0; y < 7; ++y)
        {
            const float32 radius = _rand.getNextFloat() * 9.0f + 1.0f;
            iRenderer::getInstance().drawCircle(static_cast<float32>(230 + x * 20), static_cast<float32>(25 + y * 20), radius, 8, iaColor4f(1, 1, 0, 1));
        }
    }

    iRenderer::getInstance().drawFilledCircle(500, 400, 250, 5, iaColor4f::red);
    iRenderer::getInstance().drawFilledCircle(700, 500, 100, 8, iaColor4f(1.0,1.0,0.0,0.5));
    iRenderer::getInstance().drawFilledCircle(750, 600, 50, 16, iaColor4f::green);

    // draw with dummy texture
    iRenderer::getInstance().drawTexturedRectangle(10, 170, 410, 410, iTextureResourceFactory::getInstance().getDummyTexture(), iaColor4f::white, true);

    // draw some text from wikipedia
    iaString wikipediaOpenGL = "OpenGL (Open Graphics Library) ist eine Spezifikation fuer eine plattform- und programmiersprachenunabhaengige "
                               "Programmierschnittstelle zur Entwicklung von 2D- und 3D-Computergrafik. Der OpenGL-Standard beschreibt etwa 250 "
                               "Befehle, die die Darstellung komplexer 3D-Szenen in Echtzeit erlauben. Zudem koennen andere Organisationen "
                               "(zumeist Hersteller von Grafikkarten) proprietaere Erweiterungen definieren.\n\nWikipedia";

    iRenderer::getInstance().setFont(getFont());
    iRenderer::getInstance().setFontSize(19.0f);
    iRenderer::getInstance().drawString(490, 240, wikipediaOpenGL, iaColor4f(0, 0, 0, 1), 400);

    // draw spline
    std::vector<iaVector3f> points;
    _spline.getPoints(points, 200);
    iRenderer::getInstance().setLineWidth(5);
    iRenderer::getInstance().drawLineStrip(points, iaColor4f(1, 0, 0.5, 1));

    // draw perlin noise based graph in the upper right corner
    iRenderer::getInstance().drawFilledRectangle(static_cast<float32>(getWindow()->getClientWidth() - 260), 10.0f, 250.0f, 150.0f, iaColor4f(0, 0, 0, 1));
    iRenderer::getInstance().setLineWidth(3);

    // just to make that graph move a bit
    static float32 offset = 0.0f;
    offset += 1.0f;

    float64 lastValue = _perlinNoise.getValue(offset * 0.01, 6) * 150;
    for (int x = 1; x < 250; ++x)
    {
        float64 value = _perlinNoise.getValue((offset + x) * 0.01, 6) * 150;
        iRenderer::getInstance().drawLine(static_cast<float32>(getWindow()->getClientWidth() - 260 + x - 1),
                                           static_cast<float32>(10 + lastValue),
                                           static_cast<float32>(getWindow()->getClientWidth() - 260 + x),
                                           static_cast<float32>(10 + value), iaColor4f(0, 1, 0, 1));
        lastValue = value;
    }

    ExampleBase::onRenderOrtho();

    // doughnuts <3
    iRenderer::getInstance().drawFrame(_doughnutMatrix, _doughnuts, _doughnutsFrameIndex, iaColor4f::white, true);*/

    iRenderer::getInstance().setMaterial(_particlesMaterial);
    iRenderer::getInstance().drawBuffer(_particleSystem.getVertexArray(), iRenderPrimitive::Points, _particleTargetMaterial);

}
