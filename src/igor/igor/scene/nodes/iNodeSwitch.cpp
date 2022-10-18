// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeSwitch.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iNodeSwitch::iNodeSwitch()
        : iNode()
    {
        setName(L"iNodeSwitch");
        _nodeType = iNodeType::iNodeSwitch;
        _nodeKind = iNodeKind::Node;
    }

    iNodeSwitch::iNodeSwitch(iNodeSwitch *node)
        : iNode()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = iNodeType::iNodeSwitch;
        _nodeKind = iNodeKind::Node;

        setName(node->getName());
    }

    void iNodeSwitch::onPostCopyLink(std::map<uint32, uint32> &nodeIDMap)
    {
    }

    void iNodeSwitch::setActiveChild(uint32 id)
    {
        iNodePtr child = getChild(id);
        con_assert(child != nullptr, "child with id " << id << " not found");

        if (child != nullptr)
        {
            setActiveChild(child);
        }
    }

    void iNodeSwitch::setActiveChild(const iaString &name)
    {
        iNodePtr child = getChild(name);
        con_assert(child != nullptr, "child with name " << name << " not found");

        if (child != nullptr)
        {
            setActiveChild(child);
        }
    }

    void iNodeSwitch::setActiveChild(iNodePtr node)
    {
        con_assert(node != nullptr, "zero pointer");

        std::vector<iNodePtr> copyChildren(_children);

        auto iter = copyChildren.begin();
        while (iter != copyChildren.end())
        {
            if ((*iter) != node)
            {
                (*iter)->setActive(false);
            }
            iter++;
        }

        if (node != nullptr &&
            !node->isActive())
        {
            node->setActive();
        }
    }

}; // namespace igor
