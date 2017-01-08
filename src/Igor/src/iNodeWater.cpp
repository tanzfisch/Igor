// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeWater.h>

#include <iaConsole.h>
#include <iRenderer.h>
#include <iMaterial.h>
#include <iScene.h>
#include <iNodeCamera.h>
#include <iNodeFactory.h>
#include <iMaterialResourceFactory.h>

namespace Igor
{

    iNodeWater::iNodeWater()
        : iNodeRender()
    {
        setName(L"iNodeWater");
        _nodeType = iNodeType::iNodeWater;

        _ambient.set(0, 0.4, 0.9, 0.35);
    }

    iNodeWater::iNodeWater(iNodeWater* node)
        : iNodeRender()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;
        _ambient = node->_ambient;
        _yPos = node->_yPos;

        setName(node->getName());
    }

    iNodeWater::~iNodeWater()
    {
    }

    bool iNodeWater::wasReached()
    {
        // TODO
        return true;
    }

    void iNodeWater::setWaterPosition(float32 yPos)
    {
        _yPos = yPos;
    }

    float32 iNodeWater::getWaterPosition() const
    {
        return _yPos;
    }

    void iNodeWater::setAmbient(const iaColor4f& color)
    {
        _ambient = color;
    }

    iaColor4f iNodeWater::getAmbient() const
    {
        return _ambient;
    }

    void iNodeWater::draw()
    {
        iNodeCamera* cameraNode = static_cast<iNodeCamera*>(iNodeFactory::getInstance().getNode(_scene->getCamera()));
        if (cameraNode != nullptr)
        {
            iaMatrixd model;
            iaMatrixd camMatrix;
            cameraNode->getWorldMatrix(camMatrix);

            model._pos = camMatrix._pos;
            model._pos._y = _yPos;
            iRenderer::getInstance().setModelMatrix(model);

            // TODO this is of corse super slow and just for debugging
            iRenderer::getInstance().setColor(_ambient);

            float32 f = 7000;

            for (int x = -3; x < 4; ++x)
            {
                for (int z = -3; z < 4; ++z)
                {
                    iRenderer::getInstance().drawBillboard(iaVector3f(f * 2.0f * x, 0, f * 2.0 * z), iaVector3f(f, 0, 0), iaVector3f(0, 0, f), nullptr, 10, 10);
                }
            }
        }
    }

}