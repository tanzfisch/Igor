// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "ExampleBase.h"

ExampleBase::ExampleBase(iWindowPtr window, const iaString &name, bool createBaseSetup, const iaString &skyBoxTexture, int32 zIndex)
    : iLayer(window, name, zIndex), m_displayHelpScreen(false)
{
    con_info("starting example \"" << getName() << "\"");

    if (getWindow() == nullptr)
    {
        return;
    }

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
        _viewOrtho.setClearColorActive(false);
        _viewOrtho.setClearDepthActive(false);
        _viewOrtho.setClipPlanes(0.1f, 10000.0f);
        _viewOrtho.setOrthogonal(0.0, static_cast<float32>(getWindow()->getClientWidth()), static_cast<float32>(getWindow()->getClientHeight()), 0.0);
        _viewOrtho.registerRenderDelegate(iDrawDelegate(this, &ExampleBase::onRenderOrtho));
        getWindow()->addView(&_viewOrtho, getZIndex() + 1);

        if (!skyBoxTexture.isEmpty())
        {
            // create a skybox
            iNodeSkyBox *skyBoxNode = iNodeManager::getInstance().createNode<iNodeSkyBox>();            
            // create a material for the sky box because the default material for all iNodeRender and deriving classes has no textures and uses depth test
            iShaderPtr skyboxShader = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_skybox");
            iParameters paramSkybox({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                                     {IGOR_RESOURCE_PARAM_GENERATE, true},
                                     {IGOR_RESOURCE_PARAM_SHADER, skyboxShader},
                                     {IGOR_RESOURCE_PARAM_TEXTURE0, iResourceManager::getInstance().requestResource<iTexture>(skyBoxTexture)}});
            _materialSkyBox = iResourceManager::getInstance().loadResource<iMaterial>(paramSkybox);
            // set that material
            skyBoxNode->setMaterial(_materialSkyBox);
            // and add it to the scene
            getScene()->getRoot()->insertNode(skyBoxNode);
        }

        // init fonts
        _outlineFont = iTextureFont::create(iResourceManager::getInstance().loadResource<iTexture>("igor_font_default_outline"));
        _standardFont = iTextureFont::create(iResourceManager::getInstance().loadResource<iTexture>("igor_font_default"));

        // prepare igor logo
        iParameters param({{IGOR_RESOURCE_PARAM_ALIAS, "igor_logo_splash"},
                           {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_TEXTURE},
                           {IGOR_RESOURCE_PARAM_TEXTURE_BUILD_MODE, iTextureBuildMode::Normal}});
        _igorLogo = iResourceManager::getInstance().loadResource<iTexture>(param);
    }
}

ExampleBase::~ExampleBase()
{
    // release material
    _materialSkyBox = nullptr;

    // clear scene by destroying it
    iSceneFactory::getInstance().destroyScene(_scene);
    _scene = nullptr;

    // release resources
    _standardFont = nullptr;
    _outlineFont = nullptr;
    _igorLogo = nullptr;

    _viewOrtho.unregisterRenderDelegate(iDrawDelegate(this, &ExampleBase::onRenderOrtho));

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
        iApplication::getInstance().exit();
        return true;

    case iKeyCode::F1:
        m_displayHelpScreen = !m_displayHelpScreen;
        return true;

    case iKeyCode::F6:
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
    return _standardFont;
}

iTextureFontPtr ExampleBase::getOutlineFont() const
{
    return _outlineFont;
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

void ExampleBase::onInit()
{
    // nothing to do
}

void ExampleBase::onDeinit()
{
    // nothing to do
}

void ExampleBase::onUpdate()
{
    // nothing to do
}

void ExampleBase::onRenderOrtho()
{
    // initialize view matrix with identity matrix
    iaMatrixd identity;
    iRenderer::getInstance().setViewMatrix(identity);

    iaMatrixd matrix;
    matrix.translate(0, 0, -1);
    iRenderer::getInstance().setModelMatrix(matrix);

    drawLogo();

    if (m_displayHelpScreen)
    {
        drawHelpScreen();
    }
}

void ExampleBase::drawLogo()
{
    const float32 width = static_cast<float32>(_igorLogo->getWidth());
    const float32 height = static_cast<float32>(_igorLogo->getHeight());
    const float32 x = static_cast<float32>(getWindow()->getClientWidth()) - width;
    const float32 y = static_cast<float32>(getWindow()->getClientHeight()) - height;

    iRenderer::getInstance().drawTexturedRectangle(x, y, width, height, _igorLogo, iaColor4f::white, true);
}

iaString ExampleBase::getHelpString()
{
    iaString help = "Help Screen\n"
                    "----------\n\n"
                    "[ESC] Exit\n"
                    "[F1] Display this screen\n"
                    "[F3] Cycle profiler verbosity\n"
                    "[F6] Print scene graph to console\n"
                    "[F10] Toggle wireframe\n"
                    "[F11] Toggle octree debug display\n"
                    "[F12] Toggle bounding box display\n";

    return help;
}

void ExampleBase::drawHelpScreen()
{
    const iaString &help = getHelpString();

    iaColor4f backgroundColor(0.0, 0.0, 0.0, 0.8);
    iRenderer::getInstance().drawFilledCircle(getWindow()->getClientWidth() * 0.5, getWindow()->getClientHeight() * 0.5, getWindow()->getClientHeight() * 0.4, 64, backgroundColor);
    iRenderer::getInstance().drawCircle(getWindow()->getClientWidth() * 0.5, getWindow()->getClientHeight() * 0.5, getWindow()->getClientHeight() * 0.4, 64);

    iRenderer::getInstance().setFont(getOutlineFont());
    iRenderer::getInstance().setFontSize(15.0f);
    iRenderer::getInstance().drawString(getWindow()->getClientWidth() * 0.5, getWindow()->getClientHeight() * 0.5, help, iHorizontalAlignment::Center, iVerticalAlignment::Center, iaColor4f::white);
}