// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Mica.h"
#include "Widget3DLocator.h"
#include "Widget3DEmitter.h"

#include <iaux/system/iaConsole.h>
#include <iaux/system/iaDirectory.h>
using namespace iaux;

#include <igor/resources/material/iMaterial.h>
#include <igor/scene/traversal/iNodeVisitorPrintTree.h>
#include <igor/threading/iTaskManager.h>
#include <igor/scene/nodes/iNodeSkyBox.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeSwitch.h>
#include <igor/scene/nodes/iNodeEmitter.h>
#include <igor/scene/nodes/iNodeParticleSystem.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/graphics/iRenderer.h>
#include <igor/system/iApplication.h>
#include <igor/scene/iSceneFactory.h>
#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/system/iTimer.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/scene/traversal/iNodeVisitorBoundings.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/ui/theme/iWidgetDefaultTheme.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/widgets/iWidgetScroll.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/threading/tasks/iTaskFlushTextures.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/graphics/iRenderEngine.h>
using namespace igor;

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

/*! window title definition
*/
static const wchar_t *WINDOW_TITLE_PREFIX = L"Mica";

/*! default file open folder definition
*/
static const wchar_t *DEFAULT_LOAD_SAVE_DIR = L"..\\data\\models";

Mica::~Mica()
{
	deinit();
}

iModelDataInputParameter *Mica::createDataInputParameter()
{
	iModelDataInputParameter *parameter = new iModelDataInputParameter();
	parameter->_identifier = "";
	parameter->_modelSourceType = iModelSourceType::File;
	parameter->_needsRenderContext = true;
	parameter->_loadPriority = iTask::DEFAULT_PRIORITY;
	parameter->_joinVertexes = false;
	parameter->_keepMesh = true;

	return parameter;
}

