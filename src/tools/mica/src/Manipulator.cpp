// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Manipulator.h"

Manipulator::Manipulator(iViewPtr view, iScenePtr scene, WorkspacePtr workspace)
	: _view(view), _workspace(workspace), _scene(scene)
{
	init();
}

Manipulator::~Manipulator()
{
	deinit();
}

void Manipulator::init()
{
	_view->registerRenderDelegate(iDrawDelegate(this, &Manipulator::render));

	_material = iMaterialResourceFactory_old::getInstance().createMaterial("Manipulator");
	auto material = iMaterialResourceFactory_old::getInstance().getMaterial(_material);
	material->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
	material->addShaderSource("igor/default_directional_light.frag", iShaderObjectType::Fragment);
	material->compileShader();
	material->setRenderState(iRenderState::Blend, iRenderStateValue::On);
	material->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
	iMaterialResourceFactory_old::getInstance().getMaterial(_material)->setOrder(iMaterial::RENDER_ORDER_MAX);

	_red = iMaterialResourceFactory_old::getInstance().createTargetMaterial();
	_red->setEmissive(iaColor3f(0.8f, 0.0f, 0.0f));
	_red->setSpecular(iaColor3f(0.2f, 0.0f, 0.0f));
	_red->setDiffuse(iaColor3f(0.5f, 0.0f, 0.0f));
	_red->setAmbient(iaColor3f(0.3f, 0.0f, 0.0f));
	_red->setAlpha(0.8);

	_green = iMaterialResourceFactory_old::getInstance().createTargetMaterial();
	_green->setEmissive(iaColor3f(0.0f, 0.8f, 0.0f));
	_green->setSpecular(iaColor3f(0.0f, 0.2f, 0.0f));
	_green->setDiffuse(iaColor3f(0.0f, 0.5f, 0.0f));
	_green->setAmbient(iaColor3f(0.0f, 0.3f, 0.0f));
	_green->setAlpha(0.8);

	_blue = iMaterialResourceFactory_old::getInstance().createTargetMaterial();
	_blue->setEmissive(iaColor3f(0.0f, 0.0f, 0.8f));
	_blue->setSpecular(iaColor3f(0.0f, 0.0f, 0.2f));
	_blue->setDiffuse(iaColor3f(0.0f, 0.0f, 0.5f));
	_blue->setAmbient(iaColor3f(0.0f, 0.0f, 0.3f));
	_blue->setAlpha(0.8);

	_cyan = iMaterialResourceFactory_old::getInstance().createTargetMaterial();
	_cyan->setEmissive(iaColor3f(0.0f, 0.8f, 0.8f));
	_cyan->setSpecular(iaColor3f(0.0f, 0.2f, 0.2f));
	_cyan->setDiffuse(iaColor3f(0.0f, 0.5f, 0.5f));
	_cyan->setAmbient(iaColor3f(0.0f, 0.3f, 0.3f));
	_cyan->setAlpha(0.8);

	iMeshPtr translateMesh = createTranslateMesh();
	iMeshPtr scaleMesh = createScaleMesh();
	iMeshPtr ringMesh = createRingMesh();
	iMeshPtr ringMesh2D = create2DRingMesh();
	iMeshPtr cylinder = createCylinder();

	_rootTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	_rootTransform->setActive(_visible);

	_switchNode = iNodeManager::getInstance().createNode<iNodeSwitch>();
	_rootTransform->insertNode(_switchNode);

	createTranslateModifier(translateMesh);
	createScaleModifier(scaleMesh);
	createRotateModifier(ringMesh, ringMesh2D, cylinder);
	createTransformRepresentation(cylinder);

	_scene->getRoot()->insertNode(_rootTransform);

	_materialCelShading = iMaterialResourceFactory_old::getInstance().createMaterial("ManipulatorCelShading");
	iMaterialResourceFactory_old::getInstance().getMaterial(_materialCelShading)->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
	iMaterialResourceFactory_old::getInstance().getMaterial(_materialCelShading)->addShaderSource("Mica/yellow.frag", iShaderObjectType::Fragment);
	iMaterialResourceFactory_old::getInstance().getMaterial(_materialCelShading)->compileShader();
	iMaterialResourceFactory_old::getInstance().getMaterial(_materialCelShading)->setRenderState(iRenderState::Wireframe, iRenderStateValue::On);
	iMaterialResourceFactory_old::getInstance().getMaterial(_materialCelShading)->setRenderState(iRenderState::CullFace, iRenderStateValue::On);
	iMaterialResourceFactory_old::getInstance().getMaterial(_materialCelShading)->setRenderState(iRenderState::CullFaceFunc, iRenderStateValue::Front);

	setManipulatorMode(_manipulatorMode);
}

