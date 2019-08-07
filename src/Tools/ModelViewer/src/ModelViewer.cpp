// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "ModelViewer.h"
#include "Widget3DLocator.h"
#include "Widget3DEmitter.h"

#include <iaConsole.h>
#include <iaDirectory.h>
using namespace IgorAux;

#include <iMaterial.h>
#include <iNodeVisitorPrintTree.h>
#include <iTaskManager.h>
#include <iNodeSkyBox.h>
#include <iNodeCamera.h>
#include <iNodeModel.h> 
#include <iNodeSwitch.h>
#include <iNodeEmitter.h>
#include <iNodeParticleSystem.h>
#include <iNodeTransform.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeFactory.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iNodeLight.h>
#include <iModelResourceFactory.h>
#include <iNodeVisitorBoundings.h>
#include <iMaterialResourceFactory.h>
#include <iWidgetDefaultTheme.h>
#include <iDialog.h>
#include <iWidgetGrid.h>
#include <iWidgetScroll.h>
#include <iDialogMessageBox.h>
#include <iProfiler.h>
#include <iMaterialResourceFactory.h>
#include <iTaskFlushTextures.h>
#include <iNodeMesh.h>
#include <iMesh.h>
#include <iRenderEngine.h>
using namespace Igor;

#include "UserControlMesh.h"
#include "UserControlModel.h"
#include "UserControlNode.h"
#include "UserControlParticleSystem.h"
#include "UserControlTransformation.h"
#include "UserControlLight.h"
#include "UserControlEmitter.h"
#include "UserControlMaterial.h"
#include "UserControlProperties.h"
#include "UserControlMaterialView.h"
#include "UserControlGraphView.h"

static const wchar_t* WINDOW_TITLE_PREFIX = L"Igor::Model Viewer";

static const wchar_t* DEFAULT_LOAD_SAVE_DIR = L"..\\data\\models";

ModelViewer::ModelViewer()
{
	registerWidgetTypes();
}

ModelViewer::~ModelViewer()
{
    deinit();
}

void ModelViewer::registerWidgetTypes()
{
}

iModelDataInputParameter* ModelViewer::createDataInputParameter()
{
	iModelDataInputParameter* parameter = new iModelDataInputParameter();
	parameter->_identifier = "";
	parameter->_modelSourceType = iModelSourceType::File;
	parameter->_needsRenderContext = true;
	parameter->_loadPriority = iTask::DEFAULT_PRIORITY;
	parameter->_joinVertexes = false;
	parameter->_keepMesh = true;

	return parameter;
}

