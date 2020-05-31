// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "VoxelExample.h"

#include <iaux/system/iaConsole.h>
using namespace IgorAux;

#include <igor/data/iSphere.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/graphics/iRenderer.h>
#include <igor/threading/iTaskManager.h>
#include <igor/graphics/scene/nodes/iNodeSkyBox.h>
#include <igor/graphics/scene/nodes/iNodeLight.h>
#include <igor/graphics/scene/nodes/iNodeCamera.h>
#include <igor/graphics/scene/nodes/iNodeModel.h>
#include <igor/graphics/scene/nodes/iNodeTransform.h>
#include <igor/os/iApplication.h>
#include <igor/graphics/scene/iSceneFactory.h>
#include <igor/graphics/scene/iScene.h>
#include <igor/graphics/scene/nodes/iNodeManager.h>
#include <igor/os/iMouse.h>
#include <igor/os/iTimer.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/threading/tasks/iTaskFlushModels.h>
#include <igor/threading/tasks/iTaskFlushTextures.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/graphics/terrain/data/iVoxelBlock.h>
#include <iaux/math/iaVector3.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/graphics/scene/traversal/iNodeVisitorPrintTree.h>
using namespace Igor;

#include "VoxelTerrainMeshGenerator.h"

VoxelExample::VoxelExample()
{
    init();
}

VoxelExample::~VoxelExample()
{
    deinit();
}

void VoxelExample::init()
{
    con_endl("--- Voxel Example ---");

    initViews();
    initScene();

    // load font for profiler
    _font = new iTextureFont("StandardFont.png");
    _profilerVisualizer.setVerbosity(iProfilerVerbosity::FPSAndMetrics);

    // launch resource handlers
    _flushModelsTask = iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    _flushTexturesTask = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    // register some keayboard and mouse handles
    registerHandles();

    // register model data io
    iModelResourceFactory::getInstance().registerModelDataIO("example_vtg", &VoxelTerrainMeshGenerator::createInstance);

    // generating voxels
    generateVoxelData();
}

void VoxelExample::deinit()
{
    // unregister vertex mesh generator
    iModelResourceFactory::getInstance().unregisterModelDataIO("example_vtg");

    // free some resoures
    _igorLogo = nullptr;

    // destroy scene
    iSceneFactory::getInstance().destroyScene(_scene);

    // abort resource tasks
    iTaskManager::getInstance().abortTask(_flushModelsTask);
    iTaskManager::getInstance().abortTask(_flushTexturesTask);

    unregisterHandles();

    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &VoxelExample::onRenderOrtho));

    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }

    if (_window.isOpen())
    {
        _window.close();
        _window.removeView(&_view);
        _window.removeView(&_viewOrtho);
    }
}

void VoxelExample::registerHandles()
{
    // register callbacks to all the events that are of interest to us
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &VoxelExample::onKeyDown));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &VoxelExample::onMouseMoved));
    iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &VoxelExample::onHandle));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &VoxelExample::onWindowResized));
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &VoxelExample::onWindowClosed));
}

void VoxelExample::unregisterHandles()
{
    // unregister all the callbacks
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &VoxelExample::onWindowResized));
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &VoxelExample::onWindowClosed));
    iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &VoxelExample::onHandle));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &VoxelExample::onMouseMoved));
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &VoxelExample::onKeyDown));
}

void VoxelExample::initViews()
{
    // init the view to render the scene
    _view.setClearColor(iaColor4f(0.97f, 0.97f, 1.0f, 1.0f));
    _view.setPerspective(60.0f);
    _view.setClipPlanes(0.1f, 1000.f);

    // init an other view to display the frame rate
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &VoxelExample::onRenderOrtho));

    // add the views to the window and open it
    _window.setTitle("Voxel Example");
    _window.addView(&_view);
    _window.addView(&_viewOrtho);
    _window.setClientSize(600, 600);
    _window.setCentered();
    _window.open();

    // update the orthogonal projection after we know the windows cient rectangle. the same we do after a resize
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0f);
}

