// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Widget3DLocator.h"

Widget3DLocator::Widget3DLocator(iWindowPtr window, iView *view, iScenePtr scene)
	: Widget3D(window, view, scene)
{
	_view->registerRenderDelegate(iDrawDelegate(this, &Widget3DLocator::update));

	_red = iMaterialResourceFactory_old::getInstance().createTargetMaterial();
	_red->setEmissive(iaColor3f(0.6f, 0.0f, 0.0f));
	_red->setSpecular(iaColor3f(0.2f, 0.0f, 0.0f));
	_red->setDiffuse(iaColor3f(0.5f, 0.0f, 0.0f));
	_red->setAmbient(iaColor3f(0.3f, 0.0f, 0.0f));
	_red->setAlpha(0.8);

	_green = iMaterialResourceFactory_old::getInstance().createTargetMaterial();
	_green->setEmissive(iaColor3f(0.0f, 0.6f, 0.0f));
	_green->setSpecular(iaColor3f(0.0f, 0.2f, 0.0f));
	_green->setDiffuse(iaColor3f(0.0f, 0.5f, 0.0f));
	_green->setAmbient(iaColor3f(0.0f, 0.3f, 0.0f));
	_green->setAlpha(0.8);

	_blue = iMaterialResourceFactory_old::getInstance().createTargetMaterial();
	_blue->setEmissive(iaColor3f(0.0f, 0.0f, 0.6f));
	_blue->setSpecular(iaColor3f(0.0f, 0.0f, 0.2f));
	_blue->setDiffuse(iaColor3f(0.0f, 0.0f, 0.5f));
	_blue->setAmbient(iaColor3f(0.0f, 0.0f, 0.3f));
	_blue->setAlpha(0.8);

	_materialID = iMaterialResourceFactory_old::getInstance().createMaterial("Locator");
	auto material = iMaterialResourceFactory_old::getInstance().getMaterial(_materialID);
	material->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
	material->addShaderSource("igor/default_directional_light.frag", iShaderObjectType::Fragment);
	material->setRenderState(iRenderState::Blend, iRenderStateValue::On);
	material->compileShader();
	material->setOrder(iMaterial::RENDER_ORDER_MAX);

	createLocator();
}

Widget3DLocator::~Widget3DLocator()
{
	_view->unregisterRenderDelegate(iDrawDelegate(this, &Widget3DLocator::update));

	iNodeManager::getInstance().destroyNodeAsync(_rootTransform);

	iMaterialResourceFactory_old::getInstance().destroyTargetMaterial(_red);
	iMaterialResourceFactory_old::getInstance().destroyTargetMaterial(_green);
	iMaterialResourceFactory_old::getInstance().destroyTargetMaterial(_blue);
}

void Widget3DLocator::update()
{
	iNodePtr node = iNodeManager::getInstance().getNode(_nodeID);
	if (node != nullptr)
	{
		iaMatrixd matrix;
		node->calcWorldTransformation(matrix);
		matrix._right.normalize();
		matrix._top.normalize();
		matrix._depth.normalize();
		_rootTransform->setMatrix(matrix);
		_rootTransform->setActive();
	}
	else
	{
		_rootTransform->setActive(false);
	}
}

void Widget3DLocator::createLocator()
{
	_rootTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	_scene->getRoot()->insertNode(_rootTransform);
	_rootTransform->setActive(false);

	iMeshPtr locatorMesh = createLocatorMesh();

	iNodeTransform *xTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
	_rootTransform->insertNode(xTransform);

	iNodeTransform *yTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	_rootTransform->insertNode(yTransform);

	iNodeTransform *zTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	zTransform->rotate(M_PI * 0.5, iaAxis::X);
	_rootTransform->insertNode(zTransform);

	iNodeMesh *xCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
	xCylinder->setMesh(locatorMesh);
	xCylinder->setMaterial(_materialID);
	xCylinder->setTargetMaterial(_red);
	xTransform->insertNode(xCylinder);

	iNodeMesh *yCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
	yCylinder->setMesh(locatorMesh);
	yCylinder->setMaterial(_materialID);
	yCylinder->setTargetMaterial(_green);
	yTransform->insertNode(yCylinder);

	iNodeMesh *zCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
	zCylinder->setMesh(locatorMesh);
	zCylinder->setMaterial(_materialID);
	zCylinder->setTargetMaterial(_blue);
	zTransform->insertNode(zCylinder);
}

iMeshPtr Widget3DLocator::createLocatorMesh()
{
	iMeshBuilder meshBuilder;

	iaMatrixf matrix;
	matrix.translate(0.0, -2.0, 0.0);
	matrix.scale(0.025, 4.0, 0.025);
	meshBuilder.setMatrix(matrix);
	iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 4);
	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}