void ModelViewer::init(iaString fileName)
{
    con(" -- Model Viewer --" << endl);

	iWidgetManager::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &ModelViewer::onKeyDown));
    iWidgetManager::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ModelViewer::onMouseMoved));
    iWidgetManager::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &ModelViewer::onMouseWheel));
    iWidgetManager::getInstance().registerMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &ModelViewer::onMouseKeyDown));
    iWidgetManager::getInstance().registerMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &ModelViewer::onMouseKeyUp));

    iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &ModelViewer::handle));

    _window.setSize(1280, 800);
	_window.setCentered();
    _window.setTitle(WINDOW_TITLE_PREFIX);
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &ModelViewer::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &ModelViewer::onWindowResize));

    _view.setName("MainSceneView");
    _view.setClearColor(iaColor4f(0.25f, 0.25f, 0.25f, 1.0f));
    _view.setPerspective(45.0f);
    _view.setClipPlanes(1.0f, 100000.f);
    _view.registerRenderDelegate(RenderDelegate(this, &ModelViewer::render));
    _window.addView(&_view);

    _viewOrtho.setName("GUIView");
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0f);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &ModelViewer::renderOrtho));
    _window.addView(&_viewOrtho, 10);

    _window.setDoubleClick(true);
    _window.open(); // open after adding views to prevent warning message

    _scene = iSceneFactory::getInstance().createScene();
    _scene->setName("Model Scene");
    _view.setScene(_scene);

    _groupNode = static_cast<iNodePtr>(iNodeFactory::getInstance().createNode(iNodeType::iNode));
    _groupNode->setName("ModelViewerRoot");
	_scene->getRoot()->insertNode(_groupNode);

    // init 3D user controls
	_viewWidget3D.setName("UIView");
	_viewWidget3D.setClearColor(false);
	_viewWidget3D.setClearDepth(true);
	_viewWidget3D.setPerspective(45.0f);
	_viewWidget3D.setClipPlanes(1.0f, 100000.f);
	
	_window.addView(&_viewWidget3D, 5);

	_sceneWidget3D = iSceneFactory::getInstance().createScene();
	_sceneWidget3D->setName("Modifier Scene");
	_viewWidget3D.setScene(_sceneWidget3D);

    _manipulator = new Manipulator(&_window, &_viewWidget3D, _sceneWidget3D);
    
    // cam
    _cameraCOI = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraCOI->setName("camera COI");
    _cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraHeading->setName("camera heading");
    _cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraPitch->setName("camera pitch");
    _cameraTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraTranslation->setName("camera translation");
    _camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    _camera->setName("camera");

    _scene->getRoot()->insertNode(_cameraCOI);
    _cameraCOI->insertNode(_cameraHeading);
    _cameraHeading->insertNode(_cameraPitch);
    _cameraPitch->insertNode(_cameraTranslation);
    _cameraTranslation->insertNode(_camera);
    _view.setCurrentCamera(_camera->getID());

    _cameraTranslation->translate(0, 0, 80);

    // default sky box
    _materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setOrder(iMaterial::RENDER_ORDER_MIN);
    iMaterialResourceFactory::getInstance().getMaterial(_materialSkyBox)->setName("SkyBox");

    _skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
    _skyBoxNode->setTextures(
        iTextureResourceFactory::getInstance().requestFile("skybox_default/front.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/back.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/left.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/right.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/top.png"),
        iTextureResourceFactory::getInstance().requestFile("skybox_default/bottom.png"));
    _skyBoxNode->setMaterial(_materialSkyBox);
    _skyBoxNode->setVisible(false);
    _scene->getRoot()->insertNode(_skyBoxNode);

    _font = new iTextureFont("StandardFont.png");

    // create materials
    _materialBoundingBox = iMaterialResourceFactory::getInstance().createMaterial();

    _materialCelShading = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->addShaderSource("ModelViewer/yellow.frag", iShaderObjectType::Fragment);
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->compileShader();
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->getRenderStateSet().setRenderState(iRenderState::Wireframe, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->getRenderStateSet().setRenderState(iRenderState::CullFace, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->getRenderStateSet().setRenderState(iRenderState::CullFaceFunc, iRenderStateValue::Front);

    _materialOrientationPlane = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialOrientationPlane)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialOrientationPlane)->getRenderStateSet().setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_materialOrientationPlane)->setOrder(iMaterial::RENDER_ORDER_MAX);
    iMaterialResourceFactory::getInstance().getMaterial(_materialOrientationPlane)->setName("OrientationPlane");

    // light
    _directionalLightRotate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _directionalLightRotate->setName("directional light rotate");

    _directionalLightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _directionalLightTranslate->setName("directional light translate");
    _directionalLightTranslate->translate(10000, 10000, 0);

    _lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    _lightNode->setName("directional light");
    _lightNode->setAmbient(iaColor4f(0.5f, 0.5f, 0.5f, 1.0f));
    _lightNode->setDiffuse(iaColor4f(0.9f, 0.9f, 0.9f, 1.0f));
    _lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));

    _scene->getRoot()->insertNode(_directionalLightRotate);
    _directionalLightRotate->insertNode(_directionalLightTranslate);
    _directionalLightTranslate->insertNode(_lightNode);

    _modelViewOrtho.translate(0, 0, -30);

    if (!fileName.isEmpty())
    {
        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        iModelDataInputParameter* parameter = createDataInputParameter();

        model->setModel(fileName, iResourceCacheMode::Free, parameter);
        _groupNode->insertNode(model);
    }

    initGUI();

    _outliner->setRootNode(_groupNode);

    if (fileName.isEmpty())
    {
        _fileDialog->load(iDialogFileSelectCloseDelegate(this, &ModelViewer::onFileLoadDialogClosed), DEFAULT_LOAD_SAVE_DIR);
    }
    else
    {
        _outliner->setActive();
        _outliner->setVisible();

        _propertiesDialog->setActive();
        _propertiesDialog->setVisible();
    }

    _outliner->refreshView();
    resetManipulatorMode();

    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));
}

