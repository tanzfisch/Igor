// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeSkyBox.h>

#include <iaux/system/iaConsole.h>
#include <igor/graphics/iRenderer.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/resources/material/iMaterialResourceFactory.h>

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

    void iNodeSkyBox::setAlpha(float32 alpha)
    {
        _alpha = alpha;
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
        iaMatrixd camMatrix;
        iRenderer::getInstance().getCamWorldMatrix(camMatrix);

        model._pos = camMatrix._pos;
        if (_useMatrix)
        {
            model *= _offsetMatrix;
        }
        iRenderer::getInstance().setModelMatrix(model);

        if (_alpha < 1.0f)
        {
            iMaterialPtr material = iMaterialResourceFactory::getInstance().getCurrentMaterial();
            if (material != nullptr &&
                material->isValid())
            {
                //! \todo evil hack
                if (material->getRenderState(iRenderState::Blend) == iRenderStateValue::On)
                {
                    iRenderer::getInstance().setColor(1, 1, 1, _alpha);
                }
            }
        }
        else
        {
            iRenderer::getInstance().setColor(1, 1, 1, 1);
        }

        //! \todo make VBO
        iRenderer::getInstance().drawBillboard(iaVector3f(0, 0, -_boxSize), iaVector3f(-_boxSize, 0, 0), iaVector3f(0, _boxSize, 0), _front, _textureScale, _textureScale);
        iRenderer::getInstance().drawBillboard(iaVector3f(0, 0, _boxSize), iaVector3f(_boxSize, 0, 0), iaVector3f(0, _boxSize, 0), _back, _textureScale, _textureScale);
        iRenderer::getInstance().drawBillboard(iaVector3f(-_boxSize, 0, 0), iaVector3f(0, 0, _boxSize), iaVector3f(0, _boxSize, 0), _left, _textureScale, _textureScale);
        iRenderer::getInstance().drawBillboard(iaVector3f(_boxSize, 0, 0), iaVector3f(0, 0, -_boxSize), iaVector3f(0, _boxSize, 0), _right, _textureScale, _textureScale);
        iRenderer::getInstance().drawBillboard(iaVector3f(0, _boxSize, 0), iaVector3f(_boxSize, 0, 0), iaVector3f(0, 0, -_boxSize), _top, _textureScale, _textureScale);
        iRenderer::getInstance().drawBillboard(iaVector3f(0, -_boxSize, 0), iaVector3f(_boxSize, 0, 0), iaVector3f(0, 0, _boxSize), _bottom, _textureScale, _textureScale);
    }

} // namespace igor