void VoxelExample::initScene()
{
    // create scene and bind it to view
    _scene = iSceneFactory::getInstance().createScene();
    _view.setScene(_scene);

    // create camera
    // first heading transformation node
    iNodeTransform *cameraHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraHeading = cameraHeading->getID();
    // then pitch transformation node
    iNodeTransform *cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraPitch = cameraPitch->getID();
    // and distance to origin transformation node
    iNodeTransform *cameraTranslation = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraTranslation->translate(0, 0, 120);
    // anf of corse the camera
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    // add it to the scene
    _scene->getRoot()->insertNode(cameraHeading);
    cameraHeading->insertNode(cameraPitch);
    cameraPitch->insertNode(cameraTranslation);
    cameraTranslation->insertNode(camera);
    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a
    // scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
    _view.setCurrentCamera(camera->getID());

    // create a directional light
    // transform node
    iNodeTransform *lightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    lightTranslate->translate(100, 100, 100);
    // and light node
    iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor4f(0.6f, 0.6f, 0.6f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.9f, 0.7f, 0.6f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 0.9f, 0.87f, 1.0f));
    // and add it to the scene
    _scene->getRoot()->insertNode(lightTranslate);
    lightTranslate->insertNode(lightNode);

    // reate a sky box and add it to scene
    iNodeSkyBox *skyBoxNode = iNodeManager::getInstance().createNode<iNodeSkyBox>();
    skyBoxNode->setTextures(
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/front.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/back.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/left.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/right.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/top.jpg"),
        iTextureResourceFactory::getInstance().requestFile("skybox_stars/bottom.jpg"));
    skyBoxNode->setTextureScale(1);
    // create a sky box material
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setOrder(10);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setName("SkyBox");
    // and set the sky box material
    skyBoxNode->setMaterial(_materialSkyBox);
    // insert sky box to scene
    _scene->getRoot()->insertNode(skyBoxNode);

    // set up voxel mesh material
    _voxelMeshMaterialID = iMaterialResourceFactory::getInstance().createMaterial("voxel mesh material");
    iMaterialResourceFactory::getInstance().getMaterial(_voxelMeshMaterialID)->addShaderSource("igor/terrain.vert", iShaderObjectType::Vertex);
    iMaterialResourceFactory::getInstance().getMaterial(_voxelMeshMaterialID)->addShaderSource("igor/terrain_directional_light.frag", iShaderObjectType::Fragment);
    iMaterialResourceFactory::getInstance().getMaterial(_voxelMeshMaterialID)->compileShader();

    // set up loading text material
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    // prepare igor logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
}

float32 metaballFunction(iaVector3f metaballPos, iaVector3f checkPos)
{
    return 1.0f / ((checkPos._x - metaballPos._x) * (checkPos._x - metaballPos._x) + (checkPos._y - metaballPos._y) * (checkPos._y - metaballPos._y) + (checkPos._z - metaballPos._z) * (checkPos._z - metaballPos._z));
}

