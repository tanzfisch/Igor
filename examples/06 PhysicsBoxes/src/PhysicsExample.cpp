#include "PhysicsExample.h"

#include <iaConsole.h>
using namespace IgorAux;

#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iNodeVisitorPrintTree.h>
#include <iTaskManager.h>
#include <iNodeSkyBox.h>
#include <iNodeCamera.h>
#include <iNodeModel.h> 
#include <iNodeTransform.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeFactory.h>
#include <iMouse.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iNodeLight.h>
#include <iModelResourceFactory.h>
#include <iPhysics.h>
#include <iEntity.h>
#include <iEntityFactory.h>
#include <iTaskFlushModels.h>
#include <iTaskFlushTextures.h>
#include <iMaterialResourceFactory.h>
#include <iStatistics.h>
using namespace Igor;

PhysicsExample::PhysicsExample()
{
	init();
}

PhysicsExample::~PhysicsExample()
{
	deinit();
}

void PhysicsExample::deinit()
{
    if (_taskFlushModels != nullptr)
    {
        _taskFlushModels->abort();
        _taskFlushModels = nullptr;
    }

    if (_taskFlushTextures != nullptr)
    {
        _taskFlushTextures->abort();
        _taskFlushTextures = nullptr;
    }

    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &PhysicsExample::renderOrtho));

    iEntityFactory::getInstance().destroyEntity(_floor);
    _floor = nullptr;

    auto boxIter = _boxes.begin();
    while (_boxes.end() != boxIter)
    {
        iEntityFactory::getInstance().destroyEntity((*boxIter));
        ++boxIter;
    }
    _boxes.clear();

    iSceneFactory::getInstance().destroyScene(_scene);

	_window.close();
	_window.removeView(&_view);
    _window.removeView(&_viewOrtho);

    if (_font)
    {
        delete _font;
    }

    iMouse::getInstance().unregisterMouseWheelDelegate(MouseWheelDelegate(this, &PhysicsExample::mouseWheel));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(MouseMoveFullDelegate(this, &PhysicsExample::mouseMoved));
    iKeyboard::getInstance().unregisterKeyDownDelegate(KeyDownDelegateExt(this, &PhysicsExample::keyPressed));
}

