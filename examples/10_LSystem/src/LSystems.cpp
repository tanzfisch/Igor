// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "LSystems.h"

#include <igor/resources/material/iMaterial.h>
#include <igor/scene/traversal/iNodeVisitorPrintTree.h>
#include <igor/threading/iTaskManager.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/graphics/iRenderer.h>
#include <igor/system/iApplication.h>
#include <igor/scene/iSceneFactory.h>
#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/system/iMouse.h>
#include <igor/system/iKeyboard.h>
#include <igor/system/iTimer.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/threading/tasks/iTaskFlushModels.h>
#include <igor/threading/tasks/iTaskFlushTextures.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/scene/nodes/iNodeSwitch.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>
#include <igor/scene/nodes/iNodeLODTrigger.h>
#include <igor/resources/texture/iTextureResourceFactory.h>
using namespace igor;

#include <iaux/system/iaConsole.h>
#include <iaux/data/iaString.h>
using namespace iaux;

#include "PlantMeshGenerator.h"

LSystems::LSystems()
	: ExampleBase("L-System")
{
}

void LSystems::init()
{
	// setup camera
	// we want a camera which can be rotated arround the origin
	// we will acchive that with 3 transform nodes
	// one is for the heading
	iNodeTransform *cameraHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
	// give the transform node a name. naming is optional and ist jus for helping to debug.
	// Names do not have to be unique but since it is possible to find nodes by name they better are
	cameraHeading->setName("camera heading");
	cameraHeading->rotate(M_PI * 4.0, iaAxis::Y);
	_cameraHeading = cameraHeading->getID();
	// one is for the pitch
	iNodeTransform *cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
	cameraPitch->setName("camera pitch");
	cameraPitch->rotate(0.25, iaAxis::X);
	_cameraPitch = cameraPitch->getID();
	// and one is for translation or distance from the origin
	iNodeTransform *cameraTranslation = iNodeManager::getInstance().createNode<iNodeTransform>();
	cameraTranslation->setName("camera translation");
	// translate away from origin
	cameraTranslation->translate(0, 0, 50);
	_cameraTranslation = cameraTranslation->getID();
	// from all nodes that we want to control later we save the node ID
	// and last but not least we create a camera node
	iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
	camera->setName("camera");
	// and build everything together
	// first we add the heading to the root node
	getScene()->getRoot()->insertNode(cameraHeading);
	// than the pitch to the heading node
	cameraHeading->insertNode(cameraPitch);
	// then the translation to the pitch node
	cameraPitch->insertNode(cameraTranslation);
	// and than we add the camera to the translation
	cameraTranslation->insertNode(camera);
	// and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a
	// scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
	getView().setCurrentCamera(camera->getID());

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
	getScene()->getRoot()->insertNode(lightTranslate);
	lightTranslate->insertNode(lightNode);

	// register plant mesh generator
	iModelResourceFactory::getInstance().registerModelDataIO("pg", &PlantMeshGenerator::createInstance);

	// generate the L-System
	generateLSystems();
}

void LSystems::deinit()
{
	// unregister plant mesh generator
	iModelResourceFactory::getInstance().unregisterModelDataIO("pg");
}

void LSystems::initStyle1()
{
	_lSystem.setRule('F', "FF");

	std::vector<std::pair<float64, iaString>> weightedRule1;
	weightedRule1.push_back(std::pair<float64, iaString>(0.25, "F[+X]OF[-X]+X."));
	weightedRule1.push_back(std::pair<float64, iaString>(0.25, "FO[-X]F[+X]-X."));
	weightedRule1.push_back(std::pair<float64, iaString>(0.25, "F[RX]F[LX]ORX."));
	weightedRule1.push_back(std::pair<float64, iaString>(0.25, "F[LX]FO[RX]LX."));
	_lSystem.setRule('X', weightedRule1);

	std::vector<std::pair<float64, iaString>> weightedRule2;
	weightedRule2.push_back(std::pair<float64, iaString>(0.3, "."));
	weightedRule2.push_back(std::pair<float64, iaString>(0.7, "*"));
	_lSystem.setRule('.', weightedRule2);
	_lSystem.setAgeFilter('.', iLSystemAgeFunction::Greater, 2);

	std::vector<std::pair<float64, iaString>> weightedRule3;
	weightedRule3.push_back(std::pair<float64, iaString>(0.3, "*"));
	weightedRule3.push_back(std::pair<float64, iaString>(0.7, "o"));
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

	std::vector<std::pair<float64, iaString>> weightedRule1;
	weightedRule1.push_back(std::pair<float64, iaString>(0.25, "F-[[X]+X]O+F[+FX]-X."));
	weightedRule1.push_back(std::pair<float64, iaString>(0.25, "F+[[X]-X]-F[-FX]O+X."));
	weightedRule1.push_back(std::pair<float64, iaString>(0.25, "FR[[X]LX]LFO[LFX]RX."));
	weightedRule1.push_back(std::pair<float64, iaString>(0.25, "FL[[X]RX]ORF[RFX]LX."));
	_lSystem.setRule('X', weightedRule1);

	std::vector<std::pair<float64, iaString>> weightedRule2;
	weightedRule2.push_back(std::pair<float64, iaString>(0.3, "."));
	weightedRule2.push_back(std::pair<float64, iaString>(0.7, "*"));
	_lSystem.setRule('.', weightedRule2);
	_lSystem.setAgeFilter('.', iLSystemAgeFunction::Greater, 2);

	std::vector<std::pair<float64, iaString>> weightedRule3;
	weightedRule3.push_back(std::pair<float64, iaString>(0.3, "*"));
	weightedRule3.push_back(std::pair<float64, iaString>(0.7, "o"));
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

	std::vector<std::pair<float64, iaString>> weightedRule1;
	weightedRule1.push_back(std::pair<float64, iaString>(0.5, "F[+X]O[-X]FX."));
	weightedRule1.push_back(std::pair<float64, iaString>(0.5, "F[RX]O[LX]FX."));
	_lSystem.setRule('X', weightedRule1);

	std::vector<std::pair<float64, iaString>> weightedRule2;
	weightedRule2.push_back(std::pair<float64, iaString>(0.3, "."));
	weightedRule2.push_back(std::pair<float64, iaString>(0.7, "*"));
	_lSystem.setRule('.', weightedRule2);
	_lSystem.setAgeFilter('.', iLSystemAgeFunction::Greater, 2);

	std::vector<std::pair<float64, iaString>> weightedRule3;
	weightedRule3.push_back(std::pair<float64, iaString>(0.3, "*"));
	weightedRule3.push_back(std::pair<float64, iaString>(0.7, "o"));
	_lSystem.setRule('*', weightedRule3);
	_lSystem.setAgeFilter('*', iLSystemAgeFunction::Greater, 4);

	_segmentLength = 0.25f;
	_angle = 0.5f;
	_trunkColor.set(0.0f, 0.8f, 0.0f);
	_budColor.set(0.8f, 0.8f, 0.5f);
	_flowerColor.set(1.0f, 1.0f, 1.0f);
	_leafColor.set(0.0f, 0.7f, 0.0f);
}