void VoxelExample::generateVoxelData()
{
    _rand.setSeed(static_cast<uint32>(iTimer::getInstance().getApplicationTime()));

    // if there is none create it
    if (_voxelData == nullptr)
    {
        _voxelData = new iVoxelData();
        // all voxels have a full density as default. so afterwars we need to cut holes in it
        _voxelData->setClearValue(0);
        // 255 cubic is the maximum size of a single voxel block. if you need a bigger terrain use iVoxelTerrain see example Ascent
        _voxelData->initData(255, 255, 255);
    }

    // generate new random base with time based seed
    _perlinNoise.generateBase(_rand.getNext());

    // clear the voxel data
    _voxelData->clear();

    // define some threasholds to describe the surface of caves
    const float64 from = 0.444;
    const float64 to = 0.45;
    float64 factor = 1.0 / (to - from);

    // define some threasholds to describe the surface of metaballs
    const float64 fromMeta = 0.017;
    const float64 toMeta = 0.0175;
    float64 factorMeta = 1.0 / (toMeta - fromMeta);

    const float32 coreSize = 0.3f;
    const float32 coreOffset = (1.0f - coreSize) * 0.5f;

    // define some metaballs
    std::vector<std::pair<iaVector3f, float32>> metaballs;
    for (int i = 0; i < 20; ++i)
    {
        metaballs.push_back(std::pair<iaVector3f, float32>(iaVector3f(
                                                               _rand.getNext() % static_cast<int>(_voxelData->getWidth() * coreSize) + (_voxelData->getWidth() * coreOffset),
                                                               _rand.getNext() % static_cast<int>(_voxelData->getHeight() * coreSize) + (_voxelData->getHeight() * coreOffset),
                                                               _rand.getNext() % static_cast<int>(_voxelData->getDepth() * coreSize) + (_voxelData->getDepth() * coreOffset)),
                                                           (((_rand.getNext() % 30) + 70) / 100.0) + 0.4));
    }

    // now iterate through all the voxels and define their density
    for (int64 x = 0; x < _voxelData->getWidth(); ++x)
    {
        for (int64 y = 0; y < _voxelData->getHeight(); ++y)
        {
            for (int64 z = 0; z < _voxelData->getDepth(); ++z)
            {
                float32 density = 0;

                // first figure out if a voxel is outside the sphere
                iaVector3f pos(static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(z));

                float32 noise = (_perlinNoise.getValue(iaVector3d(pos._x * 0.05, pos._y * 0.05, pos._z * 0.05), 3) - 0.5) * 0.017;

                float32 distance = 0;
                for (auto metaball : metaballs)
                {
                    distance += metaballFunction(metaball.first, pos) * metaball.second;
                }

                distance += noise;

                // determine the density of given voxel related to the metaballs
                if (distance >= toMeta)
                {
                    density = 1.0;
                }
                else
                {
                    if (distance >= fromMeta)
                    {
                        density = ((distance - fromMeta) * factorMeta);
                    }
                }

                // using some perline noise to cut holes in the sphere. this time we skip the smoothing part due to much effort and cluttering the tutorial
                // with bad understandable code. Ask the author if you'd like to know about smoothing the values
                float64 onoff = _perlinNoise.getValue(iaVector3d(pos._x * 0.04, pos._y * 0.04, pos._z * 0.04), 3, 0.5);

                // we do the same here as with the metaball surface so it a pears a little smoother
                if (onoff <= from)
                {
                    if (onoff >= to)
                    {
                        density = 1.0 - ((onoff - from) * factor);
                    }
                    else
                    {
                        density = 0.0;
                    }
                }

                if (density > 1.0)
                {
                    density = 1.0;
                }

                if (density < 0.0)
                {
                    density = 0.0;
                }

                if (density > 0.0)
                {
                    _voxelData->setVoxelDensity(iaVector3I(x, y, z), (density * 254) + 1);
                }
            }
        }
    }

    // clean up current scene before we generate a new mesh
    if (_voxelMeshTransform != iNode::INVALID_NODE_ID)
    {
        // this will also kill all the children of that node
        iNodeManager::getInstance().destroyNodeAsync(_voxelMeshTransform);
        _voxelMeshTransform = iNode::INVALID_NODE_ID;
        _voxelMeshModel = iNode::INVALID_NODE_ID;
    }

    // !!!! now you should first have a look at the VoxelTerrainMeshGenerator class before you continue !!!!
    prepareMeshGeneration();

    // set loading flag to true so we can display the loading text on screen
    _loading = true;

    // get current time so we can measure how long it took to generate the mesh
    _time = iTimer::getInstance().getApplicationTime();
}

void VoxelExample::prepareMeshGeneration()
{
    // prepar special tile information for the VoxelTerrainMeshGenerator
    TileInformation tileInformation;
    tileInformation._materialID = _voxelMeshMaterialID;
    tileInformation._voxelData = _voxelData;
    // define the input parameter so Igor knows which iModelDataIO implementation to use and how
    iModelDataInputParameter *inputParam = new iModelDataInputParameter();
    inputParam->_identifier = "example_vtg";
    inputParam->_joinVertexes = true;
    inputParam->_needsRenderContext = false;
    inputParam->_modelSourceType = iModelSourceType::Generated;
    inputParam->_loadPriority = 0;
    inputParam->_keepMesh = true;
    inputParam->_parameters.setData(reinterpret_cast<const char *>(&tileInformation), sizeof(TileInformation));
    // create a model node
    iNodeModel *voxelMeshModel = iNodeManager::getInstance().createNode<iNodeModel>();
    voxelMeshModel->setName("VoxelMeshModel");
    _voxelMeshModel = voxelMeshModel->getID();
    // tell the model node to load data with specified identifier ans the above defined parameter
    // it is important to have a unique identifier each time we generate a mesh otherwhise the cache system would return us a prvious generated mesh
    voxelMeshModel->setModel(iaString("VoxelMesh") + iaString::toString(_incarnation++), iResourceCacheMode::Keep, inputParam);
    // create a transform node to center the mesh to the origin
    iNodeTransform *voxelMeshTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    voxelMeshTransform->setName("VoxelMeshTransform");
    _voxelMeshTransform = voxelMeshTransform->getID();
    voxelMeshTransform->translate(-_voxelData->getWidth() / 2, -_voxelData->getHeight() / 2, -_voxelData->getDepth() / 2);
    // and add to scene
    voxelMeshTransform->insertNode(voxelMeshModel);
    _scene->getRoot()->insertNode(voxelMeshTransform);
}

