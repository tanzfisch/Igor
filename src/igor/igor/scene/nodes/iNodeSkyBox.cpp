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
    }

    iNodeSkyBox::iNodeSkyBox(iNodeSkyBox *node)
        : iNodeRender()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());
        setTextureScale(node->getTextureScale());

        iaMatrixd matrix;
        node->getOffsetMatrix(matrix);
        setOffsetMatrix(matrix);
        setUseOffsetMatrix(node->isOffsetMatrixUsed());
    }

    iNodeSkyBox::~iNodeSkyBox()
    {
        _front = 0;
        _back = 0;
        _left = 0;
        _right = 0;
        _top = 0;
        _bottom = 0;
    }

    void iNodeSkyBox::buildMesh()
    {
        iMeshBuilder meshBuilder;

        meshBuilder.setJoinVertexes(false);

        // top
        meshBuilder.addVertex(iaVector3f(-0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 0.0), 0);
        meshBuilder.addVertex(iaVector3f(0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 1.0), 0);
        meshBuilder.addVertex(iaVector3f(0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 1.0), 0);
        meshBuilder.addVertex(iaVector3f(-0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 0.0), 0);
        meshBuilder.addTriangle(0, 1, 2);
        meshBuilder.addTriangle(0, 2, 3);

        // bottom
        meshBuilder.addVertex(iaVector3f(-0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 0.0), 1);
        meshBuilder.addVertex(iaVector3f(0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 0.0), 1);
        meshBuilder.addVertex(iaVector3f(0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 1.0), 1);
        meshBuilder.addVertex(iaVector3f(-0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 1.0), 1);
        meshBuilder.addTriangle(4, 7, 5);
        meshBuilder.addTriangle(7, 6, 5);

        // back
        meshBuilder.addVertex(iaVector3f(-0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 0.0), 2);
        meshBuilder.addVertex(iaVector3f(0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 0.0), 2);
        meshBuilder.addVertex(iaVector3f(0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 1.0), 2);
        meshBuilder.addVertex(iaVector3f(-0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 1.0), 2);
        meshBuilder.addTriangle(8, 11, 9);
        meshBuilder.addTriangle(11, 10, 9);

        // front
        meshBuilder.addVertex(iaVector3f(-0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 1.0), 3);
        meshBuilder.addVertex(iaVector3f(0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 0.0), 3);
        meshBuilder.addVertex(iaVector3f(0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 0.0), 3);
        meshBuilder.addVertex(iaVector3f(-0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 1.0), 3);
        meshBuilder.addTriangle(12, 13, 15);
        meshBuilder.addTriangle(13, 14, 15);        

        // left
        meshBuilder.addVertex(iaVector3f(-0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 1.0), 4);
        meshBuilder.addVertex(iaVector3f(-0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 0.0), 4);
        meshBuilder.addVertex(iaVector3f(-0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 0.0), 4);
        meshBuilder.addVertex(iaVector3f(-0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 1.0), 4);
        meshBuilder.addTriangle(16, 17, 19);
        meshBuilder.addTriangle(17, 18, 19);          

        // right
        meshBuilder.addVertex(iaVector3f(0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 0.0), 5);
        meshBuilder.addVertex(iaVector3f(0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 0.0), 5);
        meshBuilder.addVertex(iaVector3f(0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 1.0), 5);
        meshBuilder.addVertex(iaVector3f(0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 1.0), 5);
        meshBuilder.addTriangle(20, 23, 21);
        meshBuilder.addTriangle(21, 23, 22);         

        // TODO iMeshPtr mesh = meshBuilder.createMesh();
    }

    float32 iNodeSkyBox::getTextureScale() const
    {
        return _textureScale;
    }

    void iNodeSkyBox::setTextures(iTexturePtr front, iTexturePtr back, iTexturePtr left, iTexturePtr right, iTexturePtr top, iTexturePtr bottom)
    {
        _front = front;
        _back = back;
        _left = left;
        _right = right;
        _top = top;
        _bottom = bottom;
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

    void iNodeSkyBox::setTextureScale(float32 scale)
    {
        _textureScale = scale;
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
        iRenderer::getInstance().setModelMatrix(model);
        const iaVector2f tiling(_textureScale, _textureScale);

        iRenderer::getInstance().drawTexturedQuad(iaVector3f(0, 0, -_boxSize), iaVector3f(-_boxSize, 0, 0), iaVector3f(0, _boxSize, 0), _front, iaColor4f::white, false, tiling);
        iRenderer::getInstance().drawTexturedQuad(iaVector3f(0, 0, _boxSize), iaVector3f(_boxSize, 0, 0), iaVector3f(0, _boxSize, 0), _back, iaColor4f::white, false, tiling);
        iRenderer::getInstance().drawTexturedQuad(iaVector3f(-_boxSize, 0, 0), iaVector3f(0, 0, _boxSize), iaVector3f(0, _boxSize, 0), _left, iaColor4f::white, false, tiling);
        iRenderer::getInstance().drawTexturedQuad(iaVector3f(_boxSize, 0, 0), iaVector3f(0, 0, -_boxSize), iaVector3f(0, _boxSize, 0), _right, iaColor4f::white, false, tiling);
        iRenderer::getInstance().drawTexturedQuad(iaVector3f(0, _boxSize, 0), iaVector3f(_boxSize, 0, 0), iaVector3f(0, 0, -_boxSize), _top, iaColor4f::white, false, tiling);
        iRenderer::getInstance().drawTexturedQuad(iaVector3f(0, -_boxSize, 0), iaVector3f(_boxSize, 0, 0), iaVector3f(0, 0, _boxSize), _bottom, iaColor4f::white, false, tiling);
    }

} // namespace igor