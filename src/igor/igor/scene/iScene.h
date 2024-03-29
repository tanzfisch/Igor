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

#ifndef __IGOR_SCENE__
#define __IGOR_SCENE__

#include <iaux/data/iaString.h>
using namespace iaux;

#include <igor/iDefines.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/scene/traversal/iNodeVisitorUpdateTransform.h>
#include <igor/data/iFrustum.h>

#include <memory>
#include <vector>
#include <set>

namespace igor
{

    class iNode;
    class iNodeCamera;
    class iNodeLight;
    typedef iNodeLight *iNodeLightPtr;
    class iNodeVolume;
    class iNodeVisitorUpdateData;
    class iOctree;
    typedef iOctree *iOctreePtr;
    class iNodeRender;
    typedef iNodeRender *iNodeRenderPtr;
    class iNodeLODTrigger;
    class iNodeLODSwitch;
    class iEvaluator;
    typedef iEvaluator *iEvaluatorPtr;

    /*! the scene graph

	\todo we should use shared pointers to nodes or just use IDs
	*/
    class IGOR_API iScene
    {
        friend class iNode;
        friend class iNodeCamera;
        friend class iNodeLight;
        friend class iNodeVolume;
        friend class iNodeModel;
        friend class iNodeVisitorUpdateTransform;
        friend class iSceneFactory;
        friend class iNodeRender;
        friend class iNodeLODTrigger;
        friend class iNodeLODSwitch;
        friend class iRenderEngine;

    public:
        /*! \returns scene name
		*/
        const iaString &getName() const;

        /*! stes name of scene

		\param name new name of scene
		*/
        void setName(const iaString &name);

        /*! \returns root node
		*/
        iNodePtr getRoot() const;

        /*! \returns list of registerred lights
		*/
        const std::vector<iNodeLightPtr> &getLights() const;

        /*! \returns list of render-ables

		does only contain render-ables that are not volumes
		*/
        const std::vector<iNodeRenderPtr> &getRenderables() const;

        /*! cyclic update of scene.

		handles transformation updates and resulting changes in octree.
		data that was loaded will now be added to scene
		*/
        void handle();

        /*! adds node to data update queue

        // TODO why is this here?

		\param node the node to add
		*/
        void addToDataUpdateQueue(iNodePtr node);

        /*! \returns list of selected nodes
		*/
        const std::vector<iNodeID> &getSelection() const;

        /*! sets the current selection
		*/
        void setSelection(const std::vector<iNodeID> &selection);

        /*! clear current selection
		*/
        void clearSelection();

    private:
        /*! sync with data loading workers
		*/
        iaMutex _mutex;

        /*! contains model nodes that just got inserted or changed
		*/
        std::vector<iNodeID> _loadingQueue;

        /*! contains model nodes to be processed in current frame

		need to store them separately because they might not all be processed within one frame
		*/
        std::vector<iNodeID> _processingQueue;

        /*! name of scene
		*/
        iaString _name = L"Scene";

        /*! root node in scene.

		there can only be one root node per scene
		*/
        iNodePtr _root = nullptr;

        /*! octree
		*/
        iOctree *_octree = nullptr;

        /*! mutex for octree access
        */
        iaMutex _mutexOctree;

        /*! list of registered cameras to the scene
		*/
        std::vector<iNodeID> _cameras;

        /*! list of registered lod triggers
		*/
        std::vector<iNodeLODTrigger *> _lodTriggers;

        /*! list of registered lod switches
		*/
        std::vector<iNodeLODSwitch *> _lodSwitches;

        /*! list of registered lights to the scene
		*/
        std::vector<iNodeLight *> _lights;

        /*! list of registered volumes to the scene
		*/
        std::vector<iNodeVolume *> _volumes;

        /*! list of registered renderables to the scene
		*/
        std::vector<iNodeRender *> _renderables;

        /*! list of evaluators
        */
        std::vector<iEvaluatorPtr> _evaluators;

        /*! transformation update visitor
		*/
        iNodeVisitorUpdateTransform _updateTransformVisitor;

        /*! currently selected nodes
		*/
        std::vector<iNodeID> _selectedNodes;

        /*! if true update data will be aborted
        */
        bool _abortUpdateData = false;

        /*! if true blocks any signals from scene
        */
        bool _blockSignals = false;

        /*! sets frustum on octree

        \param frustum the frustum to cull the scene with
        */
        void setFrustum(const iFrustumd &frustum);

        /*! returns cull result of scene

        \param[out] cullResult the result after culling the scene
        */
        void getCullResult(std::vector<void *> &cullResult);

        /*! draws the octree for debugging
        */
        void drawOctree();

        /*! handles dirty data ans tries to update it
		*/
        void updateData();

        /*! aborts update data
        */
        void abortUpdateData();

        /*! registers a camera node to the scene so it can be actually used as camera.

		\param camera node to register a camera
		*/
        void registerCamera(iNodeCamera *camera);

        /*! unregisters a camera node from scene

		\param camera node to be unregistered
		*/
        void unregisterCamera(iNodeCamera *camera);

        /*! registers a LOD trigger node

		\param trigger the trigger node to add
		*/
        void registerLODTrigger(iNodeLODTrigger *trigger);

        /*! unregisters a LOD trigger node

		\param trigger the trigger node to remove
		*/
        void unregisterLODTrigger(iNodeLODTrigger *trigger);

        /*! registers a LOD switch node

		\param switchNode the switch node to add
		*/
        void registerLODSwitch(iNodeLODSwitch *switchNode);

        /*! unregisters a LOD switch node

		\param switchNode the trigger node to remove
		*/
        void unregisterLODSwitch(iNodeLODSwitch *switchNode);

        /*! updates lod switch nodes
		*/
        void updateLOD();

        /*! register light node to scene.

		\param light node to be registered as light
		*/
        void registerLight(iNodeLight *light);

        /*! unregister light node from scene

		\param light node to be unregisterred
		*/
        void unregisterLight(iNodeLight *light);

        /*! register render-able to scene

		\param node node to be registered as render-able
		*/
        void registerRenderable(iNodeRender *node);

        /*! unregister render-able from scene

		\param node node to be unregistered from scene
		*/
        void unregisterRenderable(iNodeRender *node);

        /*! register volume node to scene

		\param volume node to be registered to scene
		*/
        void registerVolume(iNodeVolume *volume);

        /*! unregister volume node from scene

		\param volume node to be unregistered from scene
		*/
        void unregisterVolume(iNodeVolume *volume);

        /*! updates volume position in octree

		\param volume node to be updated in octree
		*/
        void updateVolume(iNodeVolume *volume);

        /*! signals the scene that a node was added

		\param node the node that was added
		*/
        void signalNodeAdded(iNodePtr node);

        /*! signals the scene that a node was removed

		\param node the node that was removed
		*/
        void signalNodeRemoved(iNodePtr node);

        /*! initializes scene and octree
		*/
        iScene();

        /*! releases memory
		*/
        virtual ~iScene();
    };

    /*! scene pointer definition
	*/
    typedef iScenePtr iScenePtr;

}; // namespace igor

#endif
