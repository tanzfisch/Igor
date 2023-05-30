// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "Example2D.h"

#include <sstream>
#include <complex>

Example2D::Example2D(iWindowPtr window)
    : ExampleBase(window, "2D Interfaces")
{
}

void Example2D::onInit()
{
    // load the background tile texture
    _backgroundTexture = iResourceManager::getInstance().requestResource<iTexture>("ice.png");
    _dummyTexture = iResourceManager::getInstance().requestResource<iTexture>("fallback_texture");

    // load a texture as sprite
    _doughnuts = iResourceManager::getInstance().loadResource<iSprite>("doughnuts.sprite");
    // setup matrix
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
    _rand.setSeed(static_cast<uint32>(iaTime::getNow().getMicroseconds()));

    initMandelbrotTexture();
    initParticleSystem();
}

static int32 mandelbrotSet(float32 real, float32 imag)
{
    std::complex<float32> c(real, imag);
    std::complex<float32> z(0, 0);

    int32 iterations = 0;
    while (abs(z) <= 2 && iterations < 255)
    {
        z = z * z + c;
        iterations++;
    }

    return iterations;
}

void Example2D::initMandelbrotTexture()
{
    const float64 width = 256;
    const float64 height = 256;
    const float64 scale = 3.0;
    const int32 maxIterations = 100;

    iPixmapPtr pixmap = iPixmap::createPixmap(width, height, iColorFormat::RGBA);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            double real = (x - width / 2.0) * scale / width;
            double imag = (y - height / 2.0) * scale / height;

            std::complex<float32> c(real, imag);
            std::complex<float32> z(0, 0);
            int32 iterations = 0;
            while (abs(z) <= 2 && iterations < maxIterations)
            {
                z = z * z + c;
                iterations++;
            }

            pixmap->setPixelRGBA(x, y, 0xff, 0xff, 0xff, iterations * 2);
        }
    }

    iParameters param({{"name", iaString("mandelbrot_texture")},
                       {"type", iaString("texture")},
                       {"cacheMode", iResourceCacheMode::Keep},
                       {"pixmap", pixmap}});

    _mandelbrotTexture = std::dynamic_pointer_cast<iTexture>(iResourceManager::getInstance().loadResource(param));
}

void Example2D::initParticleSystem()
{
    iaKeyFrameGraphColor4f colors;
    colors.setValue(1.0f, iaColor4f(0.0f, 0.4f, 1.0f, 1.0f));
    colors.setValue(0.5f, iaColor4f(0.4f, 0.8f, 1.0f, 0.8f));
    colors.setValue(0.0f, iaColor4f(0.8f, 0.8f, 1.0f, 0.0f));

    iaKeyFrameGraphVector2f visibility;
    visibility.setValue(0.0f, iaVector2f(3.0f, 8.0f));

    iaKeyFrameGraphf emission;
    emission.setValue(0.0, 1);

    iaKeyFrameGraphVector2f velocity;
    velocity.setValue(0.0f, iaVector2f(4.0f, 9.0f));

    // using positive lift here represents weight
    iaKeyFrameGraphVector2f lift;
    lift.setValue(0.0f, iaVector2f(0.02f, 0.02f));

    iaKeyFrameGraphVector2f startSize;
    startSize.setValue(0.0f, iaVector2f(40.0f, 40.0f));

    iaKeyFrameGraphf scaleSize;
    scaleSize.setValue(1.0f, 1.0f);
    scaleSize.setValue(0.0f, 3.0f);

    _particleSystem.getSystem().setMaxParticleCount(5000);
    _particleSystem.getSystem().setColorGradient(colors);
    _particleSystem.getSystem().setEmissionGradient(emission);
    _particleSystem.getSystem().setStartAgeGradient(visibility);
    _particleSystem.getSystem().setStartSizeGradient(startSize);
    _particleSystem.getSystem().setSizeScaleGradient(scaleSize);
    _particleSystem.getSystem().setStartVelocityGradient(velocity);
    _particleSystem.getSystem().setStartLiftGradient(lift);
    _particleSystem.getSystem().setAirDrag(0.985f);
    _particleSystem.getSystem().start();

    _particleSystem.getEmitter().setType(iEmitterType::Disc);
    _particleSystem.getEmitter().setSize(20.0);

    // flip emitter upside down since in ortho the coordinates are upside down
    iaMatrixd matrix;
    matrix.rotate(-130.0 / 180.0 * M_PI, iaAxis::Z);
    _particleSystem.getEmitter().setWorldMatrix(matrix);

    _particleSystem.getTargetMaterial()->setTexture(iResourceManager::getInstance().requestResource<iTexture>("particleDot.png"), 0);
    _particleSystem.getTargetMaterial()->setTexture(iResourceManager::getInstance().requestResource<iTexture>("octave1.png"), 1);
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
    _mandelbrotTexture = nullptr;
}

void Example2D::onEvent(iEvent &event)
{
    // first call example base
    ExampleBase::onEvent(event);

    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(Example2D::onKeyDown));
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
}

void Example2D::onRenderOrtho()
{
    // initialize view matrix with identity matrix
    iaMatrixd identity;
    iRenderer::getInstance().setViewMatrix(identity);

    // since the model matrix is by default an identity matrix which would cause all our 2d rendering end up at depth zero
    // and the near clipping plane of our frustum can't be zero we have to push the scene a bit away from zero
    iaMatrixd matrix;
    matrix.translate(0, 0, -1);
    iRenderer::getInstance().setModelMatrix(matrix);

    // draw some background
    const float32 width = getWindow()->getClientWidth();
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
    iRenderer::getInstance().drawFilledCircle(700, 500, 100, 8, iaColor4f(1.0, 1.0, 0.0, 0.5));
    iRenderer::getInstance().drawFilledCircle(750, 600, 50, 16, iaColor4f::green);

    // draw with dummy texture
    iRenderer::getInstance().drawTexturedRectangle(10, 170, 200, 200, _dummyTexture, iaColor4f::white, true);

    // draw with mandelbrot texture
    iRenderer::getInstance().drawTexturedRectangle(static_cast<float32>(getWindow()->getClientWidth() - 200), 180.0f, 
                                                   256.0f, 256.0f, _mandelbrotTexture, iaColor4f::black, true);

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

    // draw particle system
    iaMatrixd psmatrix;
    psmatrix.translate(0, 650, 0);
    _particleSystem.draw(psmatrix);

    // doughnuts <3
    iRenderer::getInstance().drawSprite(_doughnutMatrix, _doughnuts, _doughnutsFrameIndex, iaVector2f(64.0, 64.0), iaColor4f::white, true);
}

bool Example2D::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::Space:
        iApplication::getInstance().pause(!iApplication::getInstance().isPaused());
        return true;
    }

    return false;
}