void Manipulator::highlightSelected()
{
	if (_selectedManipulatorNodeID != iNode::INVALID_NODE_ID)
	{
		iNodePtr node = iNodeManager::getInstance().getNode(_selectedManipulatorNodeID);

		if (node->getKind() == iNodeKind::Renderable ||
			node->getKind() == iNodeKind::Volume)
		{
			iNodeRender *renderNode = static_cast<iNodeRender *>(node);
			iaMatrixd matrix = renderNode->getWorldMatrix();
			iRenderer::getInstance().setModelMatrix(matrix);

			if (node->getType() == iNodeType::iNodeMesh)
			{
				iRenderer::getInstance().setMaterial(iMaterialResourceFactory_old::getInstance().getMaterial(_materialCelShading));

				iNodeMesh *meshNode = static_cast<iNodeMesh *>(node);
				std::shared_ptr<iMeshBuffers> buffers = meshNode->getMeshBuffers();
				iRenderer::getInstance().setLineWidth(4);
				iRenderer::getInstance().drawMesh(buffers);
			}
		}
	}
}

void Manipulator::render()
{
	update();
	highlightSelected();
}

void Manipulator::createRotateModifier(iMeshPtr &ringMesh, iMeshPtr &ringMesh2D, iMeshPtr &cylinder)
{
	_roateModifier = iNodeManager::getInstance().createNode<iNode>();
	_switchNode->insertNode(_roateModifier);

	iNodeTransform *xRingTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	xRingTransform->rotate(-M_PI * 0.5, iaAxis::Z);
	xRingTransform->scale(2.0, 0.1, 2.0);
	_roateModifier->insertNode(xRingTransform);

	iNodeTransform *yRingTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	yRingTransform->scale(1.99, 0.1, 1.99);
	_roateModifier->insertNode(yRingTransform);

	iNodeTransform *zRingTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	zRingTransform->rotate(M_PI * 0.5, iaAxis::X);
	zRingTransform->scale(1.98, 0.1, 1.98);
	_roateModifier->insertNode(zRingTransform);

	iNodeMesh *xRing = iNodeManager::getInstance().createNode<iNodeMesh>();
	xRing->setMesh(ringMesh);
	xRing->setMaterial(_material);
	xRing->setTargetMaterial(_red);
	xRingTransform->insertNode(xRing);

	iNodeMesh *yRing = iNodeManager::getInstance().createNode<iNodeMesh>();
	yRing->setMesh(ringMesh);
	yRing->setMaterial(_material);
	yRing->setTargetMaterial(_green);
	yRingTransform->insertNode(yRing);

	iNodeMesh *zRing = iNodeManager::getInstance().createNode<iNodeMesh>();
	zRing->setMesh(ringMesh);
	zRing->setMaterial(_material);
	zRing->setTargetMaterial(_blue);
	zRingTransform->insertNode(zRing);

	_rotateBillboardTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	_roateModifier->insertNode(_rotateBillboardTransform);

	iNodeMesh *ring = iNodeManager::getInstance().createNode<iNodeMesh>();
	ring->setMesh(ringMesh2D);
	ring->setMaterial(_material);
	ring->setTargetMaterial(_cyan);
	_rotateBillboardTransform->insertNode(ring);

	_rotateIDs.push_back(xRing->getID());
	_rotateIDs.push_back(yRing->getID());
	_rotateIDs.push_back(zRing->getID());

	// add a locator in the middle for better orientation
	iNodeTransform *xTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
	_roateModifier->insertNode(xTransform);

	iNodeTransform *yTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	_roateModifier->insertNode(yTransform);

	iNodeTransform *zTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	zTransform->rotate(M_PI * 0.5, iaAxis::X);
	_roateModifier->insertNode(zTransform);

	iNodeMesh *xCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
	xCylinder->setMesh(cylinder);
	xCylinder->setMaterial(_material);
	xCylinder->setTargetMaterial(_red);
	xTransform->insertNode(xCylinder);

	iNodeMesh *yCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
	yCylinder->setMesh(cylinder);
	yCylinder->setMaterial(_material);
	yCylinder->setTargetMaterial(_green);
	yTransform->insertNode(yCylinder);

	iNodeMesh *zCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
	zCylinder->setMesh(cylinder);
	zCylinder->setMaterial(_material);
	zCylinder->setTargetMaterial(_blue);
	zTransform->insertNode(zCylinder);
}

