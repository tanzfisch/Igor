// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeLODTrigger.h>

#include <igor/scene/iScene.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iNodeLODTrigger::iNodeLODTrigger()
        : iNode()
    {
        setName(L"iNodeLODTrigger");
        _nodeType = iNodeType::iNodeLODTrigger;
        _nodeKind = iNodeKind::Node;
    }

    iNodeLODTrigger::iNodeLODTrigger(iNodeLODTrigger *node)
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

    void iNodeLODTrigger::onPostCopyLink(std::map<uint32, uint32> &nodeIDMap)
    {
    }

    void iNodeLODTrigger::onUpdateTransform(iaMatrixd &matrix)
    {
        _worldPosition = matrix._pos;
    }

    const iaVector3d &iNodeLODTrigger::getWorldPosition() const
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
}; // namespace igor
