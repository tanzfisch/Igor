// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include "Widget3DEmitter.h"

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

Widget3DEmitter::Widget3DEmitter(iWindow* window, iView* view, iScene* scene)
	: Widget3D(window, view, scene)
{
	_view->registerRenderDelegate(RenderDelegate(this, &Widget3DEmitter::update));

	_targetMaterial = iMaterialResourceFactory::getInstance().createTargetMaterial();
	_targetMaterial->setEmissive(iaColor3f(0.0f, 0.0f, 0.0f));
	_targetMaterial->setSpecular(iaColor3f(0.0f, 0.5f, 0.0f));
	_targetMaterial->setDiffuse(iaColor3f(0.0f, 0.5f, 0.0f));
	_targetMaterial->setAmbient(iaColor3f(0.0f, 0.5f, 0.0f));
	_targetMaterial->setAlpha(0.5);

	_material = iMaterialResourceFactory::getInstance().createMaterial("EmitterFlat");
	auto material = iMaterialResourceFactory::getInstance().getMaterial(_material);
	material->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
	material->getRenderStateSet().setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
	material->getRenderStateSet().setRenderState(iRenderState::CullFace, iRenderStateValue::Off);
	material->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
	material->addShaderSource("igor/default_directional_light.frag", iShaderObjectType::Fragment);
	material->compileShader();
	material->setOrder(iMaterial::RENDER_ORDER_MAX);

	_materialVolume = iMaterialResourceFactory::getInstance().createMaterial("EmitterVolume");
	auto materialVolume = iMaterialResourceFactory::getInstance().getMaterial(_materialVolume);
	materialVolume->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
	materialVolume->getRenderStateSet().setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
	materialVolume->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
	materialVolume->addShaderSource("igor/default_directional_light.frag", iShaderObjectType::Fragment);
	materialVolume->compileShader();
	materialVolume->setOrder(iMaterial::RENDER_ORDER_MAX);
}

Widget3DEmitter::~Widget3DEmitter()
{
	_view->unregisterRenderDelegate(RenderDelegate(this, &Widget3DEmitter::update));

	if (_rootTransform != nullptr)
	{
		iNodeFactory::getInstance().destroyNodeAsync(_rootTransform);
	}

	iMaterialResourceFactory::getInstance().destroyTargetMaterial(_targetMaterial);
	iMaterialResourceFactory::getInstance().destroyMaterial(_material);
	iMaterialResourceFactory::getInstance().destroyMaterial(_materialVolume);
}

void Widget3DEmitter::clearMeshNode()
{
	if (_meshNode != nullptr)
	{
		iNodeFactory::getInstance().destroyNodeAsync(_meshNode);
	}

	_meshNode = nullptr;
}

void Widget3DEmitter::update()
{
	iNodePtr node = iNodeFactory::getInstance().getNode(_nodeID);
	if (node == nullptr)
	{
		clearMeshNode();
		return;
	}

	if (node->getType() != iNodeType::iNodeEmitter)
	{
		clearMeshNode();
		return;
	}

	// create structure
	if (_rootTransform == nullptr)
	{
		_rootTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
		_scene->getRoot()->insertNode(_rootTransform);

		_scaleTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
		_rootTransform->insertNode(_scaleTransform);
	}

	iNodeEmitter* emitter = static_cast<iNodeEmitter*>(node);
	iEmitterType emitterType = emitter->getEmitterType();

	// change emitter type
	if (_emitterType != emitterType)
	{
		_emitterType = emitterType;
		clearMeshNode();

		// create mesh
		shared_ptr<iMesh> mesh;
		uint64 material = _material;
		switch (_emitterType)
		{
		case iEmitterType::Mesh:
			// not supported yet
			break;

		case iEmitterType::Point:
			// show nothing
			break;

		case iEmitterType::Disc:
			mesh = createDisc();
			break;

		case iEmitterType::Circle:
			mesh = createCircle();
			break;

		case iEmitterType::Sphere:
			mesh = createSphere();
			material = _materialVolume;
			break;

		case iEmitterType::Square:
			mesh = createSquare();
			break;

		case iEmitterType::Cube:
			mesh = createCube();
			material = _materialVolume;
			break;
		}

		if (mesh != nullptr)
		{
			_meshNode = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
			_meshNode->setMesh(mesh);
			_meshNode->setMaterial(material);
			_meshNode->setTargetMaterial(_targetMaterial);
			_scaleTransform->insertNode(_meshNode);
		}
	}

	// update size
	_scaleTransform->identity();
	_scaleTransform->scale(emitter->getSize(), emitter->getSize(), emitter->getSize());

	// update matrix
	iaMatrixd matrix;
	node->calcWorldTransformation(matrix);
	_rootTransform->setMatrix(matrix);
}

shared_ptr<iMesh> Widget3DEmitter::createDisc()
{
	iMeshBuilder meshBuilder;

	iMeshBuilderUtils::addCircle(meshBuilder, 1, 32);
	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}

shared_ptr<iMesh> Widget3DEmitter::createCircle()
{
	iMeshBuilder meshBuilder;

	iMeshBuilderUtils::addRing(meshBuilder, 0.95, 1, 32);
	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}

shared_ptr<iMesh> Widget3DEmitter::createCube()
{
	iMeshBuilder meshBuilder;

	iaMatrixf matrix;
	matrix.translate(0, -1, 0);
	meshBuilder.setMatrix(matrix);

	iMeshBuilderUtils::addBox(meshBuilder, 2, 2, 2);
	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}

shared_ptr<iMesh> Widget3DEmitter::createSphere()
{
	iMeshBuilder meshBuilder;

	iMeshBuilderUtils::addSphere(meshBuilder, 1, 32);
	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}

shared_ptr<iMesh> Widget3DEmitter::createSquare()
{
	iMeshBuilder meshBuilder;

	meshBuilder.addVertex(iaVector3f(-1, 0, 1));
	meshBuilder.addVertex(iaVector3f(1, 0, 1));
	meshBuilder.addVertex(iaVector3f(1, 0, -1));
	meshBuilder.addVertex(iaVector3f(-1, 0, -1));
	meshBuilder.addTriangle(0, 1, 2);
	meshBuilder.addTriangle(2, 3, 0);
	meshBuilder.calcNormals(true);
	return meshBuilder.createMesh();
}