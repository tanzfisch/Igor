// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "LSystems.h"

#include "PlantMeshGenerator.h"

LSystems::LSystems(iWindowPtr window)
	: ExampleBase(window, "L-System", true, "example_skybox_debug")
{
}

void LSystems::onInit()
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
	// scene associated with the view which we achieved by adding all those nodes on to an other starting with the root node
	getView().setCamera(camera->getID());

	// create a directional light
	// transform node
	iNodeTransform *lightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
	lightTranslate->translate(100, 100, 100);
	// and light node
	iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
	lightNode->setAmbient(iaColor3f(0.6f, 0.6f, 0.6f));
	lightNode->setDiffuse(iaColor3f(0.9f, 0.7f, 0.6f));
	lightNode->setSpecular(iaColor3f(1.0f, 0.9f, 0.87f));
	// and add it to the scene
	getScene()->getRoot()->insertNode(lightTranslate);
	lightTranslate->insertNode(lightNode);

	// register plant mesh generator
	iModelFactory::registerModelDataIO("pmg", &PlantMeshGenerator::createInstance);

	// generate the L-System
	generateLSystems();
}

void LSystems::onDeinit()
{
	// unregister plant mesh generator
	iModelFactory::unregisterModelDataIO("pmg");
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
	iParameters parameters({{IGOR_RESOURCE_PARAM_ALIAS, iaString("plant_") + iaString::toString(iterations) + iaString("_") + iaString::toString(_incarnation++)},
							{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MODEL},
							{IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Free},
							{IGOR_RESOURCE_PARAM_SUB_TYPE, "pmg"},
							{IGOR_RESOURCE_PARAM_GENERATE, true},
							{IGOR_RESOURCE_PARAM_QUIET, true},
							{"lsystem", &_lSystem},
							{IGOR_RESOURCE_PARAM_SHADER_MATERIAL, iRenderer::getInstance().getDefaultShader()},
							{IGOR_RESOURCE_PARAM_JOIN_VERTICES, true},
							{"axiom", axiom},
							{"iterations", iterations},
							{IGOR_RESOURCE_PARAM_SEED, seed},
							{"segmentLength", _segmentLength},
							{"segmentAngle", _angle},
							{"trunkColor", _trunkColor},
							{"branchColor", _branchColor},
							{"budColor", _budColor},
							{"flowerColor", _flowerColor},
							{"leafColor", _leafColor}});

	iModelPtr plant = iResourceManager::getInstance().loadResource<iModel>(parameters);

	iNodeModel *modelNode = iNodeManager::getInstance().createNode<iNodeModel>();
	modelNode->setModel(plant);

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

	// using the same seed for all instances of plants so we see the different stages of growth of the same plant
	uint64 seed = static_cast<uint64>(iaTime::getNow().getMicroseconds());

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

void LSystems::onEvent(iEvent &event)
{
	// first call example base
	ExampleBase::onEvent(event);

	event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(LSystems::onKeyDown));
	event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(LSystems::onMouseMoveEvent));
	event.dispatch<iEventMouseWheel>(IGOR_BIND_EVENT_FUNCTION(LSystems::onMouseWheelEvent));
}

bool LSystems::onMouseWheelEvent(iEventMouseWheel &event)
{
	iNodeTransform *camTranslation = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraTranslation));
	if (camTranslation != nullptr)
	{
		if (event.getWheelDelta() < 0)
		{
			camTranslation->translate(0, 0, 10);
		}
		else
		{
			camTranslation->translate(0, 0, -10);
		}
	}

	return true;
}

bool LSystems::onMouseMoveEvent(iEventMouseMove &event)
{
	if (iMouse::getInstance().getLeftButton())
	{
		const auto from = event.getLastPosition();
		const auto to = event.getPosition();

		iNodeTransform *cameraPitch = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraPitch));
		iNodeTransform *cameraHeading = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraHeading));

		if (cameraPitch != nullptr &&
			cameraHeading != nullptr)
		{
			cameraPitch->rotate((from._y - to._y) * 0.0005f, iaAxis::X);
			cameraHeading->rotate((from._x - to._x) * 0.0005f, iaAxis::Y);
			iMouse::getInstance().setCenter();
		}

		return true;
	}

	return false;
}

bool LSystems::onKeyDown(iEventKeyDown &event)
{
	switch (event.getKey())
	{
	case iKeyCode::Space:
		generateLSystems();
		return true;
	}

	return false;
}

iaString LSystems::getHelpString()
{
	static const iaString help = "\n"
								 "[Space] generate new set of plants\n";

	iaString result = ExampleBase::getHelpString();
	result += help;

	return result;
}