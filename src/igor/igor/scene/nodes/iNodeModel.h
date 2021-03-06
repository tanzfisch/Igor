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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IGOR_MODELNODE_H__
#define __IGOR_MODELNODE_H__

#include <igor/scene/nodes/iNode.h>
#include <igor/resources/model/iModel.h>
#include <igor/resources/material/iMaterial.h>

#include <iaux/data/iaString.h>
#include <iaux/system/iaEvent.h>
using namespace iaux;

namespace igor
{

    struct iModelDataInputParameter;

    /*! event triggered when model was loaded
    */
    iaEVENT(iModelReadyEvent, iModelReadyDelegate, (iNodeID modelNodeID), (modelNodeID));

    /*! represents a model within the scene

    \todo would be nice to have a callback if a model was asynchronously loaded
    */
    class IGOR_API iNodeModel : public iNode
    {

        friend class iNodeManager;
        friend class iNodeVisitorUpdateTransform;

    public:
        /*! sets model by filename and or generator parameters

        \param modelName can be a filename or any unique identity name
        \param cacheMode hint for how the resource manager is handling to cache this resource
        \param parameters optional loading parameters (must be allocated on heap and will be consumed by library)
        \param loadSynchronously load model immediately ohtherwise model loads asynchonously
        */
        void setModel(const iaString &modelName, iResourceCacheMode cacheMode = iResourceCacheMode::Cache, iModelDataInputParameter *parameters = nullptr, bool loadSynchronously = false);

        /*! \returns filename of model
        */
        iaString getModelName() const;

        /*! \returns true if model data is ready to render
        */
        bool isValid();

        /*! \returns true if model was loaded or attempted to be loaded
		*/
        bool isLoaded();

        /*! register delegate to model loaded event

        \param delegate the delegate to register
        */
        void registerModelReadyDelegate(iModelReadyDelegate delegate);

        /*! unregister delegate from model loaded event

        \param delegate the delegate to unregister
        */
        void unregisterModelReadyDelegate(iModelReadyDelegate delegate);

        /*! sets material of all mesh nodes under this model

        \param materialID the material id to set
        */
        void setMaterial(iMaterialID materialID);

    private:
        /*! event triggered when model was loaded and ready for rendering
        */
        iModelReadyEvent _modelReadyEvent;

        /*! filename of model
        */
        iaString _filename;

        /*! loading parameters

        can't delete them they belong (usually) to application
        */
        iModelDataInputParameter *_parameters;

        /*! cache mode for model to load
        */
        iResourceCacheMode _cacheMode;

        /*! shared poitner to requested model
        */
        iModelPtr _model;

        /*! true if requested model was actually loaded and subtree created
        */
        bool _loaded = false;

        /*! true if loaded and buffers are generated
        */
        bool _ready = false;

        /*! this is called just before setScene and gives the class the chance to unregister from the current scene if set.
        */
        virtual void onPreSetScene();

        /*! this is called just after setScene and gives the class the chance to register it self to the new scene.
        */
        virtual void onPostSetScene();

        /*! checks if model data was loaded and creates corresponding subtree

        \returns true if model data was present
        */
        bool onUpdateData() override;

        /*! checks if mesh buffers are generated and read for rendering

        \returns true if meshs are all ready for rendering
        */
        bool checkForBuffers();

        bool checkForBuffers(iNodePtr node);

        /*! called by update transform run

        ignores the matrix

        \param matrix world matrix for the camera
        */
        void onUpdateTransform(iaMatrixd &matrix);

        /*! recursively setting material on all mesh nodes

        \param node the current node
        \param materialID the material to set
        */
        void setMaterial(iNodePtr node, iMaterialID materialID);

        /*! initializes memeber variables
        */
        iNodeModel();

        /*! copy ctor
        */
        iNodeModel(iNodeModel *node);

        /*! release model data
        */
        virtual ~iNodeModel();
    };

    /*! model node pointer definition
    */
    typedef iNodeModel *iNodeModelPtr;

} // namespace igor

#endif // __IGOR_MODELNODE_H__
