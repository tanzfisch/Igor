// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "VoxelExample.h"

#include <iaConsole.h>
using namespace IgorAux;

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

void VoxelExample::registerHandles()
{
    // register callbacks to all the events that are of interest to us
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownSpecificDelegate(this, &VoxelExample::onKeyESCPressed), iKeyCode::ESC);
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpSpecificDelegate(this, &VoxelExample::onKeySpaceReleased), iKeyCode::Space);
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &VoxelExample::onMouseMoved));

    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &VoxelExample::onWindowResized));
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &VoxelExample::onWindowClosed));
}

void VoxelExample::unregisterHandles()
{
    // unregister all the callbacks
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &VoxelExample::onWindowResized));
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &VoxelExample::onWindowClosed));

    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &VoxelExample::onMouseMoved));
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownSpecificDelegate(this, &VoxelExample::onKeyESCPressed), iKeyCode::ESC);
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpSpecificDelegate(this, &VoxelExample::onKeySpaceReleased), iKeyCode::Space);
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
    _window.setSize(1000, 1000);
    _window.open();

    // update the orthogonal projection after we know the windows cient rectangle. the same we do after a resize
    _viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
}

void VoxelExample::initScene()
{
    // create scene and bind it to view
    _scene = iSceneFactory::getInstance().createScene();
    _view.setScene(_scene);

    // create camera and insertt in to scene
    _cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraTranslation->translate(0, 0, 70);
    _camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));

    _scene->getRoot()->insertNode(_cameraHeading);
    _cameraHeading->insertNode(_cameraPitch);
    _cameraPitch->insertNode(_cameraTranslation);
    _cameraTranslation->insertNode(_camera);

    // make it the current active camera
    _camera->makeCurrent();

    // create a directional light
    _lightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _lightTranslate->translate(100, 100, 100);

    _lightRotate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));

    _lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    _lightNode->setAmbient(iaColor4f(0.6f, 0.6f, 0.6f, 1.0f));
    _lightNode->setDiffuse(iaColor4f(0.9f, 0.7f, 0.6f, 1.0f));
    _lightNode->setSpecular(iaColor4f(1.0f, 0.9f, 0.87f, 1.0f));

    _lightRotate->insertNode(_lightTranslate);
    _lightTranslate->insertNode(_lightNode);
    _scene->getRoot()->insertNode(_lightRotate);

    // init sky box and add it to scene
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->setOrder(10);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->getMaterial()->setName("SkyBox");

    iNodeSkyBox* skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
    skyBoxNode->setTextures(
        "skybox_stars/front.jpg",
        "skybox_stars/back.jpg",
        "skybox_stars/left.jpg",
        "skybox_stars/right.jpg",
        "skybox_stars/top.jpg",
        "skybox_stars/bottom.jpg");
    skyBoxNode->setTextureScale(1);
    skyBoxNode->setMaterial(_materialSkyBox);
    _scene->getRoot()->insertNode(skyBoxNode);

    // set up voxel mesh material
    _voxelMeshMaterialID = iMaterialResourceFactory::getInstance().createMaterial("voxel mesh material");
    iMaterialResourceFactory::getInstance().getMaterial(_voxelMeshMaterialID)->addShaderSource("voxel_example_terrain.vert", iShaderObjectType::Vertex);
    iMaterialResourceFactory::getInstance().getMaterial(_voxelMeshMaterialID)->addShaderSource("voxel_example_terrain_directional_light.frag", iShaderObjectType::Fragment);
    iMaterialResourceFactory::getInstance().getMaterial(_voxelMeshMaterialID)->compileShader();
    iMaterialResourceFactory::getInstance().getMaterial(_voxelMeshMaterialID)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
}

