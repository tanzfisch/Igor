#include "Actions.h"

#include "ActionContext.h"

#include <igor/system/iClipboard.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/ui/actions/iActionManager.h>
#include <igor/scene/nodes/iNodeManager.h>
using namespace igor;

#include <vector>
#include <any>

#if 0
ActionCopyNode::ActionCopyNode()
    : iAction("mica:copyNodes")
{
    setPicturePath("icons/copy.png");
    setDescription("Copy selection");
}

void ActionCopyNode::execute(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr ||
        actionContext->getNodes().empty())
    {
        return;
    }

    iClipboard::getInstance().setData(iClipboardFormat::IgorNodes, );
}

ActionPasteNode::ActionPasteNode()
    : iAction("mica:pasteNodes")
{
    setPicturePath("icons/paste.png");
    setDescription("Paste at selection");
}

void ActionPasteNode::execute(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr ||
        actionContext->getNodes().size() != 1 ||
        iClipboard::getInstance().getFormat() != iClipboardFormat::IgorNodes)
    {
        return;
    }

    std::any data = iClipboard::getInstance().getData();
    std::vector<iNodeID> nodeIDs = std::any_cast<std::vector<iNodeID>>(data);

    iNode *dst = iNodeManager::getInstance().getNode(actionContext->getNodes()[0]);

    for (auto nodeID : nodeIDs)
    {
        iNode *node = iNodeManager::getInstance().getNode(nodeID);
        if (node == nullptr)
        {
            continue;
        }

        dst->insertNodeAsync(node);
    }
}
#endif

ActionDeleteNode::ActionDeleteNode()
    : iAction("mica:deleteNodes")
{
    setPicturePath("icons/delete.png");
    setDescription("delete selection");
}

void ActionDeleteNode::execute(const iActionContext &context)
{
    const ActionContext *actionContext = dynamic_cast<const ActionContext *>(&context);
    if (actionContext == nullptr ||
        actionContext->getNodes().empty())
    {
        return;
    }

    std::vector<iNodeID> copies;

    for (auto nodeID : actionContext->getNodes())
    {
        iNodeManager::getInstance().destroyNodeAsync(nodeID);
    }
}

void registerMicaActions()
{
    //iActionManager::getInstance().registerAction(new ActionCopyNode());
    //iActionManager::getInstance().registerAction(new ActionPasteNode());
    iActionManager::getInstance().registerAction(new ActionDeleteNode());
}