void Manipulator::createTransformRepresentation(iMeshPtr &cylinder)
{
	_transformRepresentation = iNodeManager::getInstance().createNode<iNode>();
	_switchNode->insertNode(_transformRepresentation);

	iNodeTransform *xTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
	_transformRepresentation->insertNode(xTransform);

	iNodeTransform *yTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	_transformRepresentation->insertNode(yTransform);

	iNodeTransform *zTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	zTransform->rotate(M_PI * 0.5, iaAxis::X);
	_transformRepresentation->insertNode(zTransform);

	iNodeMesh *xCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
	xCylinder->setMesh(cylinder);
	xCylinder->setMaterial(_material);
	xCylinder->setTargetMaterial(_red);
	xTransform->insertNode(xCylinder);

	iNodeMesh *yCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
	yCylinder->setMesh(cylinder);
	yCylinder->setMaterial(_material);
	yCylinder->setTargetMaterial(_green);
	yTransform->insertNode(yCylinder);

	iNodeMesh *zCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
	zCylinder->setMesh(cylinder);
	zCylinder->setMaterial(_material);
	zCylinder->setTargetMaterial(_blue);
	zTransform->insertNode(zCylinder);
}

void Manipulator::createTranslateModifier(iMeshPtr &translateMesh)
{
	_translateModifier = iNodeManager::getInstance().createNode<iNode>();
	_switchNode->insertNode(_translateModifier);

	iNodeTransform *xTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
	_translateModifier->insertNode(xTransform);

	iNodeTransform *yTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	_translateModifier->insertNode(yTransform);

	iNodeTransform *zTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	zTransform->rotate(M_PI * 0.5, iaAxis::X);
	_translateModifier->insertNode(zTransform);

	iNodeMesh *xUmbrella = iNodeManager::getInstance().createNode<iNodeMesh>();
	xUmbrella->setMesh(translateMesh);
	xUmbrella->setMaterial(_material);
	xUmbrella->setTargetMaterial(_red);
	xTransform->insertNode(xUmbrella);

	iNodeMesh *yUmbrella = iNodeManager::getInstance().createNode<iNodeMesh>();
	yUmbrella->setMesh(translateMesh);
	yUmbrella->setMaterial(_material);
	yUmbrella->setTargetMaterial(_green);
	yTransform->insertNode(yUmbrella);

	iNodeMesh *zUmbrella = iNodeManager::getInstance().createNode<iNodeMesh>();
	zUmbrella->setMesh(translateMesh);
	zUmbrella->setMaterial(_material);
	zUmbrella->setTargetMaterial(_blue);
	zTransform->insertNode(zUmbrella);

	_translateIDs.push_back(xUmbrella->getID());
	_translateIDs.push_back(yUmbrella->getID());
	_translateIDs.push_back(zUmbrella->getID());
}

