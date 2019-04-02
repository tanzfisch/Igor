// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "LSystems.h"

#include <iMaterial.h>
#include <iNodeVisitorPrintTree.h>
#include <iTaskManager.h>
#include <iNodeCamera.h>
#include <iNodeModel.h> 
#include <iNodeTransform.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeFactory.h>
#include <iMouse.h>
#include <iKeyboard.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iNodeLight.h>
#include <iModelResourceFactory.h>
#include <iTaskFlushModels.h>
#include <iTaskFlushTextures.h>
#include <iMaterialResourceFactory.h>
#include <iStatistics.h>
#include <iNodeSwitch.h>
#include <iNodeLODSwitch.h>
#include <iNodeLODTrigger.h>
#include <iSkeleton.h>
#include <iTextureResourceFactory.h>
using namespace Igor;

#include <iaConsole.h>
#include <iaString.h>
using namespace IgorAux;

#include "PlantMeshGenerator.h"

LSystems::LSystems()
{
    init();
}

LSystems::~LSystems()
{
    deinit();
}

void LSystems::init()
{
    con(" -- 3D Example --" << endl);
    
    // setup window
    _window.setTitle("Igor - LSystems");
    _window.setClientSize(1024, 768);
    _window.setCentered();
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &LSystems::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &LSystems::onWindowResized));

    // setup perspective view
    _view.setClearColor(iaColor4f(0.0f, 0.0f, 0.0f, 1));
    _view.setPerspective(45);
    _view.setClipPlanes(0.1f, 10000.f);
    _window.addView(&_view);

    // setup orthogonal view
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0f);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &LSystems::onRenderOrtho));
    _window.addView(&_viewOrtho);
    _window.open();

    // init scene
    _scene = iSceneFactory::getInstance().createScene();
    // bind scene to perspective view
    _view.setScene(_scene);

    // setup camera
    // we want a camera which can be rotated arround the origin
    // we will acchive that with 3 transform nodes
    // one is for the heading
    iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    // give the transform node a name. naming is optional and ist jus for helping to debug. 
    // Names do not have to be unique but since it is possible to find nodes by name they better are
    cameraHeading->setName("camera heading");
    cameraHeading->rotate(M_PI * 4.0, iaAxis::Y);
    _cameraHeading = cameraHeading->getID();
    // one is for the pitch
    iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    cameraPitch->setName("camera pitch");
    cameraPitch->rotate(0.25, iaAxis::X);
    _cameraPitch = cameraPitch->getID();
    // and one is for translation or distance from the origin
    iNodeTransform* cameraTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    cameraTranslation->setName("camera translation");
    // translate away from origin
    cameraTranslation->translate(0, 0, 50);
    _cameraTranslation = cameraTranslation->getID();
    // from all nodes that we want to control later we save the node ID
    // and last but not least we create a camera node
    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    camera->setName("camera");
    // and build everything together
    // first we add the heading to the root node
    _scene->getRoot()->insertNode(cameraHeading);
    // than the pitch to the heading node
    cameraHeading->insertNode(cameraPitch);
    // then the translation to the pitch node
    cameraPitch->insertNode(cameraTranslation);
    // and than we add the camera to the translation
    cameraTranslation->insertNode(camera);
    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a 
    // scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
    _view.setCurrentCamera(camera->getID());

    // create a directional light
    // transform node
    iNodeTransform* lightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    lightTranslate->translate(100, 100, 100);
    // and light node
    iNodeLight* lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    lightNode->setAmbient(iaColor4f(0.6f, 0.6f, 0.6f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.9f, 0.7f, 0.6f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 0.9f, 0.87f, 1.0f));
    // and add it to the scene
    _scene->getRoot()->insertNode(lightTranslate);
    lightTranslate->insertNode(lightNode);

    // register plant mesh generator
    iModelResourceFactory::getInstance().registerModelDataIO("pg", &PlantMeshGenerator::createInstance);

    // init render statistics
    _font = new iTextureFont("StandardFont.png");
    _statisticsVisualizer.setVerbosity(iRenderStatisticsVerbosity::FPSAndMetrics);

    // register some callbacks
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &LSystems::onKeyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &LSystems::onMouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &LSystems::onMouseWheel));

    // launch resource handlers
    _flushModelsTask = iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    _flushTexturesTask = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    // prepare igor logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending);
    material->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    material->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    material->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    material->setName("LogoMaterial");

    // generate the L-System
    generateLSystems();
}

