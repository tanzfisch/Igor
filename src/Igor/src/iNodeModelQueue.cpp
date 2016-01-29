// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeModelQueue.h>

#include <iNodeModel.h>
#include <iNodeFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    void iNodeModelQueue::addModelNode(iNodeModel* modelnode)
    {
        _mutex.lock();

        auto iter = find(_loadingQueue.begin(), _loadingQueue.end(), modelnode->getID());
        if (iter != _loadingQueue.end())
        {
            con_err("this node was already add to loading queue");
        }
        else
        {
            _loadingQueue.push_back(modelnode->getID());
        }

        _mutex.unlock();
    }

    void iNodeModelQueue::process()
    {
        _mutex.lock();

        auto iter = _loadingQueue.begin();
        while (iter != _loadingQueue.end())
        {
            _processingQueue.push_back((*iter));
            iter = _loadingQueue.erase(iter);
        }

        _mutex.unlock();

        auto iterP = _processingQueue.begin();
        while (iterP != _processingQueue.end())
        {
            iNodeModel* modelNode = static_cast<iNodeModel*>(iNodeFactory::getInstance().getNode((*iterP)));
            if (modelNode != nullptr)
            {
                if (modelNode->updateModelData())
                {
                    iterP = _processingQueue.erase(iterP);
                }
                else
                {
                    iterP++;
                }
            }
            else
            {
                // node was destroyed in the mean time
                iterP = _processingQueue.erase(iterP);
            }
        }
    }
}