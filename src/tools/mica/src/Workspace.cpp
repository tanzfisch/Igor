#include "Workspace.h"

Workspace::Workspace()
{
    _rootMica = iNodeManager::getInstance().createNode<iNode>();
    _rootUser = iNodeManager::getInstance().createNode<iNode>();

    _scene = iSceneFactory::getInstance().createScene();
    _scene->getRoot()->insertNode(_rootMica);
    _scene->getRoot()->insertNode(_rootUser);
}

Workspace::~Workspace()
{
    iSceneFactory::getInstance().destroyScene(_scene);
}

static iModelDataInputParameter *createDataInputParameter()
{
    iModelDataInputParameter *parameter = new iModelDataInputParameter();
    parameter->_identifier = "";
    parameter->_modelSourceType = iModelSourceType::File;
    parameter->_needsRenderContext = true;
    parameter->_loadPriority = iTask::DEFAULT_PRIORITY;
    parameter->_joinVertexes = false;
    parameter->_keepMesh = true;

    return parameter;
}

void Workspace::duplicateSelected()
{
    for (const auto nodeID : getSelection())
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node == nullptr)
        {
            continue;
        }

        auto parent = node->getParent();
        if (parent == nullptr)
        {
            continue;
        }

        iNodePtr nodeCopy = iNodeManager::getInstance().createCopy(node);
        parent->insertNodeAsync(nodeCopy);
    }
}

void Workspace::cutSelected()
{
    _cutNodes = _selectedNodes;
    _copiedNodes.clear();
}

void Workspace::copySelected()
{
    _copiedNodes = _selectedNodes;
    _cutNodes.clear();
}

void Workspace::deleteSelected()
{
    for (auto nodeID : _selectedNodes)
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node == nullptr)
        {
            continue;
        }

        iNodeManager::getInstance().destroyNodeAsync(node);
    }
}

void Workspace::pasteSelected()
{
    if (_selectedNodes.size() > 1)
    {
        return;
    }

    std::vector<iNodeID> newSelection;

    if (!_copiedNodes.empty())
    {
        for (auto nodeID : _copiedNodes)
        {
            iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
            if (node == nullptr)
            {
                continue;
            }

            iNodePtr newNode = iNodeManager::getInstance().createCopy(node);
            newSelection.push_back(newNode->getID());

            iNodePtr destination = nullptr;
            if (!_selectedNodes.empty())
            {
                iNodePtr destination = iNodeManager::getInstance().getNode(_selectedNodes[0]);
            }

            if (destination == nullptr)
            {
                destination = _rootUser;
            }

            destination->insertNodeAsync(newNode);
        }

        _copiedNodes.clear();
        _selectedNodes = newSelection;
    }
    else if (!_cutNodes.empty())
    {
        for (auto nodeID : _cutNodes)
        {
            iNodePtr node = iNodeManager::getInstance().getNode(nodeID);

            if (node == nullptr)
            {
                continue;
            }

            iNodePtr destination = nullptr;
            if (!_selectedNodes.empty())
            {
                iNodePtr destination = iNodeManager::getInstance().getNode(_selectedNodes[0]);
            }

            if (destination == nullptr)
            {
                destination = _rootUser;
            }

            iNodePtr parent = node->getParent();
            if (parent != nullptr)
            {
                parent->removeNodeAsync(node);
                destination->insertNodeAsync(node);
            }
        }
    }
}

void Workspace::loadFile(const iaString &filename)
{
    clear();

    if (filename.isEmpty())
    {
        return;
    }

    iNodeModel *model = iNodeManager::getInstance().createNode<iNodeModel>();
    iModelDataInputParameter *parameter = createDataInputParameter();
    model->setModel(filename, iResourceCacheMode::Free, parameter, true);

    if (model->isValid())
    {
        iNodePtr insertAt = nullptr;

        auto children = model->getChildren();
        if (children.size() > 1)
        {
            insertAt = iNodeManager::getInstance().createNode<iNode>();
            iaString groupName = "group:";
            groupName += filename;
            insertAt->setName(groupName);

            getRootUser()->insertNode(insertAt);
        }
        else
        {
            insertAt = getRootUser();
        }

        auto child = children.begin();
        while (child != children.end())
        {
            model->removeNode((*child));
            insertAt->insertNode((*child));
            child++;
        }
    }

    iNodeManager::getInstance().destroyNodeAsync(model);
}

iNodePtr Workspace::getRootMica() const
{
    return _rootMica;
}

iNodePtr Workspace::getRootUser() const
{
    return _rootUser;
}

void Workspace::clear()
{
    clearSelection();

    auto children = _rootUser->getChildren();
    for (auto child : children)
    {
        iNodeManager::getInstance().destroyNodeAsync(child);
    }
}

const std::vector<iNodeID> &Workspace::getSelection() const
{
    return _selectedNodes;
}

void Workspace::setSelection(const std::vector<iNodeID> &selection)
{
    _selectedNodes = selection;
}

void Workspace::clearSelection()
{
    _selectedNodes.clear();
}

iScene *Workspace::getScene() const
{
    return _scene;
}