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

#ifndef __IGOR_COMPONENTS__
#define __IGOR_COMPONENTS__

#include <igor/resources/texture/iTexture.h>
#include <igor/data/iQuadtree.h>

#include <iaux/data/iaString.h>
#include <iaux/data/iaColor4.h>
#include <iaux/math/iaMatrix.h>
#include <iaux/system/iaEvent.h>
using namespace iaux;

#include <entt.h>

#include <array>
#include <functional>
#include <any>

namespace igor
{
    /*! entity id definition
     */
    typedef entt::entity iEntityID;

    /*! invalid entity id definition
     */
    const iEntityID IGOR_INVALID_ENTITY_ID = entt::null;

    class iEntity;

    /*! name component used in each entity by default
     */
    struct iBaseEntityComponent
    {
        /*! name of entity
         */
        iaString _name;
    };

    /*! active component

    added or not added to an entity to filter out non active entities
    */
    struct iActiveComponent
    {
        /*! just a placeholder to satisfy entt
         */
        bool _placeholder;
    };

    /*! sprite render component
     */
    struct iSpriteRendererComponent
    {
        /*! texture to render as sprite
         */
        iTexturePtr _texture; // TODO need sprite not just texture

        /*! color to render sprite with
         */
        iaColor4f _color = iaColor4f::white;

        /*! specifies the render order within a layer
         */
        int32 _zIndex = 0;
    };

    // TODO introduce a render layer component so we only have to order by zindex within a layer and not across all

    /*! transform component representing position, orientation and scale of given entity

    3d variant
    */
    struct iTransformComponent
    {
        /*! position
         */
        iaVector3d _position;

        /*! orientation in rad
         */
        iaVector3d _orientation;

        /*! scale
         */
        iaVector3d _scale = {1.0, 1.0, 1.0};

        /*! parent entity id
         */
        iEntityID _parent = IGOR_INVALID_ENTITY_ID;

        /*! the world matrix of this transform
         */
        iaMatrixd _worldMatrix;
    };

    /*! component that holds an object in a quadtree
     */
    struct iQuadtreeComponent
    {
        /*! quadtree object
         */
        iQuadtreed::ObjectPtr _object;
    };

    struct iVelocityComponent
    {
        /*! vector to describe the velocity
         */
        iaVector3d _velocity;

        /*! vector to describe angular velocity of all three axis
         */
        iaVector3d _angularVelocity;
    };

    /*! behaviour function definition
     */
    typedef iaDelegate<void, iEntity &, void *> iBehaviourDelegate;

    /*! behaviour data
     */
    struct iBehaviourData
    {
        /*! delegate to be executed with given entity and user data
         */
        iBehaviourDelegate _delegate;

        /*! user data

        the application is responsible for storing this data in an efficient way
        */
        void *_userData = nullptr;
    };

    /*! behaviour component
     */
    struct iBehaviourComponent
    {
        /*! behaviors
         */
        std::array<iBehaviourData, IGOR_MAX_BEHAVIORS_PER_COMPONENT> _behaviour;
    };

    // iTextComponent
    // iPhysicsBodyComponent or specialized colliders and group colliders

    // iCharacterControllerComponent

    // iAudioSourceComponent
    // iAudioListenerComponent
    // iAudio remix reverb zone component
    // audio high/low pass filter

    // iParticleSystemComponent2D
    // iParticleSystemComponent3D
    // trail renderer
    // light
    // sky box
    // UI elements as component

    // constraints
    // aim at / look at / transform / rotate / scale
    // struct iMeshRenderComponent

}

#endif //  __IGOR_COMPONENTS__