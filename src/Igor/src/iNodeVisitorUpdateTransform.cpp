// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeVisitorUpdateTransform.h>
#include <iNode.h>
#include <iNodeTransform.h>
#include <iNodeRender.h>
#include <iNodeLight.h>
#include <iNodeCamera.h>
#include <iNodeVolume.h>
#include <iNodeModel.h>
#include <iNodeLODTrigger.h>
#include <iNodeLODSwitch.h>
#include <iScene.h>

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