void Mica::init(iaString fileName)
{
	con_endl(" -- Mica --");

	iWidgetManager::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &Mica::onKeyDown));
	iWidgetManager::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &Mica::onMouseMoved));
	iWidgetManager::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &Mica::onMouseWheel));
	iWidgetManager::getInstance().registerMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &Mica::onMouseKeyDown));
	iWidgetManager::getInstance().registerMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &Mica::onMouseKeyUp));

	iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iPreDrawDelegate(this, &Mica::handle));

	int32 width, height;
	_window.setSize(1280, 720);
	_window.setCentered();
	_window.setTitle(WINDOW_TITLE_PREFIX);
	_window.registerWindowCloseDelegate(WindowCloseDelegate(this, &Mica::onWindowClosed));
	_window.registerWindowResizeDelegate(WindowResizeDelegate(this, &Mica::onWindowResize));

	_view.setName("MainSceneView");
	_view.setClearColor(iaColor4f(0.25f, 0.25f, 0.25f, 1.0f));
	_view.setPerspective(45.0f);
	_view.setClipPlanes(1.0f, 100000.f);
	_view.registerRenderDelegate(iDrawDelegate(this, &Mica::render));
	_window.addView(&_view);

	_viewOrtho.setName("2D_UI_View");
	_viewOrtho.setClearColor(false);
	_viewOrtho.setClearDepth(false);
	_viewOrtho.setClipPlanes(-1.0f, 1.0f);
	_viewOrtho.setOrthogonal(0.0f, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0f);
	_viewOrtho.registerRenderDelegate(iDrawDelegate(this, &Mica::renderOrtho));
	_window.addView(&_viewOrtho, 10);

	_window.setDoubleClick(true);
	_window.open(); // open after adding views to prevent warning message

	_scene = iSceneFactory::getInstance().createScene();
	_scene->setName("Model Scene");
	_view.setScene(_scene);

	_workspace = iNodeManager::getInstance().createNode<iNode>();
	_workspace->setName("MicaRoot");
	_scene->getRoot()->insertNode(_workspace);

	// init 3D user controls
	_viewWidget3D.setName("3D_UI_View");
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
	_cameraCOI = iNodeManager::getInstance().createNode<iNodeTransform>();
	_cameraCOI->setName("camera COI");
	_cameraHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
	_cameraHeading->setName("camera heading");
	_cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
	_cameraPitch->setName("camera pitch");
	_cameraTranslation = iNodeManager::getInstance().createNode<iNodeTransform>();
	_cameraTranslation->setName("camera translation");
	_camera = iNodeManager::getInstance().createNode<iNodeCamera>();
	_camera->setName("camera");

	_scene->getRoot()->insertNode(_cameraCOI);
	_cameraCOI->insertNode(_cameraHeading);
	_cameraHeading->insertNode(_cameraPitch);
	_cameraPitch->insertNode(_cameraTranslation);
	_cameraTranslation->insertNode(_camera);
	_view.setCurrentCamera(_camera->getID());

	_cameraTranslation->translate(0, 0, 80);

	// create materials
	_materialBoundingBox = iMaterialResourceFactory::getInstance().createMaterial("materialBoundingBox");

	_materialCelShading = iMaterialResourceFactory::getInstance().createMaterial("celShadingMaterial");
	auto celShadingMaterial = iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading);
	celShadingMaterial->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
	celShadingMaterial->addShaderSource("Mica/yellow.frag", iShaderObjectType::Fragment);
	celShadingMaterial->compileShader();
	celShadingMaterial->setRenderState(iRenderState::Wireframe, iRenderStateValue::On);
	celShadingMaterial->setRenderState(iRenderState::CullFace, iRenderStateValue::On);
	celShadingMaterial->setRenderState(iRenderState::CullFaceFunc, iRenderStateValue::Front);

	_materialOrientationPlane = iMaterialResourceFactory::getInstance().createMaterial("OrientationPlane");
	auto oriPlaneMaterial = iMaterialResourceFactory::getInstance().getMaterial(_materialOrientationPlane);
	oriPlaneMaterial->setRenderState(iRenderState::Blend, iRenderStateValue::On);
	oriPlaneMaterial->setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
	oriPlaneMaterial->setOrder(iMaterial::RENDER_ORDER_MAX);

	uint64 materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial();
	auto skyBoxMaterial = iMaterialResourceFactory::getInstance().getMaterial(materialSkyBox);
	skyBoxMaterial->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
	skyBoxMaterial->setRenderState(iRenderState::Blend, iRenderStateValue::On);
	skyBoxMaterial->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
	skyBoxMaterial->setOrder(iMaterial::RENDER_ORDER_MIN);
	skyBoxMaterial->setName("SkyBox");

	// default sky box
	_defaultSkyBox = iNodeManager::getInstance().createNode<iNodeSkyBox>();
	_defaultSkyBox->setTextures(
		iTextureResourceFactory::getInstance().requestFile("skybox_default/front.png"),
		iTextureResourceFactory::getInstance().requestFile("skybox_default/back.png"),
		iTextureResourceFactory::getInstance().requestFile("skybox_default/left.png"),
		iTextureResourceFactory::getInstance().requestFile("skybox_default/right.png"),
		iTextureResourceFactory::getInstance().requestFile("skybox_default/top.png"),
		iTextureResourceFactory::getInstance().requestFile("skybox_default/bottom.png"));
	_defaultSkyBox->setMaterial(materialSkyBox);
	_defaultSkyBox->setVisible(false);
	_scene->getRoot()->insertNode(_defaultSkyBox);

	_font = new iTextureFont("StandardFont.png");

	// light
	_directionalLightRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
	_directionalLightRotate->setName("directional light rotate");

	_directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
	_directionalLightTranslate->setName("directional light translate");
	_directionalLightTranslate->translate(10000, 10000, 0);

	_lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
	_lightNode->setName("directional light");
	_lightNode->setAmbient(iaColor4f(0.5f, 0.5f, 0.5f, 1.0f));
	_lightNode->setDiffuse(iaColor4f(0.9f, 0.9f, 0.9f, 1.0f));
	_lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));

	_scene->getRoot()->insertNode(_directionalLightRotate);
	_directionalLightRotate->insertNode(_directionalLightTranslate);
	_directionalLightTranslate->insertNode(_lightNode);

	if (!fileName.isEmpty())
	{
		iNodeModel *model = iNodeManager::getInstance().createNode<iNodeModel>();
		iModelDataInputParameter *parameter = createDataInputParameter();

		model->setModel(fileName, iResourceCacheMode::Free, parameter);
		_workspace->insertNode(model);
	}

	initGUI();

	_outliner->setRootNode(_workspace);

	if (fileName.isEmpty())
	{
		if (_fileDialog == nullptr)
		{
			_fileDialog = new iDialogFileSelect();
		}
		_fileDialog->open(iDialogCloseDelegate(this, &Mica::onFileLoadDialogClosed), iFileDialogPurpose::Load, DEFAULT_LOAD_SAVE_DIR);
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

void Mica::resetManipulatorMode()
{
	setManipulatorMode(ManipulatorMode::None);
}

void Mica::deinit()
{
	deinitGUI();

	iSceneFactory::getInstance().destroyScene(_scene);
	iSceneFactory::getInstance().destroyScene(_sceneWidget3D);

	// abort flush task
	iTaskManager::getInstance().abortTask(_taskFlushTextures);

	// flush model resources
	iModelResourceFactory::getInstance().flush(&_window);

	_window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &Mica::onWindowClosed));
	_window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &Mica::onWindowResize));

	_window.close();
	_window.removeView(&_view);
	_window.removeView(&_viewOrtho);
	_window.removeView(&_viewWidget3D);

	_view.unregisterRenderDelegate(iDrawDelegate(this, &Mica::render));
	_viewOrtho.unregisterRenderDelegate(iDrawDelegate(this, &Mica::renderOrtho));

	if (_font)
	{
		delete _font;
	}

	iWidgetManager::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &Mica::onMouseWheel));
	iWidgetManager::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &Mica::onMouseMoved));
	iWidgetManager::getInstance().unregisterMouseKeyDownDelegate(iMouseKeyDownDelegate(this, &Mica::onMouseKeyDown));
	iWidgetManager::getInstance().unregisterMouseKeyUpDelegate(iMouseKeyUpDelegate(this, &Mica::onMouseKeyUp));
	iWidgetManager::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &Mica::onKeyDown));
	iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iPreDrawDelegate(this, &Mica::handle));

	if (_manipulator != nullptr)
	{
		delete _manipulator;
		_manipulator = nullptr;
	}
}

