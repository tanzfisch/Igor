// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeSkyBox.h>

#include <iaux/system/iaConsole.h>
#include <igor/renderer/iRenderer.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/mesh/iMeshBuilder.h>
#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeManager.h>

namespace igor
{

    iNodeSkyBox::iNodeSkyBox()
        : iNodeRender()
    {
        setName(L"iNodeSkyBox");
        _nodeType = iNodeType::iNodeSkyBox;
        buildMesh();
    }

    iNodeSkyBox::iNodeSkyBox(iNodeSkyBox *node)
        : iNodeRender()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());

        iaMatrixd matrix;
        node->getOffsetMatrix(matrix);
        setOffsetMatrix(matrix);
        setUseOffsetMatrix(node->isOffsetMatrixUsed());

        _mesh = node->_mesh;
    }

    void iNodeSkyBox::buildMesh()
    {
        iMeshBuilder meshBuilder;
        // manually set triangle indices
        meshBuilder.setJoinVertexes(false);

        // top
        uint32 a = meshBuilder.addVertex(iaVector3f(-0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.3333), 0);
        uint32 b = meshBuilder.addVertex(iaVector3f(0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.3333), 0);
        uint32 c = meshBuilder.addVertex(iaVector3f(0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.0), 0);
        uint32 d = meshBuilder.addVertex(iaVector3f(-0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.0), 0);
        meshBuilder.addTriangle(a, b, c);
        meshBuilder.addTriangle(a, c, d);

        // left
        a = meshBuilder.addVertex(iaVector3f(-0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.3333), 0);
        b = meshBuilder.addVertex(iaVector3f(-0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 0.3333), 0);
        c = meshBuilder.addVertex(iaVector3f(-0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 0.6666), 0);
        d = meshBuilder.addVertex(iaVector3f(-0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.6666), 0);
        
        meshBuilder.addTriangle(a, b, d);
        meshBuilder.addTriangle(b, c, d);  

        // front
        a = meshBuilder.addVertex(iaVector3f(-0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.3333), 0);
        b = meshBuilder.addVertex(iaVector3f(0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.3333), 0);
        c = meshBuilder.addVertex(iaVector3f(0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.6666), 0);
        d = meshBuilder.addVertex(iaVector3f(-0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.6666), 0);
        meshBuilder.addTriangle(a, d, b);
        meshBuilder.addTriangle(d, c, b);        

        // right
        a = meshBuilder.addVertex(iaVector3f(0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.3333), 0);
        b = meshBuilder.addVertex(iaVector3f(0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.75, 0.3333), 0);
        c = meshBuilder.addVertex(iaVector3f(0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.75, 0.6666), 0);
        d = meshBuilder.addVertex(iaVector3f(0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.6666), 0);
        meshBuilder.addTriangle(a, d, b);
        meshBuilder.addTriangle(b, d, c);          

        // back
        a = meshBuilder.addVertex(iaVector3f(-0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 0.3333), 0);
        b = meshBuilder.addVertex(iaVector3f(0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.75, 0.3333), 0);
        c = meshBuilder.addVertex(iaVector3f(0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.75, 0.6666), 0);
        d = meshBuilder.addVertex(iaVector3f(-0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 0.6666), 0);
        meshBuilder.addTriangle(a, b, d);
        meshBuilder.addTriangle(b, c, d);             

        // bottom
        a = meshBuilder.addVertex(iaVector3f(-0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.6666), 0);
        b = meshBuilder.addVertex(iaVector3f(0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.6666), 0);
        c = meshBuilder.addVertex(iaVector3f(0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 1.0), 0);
        d = meshBuilder.addVertex(iaVector3f(-0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 1.0), 0);
        meshBuilder.addTriangle(a, d, b);
        meshBuilder.addTriangle(d, c, b);

        _mesh = meshBuilder.createMesh();
    }

    void iNodeSkyBox::setTexture(iTexturePtr texture)
    {
        _texture = texture;
    }

    void iNodeSkyBox::setOffsetMatrix(iaMatrixd &offsetMatrix)
    {
        _offsetMatrix = offsetMatrix;
    }

    void iNodeSkyBox::getOffsetMatrix(iaMatrixd &offsetMatrix)
    {
        offsetMatrix = _offsetMatrix;
    }

    void iNodeSkyBox::setUseOffsetMatrix(bool useMatrix)
    {
        _useMatrix = useMatrix;
    }

    bool iNodeSkyBox::isOffsetMatrixUsed() const
    {
        return _useMatrix;
    }

    void iNodeSkyBox::setBoxSize(float32 boxSize)
    {
        _boxSize = boxSize;
    }

    float32 iNodeSkyBox::getBoxSize() const
    {
        return _boxSize;
    }

    void iNodeSkyBox::draw()
    {
        iaMatrixd model;
        const iaMatrixd &camMatrix = iRenderer::getInstance().getCamMatrix();

        model._pos = camMatrix._pos;
        if (_useMatrix)
        {
            model *= _offsetMatrix;
        }
        model.scale(_boxSize,_boxSize,_boxSize);
        iRenderer::getInstance().setModelMatrix(model);

        if(_texture->useFallback())
        {
            iTextureResourceFactory::getInstance().getDummyTexture()->bind(0);
        }
        else
        {
            _texture->bind(0);
        }
        iRenderer::getInstance().drawMesh(_mesh, nullptr);
    }

} // namespace igor