void LSystems::deinit()
{
    // unregister some callbacks
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &LSystems::onKeyPressed));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &LSystems::onMouseMoved));
    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &LSystems::onMouseWheel));
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &LSystems::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &LSystems::onWindowResized));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &LSystems::onRenderOrtho));

    // deinit statistics
    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }

    // unregister plant mesh generator
    iModelResourceFactory::getInstance().unregisterModelDataIO("pg");

    // destroy scene
    iSceneFactory::getInstance().destroyScene(_scene);

    // abort resource tasks
    iTaskManager::getInstance().abortTask(_flushModelsTask);
    iTaskManager::getInstance().abortTask(_flushTexturesTask);

    if (_window.isOpen())
    {
        _window.close();
        _window.removeView(&_view);
        _window.removeView(&_viewOrtho);
    }
}

void LSystems::initStyle1()
{
    _lSystem.setRule('F', "FF");

    vector<pair<float64, iaString>> weightedRule1;
    weightedRule1.push_back(pair<float64, iaString>(0.25, "F[+X]OF[-X]+X."));
    weightedRule1.push_back(pair<float64, iaString>(0.25, "FO[-X]F[+X]-X."));
    weightedRule1.push_back(pair<float64, iaString>(0.25, "F[RX]F[LX]ORX."));
    weightedRule1.push_back(pair<float64, iaString>(0.25, "F[LX]FO[RX]LX."));
    _lSystem.setRule('X', weightedRule1);

    vector<pair<float64, iaString>> weightedRule2;
    weightedRule2.push_back(pair<float64, iaString>(0.3, "."));
    weightedRule2.push_back(pair<float64, iaString>(0.7, "*"));
    _lSystem.setRule('.', weightedRule2);
    _lSystem.setAgeFilter('.', iLSystemAgeFunction::Greater, 2);

    vector<pair<float64, iaString>> weightedRule3;
    weightedRule3.push_back(pair<float64, iaString>(0.3, "*"));
    weightedRule3.push_back(pair<float64, iaString>(0.7, "o"));
    _lSystem.setRule('*', weightedRule3);
    _lSystem.setAgeFilter('*', iLSystemAgeFunction::Greater, 4);

    _segmentLength = 0.25f;
    _angle = 0.3f;
    _trunkColor.set(0.0f, 0.8f, 0.0f);
    _budColor.set(0.8f, 0.7f, 0.0f);
    _flowerColor.set(1.0f, 0.0f, 0.0f);
    _leafColor.set(0.0f, 0.7f, 0.0f);
}

