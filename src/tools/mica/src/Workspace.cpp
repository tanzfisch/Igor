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

void Workspace::loadFile(const iaString &filename)
{
    clear();

    if (!filename.isEmpty())
    {
        iNodeModel *model = iNodeManager::getInstance().createNode<iNodeModel>();
        iModelDataInputParameter *parameter = createDataInputParameter();

        model->setModel(filename, iResourceCacheMode::Free, parameter);
        _rootUser->insertNode(model);
    }
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