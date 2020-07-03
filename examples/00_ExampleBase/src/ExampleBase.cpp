// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "ExampleBase.h"

ExampleBase::ExampleBase(const iaString &name, bool createBaseSetup, bool createSkyBox)
    : _name(name)
{
    con_info("starting example \"" << _name << "\"");

    if (createBaseSetup)
    {
        // init window parameters
        _window.setTitle(_name);
        _window.setClientSize(1024, 768);
        _window.setVSync(true);

        // center the window on screen
        _window.setCentered();

        // setup perspective view
        _view.setClearColor(iaColor4f(0.5f, 0, 0.5f, 1));
        _view.setPerspective(45);
        _view.setClipPlanes(0.1f, 10000.f);
        _window.addView(&_view);

        // init scene
        _scene = iSceneFactory::getInstance().createScene();
        // bind scene to perspective view
        _view.setScene(_scene);

        // setup orthogonal view
        _viewOrtho.setClearColor(false);
        _viewOrtho.setClearDepth(false);
        _viewOrtho.setOrthogonal(0.0, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0);
        _viewOrtho.registerRenderDelegate(iDrawDelegate(this, &ExampleBase::onRenderOrtho));
        _window.addView(&_viewOrtho);

        // and open the window
        _window.open();

        // start resource tasks
        _taskFlushModels = iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
        _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

        // setup profiler visualisation
        _profilerVisualizer.setVerbosity(iProfilerVerbosity::FPSAndMetrics);

        if (createSkyBox)
        {
            // create a skybox
            iNodeSkyBox *skyBoxNode = iNodeManager::getInstance().createNode<iNodeSkyBox>();
            // set it up with the default skybox texture
            skyBoxNode->setTextures(
                iTextureResourceFactory::getInstance().requestFile("skybox_default/front.png"),
                iTextureResourceFactory::getInstance().requestFile("skybox_default/back.png"),
                iTextureResourceFactory::getInstance().requestFile("skybox_default/left.png"),
                iTextureResourceFactory::getInstance().requestFile("skybox_default/right.png"),
                iTextureResourceFactory::getInstance().requestFile("skybox_default/top.png"),
                iTextureResourceFactory::getInstance().requestFile("skybox_default/bottom.png"));
            // create a material for the sky box because the default material for all iNodeRender and deriving classes has no textures and uses depth test
            _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
            auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox);
            material->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
            material->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
            material->setOrder(iMaterial::RENDER_ORDER_MIN);
            material->setName("SkyBox");
            // set that material
            skyBoxNode->setMaterial(_materialSkyBox);
            // and add it to the scene
            getScene()->getRoot()->insertNode(skyBoxNode);
        }

        // init font for render profiler
        _font = new iTextureFont("StandardFont.png");

        // prepare igor logo
        _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
        _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
        auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending);
        material->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
        material->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
        material->setRenderState(iRenderState::Blend, iRenderStateValue::On);
        material->setName("LogoMaterial");
    }
}

ExampleBase::~ExampleBase()
{
    if (_window.isOpen())
    {
        // destroy materials
        if (_materialSkyBox != iMaterial::INVALID_MATERIAL_ID)
        {
            iMaterialResourceFactory::getInstance().destroyMaterial(_materialSkyBox);
        }
        iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTextureAndBlending);

        // clear scene by destoying it
        iSceneFactory::getInstance().destroyScene(_scene);
        _scene = nullptr;

        // release resources
        if (_font != nullptr)
        {
            delete _font;
            _font = nullptr;
        }

        // release logo texture
        _igorLogo = nullptr;

        // abort resource tasks
        iTaskManager::getInstance().abortTask(_taskFlushModels);
        iTaskManager::getInstance().abortTask(_taskFlushTextures);

        _viewOrtho.unregisterRenderDelegate(iDrawDelegate(this, &ExampleBase::onRenderOrtho));

        // closes the window if it was not closed already
        _window.close();
    }

    con_info("stopped example \"" << _name << "\"");
}

void ExampleBase::onEvent(iEvent &event)
{
    event.dispatch<iWindowResizeEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(ExampleBase::onWindowResize));
    event.dispatch<iKeyUpEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(ExampleBase::onKeyUp));
}

bool ExampleBase::onWindowResize(iWindowResizeEvent_TMP &event)
{
    // update the the view port projection matrix so the widget manager desktop will fit on screen
    _viewOrtho.setOrthogonal(0.0, static_cast<float32>(event.getWidth()), static_cast<float32>(event.getHeight()), 0.0);

    // do not consume this one
    return false;
}

bool ExampleBase::onKeyUp(iKeyUpEvent_TMP &event)
{
    switch (event.getKey())
    {
    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        return true;

    case iKeyCode::F8:
        _profilerVisualizer.cycleVerbosity();
        return true;

    case iKeyCode::F9:
    {
        iNodeVisitorPrintTree printTree;
        if (getScene() != nullptr)
        {
            printTree.printToConsole(getScene()->getRoot());
        }
    }
        return true;

    case iKeyCode::F10:
        getView().setWireframeVisible(!getView().isWireframeVisible());
        return true;

    case iKeyCode::F11:
        getView().setOctreeVisible(!getView().isOctreeVisible());
        return true;

    case iKeyCode::F12:
        getView().setBoundingBoxVisible(!getView().isBoundingBoxVisible());
        return true;
    }

    return false;
}

iTextureFontPtr ExampleBase::getFont() const
{
    return _font;
}

iView &ExampleBase::getView()
{
    return _view;
}

iView &ExampleBase::getViewOrtho()
{
    return _viewOrtho;
}

iScenePtr ExampleBase::getScene()
{
    return _scene;
}

iMaterialID ExampleBase::getFontMaterial() const
{
    return _materialWithTextureAndBlending;
}

void ExampleBase::onInit()
{
    // nothing to do
}

void ExampleBase::onDeinit()
{
    // nothing to do
}

const iaString &ExampleBase::getName() const
{
    return _name;
}

iWindow &ExampleBase::getWindow()
{
    return _window;
}

void ExampleBase::onPreDraw()
{
}

void ExampleBase::onPostDraw()
{
}

void ExampleBase::onRenderOrtho()
{
    iaMatrixd viewMatrix;
    iRenderer::getInstance().setViewMatrix(viewMatrix);

    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    drawLogo();

    // draw frame rate in lower right corner
    _profilerVisualizer.draw(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void ExampleBase::drawLogo()
{
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = static_cast<float32>(_igorLogo->getWidth());
    float32 height = static_cast<float32>(_igorLogo->getHeight());
    float32 x = static_cast<float32>(_window.getClientWidth()) - width;
    float32 y = static_cast<float32>(_window.getClientHeight()) - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}