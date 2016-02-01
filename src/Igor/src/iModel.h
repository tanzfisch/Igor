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

#ifndef __iMODEL__
#define __iMODEL__

#include <iDefines.h>
#include <iTask.h>

#include <iaString.h>
#include <iaMemBlock.h>
using namespace IgorAux;

namespace Igor
{

	class iNode;

    /*! state of model
    */
	enum class iModelState
	{
		NotLoaded,
		LoadFailed,
		Loaded
	};

    enum class iModelSourceType
    {
        File,
        Generated
    };

    /*! optional parameters hwo to load or generate data
    */
    struct Igor_API iModelDataInputParameter
    {
        /*! loader or generator identifier
        */
        iaString _identifier;

        /*! the general type of model source
        */
        iModelSourceType _modelSourceType = iModelSourceType::Generated;

        /*! where or not we need the render context to load this kind of data
        */
        bool _needsRenderContext = true;

        /*! if ture this model is relevant to physics
        */
        bool _passToPhysics = false;

        /*! loading priority of model
        */
        uint32 _loadPriority = iTask::DEFAULT_PRIORITY;

        /*! wether or not vertexes have to be joined
        */
        bool _joinVertexes = true;

        /*! custom parameters for loader or generator
        */
        iaMemBlock _parameters;
    };
	
    /*! Represents model data loaded from filesystem.
    */
	class iModel
	{

		friend class iModelResourceFactory;
        friend class iTaskLoadModel;

    public:

        /*! \returns filename of model
        */
        const iaString& getName() const;

        /*! \returns optional loading or generation parameters
        */
        iModelDataInputParameter* getParameters();

        /*! \returns copy to node tree data
        */
        iNode* getDataCopy();

        /*! \returns model state
        */
        iModelState getState();

	private:

		/*! model file name
		*/
		iaString _name;
        
		/*! node data
		*/
		iNode* _data = nullptr;

        /*! optional load or generate parameter
        */
        iModelDataInputParameter* _parameter = nullptr;

        /*! state of model
        */
		iModelState _state = iModelState::NotLoaded;

		/*! set data
		*/
		void setData(iNode* node);

        /*! \returns pointer to node tree data
        */
        iNode* getData();

        /*! sets model state

        \param state the new model state
        */
		void setState(iModelState state);

		/*! ctor

        \param 
		*/
        iModel(const iaString& name, iModelDataInputParameter* _parameter = nullptr);

        /*! dtor
        */
        virtual ~iModel();

        /*! so the shared_ptr is able to call the private destructor indirectly

        source: http://nealabq.com/blog/2008/11/28/factory/
        */
        __IGOR_DISABLE_WARNING__(4091);
        static struct private_deleter
        {
            void operator()(iModel* p)
            {
                delete p;
            }
        };
        __IGOR_ENABLE_WARNING__(4091);

	};

}

#endif
