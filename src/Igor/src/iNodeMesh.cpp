// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
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
using namespace std;

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

        setName(node->getName());
        setMesh(node->getMesh());
        setMeshBuffers(node->getMeshBuffers());
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

    void iNodeMesh::setTargetMaterial(const iTargetMaterial* const targetMaterial)
    {
        _targetMaterial->setAmbient(targetMaterial->getAmbient());
        _targetMaterial->setDiffuse(targetMaterial->getDiffuse());
        _targetMaterial->setSpecular(targetMaterial->getSpecular());
        _targetMaterial->setShininess(targetMaterial->getShininess());
        _targetMaterial->setEmissive(targetMaterial->getEmissive());
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

    shared_ptr<iMeshBuffers> iNodeMesh::getMeshBuffers()
    {
        return _meshBuffers;
    }

    shared_ptr<iMesh> iNodeMesh::getMesh()
    {
        return _mesh;
    }

    void iNodeMesh::draw()
    {
        if (_meshBuffers == nullptr)
        {
            if (_mesh != nullptr)
            {
                _meshBuffers = iRenderer::getInstance().createBuffers(_mesh);
                _mesh = nullptr;
            }
        }

        if (_meshBuffers != nullptr)
        {
            iRenderer::getInstance().setModelMatrix(_worldMatrix);
            iRenderer::getInstance().setTargetMaterial(_targetMaterial);
            iRenderer::getInstance().drawMesh(_meshBuffers);
        }
    }

    void iNodeMesh::setMeshBuffers(shared_ptr<iMeshBuffers> meshBuffers)
    {
        _meshBuffers = meshBuffers;

        if (_meshBuffers != nullptr)
        {
            setTransformationDirty();
            _mesh = nullptr;
        }
    }

    void iNodeMesh::setMesh(shared_ptr<iMesh> mesh)
    {
        _mesh = mesh;

        if (_mesh != nullptr)
        {
            setBoundingSphere(_mesh->getBoundingSphere());          
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

    uint32 iNodeMesh::getVertexCount()
    {
        if (nullptr != _meshBuffers)
        {
            return _meshBuffers->getVertexCount();
        }

        return 0;
    }

    uint32 iNodeMesh::getTrianglesCount()
    {
        if (nullptr != _meshBuffers)
        {
            return _meshBuffers->getTrianglesCount();
        }

        return 0;
    }

    uint32 iNodeMesh::getIndexesCount()
    {
        if (nullptr != _meshBuffers)
        {
            return _meshBuffers->getIndexesCount();
        }

        return 0;
    }

}