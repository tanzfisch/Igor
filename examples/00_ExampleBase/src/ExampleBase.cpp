// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "ExampleBase.h"

ExampleBase::ExampleBase(iWindow *window, const iaString &name, bool createBaseSetup, bool createSkyBox, int32 zIndex)
    : iLayer(window, name, zIndex)
{
    con_info("starting example \"" << getName() << "\"");

    if (getWindow() != nullptr)
    {
        // set window title with example name
        getWindow()->setTitle(getName());

        if (createBaseSetup)
        {
            // setup perspective view
            _view.setName("Example Base View");
            // set up a gray background for examples that do not set up a camera
            _view.setClearColor(iaColor4f(0.25f, 0.25f, 0.25f, 1));
            _view.setPerspective(45);
            _view.setClipPlanes(0.1f, 10000.f);
            getWindow()->addView(&_view, getZIndex());

            // init scene
            _scene = iSceneFactory::getInstance().createScene();
            // bind scene to perspective view
            _view.setScene(_scene);

            // setup orthogonal view
            _viewOrtho.setName("Logo View");
            _viewOrtho.setClearColor(false);
            _viewOrtho.setClearDepth(false);
            _viewOrtho.setOrthogonal(0.0, static_cast<float32>(getWindow()->getClientWidth()), static_cast<float32>(getWindow()->getClientHeight()), 0.0);
            _viewOrtho.registerRenderDelegate(iDrawDelegate(this, &ExampleBase::onRenderOrtho));
            getWindow()->addView(&_viewOrtho, getZIndex() + 1);

            // start resource tasks
            _taskFlushModels = iTaskManager::getInstance().addTask(new iTaskFlushModels(window));
            _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(window));

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
}

ExampleBase::~ExampleBase()
{
    if (getWindow() != nullptr &&
        getWindow()->isOpen())
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
        getWindow()->close();
    }

    con_info("stopped example \"" << getName() << "\"");
}

void ExampleBase::onEvent(iEvent &event)
{
    event.dispatch<iEventWindowResize>(IGOR_BIND_EVENT_FUNCTION(ExampleBase::onWindowResize));
    event.dispatch<iEventKeyUp>(IGOR_BIND_EVENT_FUNCTION(ExampleBase::onKeyUp));
}

bool ExampleBase::onWindowResize(iEventWindowResize &event)
{
    // update the the view port projection matrix so the widget manager desktop will fit on screen
    _viewOrtho.setOrthogonal(0.0, static_cast<float32>(event.getWindow()->getClientWidth()),
                             static_cast<float32>(event.getWindow()->getClientHeight()), 0.0);

    // do not consume this one
    return false;
}

bool ExampleBase::onKeyUp(iEventKeyUp &event)
{
    switch (event.getKey())
    {
    case iKeyCode::ESC:
        iApplication::getInstance().stop();
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

void ExampleBase::onPreDraw()
{
}

void ExampleBase::onPostDraw()
{
}

void ExampleBase::onRenderOrtho()
{
    iaMatrixd matrix;
    iRenderer::getInstance().setViewMatrix(matrix);
    matrix.translate(0, 0, -1);
    iRenderer::getInstance().setModelMatrix(matrix);

    drawLogo();
}

void ExampleBase::drawLogo()
{
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = static_cast<float32>(_igorLogo->getWidth());
    float32 height = static_cast<float32>(_igorLogo->getHeight());
    float32 x = static_cast<float32>(getWindow()->getClientWidth()) - width;
    float32 y = static_cast<float32>(getWindow()->getClientHeight()) - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}