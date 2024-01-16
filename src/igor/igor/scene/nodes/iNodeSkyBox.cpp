// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeSkyBox.h>

#include <iaux/system/iaConsole.h>
#include <igor/renderer/iRenderer.h>
#include <igor/resources/shader/iShader.h>
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

        _mesh = node->_mesh;
    }

    void iNodeSkyBox::buildMesh()
    {
        iMeshBuilder meshBuilder;
        // manually set triangle indices
        meshBuilder.setJoinVertices(false);

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
        const iaMatrixd &camMatrix = iRenderer::getInstance().getCamMatrix();

        iaMatrixd model;
        model._pos = camMatrix._pos;       
        model.scale(_boxSize,_boxSize,_boxSize);
        iRenderer::getInstance().setModelMatrix(model);

        iRenderer::getInstance().drawMesh(_mesh, getMaterial());
    }

} // namespace igor