void Manipulator::createScaleModifier(iMeshPtr &scaleMesh)
{
	_scaleModifier = iNodeManager::getInstance().createNode<iNode>();
	_switchNode->insertNode(_scaleModifier);

	iNodeTransform *xTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
	_scaleModifier->insertNode(xTransform);

	iNodeTransform *yTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	_scaleModifier->insertNode(yTransform);

	iNodeTransform *zTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	zTransform->rotate(M_PI * 0.5, iaAxis::X);
	_scaleModifier->insertNode(zTransform);

	iNodeMesh *xCube = iNodeManager::getInstance().createNode<iNodeMesh>();
	xCube->setMesh(scaleMesh);
	xCube->setMaterial(_material);
	xCube->setTargetMaterial(_red);
	xTransform->insertNode(xCube);

	iNodeMesh *yCube = iNodeManager::getInstance().createNode<iNodeMesh>();
	yCube->setMesh(scaleMesh);
	yCube->setMaterial(_material);
	yCube->setTargetMaterial(_green);
	yTransform->insertNode(yCube);

	iNodeMesh *zCube = iNodeManager::getInstance().createNode<iNodeMesh>();
	zCube->setMesh(scaleMesh);
	zCube->setMaterial(_material);
	zCube->setTargetMaterial(_blue);
	zTransform->insertNode(zCube);

	iMeshPtr cube = createCube();

	iNodeMesh *xyzCube = iNodeManager::getInstance().createNode<iNodeMesh>();
	xyzCube->setMesh(cube);
	xyzCube->setMaterial(_material);
	xyzCube->setTargetMaterial(_cyan);
	_scaleModifier->insertNode(xyzCube);

	_scaleIDs.push_back(xCube->getID());
	_scaleIDs.push_back(yCube->getID());
	_scaleIDs.push_back(zCube->getID());
	_scaleIDs.push_back(xyzCube->getID());
}

void Manipulator::update()
{
	iNodePtr node = iNodeManager::getInstance().getNode(_selectedNodeID);
	if (node == nullptr)
	{
		setVisible(false);
		return;
	}

	iaMatrixd matrix;
	node->calcWorldTransformation(matrix);

	iaMatrixd camMatrix;
	_workspace->getCameraArc()->getWorldTransformation(camMatrix);

	float64 distanceToCam = camMatrix._pos.distance(matrix._pos) * 0.1;

	matrix._right.normalize();
	matrix._top.normalize();
	matrix._depth.normalize();
	_rootTransform->setMatrix(matrix);
	_rootTransform->scale(distanceToCam, distanceToCam, distanceToCam);

	// compensate for parent orientation
	iaMatrixd parentMatrix;
	_rotateBillboardTransform->getParent()->calcWorldTransformation(parentMatrix);
	parentMatrix._pos.set(0, 0, 0);
	parentMatrix.invert();
	parentMatrix._right.normalize();
	parentMatrix._top.normalize();
	parentMatrix._depth.normalize();

	_rotateBillboardTransform->identity();
	iaMatrixd billboardMatrix;
	_rotateBillboardTransform->getMatrix(billboardMatrix);
	billboardMatrix._right = camMatrix._right;
	billboardMatrix._top = camMatrix._top;
	billboardMatrix._depth = camMatrix._depth;
	_rotateBillboardTransform->setMatrix(parentMatrix * billboardMatrix);
	_rotateBillboardTransform->rotate(M_PI * 0.5, iaAxis::X);
	_rotateBillboardTransform->scale(2.1, 2.1, 2.1);
}

void Manipulator::deinit()
{
	iMaterialResourceFactory_old::getInstance().destroyTargetMaterial(_red);
	iMaterialResourceFactory_old::getInstance().destroyTargetMaterial(_green);
	iMaterialResourceFactory_old::getInstance().destroyTargetMaterial(_blue);
	iMaterialResourceFactory_old::getInstance().destroyTargetMaterial(_cyan);
	iMaterialResourceFactory_old::getInstance().destroyMaterial(_materialCelShading);
	iMaterialResourceFactory_old::getInstance().destroyMaterial(_material);

	_view->unregisterRenderDelegate(iDrawDelegate(this, &Manipulator::render));
}

