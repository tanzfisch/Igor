// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeSkyBox.h>

#include <iaConsole.h>
#include <iRenderer.h>
#include <iMaterial.h>
#include <iScene.h>
#include <iNodeCamera.h>
#include <iMaterialResourceFactory.h>

namespace Igor
{

	iNodeSkyBox::iNodeSkyBox()
		: iNodeRender()
	{
		setName(L"iNodeSkyBox");
		_nodeType = iNodeType::iNodeSkyBox;
	}

    iNodeSkyBox::iNodeSkyBox(iNodeSkyBox* node)
        : iNodeRender()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());
        setTextureScale(node->getTextureScale());

        iaMatrixf matrix;
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

    bool iNodeSkyBox::wasReached()
    {
        // sky box is always reached
        return true;
    }

	void iNodeSkyBox::setTextures(iaString front, iaString back, iaString left, iaString right, iaString top, iaString bottom)
	{
		_front = iTextureResourceFactory::getInstance().requestFile(front);
		_back = iTextureResourceFactory::getInstance().requestFile(back);
		_left = iTextureResourceFactory::getInstance().requestFile(left);
		_right = iTextureResourceFactory::getInstance().requestFile(right);
		_top = iTextureResourceFactory::getInstance().requestFile(top);
		_bottom = iTextureResourceFactory::getInstance().requestFile(bottom);
	}

    void iNodeSkyBox::setOffsetMatrix(iaMatrixf& offsetMatrix)
	{
        _offsetMatrix = offsetMatrix;
	}

    void iNodeSkyBox::getOffsetMatrix(iaMatrixf& offsetMatrix)
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

	void iNodeSkyBox::draw()
	{
		const float32 boxsize = 10.0f;
		iNodeCamera* cameraNode = _scene->getCamera();

		if(cameraNode)
		{
			iaMatrixf model;
            iaMatrixf camMatrix;
            cameraNode->getWorldMatrix(camMatrix);

			model._pos = camMatrix._pos;
			if(_useMatrix)
			{
				model *= _offsetMatrix;
			}
			iRenderer::getInstance().setModelMatrix(model);

			if(_alpha < 1.0f)
			{
				iMaterial* material = iMaterialResourceFactory::getInstance().getCurrentMaterial();
				if(material)
				{
                    //! \todo that is evil and not ok to use like this
					if(material->getRenderStateSet().getRenderStateValue(iRenderState::Blend) == iRenderStateValue::On)
					{
                        iRenderer::getInstance().setColor(1, 1, 1, _alpha);
					}
				}
			}

			iRenderer::getInstance().drawBillboard(iaVector3f(0,0,-boxsize),iaVector3f(-boxsize,0,0),iaVector3f(0,boxsize,0), _front, _textureScale, _textureScale);
			iRenderer::getInstance().drawBillboard(iaVector3f(0,0,boxsize),iaVector3f(boxsize,0,0),iaVector3f(0,boxsize,0), _back, _textureScale, _textureScale);
			iRenderer::getInstance().drawBillboard(iaVector3f(-boxsize,0,0),iaVector3f(0,0,boxsize),iaVector3f(0,boxsize,0), _left,_textureScale,_textureScale);
			iRenderer::getInstance().drawBillboard(iaVector3f(boxsize,0,0),iaVector3f(0,0,-boxsize),iaVector3f(0,boxsize,0), _right, _textureScale, _textureScale);
			iRenderer::getInstance().drawBillboard(iaVector3f(0,boxsize,0),iaVector3f(boxsize,0,0),iaVector3f(0,0,-boxsize), _top, _textureScale, _textureScale);
			iRenderer::getInstance().drawBillboard(iaVector3f(0,-boxsize,0),iaVector3f(boxsize,0,0),iaVector3f(0,0,boxsize), _bottom, _textureScale, _textureScale);

			iRenderer::getInstance().setColor(1,1,1,1);
		}
		else
		{
			con_err("no camera node was set");
		}
	}

}