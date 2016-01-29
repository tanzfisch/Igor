//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2014-2015 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it   
// under the terms of the GNU Lesser General Public License as published by  
// the Free Software Foundation; either version 3 of the License, or (at   
// your option) any later version.                                           
// 
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
// 
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
// 
// contact: martinloga@gmx.de  

#ifndef __iMODELRESOURCEFACTORY__
#define __iMODELRESOURCEFACTORY__

#include <iModel.h>
#include <iModelDataIOOMPF.h>
#include <iModelDataIOOBJ.h>

#include <iaSingleton.h>
#include <iaMemBlock.h>
using namespace IgorAux;

#include <map>
#include <memory>
using namespace std;

namespace Igor
{

    class iWindow;
	class iNode;

    __IGOR_FUNCTION_POINTER__(CreateModelDataIOInstance, __IGOR_DEFAULTCALL__, iModelDataIO*, ());

    /*! model resource factory
    */
	class Igor_API iModelResourceFactory : public iaSingleton<iModelResourceFactory>
	{

		friend class iaSingleton<iModelResourceFactory>;
        friend class iTaskLoadModel;

    public:

        /*! requests a model to be loaded asynchronously

        if there is no separate thread that runs iTaskFlushResources present you have to run iModelResourceFactory::flush manually

        \param filename the file name of the model to be loaded if generator is used this is just a name but it must be unique
        \param parameter optional generator parameters
        \returns shared pointer to model
        */
        shared_ptr<iModel> requestModelData(const iaString& filename, iModelDataInputParameter* parameter = nullptr);

        /*! loads a model synchronously

        con only be called from main thread

        \param filename the file name of the model to be loaded
        \param instancing true: instancing will be used; false: instancing will only be used if already configured by the model to be loaded
        \returns shared pointer to model
        */
        shared_ptr<iModel> loadModelData(const iaString& filename, iModelDataInputParameter* parameter = nullptr);

        /*! loads unloads models depending on theirs request status ans reference count

        \param maxModelsAtOnce maximum amount of models zu flush at once
        \returns true if there was still work to do
        */
        bool flush(iWindow* window);

        /*! if a flush in a different thread is currently running. this will stop him asap
        */
        void interruptFlush();

        /*! exports model in specified format

        \param formatIdentifier identifier of which exporter to use
        */
        void exportModelData(iaString filename, iNode* node, const iaString& formatIdentifier = "ompf", iSaveMode saveMode = iSaveMode::KeepExternals);

        /*! registers generator

        \param generator generator to register
        */
        void registerModelDataIO(const iaString& identifier, CreateModelDataIOInstance functionPointer);

        /*! unregisters generator

        \param generator generator to unregister
        */
        void unregisterModelDataIO(const iaString& identifier);

	private:

        /*! mutex to lock model data list
        */
		mutex _mutexModels;

        /*! mutex to save the model queue
        */
        mutex _mutexModelQueue;

        /*! mutex to lock model data IO list
        */
        mutex _mutexDataIOs;

        /*! map of models
        */
		map<int64, shared_ptr<iModel>> _models;

        /*! contains models that habe to be loaded asynchronously
        */
        vector<shared_ptr<iModel>> _loadingQueue;

        /*! list of running tasks triggered by resource factory
        */
        vector<uint64> _runningTasks;

        /*! mutex to save the running tasks list
        */
        mutex _mutexRunningTasks;

        /*! flag to interrupt flush cross threads
        */
        bool _interrupFlush = false;

        /*! map of model data IOs

        key hash value of model data io identifier
        value function pointer to model data io creator
        */
        map<int64, CreateModelDataIOInstance> _modelDataIOCreators;

        /*! figures out what format we are dealing with

        \param filename file name of model to be loaded
        \returns format identifier
        */
        iModelDataIO* getModelDataIO(const iaString& identifier);

        /*! called after task was finished

        \param taskID id of the task that was finished
        */
        void onTaskFinished(uint64 taskID);

        /*! actually loading model using model loaders for specified formats

        \param filename file name of model to load
        \param parameter optional parameters
        */
        iNode* loadData(iaString filename, iModelDataInputParameter* parameter);

        /*! calculates hash value for given string

        \param text source string
        \returns hash value
        */
        int64 calcHashValue(const iaString& text);

        /*! does nothing
        */
		iModelResourceFactory();

        /*! releases all models. if not all models can be released print error message
        */
		virtual ~iModelResourceFactory();

	};

}

#endif
