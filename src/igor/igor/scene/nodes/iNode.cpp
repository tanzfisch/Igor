// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNode.h>

#include <igor/system/iApplication.h>
#include <igor/scene/iScene.h>
#include <iaux/system/iaConsole.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeTransform.h>

#include <sstream>

namespace igor
{

    iaIDGenerator64 iNode::_idGenerator;

    iNode::iNode()
    {
        _nodeID = iNode::_idGenerator.getNextID();
    }

    iNode::iNode(iNodePtr node)
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeID = iNode::_idGenerator.getNextID();
        setName(node->getName());
    }

    iNode::~iNode()
    {
        if (_parent != nullptr)
        {
            _parent->removeNode(this);
        }

        std::vector<iNodePtr> childrenCopy(_children);
        for (uint32 i = 0; i < childrenCopy.size(); ++i)
        {
            iNodeManager::getInstance().destroyNode(childrenCopy[i]);
        }
        _children.clear();

        std::vector<iNodePtr> inactiveChildrenCopy(_inactiveChildren);
        for (uint32 i = 0; i < inactiveChildrenCopy.size(); ++i)
        {
            iNodeManager::getInstance().destroyNode(inactiveChildrenCopy[i]);
        }
        _inactiveChildren.clear();
    }

    iTransformationChangeEvent &iNode::getTransformationChangeEvent()
    {
        return _transformationChangeEvent;
    }

    __IGOR_DISABLE_WARNING__(4100)
    void iNode::onPostCopyLink(std::map<iNodeID, iNodeID> &nodeIDMap)
    {
    }
    __IGOR_ENABLE_WARNING__(4100)

    bool iNode::isDataDirty()
    {
        return _queueToDirtyData;
    }

    void iNode::setDataDirty()
    {
        _queueToDirtyData = true;

        if (_scene != nullptr)
        {
            _scene->addToDataUpdateQueue(this);
        }
    }

    bool iNode::onUpdateData()
    {
        // does nothing
        return true;
    }

    bool iNode::isChild(iNodePtr child)
    {
        auto iter = _children.begin();
        while (iter != _children.end())
        {
            if ((*iter) == child)
            {
                return true;
            }
            iter++;
        }

        iter = _inactiveChildren.begin();
        while (iter != _inactiveChildren.end())
        {
            if ((*iter) == child)
            {
                return true;
            }
            iter++;
        }

        return false;
    }

    iNodePtr iNode::getChild(iNodeID id)
    {
        auto iter = _children.begin();
        while (iter != _children.end())
        {
            if ((*iter)->getID() == id)
            {
                return (*iter);
            }
            iter++;
        }

        iter = _inactiveChildren.begin();
        while (iter != _inactiveChildren.end())
        {
            if ((*iter)->getID() == id)
            {
                return (*iter);
            }
            iter++;
        }

        return 0;
    }

    iNodePtr iNode::getChild(const iaString &name)
    {
        auto iter = _children.begin();
        while (iter != _children.end())
        {
            if ((*iter)->getName() == name)
            {
                return (*iter);
            }
            iter++;
        }

        iter = _inactiveChildren.begin();
        while (iter != _inactiveChildren.end())
        {
            if ((*iter)->getName() == name)
            {
                return (*iter);
            }
            iter++;
        }

        return nullptr;
    }

    iNodeID iNode::getID() const
    {
        return _nodeID;
    }

    iNodeKind iNode::getKind() const
    {
        return _nodeKind;
    }

    iNodeType iNode::getType() const
    {
        return _nodeType;
    }

    bool iNode::isTransformationDirty()
    {
        return _transformationDirty;
    }

    void iNode::setTransformationDirty(bool dirty)
    {
        _transformationDirty = dirty;

        if (_transformationDirty)
        {
            if (isChild())
            {
                getParent()->setTransformationDirtyUp();
            }

            for (uint32 i = 0; i < _children.size(); ++i)
            {
                _children[i]->setTransformationDirtyDown();
            }
        }
        else
        {
            _transformationChangeEvent(this);
        }
    }

    void iNode::setTransformationDirtyUp()
    {
        if (!_transformationDirty)
        {
            _transformationDirty = true;

            if (getParent())
            {
                getParent()->setTransformationDirtyUp();
            }
        }
    }

    void iNode::setTransformationDirtyDown()
    {
        _transformationDirty = true;

        for (auto child : _children)
        {
            child->setTransformationDirtyDown();
        }
    }

    void iNode::calcWorldTransformation(iNodePtr currentNode, iaMatrixd &matrix)
    {
        if (currentNode->getType() == iNodeType::iNodeTransform)
        {
            iNodeTransform *tranformNode = static_cast<iNodeTransform *>(currentNode);
            iaMatrixd currentMatrix;
            tranformNode->getMatrix(currentMatrix);
            matrix = currentMatrix * matrix;
        }

        if (currentNode->isChild())
        {
            calcWorldTransformation(currentNode->getParent(), matrix);
        }
    }

    void iNode::calcWorldTransformation(iaMatrixd &matrix)
    {
        matrix.identity();
        calcWorldTransformation(this, matrix);
    }

    iNodePtr iNode::getParent()
    {
        return _parent;
    }

    void iNode::setParent(iNodePtr parent)
    {
        _parent = parent;
    }

    iaString iNode::getKindName(iNodeKind kind)
    {
        iaString result;

        switch (kind)
        {
        case iNodeKind::Node:
            result = "Node";
            break;
        case iNodeKind::Renderable:
            result = "Renderable";
            break;
        case iNodeKind::Volume:
            result = "Volume";
            break;
        case iNodeKind::Physics:
            result = "Physics";
            break;
        case iNodeKind::Light:
            result = "Light";
            break;
        case iNodeKind::Camera:
            result = "Camera";
            break;
        case iNodeKind::Transformation:
            result = "Transformation";
            break;
        case iNodeKind::Undefined:
        default:
            result = "Undefined";
            break;
        }

        return result;
    }

    iaString iNode::getTypeName(iNodeType nodeType)
    {
        std::wstringstream stream;
        stream << nodeType;
        return iaString(stream.str().data());
    }

    const iaString &iNode::getName() const
    {
        return _name;
    }

    std::vector<iaString> iNode::getInfo(bool brief) const
    {
        std::vector<iaString> info;

        iaString header = "\"";
        header += getName();
        header += "\" id:";
        header += iaString::toString(getID());
        header += " type:";
        header += iNode::getTypeName(getType());

        info.push_back(header);

        return info;
    }

    void iNode::setName(const iaString &name)
    {
        _name = name;
    }

    void iNode::setActiveAsync(bool active)
    {
        iNodeManager::getInstance().setActiveAsync(this, active);
    }

    void iNode::insertNodeAsync(iNodePtr node)
    {
        iNodeManager::getInstance().insertNodeAsync(this, node);
    }

    void iNode::removeNodeAsync(iNodePtr node)
    {
        iNodeManager::getInstance().removeNodeAsync(this, node);
    }

    void iNode::insertNode(iNodePtr node)
    {
        if (node == nullptr)
        {
            con_err("zero pointer");
            return;
        }

        if (node->isChild())
        {
            con_err("node is already a child");
            return;
        }

        node->_active = true;
        node->setParent(this);
        node->setScene(_scene);
        node->setTransformationDirty();
        _children.push_back(node);
    }

    bool iNode::isChild()
    {
        return _parent ? true : false;
    }

    __IGOR_DISABLE_WARNING__(4100)
    void iNode::onUpdateTransform(iaMatrixd &matrix)
    {
    }
    __IGOR_ENABLE_WARNING__(4100)

    void iNode::removeNode(iNodePtr node)
    {
        auto iter = _children.begin();

        while (iter != _children.end())
        {
            if ((*iter) == node)
            {
                node->setTransformationDirty();

                node->setParent(nullptr);
                node->setScene(nullptr);

                _children.erase(iter);
                break;
            }

            iter++;
        }
    }

    bool iNode::hasChildren()
    {
        return _children.size() ? true : false;
    }

    void iNode::getAllChildren(std::vector<iNodePtr> &children) const
    {
        children = _children;
        children.insert(children.end(), _inactiveChildren.begin(), _inactiveChildren.end());
    }

    const std::vector<iNodePtr> &iNode::getInactiveChildren() const
    {
        return _inactiveChildren;
    }

    const std::vector<iNodePtr> &iNode::getChildren() const
    {
        return _children;
    }

    bool iNode::isActive() const
    {
        return _active;
    }

    void iNode::setActive(bool active)
    {
        if (_active == active)
        {
            return;
        }

        _active = active;

        if (_parent == nullptr)
        {
            return;
        }

        if (!_active)
        {
            bool result = false;

            auto iter = _parent->_children.begin();
            while (iter != _parent->_children.end())
            {
                if ((*iter) == this)
                {
                    setTransformationDirty();

                    iApplication::getInstance().blockEvent(iEventType::iEventNodeRemovedFromScene);
                    setScene(nullptr);
                    iApplication::getInstance().unblockEvent(iEventType::iEventNodeRemovedFromScene);

                    _parent->_children.erase(iter);
                    _parent->_inactiveChildren.push_back(this);

                    result = true;
                    break;
                }

                iter++;
            }

            con_assert(result, "inconsistant data");
        }
        else
        {
            bool result = false;

            auto iter = _parent->_inactiveChildren.begin();
            while (iter != _parent->_inactiveChildren.end())
            {
                if ((*iter) == this)
                {
                    iApplication::getInstance().blockEvent(iEventType::iEventNodeAddedToScene);
                    setScene(_parent->getScene());
                    iApplication::getInstance().unblockEvent(iEventType::iEventNodeAddedToScene);
                    setTransformationDirty();

                    _parent->_inactiveChildren.erase(iter);
                    _parent->_children.push_back(this);

                    result = true;
                    break;
                }

                iter++;
            }

            con_assert(result, "inconsistant data");
        }
    }

    void iNode::setScene(iScenePtr scene)
    {
        if (_scene != scene)
        {
            onPreSetScene();

            if (_scene != nullptr)
            {
                _scene->signalNodeRemoved(this);
            }

            if (scene != nullptr &&
                _scene != nullptr)
            {
                con_err("node already belongs to scene \"" << scene->getName() << "\"");
            }
            else
            {
                if (scene != _scene)
                {
                    _scene = scene;

                    if (hasChildren())
                    {
                        for (uint32 i = 0; i < _children.size(); ++i)
                        {
                            _children[i]->setScene(_scene);
                        }
                    }
                }
            }

            onPostSetScene();

            if (_scene != nullptr)
            {
                _scene->signalNodeAdded(this);
            }
        }
    }

    iScenePtr iNode::getScene()
    {
        return _scene;
    }

    void iNode::onPreSetScene()
    {
        // nothing to do
    }

    void iNode::onPostSetScene()
    {
        // nothing to do
    }

    std::wostream &operator<<(std::wostream &stream, const iNodeType &nodeType)
    {
        const static std::wstring text[] = {
            L"iNode",
            L"iNodeCamera",
            L"iCelestialNode",
            L"iNodeLight",
            L"iNodeMesh",
            L"iNodeModel",
            L"iNodeRender",
            L"iNodeSkyBox",
            L"iSkyLightNode",
            L"iNodeTransform",
            L"iNodeSwitch",
            L"iNodeLODSwitch",
            L"iNodeLODTrigger",
            L"iNodePhysics",
            L"iNodeParticleSystem",
            L"iNodeEmitter",
            L"iNodeAudioListener",
            L"iNodeAudioSource",
            L"Undefined"};

        stream << text[static_cast<int>(nodeType)];
        return stream;
    }

    std::wostream &operator<<(std::wostream &stream, const iNodeKind &nodeKind)
    {
        const static std::wstring text[] = {
            L"Node",
            L"Renderable",
            L"Volume",
            L"Physics",
            L"Light",
            L"Camera",
            L"Transformation",
            L"Audio",
            L"Undefined"};

        stream << text[static_cast<int>(nodeKind)];
        return stream;
    }

}; // namespace igor
