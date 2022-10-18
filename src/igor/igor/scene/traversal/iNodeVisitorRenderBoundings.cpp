// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/traversal/iNodeVisitorRenderBoundings.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/graphics/iRenderer.h>
#include <igor/resources/material/iMaterialResourceFactory.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iNodeVisitorRenderBoundings::iNodeVisitorRenderBoundings()
    {
        _material = iMaterialResourceFactory::getInstance().createMaterial("igor.boundings");
        iMaterialResourceFactory::getInstance().getMaterial(_material)->setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    }

    bool iNodeVisitorRenderBoundings::preOrderVisit(iNodePtr node, iNodePtr nextSibling)
    {
        if (iNodeKind::Transformation == node->getKind())
        {
            _matrixStack.push_back(_currentMatrix);

            iNodeTransform *transform = static_cast<iNodeTransform *>(node);
            iaMatrixd matrix;
            transform->getMatrix(matrix);
            _currentMatrix *= matrix;
            iRenderer::getInstance().setModelMatrix(_currentMatrix);
        }

        if (iNodeKind::Volume == node->getKind())
        {
            iNodeVolume *volume = static_cast<iNodeVolume *>(node);

            iAABoxd bbox = volume->getBoundingBox();

            iRenderer::getInstance().drawBBox(bbox);
        }

        return true;
    }

    void iNodeVisitorRenderBoundings::postOrderVisit(iNodePtr node)
    {
        if (iNodeKind::Transformation == node->getKind())
        {
            con_assert(_matrixStack.size() != 0, "stack underflow");
            _currentMatrix = _matrixStack.back();
            _matrixStack.pop_back();

            iRenderer::getInstance().setModelMatrix(_currentMatrix);
        }
    }

    void iNodeVisitorRenderBoundings::preTraverse()
    {
        _currentMatrix.identity();
        iRenderer::getInstance().setColor(1, 1, 1, 1);
        iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_material));
    }

    void iNodeVisitorRenderBoundings::postTraverse()
    {
        con_assert(_matrixStack.size() == 0, "matrix stack should be empty");
    }
} // namespace igor