void ModelViewer::resetManipulatorMode()
{
    setManipulatorMode(ManipulatorMode::None);
}

void ModelViewer::deinit()
{
    deinitGUI();

    iSceneFactory::getInstance().destroyScene(_scene);
	iSceneFactory::getInstance().destroyScene(_sceneWidget3D);
	
	// abort flush task
	iTaskManager::getInstance().abortTask(_taskFlushTextures);

	// flush model resources
    iModelResourceFactory::getInstance().flush(&_window);

    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &ModelViewer::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &ModelViewer::onWindowResize));

    _window.close();
    _window.removeView(&_view);
    _window.removeView(&_viewOrtho);
	_window.removeView(&_viewWidget3D);

    _view.unregisterRenderDelegate(RenderDelegate(this, &ModelViewer::render));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &ModelViewer::renderOrtho));

    if (_font)
    {
        delete _font;
    }

    iWidgetManager::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &ModelViewer::onMouseWheel));
    iWidgetManager::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &ModelViewer::onMouseMoved));
    iWidgetManager::getInstance().unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &ModelViewer::onMouseKeyDown));
    iWidgetManager::getInstance().unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &ModelViewer::onMouseKeyUp));
	iWidgetManager::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &ModelViewer::onKeyDown));
    iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &ModelViewer::handle));    
}

void ModelViewer::onAddTransformation(uint64 atNodeID)
{
    iNodePtr destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNodeTransform* transform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transform->setName("Transformation");
    destination->insertNode(transform);
    _outliner->refreshView();
    _outliner->setSelectedNode(transform);
}

void ModelViewer::onAddGroup(uint64 atNodeID)
{
    iNodePtr destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNodePtr group = static_cast<iNodePtr>(iNodeFactory::getInstance().createNode(iNodeType::iNode));
    group->setName("Group");
    destination->insertNode(group);
    _outliner->refreshView();
    _outliner->setSelectedNode(group);
}

void ModelViewer::onAddEmitter(uint64 atNodeID)
{
    iNodePtr destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
    emitter->setName("Emitter");
    destination->insertNode(emitter);
    _outliner->refreshView();
    _outliner->setSelectedNode(emitter);
}

void ModelViewer::onAddParticleSystem(uint64 atNodeID)
{
    iNodePtr destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
    particleSystem->setName("ParticleSystem");
    destination->insertNode(particleSystem);
    _outliner->refreshView();
    _outliner->setSelectedNode(particleSystem);
}

void ModelViewer::onAddMaterial()
{
    iMaterialResourceFactory::getInstance().createMaterial("new Material");
    _outliner->refreshView();
}

void ModelViewer::onAddSwitch(uint64 atNodeID)
{
    iNodePtr destination = iNodeFactory::getInstance().getNode(atNodeID);

    if (destination == nullptr)
    {
        destination = _groupNode;
    }

    iNodeSwitch* switchNode = static_cast<iNodeSwitch*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSwitch));
    switchNode->setName("Switch");
    destination->insertNode(switchNode);
    _outliner->refreshView();
    _outliner->setSelectedNode(switchNode);
}

void ModelViewer::forceLoadingNow(iNodeModel* modelNode)
{
    if (modelNode != nullptr)
    {
        iScene* tempScene = iSceneFactory::getInstance().createScene();
        tempScene->getRoot()->insertNode(modelNode);

        // want everything to be loaded now!
        con_endl("loading data synchronously ... ");

        while (!modelNode->isLoaded())
        {
            tempScene->handle();
            // iTextureResourceFactory::getInstance().flush();
            iModelResourceFactory::getInstance().flush(&_window);
        }

        tempScene->getRoot()->removeNode(modelNode);
        iSceneFactory::getInstance().destroyScene(tempScene);
    }
}

