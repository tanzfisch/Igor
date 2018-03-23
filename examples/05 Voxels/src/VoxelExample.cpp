// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "VoxelExample.h"

#include <iaConsole.h>
using namespace IgorAux;

#include <iSphere.h>
#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iTaskManager.h>
#include <iNodeSkyBox.h>
#include <iNodeLight.h>
#include <iNodeCamera.h>
#include <iNodeModel.h> 
#include <iNodeTransform.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeFactory.h>
#include <iMouse.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iTaskFlushModels.h>
#include <iTaskFlushTextures.h>
#include <iMaterialResourceFactory.h>
#include <iVoxelData.h>
#include <iaVector3.h>
#include <iStatistics.h>
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

    // load font for statistics display
    _font = new iTextureFont("StandardFont.png");
    _statisticsVisualizer.setVerbosity(iRenderStatisticsVerbosity::FPSAndMetrics);

    // launch resource handlers
    _flushModelsTask = iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    _flushTexturesTask = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    // register some keayboard and mouse handles
    registerHandles();

    // register model data io
    iModelResourceFactory::getInstance().registerModelDataIO("vtg", &VoxelTerrainMeshGenerator::createInstance);

    // generating voxels
    generateVoxelData();

    // ok this one is kind of a workaround. In order to handle huge worlds beyond float precision we internally mess around with world positions.
    // in consequence the world positions that end up in the shader are not valid. They are relative to the camera position. To compensate we can 
    // set the world grid resolution here. It basically makes a modulo on the world coordinates so they never exceed float precision. 
    iRenderer::getInstance().setWorldGridResolution(1000.0);
}

void VoxelExample::deinit()
{
    // unregister vertex mesh generator
    iModelResourceFactory::getInstance().unregisterModelDataIO("vtg");

    // destroy scene
    iSceneFactory::getInstance().destroyScene(_scene);

    iTask* modelTask = iTaskManager::getInstance().getTask(_flushModelsTask);
    if (modelTask != nullptr)
    {
        modelTask->abort();
        _flushModelsTask = iTask::INVALID_TASK_ID;
    }

    iTask* textureTask = iTaskManager::getInstance().getTask(_flushTexturesTask);
    if (textureTask != nullptr)
    {
        textureTask->abort();
        _flushTexturesTask = iTask::INVALID_TASK_ID;
    }

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
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownSpecificDelegate(this, &VoxelExample::onKeyESCPressed), iKeyCode::ESC);
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownSpecificDelegate(this, &VoxelExample::onKeySpacePressed), iKeyCode::Space);
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
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownSpecificDelegate(this, &VoxelExample::onKeyESCPressed), iKeyCode::ESC);
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownSpecificDelegate(this, &VoxelExample::onKeySpacePressed), iKeyCode::Space);
}

void VoxelExample::initViews()
{
    // init the view to render the scene
    _view.setClearColor(iaColor4f(0.97, 0.97, 1.0, 1));
    _view.setPerspective(60);
    _view.setClipPlanes(0.1f, 1000.f);

    // init an other view to display the frame rate
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &VoxelExample::onRenderOrtho));

    // add the views to the window and open it
    _window.setTitle("Voxel Example");
    _window.addView(&_view);
    _window.addView(&_viewOrtho);
    _window.setClientSize(1024, 768);
    _window.open();

    // update the orthogonal projection after we know the windows cient rectangle. the same we do after a resize
    _viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
}

