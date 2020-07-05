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