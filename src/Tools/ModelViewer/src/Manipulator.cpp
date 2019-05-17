// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "Manipulator.h"

#include <iMaterialResourceFactory.h>
#include <iTargetMaterial.h>
#include <iRenderer.h>
#include <iMeshBuilder.h>
#include <iNodeFactory.h>
#include <iRenderStateSet.h>
#include <iNodeTransform.h>
#include <iNodeMesh.h>
#include <iNodeSwitch.h>
#include <iMeshBuilderUtils.h>
#include <iSceneFactory.h>
#include <iNodeCamera.h>
#include <iMesh.h>
using namespace Igor;

#include <iaConvert.h>
using namespace IgorAux;

Manipulator::Manipulator(iWindow* window, iView* view, iScene* scene)
{
	con_assert(window != nullptr, "zero pointer");

	_window = window;
	_view = view;
	_scene = scene;

	init();
}

Manipulator::~Manipulator()
{
	deinit();
}

void Manipulator::init()
{
	_view->registerRenderDelegate(RenderDelegate(this, &Manipulator::render));

	// camUI
	_cameraCOIUI = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_cameraCOIUI->setName("camera COI UI");
	_cameraHeadingUI = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_cameraHeadingUI->setName("camera heading UI");
	_cameraPitchUI = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_cameraPitchUI->setName("camera pitch UI");
	_cameraTranslationUI = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_cameraTranslationUI->setName("camera translation UI");
	_cameraUI = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
	_cameraUI->setName("camera UI");

	_scene->getRoot()->insertNode(_cameraCOIUI);
	_cameraCOIUI->insertNode(_cameraHeadingUI);
	_cameraHeadingUI->insertNode(_cameraPitchUI);
	_cameraPitchUI->insertNode(_cameraTranslationUI);
	_cameraTranslationUI->insertNode(_cameraUI);
	_view->setCurrentCamera(_cameraUI->getID());

	_cameraTranslationUI->translate(0, 0, 80);

	_material = iMaterialResourceFactory::getInstance().createMaterial("Manipulator");
	auto material = iMaterialResourceFactory::getInstance().getMaterial(_material);
	material->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
	material->addShaderSource("igor/default_directional_light.frag", iShaderObjectType::Fragment);
	material->compileShader();
	material->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
	material->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
	iMaterialResourceFactory::getInstance().getMaterial(_material)->setOrder(iMaterial::RENDER_ORDER_MAX);

	_red = iMaterialResourceFactory::getInstance().createTargetMaterial();
	_red->setEmissive(iaColor3f(0.8f, 0.0f, 0.0f));
	_red->setSpecular(iaColor3f(0.2f, 0.0f, 0.0f));
	_red->setDiffuse(iaColor3f(0.5f, 0.0f, 0.0f));
	_red->setAmbient(iaColor3f(0.3f, 0.0f, 0.0f));
	_red->setAlpha(0.6);

	_green = iMaterialResourceFactory::getInstance().createTargetMaterial();
	_green->setEmissive(iaColor3f(0.0f, 0.8f, 0.0f));
	_green->setSpecular(iaColor3f(0.0f, 0.2f, 0.0f));
	_green->setDiffuse(iaColor3f(0.0f, 0.5f, 0.0f));
	_green->setAmbient(iaColor3f(0.0f, 0.3f, 0.0f));
	_green->setAlpha(0.6);

	_blue = iMaterialResourceFactory::getInstance().createTargetMaterial();
	_blue->setEmissive(iaColor3f(0.0f, 0.0f, 0.8f));
	_blue->setSpecular(iaColor3f(0.0f, 0.0f, 0.2f));
	_blue->setDiffuse(iaColor3f(0.0f, 0.0f, 0.5f));
	_blue->setAmbient(iaColor3f(0.0f, 0.0f, 0.3f));
	_blue->setAlpha(0.6);

	_cyan = iMaterialResourceFactory::getInstance().createTargetMaterial();
	_cyan->setEmissive(iaColor3f(0.0f, 0.8f, 0.8f));
	_cyan->setSpecular(iaColor3f(0.0f, 0.2f, 0.2f));
	_cyan->setDiffuse(iaColor3f(0.0f, 0.5f, 0.5f));
	_cyan->setAmbient(iaColor3f(0.0f, 0.3f, 0.3f));
	_cyan->setAlpha(0.6);

	shared_ptr<iMesh> translateMesh = createTranslateMesh();
	shared_ptr<iMesh> scaleMesh = createScaleMesh();
	shared_ptr<iMesh> ringMesh = createRingMesh();
	shared_ptr<iMesh> ringMesh2D = create2DRingMesh();

	_rootTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_rootTransform->setActive(_visible);

	_switchNode = static_cast<iNodeSwitch*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSwitch));
	_rootTransform->insertNode(_switchNode);

	createTranslateModifier(translateMesh);
	createScaleModifier(scaleMesh);
	createRotateModifier(ringMesh, ringMesh2D);

	_scene->getRoot()->insertNode(_rootTransform);

	_materialCelShading = iMaterialResourceFactory::getInstance().createMaterial("ManipulatorCelShading");
	iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
	iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->addShaderSource("ModelViewer/yellow.frag", iShaderObjectType::Fragment);
	iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->compileShader();
	iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->getRenderStateSet().setRenderState(iRenderState::Wireframe, iRenderStateValue::On);
	iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->getRenderStateSet().setRenderState(iRenderState::CullFace, iRenderStateValue::On);
	iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading)->getRenderStateSet().setRenderState(iRenderState::CullFaceFunc, iRenderStateValue::Front);

	setManipulatorMode(_manipulatorMode);
}