void PhysicsExample::init()
{
	con(" -- OpenGL 3D Test --" << endl);    

	iKeyboard::getInstance().registerKeyDownDelegate(KeyDownDelegateExt(this, &PhysicsExample::keyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(MouseMoveFullDelegate(this, &PhysicsExample::mouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(MouseWheelDelegate(this, &PhysicsExample::mouseWheel));

	iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(this, &PhysicsExample::handle));

	_view.setClearColor(iaColor4f(0.5f, 0, 0.5f, 1));
	_view.setPerspective(45);
	_view.setClipPlanes(0.1f, 10000.f);

    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0, 1024, 768, 0);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &PhysicsExample::renderOrtho));

	_window.addView(&_view);
    _window.addView(&_viewOrtho);
    //_window.setFullscreen(true);
    _window.setSize(1024, 768);
	_window.open();
	_window.registerWindowCloseDelegate(WindowCloseDelegate(this, &PhysicsExample::windowClosed));

    _scene = iSceneFactory::getInstance().createScene();
    _view.setScene(_scene);

    srand(1337);

    int range = 10;
    iaMatrixf offset;

    iPhysics::getInstance().setSimulationRate(60);

    _floor = iEntityFactory::getInstance().createEntity();
    _floor->setBody(iPhysics::getInstance().createBody(iPhysics::getInstance().createBox(50, 1, 50, offset.getData()), PhysicsBodyType::Generic));
    _floor->getBody()->setMass(0);

    iPhysicsCollision* collisionBox = iPhysics::getInstance().createBox(1, 1, 1, offset.getData());

    for (int i = 0; i < 500; ++i)
    {
        iEntity* box = iEntityFactory::getInstance().createEntity();
        _boxes.push_back(box);

        box->setBody(iPhysics::getInstance().createBody(collisionBox, PhysicsBodyType::Generic));
        box->getBody()->setMass(100);

        iaMatrixf transformation;
        transformation.translate(rand() % range - (range*0.5f), rand() % range + 20, rand() % range - (range*0.5f));
        transformation.rotate(rand(), iaAxis::X);
        transformation.rotate(rand(), iaAxis::Y);
        transformation.rotate(rand(), iaAxis::Z);
        box->setMatrix(transformation);

        iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
        iNodeModel* crate = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        crate->setModel("crate.ompf");
        transformNode->insertNode(crate);

        box->setTransformNode(transformNode);

        _scene->getRoot()->insertNode(transformNode);
    }

    for (int z = -12; z < 24; z += 4)
    {
        for (int x = -12; x < 24; x += 4)
        {
            for (int i = 0; i < 10; ++i)
            {
                iEntity* box = iEntityFactory::getInstance().createEntity();
                _boxes.push_back(box);

                box->setBody(iPhysics::getInstance().createBody(collisionBox, PhysicsBodyType::Generic));
                box->getBody()->setMass(100);

                iaMatrixf transformation;
                transformation.translate(x, i + 1, z);
                box->setMatrix(transformation);

                iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
                iNodeModel* cube = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
                cube->setModel("crate.ompf");
                transformNode->insertNode(cube);

                box->setTransformNode(transformNode);

                _scene->getRoot()->insertNode(transformNode);
            }
        }
    }
    
	// cam
    _cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraHeading->setName("_cameraHeading");
    _cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraPitch->setName("_cameraPitch");
    _cameraTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraTranslation->setName("_cameraTranslation");
    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    camera->setName("camera");

    _scene->getRoot()->insertNode(_cameraHeading);
    _cameraHeading->insertNode(_cameraPitch);
    _cameraPitch->insertNode(_cameraTranslation);
    _cameraTranslation->insertNode(camera);
	camera->makeCurrent();

    _cameraTranslation->translate(0, 0, 80);
    updateCameraPosition();

    // default sky box
	_materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setName("SkyBox");
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setOrder(10);
	
	iNodeSkyBox* skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
	skyBoxNode->setTextures(
		"skybox_default/front.png",
		"skybox_default/back.png",
		"skybox_default/left.png",
		"skybox_default/right.png",
		"skybox_default/top.png",
		"skybox_default/bottom.png");
	skyBoxNode->setTextureScale(10);
    skyBoxNode->setMaterial(_materialSkyBox);
	_scene->getRoot()->insertNode(skyBoxNode);

    _font = new iTextureFont("StandardFont.png");
    
	// light
    _directionalLightRotate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _directionalLightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _directionalLightTranslate->translate(100, 100, 100);
    _directionalLightRotate->insertNode(_directionalLightTranslate);
    _lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    _lightNode->setAmbient(iaColor4f(0.3f, 0.3f, 0.3f, 1.0f));
    _lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    _lightNode->setSpecular(iaColor4f(1.0f, 01.0f, 1.0f, 1.0f));
    
    _directionalLightTranslate->insertNode(_lightNode);
    _scene->getRoot()->insertNode(_directionalLightRotate);

    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _modelViewOrtho.translate(iaVector3f(0, 0, -30));

    _taskFlushModels = new iTaskFlushModels(&_window);
    iTaskManager::getInstance().addTask(_taskFlushModels);

    _taskFlushTextures = new iTaskFlushTextures(&_window);
    iTaskManager::getInstance().addTask(_taskFlushTextures);

    iStatistics::getInstance().setDrawMode(iRenderStatisticsVerbosity::FPSAndMetrics);
}

void PhysicsExample::mouseWheel(int32 d)
{
    if (d < 0)
    {
        _cameraTranslation->translate(0, 0, 10);
    }
    else
    {
        _cameraTranslation->translate(0, 0, -10);
    }
}

void PhysicsExample::mouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window)
{ 
    if (iMouse::getInstance().getLeftButton())
    {
        _camHeading += static_cast<float32>(x1 - x2) * 0.001f;
        _camPitch += static_cast<float32>(y1 - y2) * 0.001f;

        updateCameraPosition();

        iMouse::getInstance().setCenter(true);
    }
}

void PhysicsExample::updateCameraPosition()
{
    iaMatrixf pitch;
    pitch.rotate(_camPitch, iaAxis::X);
    iaMatrixf head;
    head.rotate(_camHeading, iaAxis::Y);

    _cameraPitch->setMatrix(pitch);
    _cameraHeading->setMatrix(head);
}

void PhysicsExample::windowClosed()
{
	iApplication::getInstance().stop();
}

void PhysicsExample::keyPressed(iKeyCode key)
{
	if (key == iKeyCode::ESC)
	{
		iApplication::getInstance().stop();
	}

    if (key == iKeyCode::F1)
    {
        iNodeVisitorPrintTree printTree;
        printTree.printToConsole(_scene->getRoot());
    }
}

void PhysicsExample::handle()
{
	_scene->handle();
}

void PhysicsExample::renderOrtho()
{
    iStatistics::getInstance().drawStatistics(&_window, _font, iaColor4f(1, 1, 1, 1));
}

void PhysicsExample::run()
{  
	iApplication::getInstance().run();
}