void Mica::onAddTransformation(uint64 atNodeID)
{
	iNodePtr destination = iNodeManager::getInstance().getNode(atNodeID);

	if (destination == nullptr)
	{
		destination = _workspace;
	}

	iNodeTransform *transform = iNodeManager::getInstance().createNode<iNodeTransform>();
	transform->setName("Transformation");
	destination->insertNode(transform);
	_outliner->refreshView();
	_outliner->setSelectedNode(transform);
}

void Mica::onAddGroup(uint64 atNodeID)
{
	iNodePtr destination = iNodeManager::getInstance().getNode(atNodeID);

	if (destination == nullptr)
	{
		destination = _workspace;
	}

	iNodePtr group = iNodeManager::getInstance().createNode<iNode>();
	group->setName("Group");
	destination->insertNode(group);
	_outliner->refreshView();
	_outliner->setSelectedNode(group);
}

void Mica::onAddEmitter(uint64 atNodeID)
{
	iNodePtr destination = iNodeManager::getInstance().getNode(atNodeID);

	if (destination == nullptr)
	{
		destination = _workspace;
	}

	iNodeEmitter *emitter = iNodeManager::getInstance().createNode<iNodeEmitter>();
	emitter->setName("Emitter");
	destination->insertNode(emitter);
	_outliner->refreshView();
	_outliner->setSelectedNode(emitter);
}

