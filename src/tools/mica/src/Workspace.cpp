#include "Workspace.h"

Workspace::Workspace()
{
    _rootMica = iNodeManager::getInstance().createNode<iNode>();
    _rootUser = iNodeManager::getInstance().createNode<iNode>();

    _scene = iSceneFactory::getInstance().createScene();
    _scene->getRoot()->insertNode(_rootMica);
    _scene->getRoot()->insertNode(_rootUser);

    // cam
    _cameraArc = new CameraArc(getRootMica());
    _cameraArc->setHeading(M_PI * 0.25);
    _cameraArc->setPitch(-0.25);
}

Workspace::~Workspace()
{
    iSceneFactory::getInstance().destroyScene(_scene);
}

CameraArcPtr Workspace::getCameraArc() const
{
    return _cameraArc;
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
    _cutNodes = _scene->getSelection();
    _copiedNodes.clear();
}

void Workspace::copySelected()
{
    _copiedNodes = _scene->getSelection();
    _cutNodes.clear();
}

void Workspace::deleteSelected()
{
    for (auto nodeID : _scene->getSelection())
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node == nullptr)
        {
            continue;
        }

        iNodeManager::getInstance().destroyNodeAsync(node);
    }

    clearSelection();
}

void Workspace::pasteSelected()
{
    if (_scene->getSelection().size() > 1)
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
            if (!_scene->getSelection().empty())
            {
                iNodePtr destination = iNodeManager::getInstance().getNode(_scene->getSelection()[0]);
            }

            if (destination == nullptr)
            {
                destination = _rootUser;
            }

            destination->insertNodeAsync(newNode);
        }

        _copiedNodes.clear();
        _scene->setSelection(newSelection);
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
            if (!_scene->getSelection().empty())
            {
                destination = iNodeManager::getInstance().getNode(_scene->getSelection()[0]);
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

void Workspace::importFile(const iaString &filename)
{
    if (filename.isEmpty())
    {
        return;
    }

    iParameters parameters({{"name", filename},
                            {"type", iaString("model")},
                            {"cacheMode", iResourceCacheMode::Free},
                            {"joinVertexes", false},
                            {"keepMesh", true}});
    iNodeModel *model = iNodeManager::getInstance().createNode<iNodeModel>();
    model->setModel(iResourceManager::getInstance().loadResource<iModel>(parameters));

    if (model->isValid())
    {
        iNodePtr groupNode = nullptr;

        auto children = model->getChildren();
        if (children.size() > 1)
        {
            groupNode = iNodeManager::getInstance().createNode<iNode>();
            iaString groupName = "group:";
            groupName += filename;
            groupNode->setName(groupName);

            iNodePtr insertAt = nullptr;
            if (!getSelection().empty())
            {
                insertAt = iNodeManager::getInstance().getNode(getSelection()[0]);
            }

            if (insertAt == nullptr)
            {
                insertAt = getRootUser();
            }

            insertAt->insertNode(groupNode);
        }
        else
        {
            iNodePtr insertAt = nullptr;
            if (!getSelection().empty())
            {
                insertAt = iNodeManager::getInstance().getNode(getSelection()[0]);
            }

            if (insertAt != nullptr)
            {
                groupNode = insertAt;
            }
            else
            {
                groupNode = getRootUser();
            }
        }

        auto child = children.begin();
        while (child != children.end())
        {
            model->removeNode((*child));
            groupNode->insertNode((*child));
            child++;
        }
    }

    iNodeManager::getInstance().destroyNodeAsync(model);
}

void Workspace::importFileReference(const iaString &filename)
{
    if (filename.isEmpty())
    {
        return;
    }

    iParameters parameters({{"name", filename},
                            {"type", iaString("model")},
                            {"cacheMode", iResourceCacheMode::Free},
                            {"joinVertexes", false},
                            {"keepMesh", true}});
    iNodeModel *model = iNodeManager::getInstance().createNode<iNodeModel>();
    model->setModel(iResourceManager::getInstance().loadResource<iModel>(parameters));

    if (model->isValid())
    {
        iNodePtr insertAt = nullptr;

        if (!getSelection().empty())
        {
            insertAt = iNodeManager::getInstance().getNode(getSelection()[0]);
        }

        if (insertAt == nullptr)
        {
            insertAt = getRootUser();
        }

        insertAt->insertNode(model);
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
    iParameters parameters({{"id", iaUUID()},
                            {"filename", filename},
                            {"type", iaString("model")},
                            {"cacheMode", iResourceCacheMode::Free},
                            {"joinVertexes", false},
                            {"keepMesh", true}});
    model->setModel(iResourceManager::getInstance().loadResource<iModel>(parameters));

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
    return _scene->getSelection();
}

void Workspace::setSelection(const std::vector<iNodeID> &selection)
{
    _scene->setSelection(selection);
}

void Workspace::clearSelection()
{
    _scene->clearSelection();
}

iScenePtr Workspace::getScene() const
{
    return _scene;
}