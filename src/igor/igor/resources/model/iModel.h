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
// (c) Copyright 2012-2023 by Martin Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_MODEL__
#define __IGOR_MODEL__

#include <igor/iDefines.h>
#include <igor/threading/tasks/iTask.h>
#include <igor/scene/nodes/iNode.h>

#include <iaux/data/iaString.h>
using namespace iaux;

#include <any>

namespace igor
{

    /*! state of model
    */
    enum class iModelState
    {
        NotLoaded,
        LoadFailed,
        Loaded
    };

    /*! source type of model
    */
    enum class iModelSourceType
    {
        File,
        Generated,
        Unknown
    };

    /*! optional parameters hwo to load or generate data
    */
    struct IGOR_API iModelDataInputParameter
    {
        /*! loader or generator identifier
        */
        iaString _identifier;

        /*! the general type of model source
        */
        iModelSourceType _modelSourceType = iModelSourceType::Unknown;

        /*! where or not we need the render context to load this kind of data
        */
        bool _needsRenderContext = true;

        /*! loading priority of model
        */
        uint32 _loadPriority = iTask::DEFAULT_PRIORITY;

        /*! wether or not vertexes have to be joined
        */
        bool _joinVertexes = false;

        /*! custom parameters for loader or generator
        */
        std::any _parameters;

        /*! keep mesh flag. if true the mesh will not be deleted after buffer creation

		only if kept it can ba saved to disk
        */
        bool _keepMesh = false;
    };

    /*! model data input parameter pointer definition
    */
    typedef std::shared_ptr<iModelDataInputParameter> iModelDataInputParameterPtr;

    /*! represents model data loaded or generated

    \todo maybe we should replace pointers to nodes with IDs here
    */
    class IGOR_API iModel
    {

        friend class iModelResourceFactory;
        friend class iTaskLoadModel;

    public:
        /*! \returns filename of model
        */
        const iaString &getName() const;

        /*! \returns optional loading or generation parameters

        There might not be any parameters then it will return nullptr
        */
        iModelDataInputParameterPtr getParameters() const;

        /*! \returns copy to node tree data
        */
        iNodePtr getNodeCopy();

        /*! \returns pointer to node tree data

        only use this if you know what you are doing
        */
        iNodePtr getNode();

        /*! \returns model state
        */
        iModelState getState();

        /*! dtor
        */
        virtual ~iModel();

    private:
        /*! model file name
		*/
        iaString _name;

        /*! node data
		*/
        iNodePtr _node = nullptr;

        /*! optional load or generate parameter
        */
        iModelDataInputParameterPtr _parameter;

        /*! cache mode. initialized in ctor
        */
        iResourceCacheMode _cacheMode;

        /*! \returns cache mode
        */
        iResourceCacheMode getCacheMode() const;

        /*! state of model
        */
        iModelState _state = iModelState::NotLoaded;

        /*! set node
		*/
        void setNode(iNodePtr node);

        /*! sets model state

        \param state the new model state
        */
        void setState(iModelState state);

        /*! ctor

        \param 
		*/
        iModel(const iaString &name, iResourceCacheMode cacheMode, iModelDataInputParameterPtr parameter = iModelDataInputParameterPtr());
    };

    /*! definition of model pointer
	*/
    typedef std::shared_ptr<iModel> iModelPtr;

} // namespace igor

#endif
