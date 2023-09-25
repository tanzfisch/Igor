// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/traversal/iNodeVisitorRenderColorID.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

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
        iRenderer::getInstance().setMaterial(iRenderer::getInstance().getColorIDMaterial());
    }

    void iNodeVisitorRenderColorID::postTraverse()
    {
        con_assert(_matrixStack.size() == 0, "matrix stack should be empty");
    }
} // namespace igor