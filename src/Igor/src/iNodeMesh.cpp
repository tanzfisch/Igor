// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeMesh.h>

#include <iModel.h>
#include <iModelResourceFactory.h>
#include <iRenderer.h>
#include <iMaterial.h>
#include <iMesh.h>
#include <iMeshBuffers.h>
#include <iMaterialResourceFactory.h>
#include <iTargetMaterial.h>

#include <vector>


namespace Igor
{

	iNodeMesh::iNodeMesh()
		: iNodeVolume()
	{
		setName(L"iNodeMesh");
		_nodeType = iNodeType::iNodeMesh;

		_targetMaterial = iMaterialResourceFactory::getInstance().createTargetMaterial();
	}

	iNodeMesh::iNodeMesh(iNodeMesh* node)
		: iNodeVolume(node)
	{
		con_assert(node != nullptr, "zero pointer");

		_nodeType = node->_nodeType;
		_nodeKind = node->_nodeKind;

		_name = node->_name;
		_keepMesh = node->_keepMesh;
		_meshBuffers = node->_meshBuffers;
		_mesh = node->_mesh;

		setBoundingSphere(node->getBoundingSphere());

		_targetMaterial = iMaterialResourceFactory::getInstance().createTargetMaterial();
		setTargetMaterial(node->getTargetMaterial());
	}

	iNodeMesh::~iNodeMesh()
	{
		if (_targetMaterial != nullptr)
		{
			iMaterialResourceFactory::getInstance().destroyTargetMaterial(_targetMaterial);
			_targetMaterial = nullptr;
		}
	}

	void iNodeMesh::getInfo(std::vector<iaString>& info) const
	{
		iNode::getInfo(info);

		iaString customInfo(L"vtx:");

		customInfo += iaString::toString(getVertexCount());
		customInfo += L" tri:";
		customInfo += iaString::toString(getTrianglesCount());
		customInfo += L" idx:";
		customInfo += iaString::toString(getIndexesCount());

		if (_mesh != nullptr)
		{
			customInfo += L" texUni:";
			customInfo += iaString::toString(_mesh->getTextureUnitCount());
			customInfo += L" texCrd:";
			customInfo += iaString::toString(_mesh->getTextureCoordinatesCount());
		}

		info.push_back(customInfo);
	}

	void iNodeMesh::setTargetMaterial(const iTargetMaterial* const targetMaterial)
	{
		_targetMaterial->setAmbient(targetMaterial->getAmbient());
		_targetMaterial->setDiffuse(targetMaterial->getDiffuse());
		_targetMaterial->setSpecular(targetMaterial->getSpecular());
		_targetMaterial->setShininess(targetMaterial->getShininess());
		_targetMaterial->setEmissive(targetMaterial->getEmissive());
		_targetMaterial->setAlpha(targetMaterial->getAlpha());

		for (int i = 0; i < 4; ++i)
		{
			if (targetMaterial->hasTextureUnit(i))
			{
				_targetMaterial->setTexture(targetMaterial->getTexture(i), i);
			}
		}
	}

	iTargetMaterial* iNodeMesh::getTargetMaterial()
	{
		return _targetMaterial;
	}

	std::shared_ptr<iMeshBuffers> iNodeMesh::getMeshBuffers()
	{
		return _meshBuffers;
	}

	iMeshPtr iNodeMesh::getMesh()
	{
		return _mesh;
	}

	void iNodeMesh::setKeepMesh(bool keepMesh)
	{
		_keepMesh = keepMesh;
	}

	bool iNodeMesh::getKeepMesh() const
	{
		return _keepMesh;
	}

	void iNodeMesh::draw()
	{
		if (_meshBuffers != nullptr &&
			_meshBuffers->isReady())
		{
			iRenderer::getInstance().setModelMatrix(_worldMatrix);
			iRenderer::getInstance().setTargetMaterial(_targetMaterial);
			iRenderer::getInstance().drawMesh(_meshBuffers);
		}
	}

	void iNodeMesh::setMeshBuffers(std::shared_ptr<iMeshBuffers> meshBuffers)
	{
		_meshBuffers = meshBuffers;

		if (_meshBuffers != nullptr)
		{
			setTransformationDirty();

			con_assert(_keepMesh == false, "inconsitant data");
			_mesh = nullptr;
		}
	}

	void iNodeMesh::setMesh(iMeshPtr mesh)
	{
		_mesh = mesh;

		if (_mesh != nullptr)
		{
			_meshBuffers = iRenderer::getInstance().createBuffersAsync(_mesh);
			setBoundingSphere(_mesh->getBoundingSphere());
			setBoundingBox(_mesh->getBoundingBox());

			if (!_keepMesh)
			{
				_mesh = nullptr;
			}
		}

		setTransformationDirty();
	}

	iaColor3f iNodeMesh::getAmbient() const
	{
		return _targetMaterial->getAmbient();
	}

	iaColor3f iNodeMesh::getEmissive() const
	{
		return _targetMaterial->getEmissive();
	}

	iaColor3f iNodeMesh::getSpecular() const
	{
		return _targetMaterial->getSpecular();
	}

	iaColor3f iNodeMesh::getDiffuse() const
	{
		return _targetMaterial->getDiffuse();
	}

	float32 iNodeMesh::getShininess() const
	{
		return _targetMaterial->getShininess();
	}

	uint32 iNodeMesh::getVertexCount() const
	{
		if (nullptr != _meshBuffers)
		{
			return _meshBuffers->getVertexCount();
		}

		return 0;
	}

	uint32 iNodeMesh::getTrianglesCount() const
	{
		if (nullptr != _meshBuffers)
		{
			return _meshBuffers->getTrianglesCount();
		}

		return 0;
	}

	uint32 iNodeMesh::getIndexesCount() const
	{
		if (nullptr != _meshBuffers)
		{
			return _meshBuffers->getIndexesCount();
		}

		return 0;
	}

}