void VoxelExample::initScene()
{
    // create scene and bind it to view
    _scene = iSceneFactory::getInstance().createScene();
    _view.setScene(_scene);

    // create camera
    // first heading transformation node
    iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraHeading = cameraHeading->getID();
    // then pitch transformation node
    iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraPitch = cameraPitch->getID();
    // and distance to origin transformation node
    iNodeTransform* cameraTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    cameraTranslation->translate(0, 0, 100);
    // anf of corse the camera
    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
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

    // reate a sky box and add it to scene
    iNodeSkyBox* skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
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
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->setOrder(10);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->getMaterial()->setName("SkyBox");
    // and set the sky box material
    skyBoxNode->setMaterial(_materialSkyBox);
    // insert sky box to scene
    _scene->getRoot()->insertNode(skyBoxNode);

    // set up voxel mesh material
    _voxelMeshMaterialID = iMaterialResourceFactory::getInstance().createMaterial("voxel mesh material");
    iMaterialResourceFactory::getInstance().getMaterial(_voxelMeshMaterialID)->addShaderSource("igor_terrain.vert", iShaderObjectType::Vertex);
    iMaterialResourceFactory::getInstance().getMaterial(_voxelMeshMaterialID)->addShaderSource("igor_terrain_directional_light.frag", iShaderObjectType::Fragment);
    iMaterialResourceFactory::getInstance().getMaterial(_voxelMeshMaterialID)->compileShader();
    iMaterialResourceFactory::getInstance().getMaterial(_voxelMeshMaterialID)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);

    // set up loading text material
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    // prepare igor logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
}

float32 metaballFunction(iaVector3f metaballPos, iaVector3f checkPos)
{
    return 1.0f / ((checkPos._x - metaballPos._x) * (checkPos._x - metaballPos._x) + (checkPos._y - metaballPos._y) * (checkPos._y - metaballPos._y) + (checkPos._z - metaballPos._z) * (checkPos._z - metaballPos._z));
}