void ModelViewer::centerCamOnNode(iNodePtr node)
{
    if (node != nullptr)
    {
        iNodeVisitorBoundings visitorBoundings;
        visitorBoundings.traverseTree(node);
        iSphered sphere;
        visitorBoundings.getSphere(sphere);

        iaMatrixd coiMatrix;
        coiMatrix._pos = sphere._center;
        _cameraCOI->setMatrix(coiMatrix);
        _manipulator->setCamCOI(coiMatrix);

        if (sphere._radius > 0.0f)
        {
            _camDistance = sphere._radius * 4.0f;
        }
        else
        {
            _camDistance = 1.0f;
        }
    }
}

void ModelViewer::onImportFile(uint64 nodeID)
{
    _fileDialog->load(iDialogFileSelectCloseDelegate(this, &ModelViewer::onImportFileDialogClosed), DEFAULT_LOAD_SAVE_DIR);
}

void ModelViewer::onImportFileReference(uint64 nodeID)
{
    _fileDialog->load(iDialogFileSelectCloseDelegate(this, &ModelViewer::onImportFileReferenceDialogClosed), DEFAULT_LOAD_SAVE_DIR);
}

void ModelViewer::onLoadFile()
{
    _fileDialog->load(iDialogFileSelectCloseDelegate(this, &ModelViewer::onFileLoadDialogClosed), DEFAULT_LOAD_SAVE_DIR);
}

void ModelViewer::onSaveFile()
{
    _fileDialog->save(iDialogFileSelectCloseDelegate(this, &ModelViewer::onFileSaveDialogClosed), DEFAULT_LOAD_SAVE_DIR);
}

void ModelViewer::onFileSaveDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    if (fileDialogReturnValue == iFileDialogReturnValue::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        std::vector<iNodePtr> children = _groupNode->getChildren();
        children.insert(children.end(), _groupNode->getInactiveChildren().begin(), _groupNode->getInactiveChildren().end());

        if (children.empty())
        {
            con_warn("nothing to save");
        }
        else if (children.size() == 1)
        {
            iModelResourceFactory::getInstance().exportModelData(filename, children[0]);
        }
        else
        {
            iModelResourceFactory::getInstance().exportModelData(filename, _groupNode);
        }
    }
}

void ModelViewer::onImportFileDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    iNodePtr selectNode = nullptr;

    if (fileDialogReturnValue == iFileDialogReturnValue::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        iModelDataInputParameter* parameter = createDataInputParameter();
        model->setModel(filename, iResourceCacheMode::Free, parameter);

        forceLoadingNow(model);

		if (model->isValid())
		{
			iNodePtr groupNode = nullptr;

			auto children = model->getChildren();
			if (children.size() > 1)
			{
				groupNode = static_cast<iNodePtr>(iNodeFactory::getInstance().createNode(iNodeType::iNode));
				iaString groupName = "group:";
				groupName += filename;
				groupNode->setName(groupName);

				iNodePtr cursorNode = iNodeFactory::getInstance().getNode(_selectedNodeID);
				if (cursorNode != nullptr)
				{
					cursorNode->insertNode(groupNode);
				}
				else
				{
					_groupNode->insertNode(groupNode);
				}

				selectNode = groupNode;
			}
			else
			{
				iNodePtr cursorNode = iNodeFactory::getInstance().getNode(_selectedNodeID);
				if (cursorNode != nullptr)
				{
					groupNode = cursorNode;
				}
				else
				{
					groupNode = _groupNode;
				}

				if (!children.empty())
				{
					selectNode = children.front();
				}
			}

			auto child = children.begin();
			while (child != children.end())
			{
				model->removeNode((*child));
				groupNode->insertNode((*child));
				child++;
			}
		}

        iNodeFactory::getInstance().destroyNodeAsync(model);
    }

    _outliner->setActive();
    _outliner->setVisible();
    _outliner->refreshView();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();

    _outliner->setSelectedNode(selectNode);
    centerCamOnSelectedNode();
}