void Manipulator::setVisible(bool visible)
{
	_visible = visible;
	_rootTransform->setActive(_visible);
}

bool Manipulator::isVisible() const
{
	return _visible;
}

iMeshPtr Manipulator::createRingMesh()
{
	iMeshBuilder meshBuilder;
	iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 64, false);
	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}

iMeshPtr Manipulator::create2DRingMesh()
{
	iMeshBuilder meshBuilder;
	iMeshBuilderUtils::addRing(meshBuilder, 0.99, 1, 64);
	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}

iMeshPtr Manipulator::createScaleMesh()
{
	iMeshBuilder meshBuilder;

	iaMatrixf matrix;
	matrix.scale(0.02, 1.5, 0.02);
	meshBuilder.setMatrix(matrix);
	iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 6);

	matrix.identity();
	matrix.translate(0, 1.5, 0);
	matrix.scale(0.25, 0.25, 0.25);
	meshBuilder.setMatrix(matrix);
	iMeshBuilderUtils::addBox(meshBuilder, 1, 1, 1);

	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}

iMeshPtr Manipulator::createCube()
{
	iMeshBuilder meshBuilder;

	iaMatrixf matrix;
	matrix.scale(0.25, 0.25, 0.25);
	meshBuilder.setMatrix(matrix);
	iMeshBuilderUtils::addBox(meshBuilder, 1, 1, 1);

	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}

iMeshPtr Manipulator::createCylinder()
{
	iMeshBuilder meshBuilder;
	meshBuilder.setJoinVertexes(true);

	iaMatrixf matrix;
	matrix.scale(0.025, 1.5, 0.025);
	meshBuilder.setMatrix(matrix);
	iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 6);

	meshBuilder.calcNormals(true);

	return meshBuilder.createMesh();
}

iMeshPtr Manipulator::createTranslateMesh()
{
	iMeshBuilder meshBuilder;
	meshBuilder.setJoinVertexes(false);

	iaMatrixf matrix;

	matrix.translate(0, 1.5, 0);
	matrix.scale(0.1, 0.5, 0.1);
	meshBuilder.setMatrix(matrix);
	iMeshBuilderUtils::addCone(meshBuilder, 1, 1, 6);

	matrix.identity();
	matrix.scale(0.025, 1.5, 0.025);
	meshBuilder.setMatrix(matrix);
	iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 6);

	meshBuilder.calcNormals(true);

	return meshBuilder.createMesh();
}

void Manipulator::setManipulatorMode(ManipulatorMode manipulatorMode)
{
	_manipulatorMode = manipulatorMode;

	switch (_manipulatorMode)
	{
	case ManipulatorMode::None:
		_switchNode->setActiveChild(_transformRepresentation);
		break;

	case ManipulatorMode::Translate:
		_switchNode->setActiveChild(_translateModifier);
		break;

	case ManipulatorMode::Scale:
		_switchNode->setActiveChild(_scaleModifier);
		break;

	case ManipulatorMode::Rotate:
		_switchNode->setActiveChild(_roateModifier);
		break;
	}
}

ManipulatorMode Manipulator::getManipulatorMode() const
{
	return _manipulatorMode;
}

void Manipulator::setNodeID(uint64 nodeID)
{
	iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
	if (node != nullptr)
	{
		_selectedNodeID = nodeID;
	}
	else
	{
		_selectedNodeID = iNode::INVALID_NODE_ID;
	}
}

uint64 Manipulator::getNodeID() const
{
	return _selectedNodeID;
}

void Manipulator::scale(const iaVector3d &vec, iaMatrixd &matrix)
{
	const iaVector3d dir[] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 1, 1}};
	iaVector3d scale;

	for (int i = 0; i < 4; ++i)
	{
		if (_selectedManipulatorNodeID == _scaleIDs[i])
		{
			scale = vec.project(dir[i]) + iaVector3d(1, 1, 1);
			matrix.scale(scale);
			return;
		}
	}
}

