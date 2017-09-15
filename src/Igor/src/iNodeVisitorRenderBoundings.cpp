// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeVisitorRenderBoundings.h>
#include <iNode.h>
#include <iNodeTransform.h>
#include <iRenderer.h>

#include <IgorAux.h>
#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

		iNodeVisitorRenderBoundings::iNodeVisitorRenderBoundings()
		{
            // TODO copnfigure material
		}

		void iNodeVisitorRenderBoundings::run(iNode* node)
		{
			traverseTree(node);
		}
		
		bool iNodeVisitorRenderBoundings::preOrderVisit(iNode* node)
		{
            if (iNodeKind::Transformation == node->getKind())
            {
                _matrixStack.push_back(_currentMatrix);

                iNodeTransform* transform = static_cast<iNodeTransform*>(node);
                iaMatrixd matrix;
                transform->getMatrix(matrix);
                _currentMatrix *= matrix;
            }
            else if (iNodeKind::Volume == node->getKind())
            {
                iRenderer::getInstance().setModelMatrix(_currentMatrix);
                iRenderer::getInstance().drawBox(iaVector3f(-1, -1, -1), iaVector3f(1, 1, 1));
            }

            return true;
		}

		void iNodeVisitorRenderBoundings::postOrderVisit(iNode* node)
		{
            if (iNodeKind::Transformation == node->getKind())
            {
                con_assert(_matrixStack.size() != 0, "stack underflow");
                _currentMatrix = _matrixStack.back();
                _matrixStack.pop_back();
            }
		}

		void iNodeVisitorRenderBoundings::preTraverse()
		{
            _currentMatrix.identity();
            // TODO set material
		}

		void iNodeVisitorRenderBoundings::postTraverse()
		{
            con_assert(_matrixStack.size() == 0, "matrix stack should be empty");
		}
}