void ModelViewer::onImportFileReferenceDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    iNodePtr selectNode = nullptr;

    if (fileDialogReturnValue == iFileDialogReturnValue::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        iModelDataInputParameter* parameter = createDataInputParameter();

        model->setModel(filename, iResourceCacheMode::Free, parameter);
        forceLoadingNow(model);

		if (model->isValid())
		{
			iNodePtr cursorNode = iNodeFactory::getInstance().getNode(_selectedNodeID);
			if (cursorNode != nullptr)
			{
				cursorNode->insertNode(model);
			}
			else
			{
				_groupNode->insertNode(model);
			}

			selectNode = model;
		}
    }

    _outliner->setActive();
    _outliner->setVisible();
    _outliner->refreshView();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();

    _outliner->setSelectedNode(selectNode);
    centerCamOnSelectedNode();
}

void ModelViewer::onFileLoadDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
{
    iNodePtr selectNode = nullptr;

    if (fileDialogReturnValue == iFileDialogReturnValue::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        if (_groupNode->getChildren().size() > 0)
        {
            auto children = _groupNode->getChildren();
            auto childIter = children.begin();
            while (childIter != children.end())
            {
                _groupNode->removeNode((*childIter));
                iNodeFactory::getInstance().destroyNodeAsync((*childIter));
				childIter++;
            }
        }

        iNodeModel* model = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
        iModelDataInputParameter* parameter = createDataInputParameter();

        model->setModel(filename, iResourceCacheMode::Free, parameter);
        forceLoadingNow(model);

		if (model->isValid())
		{
			iNodePtr insertAt = nullptr;

			auto children = model->getChildren();
			if (children.size() > 1)
			{
				insertAt = static_cast<iNodePtr>(iNodeFactory::getInstance().createNode(iNodeType::iNode));
				iaString groupName = "group:";
				groupName += filename;
				insertAt->setName(groupName);

				_groupNode->insertNode(insertAt);
				selectNode = insertAt;
			}
			else
			{
				insertAt = _groupNode;
				selectNode = children.front();
			}

			auto child = children.begin();
			while (child != children.end())
			{
				model->removeNode((*child));
				insertAt->insertNode((*child));
				child++;
			}
		}

        iNodeFactory::getInstance().destroyNodeAsync(model);
    }

    _outliner->setActive();
    _outliner->setVisible();
    _outliner->refreshView();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();

    _outliner->setSelectedNode(selectNode);
    centerCamOnSelectedNode();
}

void ModelViewer::initGUI()
{
	_propertiesDialog = new PropertiesDialog();
	_outliner = new Outliner();
	
    _widgetTheme = new iWidgetDefaultTheme("StandardFont.png", "WidgetThemePattern.png");
    iWidgetManager::getInstance().setTheme(_widgetTheme);
    iWidgetManager::getInstance().setDesktopDimensions(_window.getClientWidth(), _window.getClientHeight());

    _outliner->registerOnExitModelViewer(ExitModelViewerDelegate(this, &ModelViewer::onExitModelViewer));
    _outliner->registerOnLoadFile(LoadFileDelegate(this, &ModelViewer::onLoadFile));
    _outliner->registerOnImportFile(ImportFileDelegate(this, &ModelViewer::onImportFile));
    _outliner->registerOnImportFileReference(ImportFileReferenceDelegate(this, &ModelViewer::onImportFileReference));
    _outliner->registerOnSaveFile(SaveFileDelegate(this, &ModelViewer::onSaveFile));
    _outliner->registerOnAddTransformation(AddTransformationDelegate(this, &ModelViewer::onAddTransformation));
    _outliner->registerOnAddSwitch(AddSwitchDelegate(this, &ModelViewer::onAddSwitch));
    _outliner->registerOnAddGroup(AddGroupDelegate(this, &ModelViewer::onAddGroup));
    _outliner->registerOnAddEmitter(AddEmitterDelegate(this, &ModelViewer::onAddEmitter));
    _outliner->registerOnAddParticleSystem(AddParticleSystemDelegate(this, &ModelViewer::onAddParticleSystem));
    _outliner->registerOnAddMaterial(AddMaterialDelegate(this, &ModelViewer::onAddMaterial));

    _fileDialog = new iDialogFileSelect();
    _messageBox = new iDialogMessageBox();

    _propertiesDialog->registerStructureChangedDelegate(StructureChangedDelegate(_outliner, &Outliner::refreshView));

    _outliner->registerOnGraphSelectionChanged(GraphSelectionChangedDelegate(_propertiesDialog, &PropertiesDialog::onGraphViewSelectionChanged));
    _outliner->registerOnGraphSelectionChanged(GraphSelectionChangedDelegate(this, &ModelViewer::onGraphViewSelectionChanged));
    _outliner->registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate(_propertiesDialog, &PropertiesDialog::onMaterialSelectionChanged));
}

