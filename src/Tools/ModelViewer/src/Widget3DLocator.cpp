// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include "Widget3DLocator.h"

#include <iMeshBuilder.h>
#include <iMeshBuilderUtils.h>
#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iNodeMesh.h>
#include <iMaterialResourceFactory.h>
#include <iRenderer.h>
#include <iView.h>
#include <iTargetMaterial.h>
#include <iScene.h>
using namespace Igor;

Widget3DLocator::Widget3DLocator(iWindow* window, iView* view, iScene* scene)
	: Widget3D(window, view, scene)
{
	_view->registerRenderDelegate(RenderDelegate(this, &Widget3DLocator::update));

	_red = iMaterialResourceFactory::getInstance().createTargetMaterial();
	_red->setEmissive(iaColor3f(0.6f, 0.0f, 0.0f));
	_red->setSpecular(iaColor3f(0.2f, 0.0f, 0.0f));
	_red->setDiffuse(iaColor3f(0.5f, 0.0f, 0.0f));
	_red->setAmbient(iaColor3f(0.3f, 0.0f, 0.0f));

	_green = iMaterialResourceFactory::getInstance().createTargetMaterial();
	_green->setEmissive(iaColor3f(0.0f, 0.6f, 0.0f));
	_green->setSpecular(iaColor3f(0.0f, 0.2f, 0.0f));
	_green->setDiffuse(iaColor3f(0.0f, 0.5f, 0.0f));
	_green->setAmbient(iaColor3f(0.0f, 0.3f, 0.0f));

	_blue = iMaterialResourceFactory::getInstance().createTargetMaterial();
	_blue->setEmissive(iaColor3f(0.0f, 0.0f, 0.6f));
	_blue->setSpecular(iaColor3f(0.0f, 0.0f, 0.2f));
	_blue->setDiffuse(iaColor3f(0.0f, 0.0f, 0.5f));
	_blue->setAmbient(iaColor3f(0.0f, 0.0f, 0.3f));

	_material = iMaterialResourceFactory::getInstance().createMaterial("Manipulator");
	iMaterialResourceFactory::getInstance().getMaterial(_material)->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
	iMaterialResourceFactory::getInstance().getMaterial(_material)->addShaderSource("igor/default_directional_light.frag", iShaderObjectType::Fragment);
	iMaterialResourceFactory::getInstance().getMaterial(_material)->compileShader();
	iMaterialResourceFactory::getInstance().getMaterial(_material)->setOrder(iMaterial::RENDER_ORDER_MAX);

	createLocator();
}

Widget3DLocator::~Widget3DLocator()
{
	_view->unregisterRenderDelegate(RenderDelegate(this, &Widget3DLocator::update));

	iNodeFactory::getInstance().destroyNodeAsync(_rootTransform);

	iMaterialResourceFactory::getInstance().destroyTargetMaterial(_red);
	iMaterialResourceFactory::getInstance().destroyTargetMaterial(_green);
	iMaterialResourceFactory::getInstance().destroyTargetMaterial(_blue);
}

void Widget3DLocator::update()
{
	iNodePtr node = iNodeFactory::getInstance().getNode(_nodeID);
	if (node != nullptr)
	{
		iaMatrixd matrix;
		node->calcWorldTransformation(matrix);
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
	_rootTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_scene->getRoot()->insertNode(_rootTransform);
	_rootTransform->setActive(false);

	shared_ptr<iMesh> locatorMesh = createLocatorMesh();

	iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
	_rootTransform->insertNode(xTransform);

	iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	_rootTransform->insertNode(yTransform);

	iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
	zTransform->rotate(M_PI * 0.5, iaAxis::X);
	_rootTransform->insertNode(zTransform);

	iNodeMesh* xCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	xCylinder->setMesh(locatorMesh);
	xCylinder->setMaterial(_material);
	xCylinder->setTargetMaterial(_red);
	xTransform->insertNode(xCylinder);

	iNodeMesh* yCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	yCylinder->setMesh(locatorMesh);
	yCylinder->setMaterial(_material);
	yCylinder->setTargetMaterial(_green);
	yTransform->insertNode(yCylinder);

	iNodeMesh* zCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
	zCylinder->setMesh(locatorMesh);
	zCylinder->setMaterial(_material);
	zCylinder->setTargetMaterial(_blue);
	zTransform->insertNode(zCylinder);
}

shared_ptr<iMesh> Widget3DLocator::createLocatorMesh()
{
	iMeshBuilder meshBuilder;

	iaMatrixf matrix;
	matrix.scale(0.02, 2.0, 0.02);
	meshBuilder.setMatrix(matrix);
	iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 6);
	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}