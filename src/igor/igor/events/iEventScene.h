//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef __IGOR_EVENTSCENE__
#define __IGOR_EVENTSCENE__

#include <igor/events/iEvent.h>

#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNode.h>

namespace igor
{

    /*! node added to scene event
    */
    class IGOR_API iEventNodeAddedToScene : public iEvent
    {
    public:
        /*! init members

        \param scene the scene the node was added to
        \param nodeID id of the node that was added to the scene
        */
        iEventNodeAddedToScene(iScenePtr scene, iNodeID nodeID);

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns the scene
        */
        iScenePtr getScene() const;

        /*! \returns the node id
        */
        iNodeID getNodeID() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Scene)
        IGOR_EVENT_CLASS_TYPE(iEventNodeAddedToScene)

    private:
        /*! the scene the node was added to
        */
        iScenePtr _scene;

        /*! id of the node that was added to the scene
        */
        iNodeID _nodeID;
    };

    /*! node removed from scene event
    */
    class IGOR_API iEventNodeRemovedFromScene : public iEvent
    {
    public:
        /*! init members

        \param scene the scene the node was removed from
        \param nodeID id of the node that was removed from the scene
        */
        iEventNodeRemovedFromScene(iScenePtr scene, iNodeID nodeID);

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns the scene
        */
        iScenePtr getScene() const;

        /*! \returns the node id
        */
        iNodeID getNodeID() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Scene)
        IGOR_EVENT_CLASS_TYPE(iEventNodeRemovedFromScene)

    private:
        /*! the scene the node was removed from
        */
        iScenePtr _scene;

        /*! id of the node that was removed from the scene
        */
        iNodeID _nodeID;
    };

    /*! scene selection change event

    \todo is this something that belong in to the engine or does it belong in to Mica?
    */
    class IGOR_API iEventSceneSelectionChanged : public iEvent
    {
    public:
        /*! init members

        \param scene the scene the node was added to
        \param nodeID id of the node that was added to the scene
        */
        iEventSceneSelectionChanged(iScenePtr scene);

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns the scene
        */
        iScenePtr getScene() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Scene)
        IGOR_EVENT_CLASS_TYPE(iEventSceneSelectionChanged)

    private:
        /*! the scene the selection was changed of
        */
        iScenePtr _scene;
    };
}; // namespace igor

#endif // __IGOR_EVENTKEYBOARD__