void ModelViewer::onGraphViewSelectionChanged(uint64 nodeID)
{
    _selectedNodeID = nodeID;
    _manipulator->setNodeID(_selectedNodeID);
	resetManipulatorMode();

	// todo caching?
	if (_widget3D != nullptr)
	{
		delete _widget3D;
		_widget3D = nullptr;
	}

	iNode* node = iNodeFactory::getInstance().getNode(_selectedNodeID);
	if (node)
	{
		switch (node->getType())
		{
		case iNodeType::iNodeTransform:
			_widget3D = new Widget3DLocator(&_window, &_viewWidget3D, _sceneWidget3D);			
			break;

		case iNodeType::iNodeEmitter:
			_widget3D = new Widget3DEmitter(&_window, &_viewWidget3D, _sceneWidget3D);
			break;
		}
	}

	if (_widget3D != nullptr)
	{
		_widget3D->setNodeID(_selectedNodeID);
	}
}

void ModelViewer::setManipulatorMode(ManipulatorMode manipulatorMode)
{
    iNodePtr node = iNodeFactory::getInstance().getNode(_selectedNodeID);

    if (node != nullptr &&
        node->getKind() == iNodeKind::Transformation)
    {
        _manipulator->setVisible(true);
        _manipulator->setManipulatorMode(manipulatorMode);
    }
    else
    {
        _manipulator->setVisible(false);
        _manipulator->setManipulatorMode(ManipulatorMode::None);
    }
}

void ModelViewer::deinitGUI()
{
    if (_propertiesDialog != nullptr)
    {
		delete _propertiesDialog;
		_propertiesDialog = nullptr;
    }

	if (_outliner != nullptr)
	{
		delete _outliner;
		_outliner = nullptr;
	}

	if (_fileDialog != nullptr)
	{
		iWidgetManager::getInstance().destroyDialog(_fileDialog);
		_fileDialog = nullptr;
	}

	if (_messageBox != nullptr)
    {
        iWidgetManager::getInstance().destroyDialog(_messageBox);
        _messageBox = nullptr;
    }

    iWidgetManager::getInstance().setTheme(nullptr);
    if (_widgetTheme != nullptr)
    {
        delete _widgetTheme;
        _widgetTheme = nullptr;
    }
}

void ModelViewer::onWindowResize(int32 clientWidth, int32 clientHeight)
{
    iWidgetManager::getInstance().setDesktopDimensions(_window.getClientWidth(), _window.getClientHeight());
    _viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
}

void ModelViewer::updateCamDistanceTransform()
{
    _cameraTranslation->identity();
    _cameraTranslation->translate(0, 0, _camDistance);

    iaMatrixd matrix;
    matrix.translate(0, 0, _camDistance);
    _manipulator->setCamTranslate(matrix);
}

void ModelViewer::onMouseKeyDown(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::MouseLeft:
        if (!iKeyboard::getInstance().getKey(iKeyCode::LAlt))
        {
            _manipulator->onMouseKeyDown(key);
        }
        break;
    }
}

void ModelViewer::pickcolorID()
{
    bool wasVisible = _skyBoxNode->isVisible();
    _skyBoxNode->setVisible(false);

    uint64 nodeID = _view.pickcolorID(iMouse::getInstance().getPos()._x, iMouse::getInstance().getPos()._y);
    iNodePtr node = iNodeFactory::getInstance().getNode(nodeID);
    _outliner->setSelectedNode(node);

    _skyBoxNode->setVisible(wasVisible);
}

