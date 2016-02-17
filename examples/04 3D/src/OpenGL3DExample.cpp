#include "OpenGL3DExample.h"

#include <iaConsole.h>
#include <iaString.h>
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
#include <iTaskFlushModels.h>
#include <iTaskFlushTextures.h>
#include <iOctree.h>
#include <iaString.h>
#include <iMaterialResourceFactory.h>
#include <iStatistics.h>
#include <iNodeSwitch.h>
#include <iNodeLODSwitch.h>
#include <iNodeLODTrigger.h>
using namespace Igor;

OpenGL3DExample::OpenGL3DExample()
{
	init();
}

OpenGL3DExample::~OpenGL3DExample()
{
    deinit();
}

void OpenGL3DExample::init()
{
	con(" -- OpenGL 3D Test --" << endl);    
    
    _window.setTitle("Igor - OpenGL 3D Test");
    _window.setSize(1280, 758);
    _window.setCentered();
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &OpenGL3DExample::windowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &OpenGL3DExample::windowResized));

    _view.setClearColor(iaColor4f(0.5f, 0, 0.5f, 1));
    _view.setPerspective(45);
    _view.setClipPlanes(0.1f, 10000.f);
    _view.registerRenderDelegate(RenderDelegate(this, &OpenGL3DExample::renderOctree));

    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &OpenGL3DExample::renderInfo));

    _window.addView(&_view);
    _window.addView(&_viewOrtho);
    _window.open();

    // init scene
    _scene = iSceneFactory::getInstance().createScene();
    _view.setScene(_scene);

    // create cam
    _cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraHeading->setName("_cameraHeading");
    _cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraPitch->setName("_cameraPitch");
    _cameraTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraTranslation->setName("_cameraTranslation");
    _cameraTranslation->translate(0, 0, 10);
    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    camera->setName("camera");

    _allObjectsHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _allObjectsHeading->setName("allObjectsHeading");
    _allObjectsPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _allObjectsPitch->setName("allObjectsPitch");

    iNodeTransform* catTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    catTransform->setName("catTransform");
    catTransform->translate(1, 0, 0);
    _catModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    _catModel->setModel("cat.ompf");

    iNodeTransform* teapotTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    teapotTransform->setName("teapotTransform");
    teapotTransform->translate(1, 0, 0);
    teapotTransform->scale(0.33, 0.33, 0.33);
    _teapotModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    _teapotModel->setModel("teapot.ompf");
    
    iNodeTransform* crateTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    crateTransform->setName("crateTransform");
    crateTransform->translate(1, 0.5, 0);
    _crateModel = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    _crateModel->setModel("crate.ompf");

    iNodeTransform* lod0Transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    lod0Transform->setName("lod0Transform");
    lod0Transform->translate(-1, 0, 0);
    iNodeModel* lod0Model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    lod0Model->setModel("cat.ompf");

    iNodeTransform* lod1Transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    lod1Transform->setName("lod1Transform");
    lod1Transform->translate(-1, 0, 0);
    lod1Transform->scale(0.33, 0.33, 0.33);
    iNodeModel* lod1Model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    lod1Model->setModel("teapot.ompf");

    iNodeTransform* lod2Transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    lod2Transform->setName("lod0Transform");
    lod2Transform->translate(-1, 0.5, 0);
    iNodeModel* lod2Model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    lod2Model->setModel("crate.ompf");

    iNodeLODTrigger* lodtrigger = static_cast<iNodeLODTrigger*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLODTrigger));
    iNodeLODSwitch* lodswitch = static_cast<iNodeLODSwitch*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLODSwitch));

    _switchNode = static_cast<iNodeSwitch*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSwitch));

    _scene->getRoot()->insertNode(_allObjectsHeading);
    _allObjectsHeading->insertNode(_allObjectsPitch);

    catTransform->insertNode(_catModel);
    teapotTransform->insertNode(_teapotModel);
    crateTransform->insertNode(_crateModel);
    
    _switchNode->insertNode(crateTransform);
    _switchNode->insertNode(teapotTransform);
    _switchNode->insertNode(catTransform);
    _switchNode->setActiveChild("crateTransform");

    lod0Transform->insertNode(lod0Model);
    lod1Transform->insertNode(lod1Model);
    lod2Transform->insertNode(lod2Model);

    lodswitch->insertNode(lod0Transform);
    lodswitch->insertNode(lod1Transform);
    lodswitch->insertNode(lod2Transform);

    lodswitch->setThresholds(lod0Transform, 0.0f, 5.0f);
    lodswitch->setThresholds(lod1Transform, 5.0f, 10.01f);
    lodswitch->setThresholds(lod2Transform, 10.01f, 20.0f);
    lodswitch->addTrigger(lodtrigger);

    _allObjectsPitch->insertNode(lodswitch);
    _allObjectsPitch->insertNode(_switchNode);
    
    // add cam
    _scene->getRoot()->insertNode(_cameraHeading);
    _cameraHeading->insertNode(_cameraPitch);
    _cameraPitch->insertNode(_cameraTranslation);
    _cameraTranslation->insertNode(camera);
    camera->insertNode(lodtrigger);
	camera->makeCurrent();

    // init sky box
	_materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->setOrder(10);
    iMaterialResourceFactory::getInstance().getMaterialGroup(_materialSkyBox)->getMaterial()->setName("SkyBox");
	
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
    
	// light
    _directionalLightRotate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _directionalLightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _directionalLightTranslate->translate(100, 100, 100);
    _directionalLightRotate->insertNode(_directionalLightTranslate);
    _lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    _lightNode->setAmbient(iaColor4f(0.3f, 0.3f, 0.3f, 1.0f));
    _lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    _lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    
    _directionalLightTranslate->insertNode(_lightNode);
    _scene->getRoot()->insertNode(_directionalLightRotate);

    _octreeMaterial = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->setName("Octree");
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->getRenderStateSet().setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->getRenderStateSet().setRenderState(iRenderState::Wireframe, iRenderStateValue::On);
     
    // init render statistics
    _font = new iTextureFont("StandardFont.png");
    iStatistics::getInstance().setVerbosity(iRenderStatisticsVerbosity::FPSAndMetrics);

    // animation
    _animationTimingHandle = new iTimerHandle();
    _animationTimingHandle->setIntervall(10);
    _animationTimingHandle->registerTimerDelegate(TimerDelegate(this, &OpenGL3DExample::handleAnimation));

    // start model and texture loading tasks
    _taskFlushModels = new iTaskFlushModels(&_window);
    iTaskManager::getInstance().addTask(_taskFlushModels);

    _taskFlushTextures = new iTaskFlushTextures(&_window);
    iTaskManager::getInstance().addTask(_taskFlushTextures);

    // register some callbacks
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &OpenGL3DExample::keyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &OpenGL3DExample::mouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &OpenGL3DExample::mouseWheel));
    iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(_scene, &iScene::handle));
}