void LSystems::initStyle2()
{
    _lSystem.setRule('F', "FF");

    vector<pair<float64, iaString>> weightedRule1;
    weightedRule1.push_back(pair<float64, iaString>(0.25, "F-[[X]+X]O+F[+FX]-X."));
    weightedRule1.push_back(pair<float64, iaString>(0.25, "F+[[X]-X]-F[-FX]O+X."));
    weightedRule1.push_back(pair<float64, iaString>(0.25, "FR[[X]LX]LFO[LFX]RX."));
    weightedRule1.push_back(pair<float64, iaString>(0.25, "FL[[X]RX]ORF[RFX]LX."));
    _lSystem.setRule('X', weightedRule1);

    vector<pair<float64, iaString>> weightedRule2;
    weightedRule2.push_back(pair<float64, iaString>(0.3, "."));
    weightedRule2.push_back(pair<float64, iaString>(0.7, "*"));
    _lSystem.setRule('.', weightedRule2);
    _lSystem.setAgeFilter('.', iLSystemAgeFunction::Greater, 2);

    vector<pair<float64, iaString>> weightedRule3;
    weightedRule3.push_back(pair<float64, iaString>(0.3, "*"));
    weightedRule3.push_back(pair<float64, iaString>(0.7, "o"));
    _lSystem.setRule('*', weightedRule3);
    _lSystem.setAgeFilter('*', iLSystemAgeFunction::Greater, 4);

    _segmentLength = 0.25f;
    _angle = 0.25f;
    _trunkColor.set(0.0f, 0.7f, 0.0f);
    _budColor.set(0.5f, 0.5f, 0.9f);
    _flowerColor.set(1.0f, 0.0f, 1.0f);
    _leafColor.set(0.0f, 0.6f, 0.0f);
}

void LSystems::initStyle3()
{
    _lSystem.setRule('F', "FF");

    vector<pair<float64, iaString>> weightedRule1;
    weightedRule1.push_back(pair<float64, iaString>(0.5, "F[+X]O[-X]FX."));
    weightedRule1.push_back(pair<float64, iaString>(0.5, "F[RX]O[LX]FX."));
    _lSystem.setRule('X', weightedRule1);

    vector<pair<float64, iaString>> weightedRule2;
    weightedRule2.push_back(pair<float64, iaString>(0.3, "."));
    weightedRule2.push_back(pair<float64, iaString>(0.7, "*"));
    _lSystem.setRule('.', weightedRule2);
    _lSystem.setAgeFilter('.', iLSystemAgeFunction::Greater, 2);

    vector<pair<float64, iaString>> weightedRule3;
    weightedRule3.push_back(pair<float64, iaString>(0.3, "*"));
    weightedRule3.push_back(pair<float64, iaString>(0.7, "o"));
    _lSystem.setRule('*', weightedRule3);
    _lSystem.setAgeFilter('*', iLSystemAgeFunction::Greater, 4);

    _segmentLength = 0.25f;
    _angle = 0.5f;
    _trunkColor.set(0.0f, 0.8f, 0.0f);
    _budColor.set(0.8f, 0.8f, 0.5f);
    _flowerColor.set(1.0f, 1.0f, 1.0f);
    _leafColor.set(0.0f, 0.7f, 0.0f);
}

uint64 LSystems::generatePlant(const iaMatrixd& matrix, const iaString& axiom, uint32 iterations, uint64 seed)
{
    PlantInformation plantInformation;
    plantInformation._lSystem = &_lSystem;
    for (int i = 0; i < 10, i < axiom.getSize(); ++i)
    {
        plantInformation._axiom[i] = axiom[i];
    }

    plantInformation._iterations = iterations;
    plantInformation._materialID = iMaterialResourceFactory::getInstance().getDefaultMaterialID();
    plantInformation._seed = seed;
    plantInformation._segmentAngle = _angle;
    plantInformation._segmentLenght = _segmentLength;
    plantInformation._branchColor = _branchColor;
    plantInformation._trunkColor = _trunkColor;
    plantInformation._budColor = _budColor;
    plantInformation._leafColor = _leafColor;
    plantInformation._flowerColor = _flowerColor;

    iModelDataInputParameter* inputParam = new iModelDataInputParameter();
    inputParam->_identifier = "pg";
    inputParam->_joinVertexes = true;
    inputParam->_needsRenderContext = false;
    inputParam->_modelSourceType = iModelSourceType::Generated;
    inputParam->_loadPriority = 0;
    inputParam->_parameters.setData(reinterpret_cast<const char*>(&plantInformation), sizeof(PlantInformation));

    iNodeModel* modelNode = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    modelNode->setModel(iaString("plant_") + iaString::itoa(iterations) + iaString("_") + iaString::itoa(_incarnation++), iResourceCacheMode::Free, inputParam);

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->setMatrix(matrix);

    // and add to scene
    transformNode->insertNode(modelNode);

    iNodePtr groupNode = static_cast<iNodePtr>(iNodeFactory::getInstance().getNode(_groupNodeID));
    groupNode->insertNode(transformNode);

    return modelNode->getID();
}