void ModelViewer::onMouseKeyUp(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::MouseLeft:

        if (!iKeyboard::getInstance().getKey(iKeyCode::LAlt) &&
            !_manipulator->isSelected())
        {
            pickcolorID();
            resetManipulatorMode();
        }

        _manipulator->onMouseKeyUp(key);
        break;
    }
}

void ModelViewer::onMouseWheel(int32 d)
{
    if (d < 0)
    {
        _camDistance *= 2.0f;
    }
    else
    {
        _camDistance *= 0.5f;
    }
}

void ModelViewer::onMouseMoved(const iaVector2i& from, const iaVector2i& to, iWindow* window)
{
    const float64 rotateSensitivity = 0.0075;
	const float64 translateSensitivity = 1.0;

    if (iMouse::getInstance().getLeftButton())
    {
        if (iKeyboard::getInstance().getKey(iKeyCode::LAlt))
        {
            _cameraPitch->rotate((from._y - to._y) * rotateSensitivity, iaAxis::X);
            _cameraHeading->rotate((from._x - to._x) * rotateSensitivity, iaAxis::Y);

            iaMatrixd matrix;
            _cameraPitch->getMatrix(matrix);
            _manipulator->setCamPitch(matrix);

            _cameraHeading->getMatrix(matrix);
            _manipulator->setCamHeading(matrix);
        }
        else
        {
            _manipulator->onMouseMoved(from, to, window);
        }
    }

    if (iMouse::getInstance().getRightButton())
    {
        _directionalLightRotate->rotate((from._y - to._y) * rotateSensitivity, iaAxis::X);
        _directionalLightRotate->rotate((from._x - to._x) * rotateSensitivity, iaAxis::Y);
    }

	if (iMouse::getInstance().getMiddleButton())
	{
		if (iKeyboard::getInstance().getKey(iKeyCode::LAlt))
		{
			iaMatrixd camWorldMatrix;
			_camera->calcWorldTransformation(camWorldMatrix);
			iaVector3d fromWorld = camWorldMatrix * _view.unProject(iaVector3d(from._x, from._y, 0), camWorldMatrix);
			iaVector3d toWorld = camWorldMatrix * _view.unProject(iaVector3d(to._x, to._y, 0), camWorldMatrix);
			
			iaMatrixd camTranslateMatrix;
			_cameraTranslation->getMatrix(camTranslateMatrix);
			float64 translateFactor = camTranslateMatrix._pos.length() * translateSensitivity;

			_cameraCOI->translate((fromWorld - toWorld) * translateFactor);
			iaMatrixd coiMatrix;
			_cameraCOI->getMatrix(coiMatrix);
			_manipulator->setCamCOI(coiMatrix);
		}
	}
}

void ModelViewer::onExitModelViewer()
{
    iApplication::getInstance().stop();
}

void ModelViewer::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void ModelViewer::onKeyDown(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::F:
    {
        centerCamOnSelectedNode();
    }
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

    case iKeyCode::Q:
        setManipulatorMode(ManipulatorMode::None);
        break;

    case iKeyCode::W:
        setManipulatorMode(ManipulatorMode::Translate);
        break;

    case iKeyCode::E:
        setManipulatorMode(ManipulatorMode::Rotate);
        break;

    case iKeyCode::R:
        setManipulatorMode(ManipulatorMode::Scale);
        break;

	case iKeyCode::N:
		if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
		{
			clearScene();
		}
		break;

	case iKeyCode::D:
		if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
		{
			_outliner->duplicateSelected();
		}
		break;

	case iKeyCode::X:
		if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
		{
			_outliner->cutSelected();
		}
		break;

	case iKeyCode::C:
		if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
		{
			_outliner->copySelected();
		}
		break;

	case iKeyCode::V:
		if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
		{
			_outliner->pasteSelected();
		}
		break;

	case iKeyCode::O:
		if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
		{
			_outliner->fileOpen();
		}
		break;

	case iKeyCode::S:
		if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
		{
			_outliner->fileSave();
		}
		break;

	case iKeyCode::Delete:
		_outliner->deleteSelected();
		break;

    }
}

