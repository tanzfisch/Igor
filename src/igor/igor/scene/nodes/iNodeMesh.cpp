// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeMesh.h>

#include <igor/renderer/iRenderer.h>
#include <igor/resources/model/iModel.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/mesh/iMeshBuffers.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/material/iTargetMaterial.h>

#include <vector>

namespace igor
{

    iNodeMesh::iNodeMesh()
        : iNodeVolume()
    {
        setName(L"iNodeMesh");
        _nodeType = iNodeType::iNodeMesh;

        _targetMaterial = iMaterialResourceFactory::getInstance().createTargetMaterial();
    }

    iNodeMesh::iNodeMesh(iNodeMesh *node)
        : iNodeVolume(node)
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        _name = node->_name;
        _keepMeshData = node->_keepMeshData;
        _mesh = node->_mesh;

        if (_mesh != nullptr)
        {
            setBoundingBox(_mesh->getBoundingBox());
        }

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

    void iNodeMesh::getInfo(std::vector<iaString> &info) const
    {
        iNode::getInfo(info);

        iaString customInfo(L"vtx:");

        uint32 vertexCount = 0;
        uint32 trianglesCount = 0;
        uint32 indexesCount = 0;

        if (_mesh != nullptr)
        {
            vertexCount = _mesh->getVertexCount();
            trianglesCount = _mesh->getTrianglesCount();
            indexesCount = _mesh->getIndexCount();
        }

        customInfo += iaString::toString(vertexCount);
        customInfo += L" tri:";
        customInfo += iaString::toString(trianglesCount);
        customInfo += L" idx:";
        customInfo += iaString::toString(indexesCount);

        if (_mesh != nullptr)
        {
            customInfo += L" texUni:";
            customInfo += iaString::toString(_mesh->getTextureUnitCount());
            customInfo += L" texCrd:";
            customInfo += iaString::toString(_mesh->getTextureCoordinatesCount());
        }

        info.push_back(customInfo);
    }

    void iNodeMesh::setTargetMaterial(const iTargetMaterial *const targetMaterial)
    {
        *_targetMaterial = *targetMaterial;
    }

    iTargetMaterial *iNodeMesh::getTargetMaterial()
    {
        return _targetMaterial;
    }

    iMeshPtr iNodeMesh::getMesh()
    {
        return _mesh;
    }

    void iNodeMesh::setKeepMeshData(bool keepMesh)
    {
        _keepMeshData = keepMesh;
    }

    bool iNodeMesh::getKeepMeshData() const
    {
        return _keepMeshData;
    }

    void iNodeMesh::draw()
    {
        iRenderer::getInstance().setModelMatrix(_worldMatrix);
        iRenderer::getInstance().drawMesh(_mesh, _targetMaterial);
    }

    void iNodeMesh::setMesh(iMeshPtr mesh)
    {
        _mesh = mesh;

        if (_mesh != nullptr)
        {
            setBoundingBox(_mesh->getBoundingBox());
        }

        setTransformationDirty(); // TODO why do we need this?
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

} // namespace igor