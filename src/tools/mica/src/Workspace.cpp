#include "Workspace.h"

Workspace::Workspace()
{
    _scene = iSceneFactory::getInstance().createScene();
}

Workspace::~Workspace()
{
    iSceneFactory::getInstance().destroyScene(_scene);
}

iNodePtr Workspace::getRoot() const
{
    return _scene->getRoot();
}

void Workspace::clear()
{
    clearSelection();

    iNodePtr root = _scene->getRoot();
    if (root != nullptr)
    {
        auto children = root->getChildren();
        for (auto child : children)
        {
            iNodeManager::getInstance().destroyNodeAsync(child);
        }
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