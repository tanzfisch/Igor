// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeMesh.h>

#include <igor/renderer/iRenderer.h>
#include <igor/resources/shader/iShader.h>
#include <igor/resources/mesh/iMesh.h>

#include <vector>

namespace igor
{

    iNodeMesh::iNodeMesh()
        : iNodeVolume()
    {
        setName(L"iNodeMesh");
        _nodeType = iNodeType::iNodeMesh;
    }

    iNodeMesh::iNodeMesh(iNodeMesh *node)
        : iNodeVolume(node)
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        _name = node->_name;
        _mesh = node->_mesh;

        if (_mesh != nullptr)
        {
            setBoundingBox(_mesh->getBoundingBox());
        }
    }

    iNodeMesh::~iNodeMesh()
    {
    }

    std::vector<iaString> iNodeMesh::getInfo(bool brief) const
    {
        std::vector<iaString> info = iNode::getInfo(brief);

        if(brief)
        {
            return info;
        }

        iaString topologyInfo(L"vtx:");

        uint32 vertexCount = 0;
        uint32 trianglesCount = 0;
        uint32 indexesCount = 0;

        if (_mesh != nullptr)
        {
            vertexCount = _mesh->getVertexCount();
            trianglesCount = _mesh->getTrianglesCount();
            indexesCount = _mesh->getIndexCount();
        }

        topologyInfo += iaString::toString(vertexCount);
        topologyInfo += L" tri:";
        topologyInfo += iaString::toString(trianglesCount);
        topologyInfo += L" idx:";
        topologyInfo += iaString::toString(indexesCount);

        if (_mesh != nullptr)
        {
            topologyInfo += L" texUni:";
            topologyInfo += iaString::toString(_mesh->getTextureUnitCount());
            topologyInfo += L" texCrd:";
            topologyInfo += iaString::toString(_mesh->getTextureCoordinatesCount());
        }

        info.push_back(topologyInfo);

        iaString materialInfo("material:");
        iMaterialPtr material = getMaterial();
        materialInfo += ((material == nullptr) ? iaString("none") : material->getInfo());

        info.push_back(materialInfo);

        return info;
    }

    iMeshPtr iNodeMesh::getMesh()
    {
        return _mesh;
    }

    void iNodeMesh::draw()
    {
        if(!_mesh->isValid())
        {
            return;
        }
        iRenderer::getInstance().setModelMatrix(_worldMatrix);
        iRenderer::getInstance().drawMesh(_mesh, getMaterial());
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
        return getMaterial()->getAmbient();
    }

    iaColor3f iNodeMesh::getEmissive() const
    {
        return getMaterial()->getEmissive();
    }

    iaColor3f iNodeMesh::getSpecular() const
    {
        return getMaterial()->getSpecular();
    }

    iaColor3f iNodeMesh::getDiffuse() const
    {
        return getMaterial()->getDiffuse();
    }

    float32 iNodeMesh::getShininess() const
    {
        return getMaterial()->getShininess();
    }

} // namespace igor