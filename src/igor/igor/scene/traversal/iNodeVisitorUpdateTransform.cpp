// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/traversal/iNodeVisitorUpdateTransform.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/iScene.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	bool iNodeVisitorUpdateTransform::preOrderVisit(iNodePtr node, iNodePtr nextSibling)
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

	void iNodeVisitorUpdateTransform::postOrderVisit(iNodePtr node)
	{
		if (iNodeKind::Transformation == node->getKind())
		{
			con_assert(_matrixStack.size() != 0, "stack underflow");
			_currentMatrix = _matrixStack.back();
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
} // namespace igor