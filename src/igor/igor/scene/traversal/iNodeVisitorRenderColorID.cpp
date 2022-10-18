// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/traversal/iNodeVisitorRenderColorID.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/graphics/iRenderer.h>
#include <igor/resources/material/iMaterialResourceFactory.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iNodeVisitorRenderColorID::iNodeVisitorRenderColorID()
    {
        _material = iMaterialResourceFactory::getInstance().createMaterial();
        iMaterialResourceFactory::getInstance().getMaterial(_material)->addShaderSource("default.vert", iShaderObjectType::Vertex);
        iMaterialResourceFactory::getInstance().getMaterial(_material)->addShaderSource("default_directional_light.frag", iShaderObjectType::Fragment);
        iMaterialResourceFactory::getInstance().getMaterial(_material)->compileShader();
        iMaterialResourceFactory::getInstance().getMaterial(_material)->setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    }

    bool iNodeVisitorRenderColorID::preOrderVisit(iNodePtr node, iNodePtr nextSibling)
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

        if (iNodeKind::Renderable == node->getKind())
        {
            iNodeRender *renderNode = static_cast<iNodeRender *>(node);
            renderNode->draw();
        }

        return true;
    }

    void iNodeVisitorRenderColorID::postOrderVisit(iNodePtr node)
    {
        if (iNodeKind::Transformation == node->getKind())
        {
            con_assert(_matrixStack.size() != 0, "stack underflow");
            _currentMatrix = _matrixStack.back();
            _matrixStack.pop_back();

            iRenderer::getInstance().setModelMatrix(_currentMatrix);
        }
    }

    void iNodeVisitorRenderColorID::preTraverse()
    {
        _currentMatrix.identity();
        iRenderer::getInstance().setColor(1, 1, 1, 1);
        iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_material));
    }

    void iNodeVisitorRenderColorID::postTraverse()
    {
        con_assert(_matrixStack.size() == 0, "matrix stack should be empty");
    }
} // namespace igor