void VoxelExample::generateVoxelData()
{
    _rand.setSeed(static_cast<uint32>(iTimer::getInstance().getTime()));

    // if there is none create it
    if (_voxelData == nullptr)
    {
        _voxelData = new iVoxelData();
        // all voxels have a full density as default. so afterwars we need to cut holes in it
        _voxelData->setClearValue(255);
        _voxelData->initData(120, 120, 120);
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

    // define some metaballs
    vector<pair<iaVector3f, float32>> metaballs;
    for (int i = 0; i < 20; ++i)
    {
        metaballs.push_back(pair<iaVector3f, float32>(iaVector3f(_rand.getNext() % static_cast<int>(_voxelData->getWidth() * 0.6) + (_voxelData->getWidth() * 0.2),
            _rand.getNext() % static_cast<int>(_voxelData->getHeight()*0.6) + (_voxelData->getHeight()* 0.2),
            _rand.getNext() % static_cast<int>(_voxelData->getDepth()*0.6) + (_voxelData->getDepth()*0.2)), (((_rand.getNext() % 90) + 10) / 100.0) + 0.6));
    }

    // now iterate through all the voxels and define their density
    for (int64 x = 0; x < _voxelData->getWidth(); ++x)
    {
        for (int64 y = 0; y < _voxelData->getHeight(); ++y)
        {
            for (int64 z = 0; z < _voxelData->getDepth(); ++z)
            {
                // first figure out if a voxel is outside the sphere
                iaVector3f pos(x, y, z);

                float32 distance = 0;
                for (auto metaball : metaballs)
                {
                    distance += metaballFunction(metaball.first, pos) * metaball.second;
                }

                if (distance <= toMeta)
                {
                    if (distance >= fromMeta)
                    {
                        // at the edge of the sphere.
                        // now we can use the fractional part of the distance to determine how much more than a full voxel we are away from the center
                        // and use this to set the density. this way we get a smooth sphere.
                        float32 denstity = ((distance - fromMeta) * factorMeta);

                        // the density by the way goes from 0-255 but the zero is interpreted as outside ans the 1 is inside but with zero density
                        // so to calculate a propper density we need to multiply the density with 254 and to make it alwasy beein "inside" we add one
                        _voxelData->setVoxelDensity(iaVector3I(x, y, z), (denstity * 254) + 1);
                    }
                    else
                    {
                        // outside metaball
                        _voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                    }
                }

                // using some perline noise to cut holes in the sphere. this time we skip the smoothing part due to much effort and cluttering the tutorial 
                // with bad understandable code. Ask the author if you'd like to know about smoothing the values
                float64 onoff = _perlinNoise.getValue(iaVector3d(pos._x * 0.03, pos._y * 0.03, pos._z * 0.03), 4, 0.5);

                // we do the same here as with the metaball surface so it a pears a little smoother
                if (onoff <= from)
                {
                    if (onoff >= to)
                    {
                        float64 gradient = 1.0 - ((onoff - from) * factor);
                        _voxelData->setVoxelDensity(iaVector3I(x, y, z), (gradient * 254) + 1);
                    }
                    else
                    {
                        _voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                    }
                }
            }
        }
    }

    // clean up current scene before we generate a new mesh
    if (_voxelMeshTransform != iNode::INVALID_NODE_ID)
    {
        // this will also kill all the children of that node
        iNodeFactory::getInstance().destroyNodeAsync(_voxelMeshTransform);
        _voxelMeshTransform = iNode::INVALID_NODE_ID;
        _voxelMeshModel = iNode::INVALID_NODE_ID;
    }
    
    // !!!! now you should first have a look at the VoxelTerrainMeshGenerator class before you continue !!!!
    prepareMeshGeneration();

    // set loading flag to true so we can display the loading text on screen
    _loading = true;

    // get current time so we can measure how long it took to generate the mesh
    _time = iTimer::getInstance().getTime();
}

void VoxelExample::prepareMeshGeneration()
{
    // prepar special tile information for the VoxelTerrainMeshGenerator
    TileInformation tileInformation;
    tileInformation._materialID = _voxelMeshMaterialID;
    tileInformation._voxelData = _voxelData;
    // define the input parameter so Igor knows which iModelDataIO implementation to use and how
    iModelDataInputParameter* inputParam = new iModelDataInputParameter();
    inputParam->_identifier = "vtg";
    inputParam->_joinVertexes = true;
    inputParam->_needsRenderContext = false;
    inputParam->_modelSourceType = iModelSourceType::Generated;
    inputParam->_loadPriority = 0;
    inputParam->_parameters.setData(reinterpret_cast<const char*>(&tileInformation), sizeof(TileInformation));
    // create a model node
    iNodeModel* voxelMeshModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    _voxelMeshModel = voxelMeshModel->getID();
    // tell the model node to load data with specified identifier ans the above defined parameter
    // it is important to have a unique identifier each time we generate a mesh otherwhise the cache system would return us a prvious generated mesh
    voxelMeshModel->setModel(iaString("VoxelMesh") + iaString::itoa(_incarnation++), iResourceCacheMode::Free, inputParam);
    // create a transform node to center the mesh to the origin
    iNodeTransform* voxelMeshTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _voxelMeshTransform = voxelMeshTransform->getID();
    voxelMeshTransform->translate(-_voxelData->getWidth() / 2, -_voxelData->getHeight() / 2, -_voxelData->getDepth() / 2);
    // and add to scene
    voxelMeshTransform->insertNode(voxelMeshModel);
    _scene->getRoot()->insertNode(voxelMeshTransform);
}

void VoxelExample::onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
{
    if (iMouse::getInstance().getLeftButton())
    {
        float32 dx = static_cast<float32>(x1 - x2) * 0.005f;
        float32 dy = static_cast<float32>(y1 - y2) * 0.005f;
        iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_cameraHeading));
        iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_cameraPitch));
        if (cameraHeading != nullptr &&
            cameraPitch != nullptr)
        {
            cameraHeading->rotate(dx, iaAxis::Y);
            cameraPitch->rotate(dy, iaAxis::X);
        }

        iMouse::getInstance().setCenter(true);
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

void VoxelExample::onKeySpacePressed()
{
    generateVoxelData();
}

void VoxelExample::onKeyESCPressed()
{
    iApplication::getInstance().stop();
}

void VoxelExample::onRenderOrtho()
{
    iaMatrixd viewMatrix;
    iRenderer::getInstance().setViewMatrix(viewMatrix);

    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 1));

    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);

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
    _statisticsVisualizer.drawStatistics(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void VoxelExample::drawLogo()
{
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);
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
        iNodeModel* voxelMeshModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().getNode(_voxelMeshModel));

        if (voxelMeshModel != nullptr &&
            voxelMeshModel->isReady())
        {
            if (_loading)
            {
                _loading = false;
                con_endl("generation time: " << iTimer::getInstance().getTime() - _time << "ms");
            }
        }
    }
}

void VoxelExample::run()
{
    iApplication::getInstance().run();
}

