// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeLODTrigger.h>

#include <iScene.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
	
	iNodeLODTrigger::iNodeLODTrigger()
		: iNode()
	{
		setName(L"iNodeLODTrigger");
		_nodeType = iNodeType::iNodeLODTrigger;
		_nodeKind = iNodeKind::Node;
	}

    iNodeLODTrigger::iNodeLODTrigger(iNodeLODTrigger* node)
        : iNode()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = iNodeType::iNodeLODTrigger;
        _nodeKind = iNodeKind::Node;

        setName(node->getName());
    }

	iNodeLODTrigger::~iNodeLODTrigger()
	{
		setScene(nullptr);
	}

    void iNodeLODTrigger::onPostCopyLink(std::map<uint32, uint32>& nodeIDMap)
    {

    }

    void iNodeLODTrigger::onUpdateTransform(iaMatrixd& matrix)
    {
        _worldPosition = matrix._pos;
    }

    const iaVector3d& iNodeLODTrigger::getWorldPosition() const
    {
        return _worldPosition;
    }

    void iNodeLODTrigger::onPreSetScene()
    {
        if (getScene())
        {
            getScene()->unregisterLODTrigger(this);
        }
    }

    void iNodeLODTrigger::onPostSetScene()
    {
        if (getScene())
        {
            getScene()->registerLODTrigger(this);
        }
    }
};