void Manipulator::translate(const iaVector3d &vec, iaMatrixd &matrix)
{
	const iaVector3d dir[] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	iaVector3d translate;

	for (int i = 0; i < 3; ++i)
	{
		if (_selectedManipulatorNodeID == _translateIDs[i])
		{
			translate = vec.project(dir[i]);
			matrix.translate(translate);
			return;
		}
	}
}

void Manipulator::rotate(const iaVector2d &from, const iaVector2d &to, iaMatrixd &matrix)
{
	iNode *node = iNodeManager::getInstance().getNode(_selectedNodeID);
	iaMatrixd transformWorldMatrix;
	node->calcWorldTransformation(transformWorldMatrix);

	iaMatrixd camWorldMatrix;
	_workspace->getCameraArc()->getWorldTransformation(camWorldMatrix);
	iaVector3d center = _view->project(transformWorldMatrix._pos, camWorldMatrix);

	iaVector2d center2D(center._x, center._y);

	iaVector2d a = from - center2D;
	iaVector2d b = to - center2D;

	float64 angle = b.angle() - a.angle();

	for (int i = 0; i < 3; ++i)
	{
		if (_selectedManipulatorNodeID == _rotateIDs[i])
		{
			iaAxis axis = static_cast<iaAxis>(i);
			float64 scalar = 0;

			iaVector3d toCam = camWorldMatrix._pos - matrix._pos;

			switch (axis)
			{
			case iaAxis::X:
				scalar = toCam * matrix._right;
				break;
			case iaAxis::Y:
				scalar = toCam * matrix._top;
				break;
			case iaAxis::Z:
				scalar = toCam * matrix._depth;
				break;
			}

			if (scalar < 0)
			{
				angle = -angle;
			}

			matrix.rotate(angle, static_cast<iaAxis>(i));
			return;
		}
	}
}

void Manipulator::onMouseMoved(const iaVector2i &from, const iaVector2i &to)
{
	if (_selectedManipulatorNodeID != iNode::INVALID_NODE_ID)
	{
		iaVector2d fromd = from.convert<float64>();
		iaVector2d tod = to.convert<float64>();

		iNodePtr node = iNodeManager::getInstance().getNode(_selectedNodeID);
		if (node != nullptr &&
			node->getType() == iNodeType::iNodeTransform)
		{
			iaMatrixd camWorldMatrix;
			_workspace->getCameraArc()->getWorldTransformation(camWorldMatrix);

			iaVector3d fromWorld = camWorldMatrix * _view->unProject(iaVector3d(from._x, from._y, 0), camWorldMatrix);
			iaVector3d toWorld = camWorldMatrix * _view->unProject(iaVector3d(to._x, to._y, 0), camWorldMatrix);

			iNodeTransform *transformNode = static_cast<iNodeTransform *>(node);
			iaMatrixd transformWorldMatrix;
			transformNode->calcWorldTransformation(transformWorldMatrix);
			transformWorldMatrix.invert();
			fromWorld = transformWorldMatrix * fromWorld;
			toWorld = transformWorldMatrix * toWorld;

			float64 distance = _workspace->getCameraArc()->getDistance();

			iaMatrixd nodeMatrix;
			transformNode->getMatrix(nodeMatrix);

			switch (_manipulatorMode)
			{
			case ManipulatorMode::None:
				break;
			case ManipulatorMode::Rotate:
				rotate(fromd, tod, nodeMatrix);
				break;
			case ManipulatorMode::Scale:
				scale((toWorld - fromWorld) * distance * 2, nodeMatrix);
				break;
			case ManipulatorMode::Translate:
				translate((toWorld - fromWorld) * distance, nodeMatrix);
				break;
			}

			transformNode->setMatrix(nodeMatrix);
		}
	}
}

bool Manipulator::isSelected() const
{
	return (_selectedManipulatorNodeID != iNode::INVALID_NODE_ID) ? true : false;
}

void Manipulator::select()
{
	_selectedManipulatorNodeID = _view->pickcolorID(iMouse::getInstance().getPos()._x, iMouse::getInstance().getPos()._y);
}

void Manipulator::unselect()
{
	_selectedManipulatorNodeID = iNode::INVALID_NODE_ID;
}