void Mica::onAddParticleSystem(uint64 atNodeID)
{
	iNodePtr destination = iNodeManager::getInstance().getNode(atNodeID);

	if (destination == nullptr)
	{
		destination = _workspace;
	}

	iNodeParticleSystem *particleSystem = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
	particleSystem->setName("ParticleSystem");
	destination->insertNode(particleSystem);
	_outliner->refreshView();
	_outliner->setSelectedNode(particleSystem);
}

void Mica::onAddMaterial()
{
	iMaterialResourceFactory::getInstance().createMaterial("new Material");
	_outliner->refreshView();
}

void Mica::onAddSwitch(uint64 atNodeID)
{
	iNodePtr destination = iNodeManager::getInstance().getNode(atNodeID);

	if (destination == nullptr)
	{
		destination = _workspace;
	}

	iNodeSwitch *switchNode = iNodeManager::getInstance().createNode<iNodeSwitch>();
	switchNode->setName("Switch");
	destination->insertNode(switchNode);
	_outliner->refreshView();
	_outliner->setSelectedNode(switchNode);
}

void Mica::forceLoadingNow(iNodeModel *modelNode)
{
	if (modelNode != nullptr)
	{
		iScene *tempScene = iSceneFactory::getInstance().createScene();
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

void Mica::frameOnNode(iNodePtr node)
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

void Mica::onImportFile()
{
	if (_fileDialog == nullptr)
	{
		_fileDialog = new iDialogFileSelect();
		_fileDialog->open(iDialogCloseDelegate(this, &Mica::onImportFileDialogClosed), iFileDialogPurpose::Load, DEFAULT_LOAD_SAVE_DIR);
	}
}

void Mica::onImportFileReference()
{
	if (_fileDialog == nullptr)
	{
		_fileDialog = new iDialogFileSelect();
		_fileDialog->open(iDialogCloseDelegate(this, &Mica::onImportFileReferenceDialogClosed), iFileDialogPurpose::Load, DEFAULT_LOAD_SAVE_DIR);
	}
}

void Mica::onLoadFile()
{
	if (_fileDialog == nullptr)
	{
		_fileDialog = new iDialogFileSelect();
		_fileDialog->open(iDialogCloseDelegate(this, &Mica::onFileLoadDialogClosed), iFileDialogPurpose::Load, DEFAULT_LOAD_SAVE_DIR);
	}
}

void Mica::onSaveFile()
{
	if (_fileDialog == nullptr)
	{
		_fileDialog = new iDialogFileSelect();
		_fileDialog->open(iDialogCloseDelegate(this, &Mica::onFileSaveDialogClosed), iFileDialogPurpose::Save, DEFAULT_LOAD_SAVE_DIR);
	}
}

void Mica::onFileSaveDialogClosed(iDialogPtr dialog)
{
	if (_fileDialog != dialog)
	{
		return;
	}

	if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
	{
		iaString filename = _fileDialog->getFullPath();

		std::vector<iNodePtr> children = _workspace->getChildren();
		children.insert(children.end(), _workspace->getInactiveChildren().begin(), _workspace->getInactiveChildren().end());

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
			iModelResourceFactory::getInstance().exportModelData(filename, _workspace);
		}
	}

	delete _fileDialog;
	_fileDialog = nullptr;
}

void Mica::onImportFileDialogClosed(iDialogPtr dialog)
{
	if (_fileDialog != dialog)
	{
		return;
	}

	iNodePtr selectNode = nullptr;

	if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
	{
		iaString filename = _fileDialog->getFullPath();

		iNodeModel *model = iNodeManager::getInstance().createNode<iNodeModel>();
		iModelDataInputParameter *parameter = createDataInputParameter();
		model->setModel(filename, iResourceCacheMode::Free, parameter);

		forceLoadingNow(model);

		if (model->isValid())
		{
			iNodePtr groupNode = nullptr;

			auto children = model->getChildren();
			if (children.size() > 1)
			{
				groupNode = iNodeManager::getInstance().createNode<iNode>();
				iaString groupName = "group:";
				groupName += filename;
				groupNode->setName(groupName);

				iNodePtr cursorNode = iNodeManager::getInstance().getNode(_selectedNodeID);
				if (cursorNode != nullptr)
				{
					cursorNode->insertNode(groupNode);
				}
				else
				{
					_workspace->insertNode(groupNode);
				}

				selectNode = groupNode;
			}
			else
			{
				iNodePtr cursorNode = iNodeManager::getInstance().getNode(_selectedNodeID);
				if (cursorNode != nullptr)
				{
					groupNode = cursorNode;
				}
				else
				{
					groupNode = _workspace;
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

		iNodeManager::getInstance().destroyNodeAsync(model);
	}

	_outliner->setActive();
	_outliner->setVisible();
	_outliner->refreshView();

	_propertiesDialog->setActive();
	_propertiesDialog->setVisible();

	_outliner->setSelectedNode(selectNode);
	frameOnSelectedNode();

	delete _fileDialog;
	_fileDialog = nullptr;
}

void Mica::onImportFileReferenceDialogClosed(iDialogPtr dialog)
{
	if (_fileDialog != dialog)
	{
		return;
	}

	iNodePtr selectNode = nullptr;

	if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
	{
		iaString filename = _fileDialog->getFullPath();

		iNodeModel *model = iNodeManager::getInstance().createNode<iNodeModel>();
		iModelDataInputParameter *parameter = createDataInputParameter();

		model->setModel(filename, iResourceCacheMode::Free, parameter);
		forceLoadingNow(model);

		if (model->isValid())
		{
			iNodePtr cursorNode = iNodeManager::getInstance().getNode(_selectedNodeID);
			if (cursorNode != nullptr)
			{
				cursorNode->insertNode(model);
			}
			else
			{
				_workspace->insertNode(model);
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
	frameOnSelectedNode();

	delete _fileDialog;
	_fileDialog = nullptr;
}

void Mica::onFileLoadDialogClosed(iDialogPtr dialog)
{
	if (_fileDialog != dialog)
	{
		return;
	}

	iNodePtr selectNode = nullptr;

	if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
	{
		iaString filename = _fileDialog->getFullPath();

		if (_workspace->getChildren().size() > 0)
		{
			auto children = _workspace->getChildren();
			auto childIter = children.begin();
			while (childIter != children.end())
			{
				_workspace->removeNode((*childIter));
				iNodeManager::getInstance().destroyNodeAsync((*childIter));
				childIter++;
			}
		}

		iNodeModel *model = iNodeManager::getInstance().createNode<iNodeModel>();
		iModelDataInputParameter *parameter = createDataInputParameter();

		model->setModel(filename, iResourceCacheMode::Free, parameter);
		forceLoadingNow(model);

		if (model->isValid())
		{
			iNodePtr insertAt = nullptr;

			auto children = model->getChildren();
			if (children.size() > 1)
			{
				insertAt = iNodeManager::getInstance().createNode<iNode>();
				iaString groupName = "group:";
				groupName += filename;
				insertAt->setName(groupName);

				_workspace->insertNode(insertAt);
				selectNode = insertAt;
			}
			else
			{
				insertAt = _workspace;
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

		iNodeManager::getInstance().destroyNodeAsync(model);
	}

	_outliner->setActive();
	_outliner->setVisible();
	_outliner->refreshView();

	_propertiesDialog->setActive();
	_propertiesDialog->setVisible();

	_outliner->setSelectedNode(selectNode);
	frameOnSelectedNode();

	delete _fileDialog;
	_fileDialog = nullptr;
}

void Mica::initGUI()
{
	_propertiesDialog = new PropertiesDialog();
	_outliner = new Outliner();

	_widgetTheme = new iWidgetDefaultTheme("StandardFont.png", "WidgetThemePattern.png");
	iWidgetManager::getInstance().setTheme(_widgetTheme);
	iWidgetManager::getInstance().setDesktopDimensions(_window.getClientWidth(), _window.getClientHeight());

	_outliner->registerOnExitMica(ExitMicaDelegate(this, &Mica::onExitMica));
	_outliner->registerOnLoadFile(LoadFileDelegate(this, &Mica::onLoadFile));
	_outliner->registerOnImportFile(ImportFileDelegate(this, &Mica::onImportFile));
	_outliner->registerOnImportFileReference(ImportFileReferenceDelegate(this, &Mica::onImportFileReference));
	_outliner->registerOnSaveFile(SaveFileDelegate(this, &Mica::onSaveFile));
	_outliner->registerOnAddTransformation(AddTransformationDelegate(this, &Mica::onAddTransformation));
	_outliner->registerOnAddSwitch(AddSwitchDelegate(this, &Mica::onAddSwitch));
	_outliner->registerOnAddGroup(AddGroupDelegate(this, &Mica::onAddGroup));
	_outliner->registerOnAddEmitter(AddEmitterDelegate(this, &Mica::onAddEmitter));
	_outliner->registerOnAddParticleSystem(AddParticleSystemDelegate(this, &Mica::onAddParticleSystem));
	_outliner->registerOnAddMaterial(AddMaterialDelegate(this, &Mica::onAddMaterial));

	_propertiesDialog->registerStructureChangedDelegate(StructureChangedDelegate(_outliner, &Outliner::refreshView));

	_outliner->registerOnGraphSelectionChanged(GraphSelectionChangedDelegate(_propertiesDialog, &PropertiesDialog::onGraphViewSelectionChanged));
	_outliner->registerOnGraphSelectionChanged(GraphSelectionChangedDelegate(this, &Mica::onGraphViewSelectionChanged));
	_outliner->registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate(_propertiesDialog, &PropertiesDialog::onMaterialSelectionChanged));
}

void Mica::onGraphViewSelectionChanged(uint64 nodeID)
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

	iNode *node = iNodeManager::getInstance().getNode(_selectedNodeID);
	if (node)
	{
		switch (node->getType())
		{
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

void Mica::setManipulatorMode(ManipulatorMode manipulatorMode)
{
	iNodePtr node = iNodeManager::getInstance().getNode(_selectedNodeID);

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

void Mica::deinitGUI()
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
		delete _fileDialog;
		_fileDialog = nullptr;
	}

	iWidgetManager::getInstance().setTheme(nullptr);
	if (_widgetTheme != nullptr)
	{
		delete _widgetTheme;
		_widgetTheme = nullptr;
	}
}

void Mica::onWindowResize(int32 clientWidth, int32 clientHeight)
{
	iWidgetManager::getInstance().setDesktopDimensions(_window.getClientWidth(), _window.getClientHeight());
	_viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
}

void Mica::updateCamDistanceTransform()
{
	_cameraTranslation->identity();
	_cameraTranslation->translate(0, 0, _camDistance);

	iaMatrixd matrix;
	matrix.translate(0, 0, _camDistance);
	_manipulator->setCamTranslate(matrix);
}

void Mica::onMouseKeyDown(iKeyCode key)
{
	switch (key)
	{
	case iKeyCode::MouseLeft:
		if (!iKeyboard::getInstance().getKey(iKeyCode::Alt))
		{
			_manipulator->onMouseKeyDown(key);
		}
		break;
	}
}

iNodePtr Mica::getNodeAt(int32 x, int32 y)
{
	bool wasVisible = _defaultSkyBox->isVisible();
	_defaultSkyBox->setVisible(false);

	uint64 nodeID = _view.pickcolorID(x, y);
	iNodePtr node = iNodeManager::getInstance().getNode(nodeID);

	_defaultSkyBox->setVisible(wasVisible);

	return node;
}

void Mica::onMouseKeyUp(iKeyCode key)
{
	switch (key)
	{
	case iKeyCode::MouseLeft:

		if (!iKeyboard::getInstance().getKey(iKeyCode::Alt) &&
			!_manipulator->isSelected())
		{
			_outliner->setSelectedNode(getNodeAt(iMouse::getInstance().getPos()._x, iMouse::getInstance().getPos()._y));
			resetManipulatorMode();
		}

		_manipulator->onMouseKeyUp(key);
		break;
	}
}

void Mica::onMouseWheel(int32 d)
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

void Mica::onMouseMoved(const iaVector2i &from, const iaVector2i &to, iWindow *window)
{
	const float64 rotateSensitivity = 0.0075;
	const float64 translateSensitivity = 1.0;

	if (iMouse::getInstance().getLeftButton())
	{
		if (iKeyboard::getInstance().getKey(iKeyCode::Alt))
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
		if (iKeyboard::getInstance().getKey(iKeyCode::Alt))
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

void Mica::onExitMica()
{
	iApplication::getInstance().stop();
}

void Mica::onWindowClosed()
{
	iApplication::getInstance().stop();
}

void Mica::onKeyDown(iKeyCode key)
{
	switch (key)
	{
	case iKeyCode::F:
	{
		frameOnSelectedNode();
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
			clearWorkspace();
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

void Mica::clearWorkspace()
{
	std::vector<iNodePtr> copyChildren(_workspace->getChildren());
	for (auto child : copyChildren)
	{
		_workspace->removeNode(child);
		iNodeManager::getInstance().destroyNodeAsync(child);
	}

	iModelResourceFactory::getInstance().flush(&_window);
	_outliner->refreshView();
}

void Mica::frameOnSelectedNode()
{
	iNodePtr node = iNodeManager::getInstance().getNode(_selectedNodeID);
	frameOnNode(node);
}

void Mica::handle()
{
	_scene->handle();
}

void Mica::renderNodeSelected(uint64 nodeID)
{
	if (nodeID == iNode::INVALID_NODE_ID)
	{
		return;
	}

	iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
	if (node == nullptr)
	{
		return;
	}

	if (node->getKind() != iNodeKind::Renderable &&
		node->getKind() != iNodeKind::Volume)
	{
		return;
	}

	iNodeRender *renderNode = static_cast<iNodeRender *>(node);
	iaMatrixd matrix = renderNode->getWorldMatrix();
	iRenderer::getInstance().setModelMatrix(matrix);

	if (node->getType() == iNodeType::iNodeMesh)
	{
		iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading));

		iNodeMesh *meshNode = static_cast<iNodeMesh *>(node);
		std::shared_ptr<iMeshBuffers> buffers = meshNode->getMeshBuffers();
		iRenderer::getInstance().setLineWidth(4);
		iRenderer::getInstance().drawMesh(buffers);
	}
	else
	{
		if (node->getKind() == iNodeKind::Volume)
		{
			iNodeVolume *renderVolume = static_cast<iNodeVolume *>(node);
			iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_materialBoundingBox));

			iAABoxd box = renderVolume->getBoundingBox();

			iRenderer::getInstance().setColor(1, 1, 0, 1);
			iRenderer::getInstance().drawBBox(box);
		}
	}
}

void Mica::render()
{
	updateCamDistanceTransform();
	renderNodeSelected(_selectedNodeID);
	renderOrientationPlane();
}

void Mica::renderOrientationPlane()
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

void Mica::renderOrtho()
{
	// reset matrices
	iaMatrixd identity;
	iRenderer::getInstance().setViewMatrix(identity);
	iRenderer::getInstance().setModelMatrix(identity);

	// render widgets
	iWidgetManager::getInstance().draw();

	// render profiler
	iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
	_profilerVisualizer.draw(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void Mica::run(iaString fileName)
{
	init(fileName);
	iApplication::getInstance().run();
}
