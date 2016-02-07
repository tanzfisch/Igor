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

#ifndef __iSCENE__
#define __iSCENE__

#include <iaString.h>
#include <iaEvent.h>
using namespace IgorAux;

#include <iDefines.h>
#include <iNodeVisitorUpdateTransform.h>

#include<memory>
#include<vector>
using namespace std;

namespace Igor
{

	class iNode;
	class iNodeCamera;
	class iNodeLight;
	class iNodeVolume;
	class iNodeVisitorUpdateData;
	class iOctree;
    class iNodeRender;
    class iNodeLODTrigger;
    class iNodeLODSwitch;

    /*! event triggered when node was added to scene

    \param nodeID node that was added to scene
    */
    iaEVENT(iAddedNodeEvent, iAddedNodeDelegate, void, (uint32 nodeID), (nodeID));

    /*! event triggered when node was removed from scene

    \param nodeID node that was removed from scene
    */
    iaEVENT(iRemovedNodeEvent, iRemovedNodeDelegate, void, (uint32 nodeID), (nodeID));

    /*! the scene graph

    \todo we should use shared pointers to nodes
    */
	class Igor_API iScene
	{

        friend class iNode;
		friend class iNodeCamera;
		friend class iNodeLight;
		friend class iNodeVolume;
		friend class iNodeVisitorUpdateTransform;
        friend class iSceneFactory;
        friend class iNodeRender;
        friend class iNodeLODTrigger;
        friend class iNodeLODSwitch;

    public:

        /*! \returns scene name
        */
        iaString getName() const;

        /*! stes name of scene

        \param name new name of scene
        */
        void setName(const iaString& name);

        /*! \returns root node
        */
        iNode* getRoot();

        /*! \returns octree
        */
        iOctree* getOctree();

        /*! \returns current camera
        */
        iNodeCamera* getCamera();

        /*! \returns list of registerred lights
        */
        vector<iNodeLight*>& getLights();

        /*! \returns list of renderables

        does only contain renderables that are not volumes
        */
        vector<iNodeRender*>& getRenderables();

        /*! registers delegate to added node event

        \param addedNodeDelegate delegate to register
        */
        void registerAddedNodeDelegate(iAddedNodeDelegate addedNodeDelegate);

        /*! unregisters delegate from added node event

        \param addedNodeDelegate delegate to unregister
        */
        void unregisterAddedNodeDelegate(iAddedNodeDelegate addedNodeDelegate);

        /*! registers delegate to removed node event

        \param removedNodeDelegate delegate to register
        */
        void registerRemovedNodeDelegate(iRemovedNodeDelegate removedNodeDelegate);

        /*! unregisters delegate from removed node event

        \param removedNodeDelegate delegate to unregister
        */
        void unregisterRemovedNodeDelegate(iRemovedNodeDelegate removedNodeDelegate);

        /*! cyclic update of scene.

        handles transformation updates and resulting changes in octree.
        data that was loaded will now be added to scene
        */
        void handle();

	private:

        uint32 _updateLODSectionID = 0;
        uint32 _processModelNodesSectionID = 0;
        uint32 _updateTransformSectionID = 0;

        /*! added node event
        */
        iAddedNodeEvent _addedNode;

        /*! removed node event
        */
        iRemovedNodeEvent _removedNode;

        /*! name of scene
        */
		iaString _name = L"Scene";

        /*! root node in scene.

        there can only be one root node per scene
        */
		iNode* _root = nullptr;

        /*! octree
        */
        iOctree* _octree = nullptr;

        /*! current camera in use
        */
		iNodeCamera* _camera = nullptr;

        /*! list of registered cameras to the scene
        */
		vector<iNodeCamera*> _cameras;

        /*! list of registered lod triggers
        */
        vector<iNodeLODTrigger*> _lodTriggers;

        /*! list of registered lod switches
        */
        vector<iNodeLODSwitch*> _lodSwitches;

        /*! list of registered lights to the scene
        */
        vector<iNodeLight*> _lights;

        /*! list of registered volumes to the scene
        */
        vector<iNodeVolume*> _volumes;

        /*! list of registered renderables to the scene
        */
        vector<iNodeRender*> _renderables;

        /*! transformation update visitor
        */
		iNodeVisitorUpdateTransform _updateTransformVisitor;

        /*! registers a camera node to the scene so it can be actually used as camera. 

        \param camera node to register a camera
        */
		void registerCamera(iNodeCamera* camera);

        /*! unregisters a camera node from scene

        \param camera node to be unregistered
        */
        void unregisterCamera(iNodeCamera* camera);

        /*! set a camera as default camera

        does print error if camera is not registered to scene

        \param camera pointer to camera that is the current one now
        */
		void setCamera(iNodeCamera* camera);

        /*! registers a LOD trigger node

        \param trigger the trigger node to add
        */
        void registerLODTrigger(iNodeLODTrigger* trigger);

        /*! unregisters a LOD trigger node

        \param trigger the trigger node to remove
        */
        void unregisterLODTrigger(iNodeLODTrigger* trigger);

        /*! registers a LOD switch node

        \param switchNode the switch node to add
        */
        void registerLODSwitch(iNodeLODSwitch* switchNode);

        /*! unregisters a LOD switch node

        \param switchNode the trigger node to remove
        */
        void unregisterLODSwitch(iNodeLODSwitch* switchNode);

        /*! updates lod switch nodes
        */
        void updateLOD();

        /*! register light node to scene.

        \param light node to be registered as light
        */
		void registerLight(iNodeLight* light);

        /*! unregister light node from scene

        \param light node to be unregisterred
        */
		void unregisterLight(iNodeLight* light);

        /*! register renderable to scene

        \param node node to be registered as renderable
        */
        void registerRenderable(iNodeRender* node);

        /*! unregister renderable from scene

        \param node node to be unregistered from scene
        */
        void unregisterRenderable(iNodeRender* node);

        /*! register volume node to scene

        \param volume node to be registerred to scene
        */
        void registerVolume(iNodeVolume* volume);

        /*! unregister volume node from scene

        \param volume node to be unregisterred from scene
        */
        void unregisterVolume(iNodeVolume* volume);

        /*! updates volume position in octree

        \param volume node to be updated in octree
        */
		void updateVolume(iNodeVolume* volume);

        /*! signals the scene that a node was added

        \param nodeID the node's id that was added
        */
        void signalNodeAdded(uint32 nodeID);

        /*! signals the scene that a node was removed

        \param nodeID the node's id that was removed
        */
        void signalNodeRemoved(uint32 nodeID);

        /*! initializes scene and octree
        */
        iScene();

        /*! releases memory
        */
        virtual ~iScene();

	};

};

#endif