bool LSystems::checkIfDone()
{
    bool result = true;
    for (auto id : _plantsInProgress)
    {
        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().getNode(id));
        if (!model->isReady())
        {
            result = false;
        }
    }

    return result;
}

void LSystems::generateLSystems()
{
    if (!checkIfDone())
    {
        return;
    }

    if (_groupNodeID != iNode::INVALID_NODE_ID)
    {
        iNodeFactory::getInstance().destroyNodeAsync(_groupNodeID);
        _groupNodeID = iNode::INVALID_NODE_ID;
    }
    
    _styleCounter = (++_styleCounter) % 3;

    _lSystem.clear();

    switch (_styleCounter)
    {
    case 0:
        initStyle1();
        break;

    case 1:
        initStyle2();
        break;

    case 2:
        initStyle3();
        break;
    }

    iaMatrixf currentMatrix;

    // using the same seed for all instances of plants so we see the different stages of growth of the same plant
    uint64 seed = static_cast<uint64>(iTimer::getInstance().getApplicationTime());

    iNodePtr groupNode = static_cast<iNodePtr>(iNodeFactory::getInstance().createNode(iNodeType::iNode));
    _groupNodeID = groupNode->getID();
    _scene->getRoot()->insertNode(groupNode);

    _plantsInProgress.clear();

    for (int i = 0; i < 7; ++i)
    {
        iaMatrixd matrix;
        matrix.translate(-15 + i* 5, -15, 0);
        _plantsInProgress.push_back(generatePlant(matrix, "X", i, seed));
    }
}

void LSystems::onMouseWheel(int32 d)
{
    iNodeTransform* camTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_cameraTranslation));
    if (camTranslation != nullptr)
    {
        if (d < 0)
        {
            camTranslation->translate(0, 0, 10);
        }
        else
        {
            camTranslation->translate(0, 0, -10);
        }
    }
}

void LSystems::onMouseMoved(const iaVector2i& from, const iaVector2i& to, iWindow* _window)
{
    if (iMouse::getInstance().getLeftButton())
    {
        iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_cameraPitch));
        iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_cameraHeading));

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((from._y - to._y) * 0.005f, iaAxis::X);
            cameraHeading->rotate((from._x - to._x) * 0.005f, iaAxis::Y);
            iMouse::getInstance().setCenter(true);
        }
    }
}

void LSystems::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void LSystems::onWindowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(clientWidth), static_cast<float32>(clientHeight), 0.0f);
}

void LSystems::onKeyPressed(iKeyCode key)
{
    if (key == iKeyCode::ESC)
    {
        iApplication::getInstance().stop();
    }

    if (key == iKeyCode::F1)
    {
        iNodeVisitorPrintTree printTree;
        if (_scene != nullptr)
        {
            printTree.printToConsole(_scene->getRoot());
        }
    }

    if (key == iKeyCode::Space)
    {
        generateLSystems();
    }
}

void LSystems::onRenderOrtho()
{
    iaMatrixd viewMatrix;
    iRenderer::getInstance().setViewMatrix(viewMatrix);

    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    drawLogo();

    // draw frame rate in lower right corner
    _statisticsVisualizer.drawStatistics(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void LSystems::drawLogo()
{
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = static_cast<float32>(_igorLogo->getWidth());
    float32 height = static_cast<float32>(_igorLogo->getHeight());
    float32 x = static_cast<float32>(_window.getClientWidth()) - width;
    float32 y = static_cast<float32>(_window.getClientHeight()) - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}

void LSystems::run()
{
    iApplication::getInstance().run();
}