uint64 LSystems::generatePlant(const iaMatrixd &matrix, const iaString &axiom, uint32 iterations, uint64 seed)
{
	PlantInformation plantInformation;
	plantInformation._lSystem = &_lSystem;
	for (int64 i = 0; i < 10, i < axiom.getLength(); ++i)
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

	iModelDataInputParameter *inputParam = new iModelDataInputParameter();
	inputParam->_identifier = "pg";
	inputParam->_joinVertexes = true;
	inputParam->_needsRenderContext = false;
	inputParam->_modelSourceType = iModelSourceType::Generated;
	inputParam->_loadPriority = 0;
	inputParam->_parameters.setData(reinterpret_cast<const char *>(&plantInformation), sizeof(PlantInformation));

	iNodeModel *modelNode = iNodeManager::getInstance().createNode<iNodeModel>();
	modelNode->setModel(iaString("plant_") + iaString::toString(iterations) + iaString("_") + iaString::toString(_incarnation++), iResourceCacheMode::Free, inputParam);

	iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();
	transformNode->setMatrix(matrix);

	// and add to scene
	transformNode->insertNode(modelNode);

	iNodePtr groupNode = static_cast<iNodePtr>(iNodeManager::getInstance().getNode(_groupNodeID));
	groupNode->insertNode(transformNode);

	return modelNode->getID();
}

bool LSystems::checkIfDone()
{
	bool result = true;
	for (auto id : _plantsInProgress)
	{
		iNodeModel *model = static_cast<iNodeModel *>(iNodeManager::getInstance().getNode(id));
		if (!model->isValid())
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
		iNodeManager::getInstance().destroyNodeAsync(_groupNodeID);
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
	uint64 seed = static_cast<uint64>(iaTime::now().getMicrosenconds());

	iNodePtr groupNode = iNodeManager::getInstance().createNode<iNode>();
	_groupNodeID = groupNode->getID();
	getScene()->getRoot()->insertNode(groupNode);

	_plantsInProgress.clear();

	for (int i = 0; i < 7; ++i)
	{
		iaMatrixd matrix;
		matrix.translate(-15 + i * 5, -15, 0);
		_plantsInProgress.push_back(generatePlant(matrix, "X", i, seed));
	}
}

void LSystems::onMouseWheel(int32 d)
{
	iNodeTransform *camTranslation = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraTranslation));
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

void LSystems::onMouseMovedFull(const iaVector2i &from, const iaVector2i &to, iWindow *window)
{
	if (iMouse::getInstance().getLeftButton())
	{
		iNodeTransform *cameraPitch = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraPitch));
		iNodeTransform *cameraHeading = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraHeading));

		if (cameraPitch != nullptr &&
			cameraHeading != nullptr)
		{
			cameraPitch->rotate((from._y - to._y) * 0.005f, iaAxis::X);
			cameraHeading->rotate((from._x - to._x) * 0.005f, iaAxis::Y);
			iMouse::getInstance().setCenter();
		}
	}

	ExampleBase::onMouseMovedFull(from, to, window);
}

void LSystems::onKeyDown(iKeyCode key)
{
	switch (key)
	{
	case iKeyCode::Space:
		generateLSystems();
		break;
	}

	ExampleBase::onKeyDown(key);
}