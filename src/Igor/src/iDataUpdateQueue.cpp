// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iDataUpdateQueue.h>

#include <iNodeModel.h>
#include <iNodeFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    void iDataUpdateQueue::addNode(uint32 nodeID)
    {
        _mutex.lock();

        auto iter = find(_loadingQueue.begin(), _loadingQueue.end(), nodeID);
        if (iter != _loadingQueue.end())
        {
            con_err("this node was already added to update queue");
        }
        else
        {
            _loadingQueue.push_back(nodeID);
        }

        _mutex.unlock();
    }

    void iDataUpdateQueue::process()
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
            iNode* node = iNodeFactory::getInstance().getNode((*iterP));
            if (node != nullptr)
            {
                if (node->onUpdateData())
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