void OpenGL3DExample::deinit()
{
    // unregister some callbacks
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &OpenGL3DExample::keyPressed));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &OpenGL3DExample::mouseMoved));
    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &OpenGL3DExample::mouseWheel));
    iApplication::getInstance().unregisterApplicationHandleDelegate(ApplicationHandleDelegate(_scene, &iScene::handle));

    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &OpenGL3DExample::windowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &OpenGL3DExample::windowResized));

    _view.unregisterRenderDelegate(RenderDelegate(this, &OpenGL3DExample::renderOctree));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &OpenGL3DExample::renderInfo));

    // deinit statistics
    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }
    
    // stop light animation
    if (_animationTimingHandle)
    {
        _animationTimingHandle->unregisterTimerDelegate(TimerDelegate(this, &OpenGL3DExample::handleAnimation));
        delete _animationTimingHandle;
        _animationTimingHandle = nullptr;
    }

    // stop resource managers
    if (_taskFlushTextures != nullptr)
    {
        _taskFlushTextures->abort();
        _taskFlushTextures = nullptr; // no need to delete. task manager did that already
    }

    if (_taskFlushModels != nullptr)
    {
        _taskFlushModels->abort();
        _taskFlushModels = nullptr; // no need to delete. task manager did that already
    }

    iSceneFactory::getInstance().destroyScene(_scene);
    _scene = nullptr;

    iMaterialResourceFactory::getInstance().destroyMaterial(_materialSkyBox);
    iMaterialResourceFactory::getInstance().destroyMaterial(_octreeMaterial);

    if (_window.isOpen())
    {
        _window.close();
        _window.removeView(&_view);
        _window.removeView(&_viewOrtho);
    }

	if (_font)
	{
		delete _font;
        _font = nullptr;
	}

}

void OpenGL3DExample::renderOctree()
{
    if (_renderOctree)
    {
        iaMatrixf model;
        iRenderer::getInstance().setModelMatrix(model);

        iMaterialResourceFactory::getInstance().setMaterial(_octreeMaterial);
        iRenderer::getInstance().setColor(0, 1.0f, 0, 0.5f);
        _scene->getOctree()->draw();
    }
}

void OpenGL3DExample::mouseWheel(int32 d)
{
    if (d < 0)
    {
        _cameraTranslation->translate(0, 0, 1);
    }
    else
    {
        _cameraTranslation->translate(0, 0, -1);
    }
}

void OpenGL3DExample::mouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
{ 
    if (iMouse::getInstance().getRightButton())
    {
        _allObjectsPitch->rotate((y2 - y1) * 0.005f, iaAxis::X);
        _allObjectsHeading->rotate((x2 - x1) * 0.005f, iaAxis::Y);

        iMouse::getInstance().setCenter(true);
    }
    else if (iMouse::getInstance().getLeftButton())
    {
        _cameraPitch->rotate((y2 - y1) * 0.005f, iaAxis::X);
        _cameraHeading->rotate((x1 - x2) * 0.005f, iaAxis::Y);

        iMouse::getInstance().setCenter(true);
    }
}

void OpenGL3DExample::windowClosed()
{
    iApplication::getInstance().stop();
}

void OpenGL3DExample::windowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0, clientWidth, clientHeight, 0);
}

void OpenGL3DExample::keyPressed(iKeyCode key)
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

    if (key == iKeyCode::F2)
    {
        _renderOctree = !_renderOctree;
    }

    if (key == iKeyCode::Space)
    {
        _activeNode++;
        if (_activeNode > 2)
        {
            _activeNode = 0;
        }

        switch (_activeNode)
        {
        case 0:
            _switchNode->setActiveChild("crateTransform");
            break;
        case 1:
            _switchNode->setActiveChild("catTransform");
            break;
        case 2:
            _switchNode->setActiveChild("teapotTransform");
            break;
        }
    }
}

void OpenGL3DExample::handleAnimation()
{  
    _directionalLightRotate->rotate(0.005f, iaAxis::Y);
}

void OpenGL3DExample::renderInfo()
{
    if (_font != nullptr)
    {
        iStatistics::getInstance().drawStatistics(&_window, _font, iaColor4f(1, 1, 1, 1));
    }
}

void OpenGL3DExample::run()
{  
	iApplication::getInstance().run();
}