void Manipulator::highlightSelected()
{
	if (_selectedManipulatorNodeID != iNode::INVALID_NODE_ID)
	{
		iNodePtr node = iNodeFactory::getInstance().getNode(_selectedManipulatorNodeID);

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
				shared_ptr<iMeshBuffers> buffers = meshNode->getMeshBuffers();
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

void Manipulator::createRotateModifier(shared_ptr<iMesh>& ringMesh, shared_ptr<iMesh>& ringMesh2D)
{
	_roateModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
	_switchNode->insertNode(_roateModifier);

	iNodeTransform* xRingTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	xRingTransform->rotate(-M_PI * 0.5, iaAxis::Z);
	xRingTransform->scale(2.0, 0.1, 2.0);
	_roateModifier->insertNode(xRingTransform);

	iNodeTransform* yRingTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	yRingTransform->scale(1.99, 0.1, 1.99);
	_roateModifier->insertNode(yRingTransform);

	iNodeTransform* zRingTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	zRingTransform->rotate(M_PI * 0.5, iaAxis::X);
	zRingTransform->scale(1.98, 0.1, 1.98);
	_roateModifier->insertNode(zRingTransform);

	iNodeMesh* xRing = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	xRing->setMesh(ringMesh);
	xRing->setMaterial(_material);
	xRing->setTargetMaterial(_red);
	xRingTransform->insertNode(xRing);

	iNodeMesh* yRing = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	yRing->setMesh(ringMesh);
	yRing->setMaterial(_material);
	yRing->setTargetMaterial(_green);
	yRingTransform->insertNode(yRing);

	iNodeMesh* zRing = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	zRing->setMesh(ringMesh);
	zRing->setMaterial(_material);
	zRing->setTargetMaterial(_blue);
	zRingTransform->insertNode(zRing);

	_rotateBillboardTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_roateModifier->insertNode(_rotateBillboardTransform);

	iNodeMesh* ring = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	ring->setMesh(ringMesh2D);
	ring->setMaterial(_material);
	ring->setTargetMaterial(_cyan);
	_rotateBillboardTransform->insertNode(ring);

	_rotateIDs.push_back(xRing->getID());
	_rotateIDs.push_back(yRing->getID());
	_rotateIDs.push_back(zRing->getID());
}

void Manipulator::createTranslateModifier(shared_ptr<iMesh>& translateMesh)
{
	_translateModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
	_switchNode->insertNode(_translateModifier);

	iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
	_translateModifier->insertNode(xTransform);

	iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_translateModifier->insertNode(yTransform);

	iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	zTransform->rotate(M_PI * 0.5, iaAxis::X);
	_translateModifier->insertNode(zTransform);

	iNodeMesh* xUmbrella = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	xUmbrella->setMesh(translateMesh);
	xUmbrella->setMaterial(_material);
	xUmbrella->setTargetMaterial(_red);
	xTransform->insertNode(xUmbrella);

	iNodeMesh* yUmbrella = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	yUmbrella->setMesh(translateMesh);
	yUmbrella->setMaterial(_material);
	yUmbrella->setTargetMaterial(_green);
	yTransform->insertNode(yUmbrella);

	iNodeMesh* zUmbrella = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	zUmbrella->setMesh(translateMesh);
	zUmbrella->setMaterial(_material);
	zUmbrella->setTargetMaterial(_blue);
	zTransform->insertNode(zUmbrella);

	_translateIDs.push_back(xUmbrella->getID());
	_translateIDs.push_back(yUmbrella->getID());
	_translateIDs.push_back(zUmbrella->getID());
}

void Manipulator::createScaleModifier(shared_ptr<iMesh>& scaleMesh)
{
	_scaleModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
	_switchNode->insertNode(_scaleModifier);

	iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
	_scaleModifier->insertNode(xTransform);

	iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_scaleModifier->insertNode(yTransform);

	iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	zTransform->rotate(M_PI * 0.5, iaAxis::X);
	_scaleModifier->insertNode(zTransform);

	iNodeMesh* xCube = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	xCube->setMesh(scaleMesh);
	xCube->setMaterial(_material);
	xCube->setTargetMaterial(_red);
	xTransform->insertNode(xCube);

	iNodeMesh* yCube = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	yCube->setMesh(scaleMesh);
	yCube->setMaterial(_material);
	yCube->setTargetMaterial(_green);
	yTransform->insertNode(yCube);

	iNodeMesh* zCube = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	zCube->setMesh(scaleMesh);
	zCube->setMaterial(_material);
	zCube->setTargetMaterial(_blue);
	zTransform->insertNode(zCube);

	_scaleIDs.push_back(xCube->getID());
	_scaleIDs.push_back(yCube->getID());
	_scaleIDs.push_back(zCube->getID());
}

void Manipulator::update()
{
	iaMatrixd matrix;

	iNodePtr node = iNodeFactory::getInstance().getNode(_selectedNodeID);
	if (node != nullptr)
	{
		node->calcWorldTransformation(matrix);
	}
	else
	{
		setVisible(false);
		return;
	}

	iaMatrixd camMatrix;
	_cameraUI->calcWorldTransformation(camMatrix);

	float64 distanceToCam = camMatrix._pos.distance(matrix._pos) * 0.1;

	matrix._right.normalize();
	matrix._top.normalize();
	matrix._depth.normalize();
	_rootTransform->setMatrix(matrix);
	_rootTransform->scale(distanceToCam, distanceToCam, distanceToCam);

	// compensate for parent orientation
	iaMatrixd parentMatrix;
	_rotateBillboardTransform->getParent()->calcWorldTransformation(parentMatrix);
	parentMatrix._pos.set(0,0,0);
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
	iMaterialResourceFactory::getInstance().destroyTargetMaterial(_red);
	iMaterialResourceFactory::getInstance().destroyTargetMaterial(_green);
	iMaterialResourceFactory::getInstance().destroyTargetMaterial(_blue);
	iMaterialResourceFactory::getInstance().destroyTargetMaterial(_cyan);
	iMaterialResourceFactory::getInstance().destroyMaterial(_materialCelShading);
	iMaterialResourceFactory::getInstance().destroyMaterial(_material);
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

shared_ptr<iMesh> Manipulator::createRingMesh()
{
	iMeshBuilder meshBuilder;
	iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 64, false);
	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}

shared_ptr<iMesh> Manipulator::create2DRingMesh()
{
	iMeshBuilder meshBuilder;
	iMeshBuilderUtils::addRing(meshBuilder, 0.99, 1, 64);
	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}

shared_ptr<iMesh> Manipulator::createScaleMesh()
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

shared_ptr<iMesh> Manipulator::createTranslateMesh()
{
	iMeshBuilder meshBuilder;
	meshBuilder.setJoinVertexes(false);

	iaMatrixf matrix;

	matrix.translate(0, 1.5, 0);
	matrix.scale(0.1, 0.5, 0.1);
	meshBuilder.setMatrix(matrix);
	iMeshBuilderUtils::addCone(meshBuilder, 1, 1, 6);

	matrix.identity();
	matrix.scale(0.02, 1.5, 0.02);
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
		_switchNode->setActive(false);
		break;

	case ManipulatorMode::Translate:
		_switchNode->setActive(true);
		_switchNode->setActiveChild(_translateModifier);
		break;

	case ManipulatorMode::Scale:
		_switchNode->setActive(true);
		_switchNode->setActiveChild(_scaleModifier);
		break;

	case ManipulatorMode::Rotate:
		_switchNode->setActive(true);
		_switchNode->setActiveChild(_roateModifier);
		break;
	}
}

ManipulatorMode Manipulator::getManipulatorMode() const
{
	return _manipulatorMode;
}

void Manipulator::setCamCOI(const iaMatrixd& matrix)
{
	_cameraCOIUI->setMatrix(matrix);
}

void Manipulator::setCamHeading(const iaMatrixd& matrix)
{
	_cameraHeadingUI->setMatrix(matrix);
}

void Manipulator::setCamPitch(const iaMatrixd& matrix)
{
	_cameraPitchUI->setMatrix(matrix);
}

void Manipulator::setCamTranslate(const iaMatrixd& matrix)
{
	_cameraTranslationUI->setMatrix(matrix);
}

void Manipulator::setNodeID(uint64 nodeID)
{
	iNodePtr node = iNodeFactory::getInstance().getNode(nodeID);
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

void Manipulator::scale(const iaVector3d& vec, iaMatrixd& matrix)
{
	const iaVector3d dir[] = { { 1,0,0 },{ 0,1,0 },{ 0,0,1 } };
	iaVector3d scale;

	for (int i = 0; i < 3; ++i)
	{
		if (_selectedManipulatorNodeID == _scaleIDs[i])
		{
			scale = vec.project(dir[i]) + iaVector3d(1, 1, 1);
			matrix.scale(scale);
			return;
		}
	}
}

void Manipulator::translate(const iaVector3d& vec, iaMatrixd& matrix)
{
	const iaVector3d dir[] = { { 1,0,0 },{ 0,1,0 },{ 0,0,1 } };
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

void Manipulator::rotate(const iaVector2d& from, const iaVector2d& to, iaMatrixd& matrix)
{
	iNode* node = iNodeFactory::getInstance().getNode(_selectedNodeID);
	iaMatrixd transformWorldMatrix;
	node->calcWorldTransformation(transformWorldMatrix);

	iaMatrixd camWorldMatrix;
	_cameraUI->calcWorldTransformation(camWorldMatrix);
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

void Manipulator::onMouseMoved(const iaVector2i& from, const iaVector2i& to, iWindow* window)
{
	if (_selectedManipulatorNodeID != iNode::INVALID_NODE_ID)
	{
		iaVector2d fromd;
		iaVector2d tod;

		iaConvert::convert(from, fromd);
		iaConvert::convert(to, tod);

		iNodePtr node = iNodeFactory::getInstance().getNode(_selectedNodeID);
		if (node != nullptr &&
			node->getType() == iNodeType::iNodeTransform)
		{
			iaMatrixd camWorldMatrix;
			_cameraUI->calcWorldTransformation(camWorldMatrix);
			iaVector3d fromWorld = camWorldMatrix * _view->unProject(iaVector3d(from._x, from._y, 0), camWorldMatrix);
			iaVector3d toWorld = camWorldMatrix * _view->unProject(iaVector3d(to._x, to._y, 0), camWorldMatrix);

			iNodeTransform* transformNode = static_cast<iNodeTransform*>(node);
			iaMatrixd transformWorldMatrix;
			transformNode->calcWorldTransformation(transformWorldMatrix);
			transformWorldMatrix.invert();
			fromWorld = transformWorldMatrix * fromWorld;
			toWorld = transformWorldMatrix * toWorld;

			iaMatrixd camTranslateMatrix;
			_cameraTranslationUI->getMatrix(camTranslateMatrix);
			float64 translateFactor = camTranslateMatrix._pos.length();

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
				scale((toWorld - fromWorld) * translateFactor, nodeMatrix);
				break;
			case ManipulatorMode::Translate:
				translate((toWorld - fromWorld) * translateFactor, nodeMatrix);
				break;
			}

			transformNode->setMatrix(nodeMatrix);
		}
	}
}

void Manipulator::onMouseWheel(int32 d)
{

}

bool Manipulator::isSelected() const
{
	return (_selectedManipulatorNodeID != iNode::INVALID_NODE_ID) ? true : false;
}

void Manipulator::onMouseKeyDown(iKeyCode key)
{
	_selectedManipulatorNodeID = _view->pickcolorID(iMouse::getInstance().getPos()._x, iMouse::getInstance().getPos()._y);
}

void Manipulator::onMouseKeyUp(iKeyCode key)
{
	_selectedManipulatorNodeID = iNode::INVALID_NODE_ID;
}