void VoxelExample::onMouseMoved(const iaVector2i &from, const iaVector2i &to, iWindow *_window)
{
    if (iMouse::getInstance().getLeftButton())
    {
        float32 dx = static_cast<float32>(from._x - to._x) * 0.005f;
        float32 dy = static_cast<float32>(from._y - to._y) * 0.005f;
        iNodeTransform *cameraHeading = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraHeading));
        iNodeTransform *cameraPitch = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraPitch));
        if (cameraHeading != nullptr &&
            cameraPitch != nullptr)
        {
            cameraHeading->rotate(dx, iaAxis::Y);
            cameraPitch->rotate(dy, iaAxis::X);
        }

        iMouse::getInstance().setCenter();
    }
}

void VoxelExample::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void VoxelExample::onWindowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0, clientWidth, clientHeight, 0);
}

void VoxelExample::onKeyDown(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::Space:
        generateVoxelData();
        break;

    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        break;

    case iKeyCode::F4:

        iModelResourceFactory::getInstance().exportModelData("voxelExample.ompf", _scene->getRoot()->getChild("VoxelMeshTransform")->getChild("VoxelMeshModel"), "ompf", iSaveMode::EmbedExternals);
        break;

    case iKeyCode::F8:
        _profilerVisualizer.cycleVerbosity();
        break;

    case iKeyCode::F9:
    {
        iNodeVisitorPrintTree printTree;
        if (_scene != nullptr)
        {
            printTree.printToConsole(_scene->getRoot());
        }
    }
    break;

    case iKeyCode::F10:
        _view.setWireframeVisible(!_view.isWireframeVisible());
        break;

    case iKeyCode::F11:
        _view.setOctreeVisible(!_view.isOctreeVisible());
        break;

    case iKeyCode::F12:
        _view.setBoundingBoxVisible(!_view.isBoundingBoxVisible());
        break;
    }
}

void VoxelExample::onRenderOrtho()
{
    iaMatrixd viewMatrix;
    iRenderer::getInstance().setViewMatrix(viewMatrix);

    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 1));

    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);

    iRenderer::getInstance().setFont(_font);
    iRenderer::getInstance().setFontSize(25.0f);

    if (_loading)
    {
        iRenderer::getInstance().drawString(_window.getClientWidth() * 0.5, _window.getClientHeight() * 0.5, "loading ...", iHorizontalAlignment::Center, iVerticalAlignment::Center);
    }
    else
    {
        iRenderer::getInstance().drawString(_window.getClientWidth() * 0.5, _window.getClientHeight() * 0.1, "press [Space] to recreate", iHorizontalAlignment::Center, iVerticalAlignment::Center);
    }

    drawLogo();

    // draw frame rate in lower right corner
    _profilerVisualizer.draw(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void VoxelExample::drawLogo()
{
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = static_cast<float32>(_igorLogo->getWidth());
    float32 height = static_cast<float32>(_igorLogo->getHeight());
    float32 x = static_cast<float32>(_window.getClientWidth()) - width;
    float32 y = static_cast<float32>(_window.getClientHeight()) - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}

void VoxelExample::onHandle()
{
    // detect if loading is done
    if (_voxelMeshModel != iNode::INVALID_NODE_ID)
    {
        iNodeModel *voxelMeshModel = static_cast<iNodeModel *>(iNodeManager::getInstance().getNode(_voxelMeshModel));

        if (voxelMeshModel != nullptr &&
            voxelMeshModel->isValid())
        {
            if (_loading)
            {
                _loading = false;
                con_endl("generation time: " << iTimer::getInstance().getApplicationTime() - _time << "ms");
            }
        }
    }
}

void VoxelExample::run()
{
    iApplication::getInstance().run();
}
