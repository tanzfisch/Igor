// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNode.h>

#include <iScene.h>
#include <iaConsole.h>
#include <iNodeFactory.h>
#include <iNodeTransform.h>

namespace Igor
{

	uint32 iNode::_nextID = iNode::INVALID_NODE_ID + 1;
    mutex iNode::_mutexID;

	iNode::iNode()
	{
        iNode::_mutexID.lock();
		_nodeID = iNode::_nextID++;
        iNode::_mutexID.unlock();
	}

    iNode::iNode(iNode* node)
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeID = iNode::_nextID++;
        setName(node->getName());
    }

	iNode::~iNode()
	{
        if (_parent != nullptr)
        {
            _parent->removeNode(this);
        }

        vector<iNode*> childrenCopy(_children);
		for (uint32 i = 0; i < childrenCopy.size(); ++i)
		{
			iNodeFactory::getInstance().destroyNode(childrenCopy[i]);
		}
		_children.clear();

        vector<iNode*> inactiveChildrenCopy(_inactiveChildren);
        for (uint32 i = 0; i < inactiveChildrenCopy.size(); ++i)
        {
            iNodeFactory::getInstance().destroyNode(inactiveChildrenCopy[i]);
        }
        _inactiveChildren.clear();
	}

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

    bool iNode::isChild(iNode* child)
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

    iNode* iNode::getChild(uint32 id)
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

    iNode* iNode::getChild(iaString name)
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

    uint32 iNode::getID() const
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
		if (!_transformationDirty)
		{
			_transformationDirty = true;

			if (hasChildren())
			{
				for (auto child : _children)
				{
                    child->setTransformationDirtyDown();
				}
			}
		}
	}

    void iNode::calcWorldTransformation(iNode* currentNode, iaMatrixf& matrix)
    {
        if (currentNode->getType() == iNodeType::iNodeTransform)
        {
            iNodeTransform* tranformNode = static_cast<iNodeTransform*>(currentNode);
            iaMatrixf currentMatrix;
            tranformNode->getMatrix(currentMatrix);
            matrix = currentMatrix * matrix;
        }

        if (currentNode->isChild())
        {
            calcWorldTransformation(currentNode->getParent(), matrix);
        }
    }

    void iNode::calcWorldTransformation(iaMatrixf& matrix)
    {
        matrix.identity();
        calcWorldTransformation(this, matrix);
    }

	iNode* iNode::getParent()
	{
		return _parent;
	}

	void iNode::setParent(iNode* parent)
	{
		_parent = parent;
	}

	iaString iNode::getName() const
	{
		return _name;
	}

	void iNode::setName(iaString name)
	{
		_name = name;
	}

    void iNode::insertNodeAsync(iNode* node)
    {
        iNodeFactory::getInstance().insertNodeAsync(this, node);
    }

    void iNode::removeNodeAsync(iNode* node)
    {
        iNodeFactory::getInstance().removeNodeAsync(this, node);
    }

	void iNode::insertNode(iNode* node)
	{
		if (!node->isChild())
		{
            node->_active = true;
			node->setParent(this);
			node->setScene(_scene);
            node->setTransformationDirty();
			_children.push_back(node);
		}
		else
		{
			con_err("node is already a child");
		}
	}

	bool iNode::isChild()
	{
		return _parent ? true : false;
	}

    void iNode::onUpdateTransform(iaMatrixf& matrix)
    {
        // does nothing
    }

	void iNode::removeNode(iNode* node)
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

    vector<iNode*>& iNode::getInactiveChildren()
    {
        return _inactiveChildren;
    }

	vector<iNode*>& iNode::getChildren()
	{
		return _children;
	}

    bool iNode::isActive() const
    {
        return _active;
    }

    void iNode::setActive(bool active)
    {
        if (_active != active)
        {
            _active = active;

            if (!_active)
            {
                if (_parent != nullptr)
                {
                    bool result = false;
                    
                    auto iter = _parent->_children.begin();
                    while (iter != _parent->_children.end())
                    {
                        if ((*iter) == this)
                        {
                            setTransformationDirty();
                            setScene(nullptr);

                            _parent->_children.erase(iter);
                            _parent->_inactiveChildren.push_back(this);

                            result = true;
                            break;
                        }

                        iter++;
                    }

                    con_assert(result, "inconsistant data");
                }
            }
            else
            {
                if (_parent != nullptr)
                {
                    bool result = false;

                    auto iter = _parent->_inactiveChildren.begin();
                    while (iter != _parent->_inactiveChildren.end())
                    {
                        if ((*iter) == this)
                        {
                            setScene(_parent->getScene());
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
        }
    }

	void iNode::setScene(iScene* scene)
	{
        if (_scene != scene)
        {
            onPreSetScene();

            if (_scene != nullptr)
            {
                _scene->signalNodeRemoved(getID());
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
                _scene->signalNodeAdded(getID());
            }
        }
	}

	iScene* iNode::getScene()
	{
		return _scene;
	}

    void iNode::onPreSetScene()
    {
    }

    void iNode::onPostSetScene()
    {
    }

};