void ModelViewer::clearScene()
{
	std::vector<iNodePtr> copyChildren(_groupNode->getChildren());
	for (auto child : copyChildren)
	{
		_groupNode->removeNode(child);
		iNodeFactory::getInstance().destroyNodeAsync(child);
	}

	iModelResourceFactory::getInstance().flush(&_window);
	_outliner->refreshView();
}

void ModelViewer::centerCamOnSelectedNode()
{
    iNodePtr node = iNodeFactory::getInstance().getNode(_selectedNodeID);
    centerCamOnNode(node);
}

void ModelViewer::handle()
{
    _scene->handle();
}

void ModelViewer::renderNodeSelected(uint64 nodeID)
{
    if (nodeID != iNode::INVALID_NODE_ID)
    {
        iNodePtr node = iNodeFactory::getInstance().getNode(nodeID);

        if (node->getKind() == iNodeKind::Renderable ||
            node->getKind() == iNodeKind::Volume)
        {
            iNodeRender* renderNode = static_cast<iNodeRender*>(node);
            iaMatrixd matrix = renderNode->getWorldMatrix();
            iRenderer::getInstance().setModelMatrix(matrix);

            if (node->getType() == iNodeType::iNodeMesh)
            {
                iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading));

                iNodeMesh* meshNode = static_cast<iNodeMesh*>(node);
                std::shared_ptr<iMeshBuffers> buffers = meshNode->getMeshBuffers();
                iRenderer::getInstance().setLineWidth(4);
                iRenderer::getInstance().drawMesh(buffers);
            }
            else
            {
                if (node->getKind() == iNodeKind::Volume)
                {
                    iNodeVolume* renderVolume = static_cast<iNodeVolume*>(node);
                    iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_materialBoundingBox));

                    iAABoxd box = renderVolume->getBoundingBox();

                    iRenderer::getInstance().setColor(1, 1, 0, 1);
                    iRenderer::getInstance().drawBBox(box);
                }
            }
        }
    }
}

void ModelViewer::render()
{
    updateCamDistanceTransform();
    renderNodeSelected(_selectedNodeID);
    renderOrientationPlane();
}

void ModelViewer::renderOrientationPlane()
{
    iaMatrixd identity;
    iRenderer::getInstance().setModelMatrix(identity);

    iRenderer::getInstance().setMaterial(_materialOrientationPlane);
    iRenderer::getInstance().setLineWidth(1);
    
    for (int i = -20; i < 21; ++i)
    {
        if (i % 2 == 0)
        {
            iRenderer::getInstance().setColor(1.0f, 1.0f, 1.0f, 0.5f);
        }
        else
        {
            iRenderer::getInstance().setColor(1.0f, 1.0f, 1.0f, 0.25f);
        }

        iRenderer::getInstance().drawLine(iaVector3f(-20.0f, 0.0f, i), iaVector3f(20.0f, 0.0f, i));
        iRenderer::getInstance().drawLine(iaVector3f(i, 0.0f, 20.0f), iaVector3f(i, 0.0f, -20.0f));
    }

	iRenderer::getInstance().setColor(1.0f, 0.0f, 0.0f, 1.0f);
	iRenderer::getInstance().drawLine(iaVector3f(0.0f, 0.0f, 0.0f), iaVector3f(20.0f, 0.0f, 0.0f));
	iRenderer::getInstance().setColor(0.0f, 0.0f, 1.0f, 1.0f);
	iRenderer::getInstance().drawLine(iaVector3f(0.0f, 0.0f, 0.0f), iaVector3f(0.0f, 0.0f, 20.0f));
}

void ModelViewer::renderOrtho()
{
    iaMatrixd identity;
    iRenderer::getInstance().setViewMatrix(identity);
    iRenderer::getInstance().setModelMatrix(_modelViewOrtho);

    iWidgetManager::getInstance().draw();
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    _profilerVisualizer.draw(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void ModelViewer::run(iaString fileName)
{
    init(fileName);
    iApplication::getInstance().run();
}