void VoxelExample::generateVoxelData()
{
    // if there is none create it
    if (_voxelData == nullptr)
    {
        _voxelData = new iVoxelData();
        // all voxels have a full density as default. so afterwars we need to cut holes in it
        _voxelData->setClearValue(255);
        _voxelData->initData(70, 70, 70);
    }

    // generate new random base with time based seed
    _perlinNoise.generateBase(static_cast<uint32>(iTimer::getInstance().getTime()));

    // clear the voxel data
    _voxelData->clear();

    // so we want a sphere with a center and radius
    iaVector3f center(_voxelData->getWidth() / 2.0f, _voxelData->getHeight() / 2.0f, _voxelData->getDepth() / 2.0f);
    float32 radius = center._x - 4.0f;

    // now iterate though all the voxels and define thair density
    for (int64 x = 0; x < _voxelData->getWidth(); ++x)
    {
        for (int64 y = 0; y < _voxelData->getHeight(); ++y)
        {
            for (int64 z = 0; z < _voxelData->getDepth(); ++z)
            {
                // first figure out if a voxel is outside the sphere
                iaVector3f pos(x,y,z);
                float32 distance = center.distance(pos);

                if(distance > radius - 1.0f)
                {
                    if (distance > radius)
                    {   
                        // outside sphere
                        _voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                    }
                    else
                    {
                        // at the edge of the sphere. 
                        // now we can use the fractional part of the distance to determine how much more than a full voxel we are away from the center
                        // and use this to set the density. this way we get a smooth sphere.
                        float32 denstity = 1 - (distance - (radius - 1.0f));

                        // the density by the way goes from 0-255 but the zero is interpreted as outside ans the 1 is inside but with zero density
                        // so to calculate a propper density we need to multiply the density with 254 and to make it alwasy beein "inside" we add one
                        _voxelData->setVoxelDensity(iaVector3I(x, y, z), (denstity * 254) + 1);
                    }
                }

                // using some perline noise to cut holes in the sphere. this time we skip the smoothing part due to much effort and cluttering the tutorial 
                // with bad understandable code. Ask the author if you'd like to know about smoothing the values
                float64 onoff = _perlinNoise.getValue(iaVector3d(x * 0.04, y * 0.04, z * 0.04), 2, 0.5);
                if (onoff < 0.5)
                {
                    _voxelData->setVoxelDensity(iaVector3I(x, y, z), 0);
                }
            }
        }
    }

    // cleanup the scene
    if (_voxelMeshTransform != nullptr)
    {
        // this will also kill all the children of that node
        iNodeFactory::getInstance().destroyNode(_voxelMeshTransform);
        _voxelMeshTransform = nullptr;
        _voxelMeshModel = nullptr;
    }

    // !!!! now you should first have a look at the VoxelTerrainMeshGenerator class before you continue !!!!

    // prepar 
    TileInformation tileInformation;
    tileInformation._materialID = _voxelMeshMaterialID;
    tileInformation._voxelData = _voxelData;

    iModelDataInputParameter* inputParam = new iModelDataInputParameter(); // will be deleted by iModel
    inputParam->_identifier = "vtg";
    inputParam->_joinVertexes = true;
    inputParam->_needsRenderContext = false;
    inputParam->_modelSourceType = iModelSourceType::Generated;
    inputParam->_loadPriority = 0;
    inputParam->_parameters.setData(reinterpret_cast<const char*>(&tileInformation), sizeof(TileInformation));

    _voxelMeshModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    _voxelMeshModel->setModel("VoxelMesh", inputParam);

    _voxelMeshTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _voxelMeshTransform->translate(-_voxelData->getWidth() / 2, -_voxelData->getHeight() / 2, -_voxelData->getDepth() / 2);
    _voxelMeshTransform->insertNode(_voxelMeshModel);
    _scene->getRoot()->insertNode(_voxelMeshTransform);
}

void VoxelExample::init()
{
    con_endl("--- Voxel Example ---");

    initViews();
    initScene();

    // load font for statistics display
    _font = new iTextureFont("StandardFont.png");
    iStatistics::getInstance().setVerbosity(iRenderStatisticsVerbosity::FPSAndMetrics);

    // launch resource handlers
    iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    registerHandles();

    iModelResourceFactory::getInstance().registerModelDataIO("vtg", &VoxelTerrainMeshGenerator::createInstance);
    generateVoxelData();
}

void VoxelExample::deinit()
{
    iModelResourceFactory::getInstance().unregisterModelDataIO("vtg");
    unregisterHandles();

    iSceneFactory::getInstance().destroyScene(_scene);

    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &VoxelExample::onRenderOrtho));

    _window.close();
    _window.removeView(&_view);
    _window.removeView(&_viewOrtho);

    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }
}

void VoxelExample::onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
{
    if (iMouse::getInstance().getRightButton())
    {
        float32 dx = static_cast<float32>(x1 - x2) * 0.005f;
        float32 dy = static_cast<float32>(y1 - y2) * 0.005f;
        _lightRotate->rotate(dx, iaAxis::Y);
        _lightRotate->rotate(dy, iaAxis::X);

        iMouse::getInstance().setCenter(true);
    }

    if (iMouse::getInstance().getLeftButton())
    {
        float32 dx = static_cast<float32>(x1 - x2) * 0.005f;
        float32 dy = static_cast<float32>(y1 - y2) * 0.005f;
        _cameraHeading->rotate(dx, iaAxis::Y);
        _cameraPitch->rotate(dy, iaAxis::X);

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

void VoxelExample::onKeySpaceReleased()
{
    generateVoxelData();
}

void VoxelExample::onKeyESCPressed()
{
    iApplication::getInstance().stop();
}

void VoxelExample::onRenderOrtho()
{
    iStatistics::getInstance().drawStatistics(&_window, _font, iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
}

void VoxelExample::run()
{
    iApplication::getInstance().run();
}
