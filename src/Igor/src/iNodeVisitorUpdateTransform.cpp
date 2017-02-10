// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeVisitorUpdateTransform.h>
#include <iNode.h>
#include <iNodeTransform.h>
#include <iScene.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	bool iNodeVisitorUpdateTransform::preOrderVisit(iNode* node)
	{
        if (iNodeKind::Transformation == node->getKind())
        {
            _matrixStack.push_back(_currentMatrix);
        }

        if (node->isTransformationDirty())
		{
            node->onUpdateTransform(_currentMatrix);
			node->setTransformationDirty(false);
			return true;
		}
		else
		{
			return false;
		}
	}

	void iNodeVisitorUpdateTransform::postOrderVisit(iNode* node)
	{
        if (iNodeKind::Transformation == node->getKind())
        {
            _currentMatrix = _matrixStack.back();
            con_assert(_matrixStack.size() != 0, "stack underflow");
            _matrixStack.pop_back();
        }
	}

	void iNodeVisitorUpdateTransform::preTraverse()
	{
		_currentMatrix.identity();
	}

	void iNodeVisitorUpdateTransform::postTraverse()
	{
        con_assert(_matrixStack.size() == 0, "matrix stack should be empty");
	}
}