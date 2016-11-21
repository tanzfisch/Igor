// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeSwitch.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
	
	iNodeSwitch::iNodeSwitch()
		: iNode()
	{
		setName(L"iNodeSwitch");
		_nodeType = iNodeType::iNodeSwitch;
		_nodeKind = iNodeKind::Node;
	}

    iNodeSwitch::iNodeSwitch(iNodeSwitch* node)
        : iNode()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = iNodeType::iNodeSwitch;
        _nodeKind = iNodeKind::Node;

        setName(node->getName());
    }

    void iNodeSwitch::onPostCopyLink(map<uint32, uint32>& nodeIDMap)
    {

    }

    void iNodeSwitch::setActiveChild(uint32 id)
    {
        iNode* child = getChild(id);
        con_assert(child != nullptr, "child with id " << id << " not found");

        if (child != nullptr)
        {
            setActiveChild(child);
        }
    }

    void iNodeSwitch::setActiveChild(const iaString& name)
    {
        iNode* child = getChild(name);
        con_assert(child != nullptr, "child with name " << name << " not found");

        if (child != nullptr)
        {
            setActiveChild(child);
        }
    }

    void iNodeSwitch::setActiveChild(iNode* node)
    {
        con_assert(node != nullptr, "zero pointer");

        vector<iNode*> copyChildren(_children);

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

};
