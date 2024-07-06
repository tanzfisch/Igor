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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_COMPONENTS_H
#define IGOR_COMPONENTS_H

#include <igor/entities/iEntityComponent.h>

#include <igor/resources/texture/iTexture.h>
#include <igor/resources/sprite/iSprite.h>
#include <igor/resources/animation/iAnimationController.h>

#include <iaux/data/iaString.h>
#include <iaux/data/iaColor4.h>
#include <iaux/math/iaMatrix.h>
#include <iaux/system/iaEvent.h>
using namespace iaux;

#include <array>
#include <functional>
#include <any>

namespace igor
{

    /*! entity pointer definition
     */
    class iEntity;
    typedef iEntity *iEntityPtr;

    // TODO introduce a render layer component so we only have to order by zIndex within a layer and not across all

    /*! velocity component
     */
    class iVelocityComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iVelocityComponent(const iaVector3d &velocity = iaVector3d(), const iaVector3d &angularVelocity = iaVector3d(), const iaString &name = "velocity")
            : iEntityComponent(name), _velocity(velocity), _angularVelocity(angularVelocity)
        {
        }

        /*! vector to describe the velocity
         */
        iaVector3d _velocity;

        /*! vector to describe angular velocity of all three axis
         */
        iaVector3d _angularVelocity;
    };

    /*! behaviour function definition
     */
    typedef iaDelegate<void, iEntityPtr, std::any &> iBehaviourDelegate;

    /*! behaviour data
     */
    struct iBehaviourData
    {
        /*! delegate to be executed with given entity and user data
         */
        iBehaviourDelegate _delegate;

        /*! user data

        \todo this is not ideal maybe fix in #300
        */
        std::any _userData;

        /*! optional name of behaviour
        */
        iaString _name;
    };

    /*! behaviour component
     */
    class iBehaviourComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iBehaviourComponent(const iaString &name = "behaviour")
            : iEntityComponent(name)
        {
        }

        /*! behaviors
         */
        std::vector<iBehaviourData> _behaviors;
    };

    /*! global boundary contraint type
     */
    enum class iGlobalBoundaryType
    {
        /*! no constraint
         */
        None,
        /*! if the entity leaves the bounds it reappears at the other end of the boundaries
         */
        Repeat
    };

    /*! keeps entities within some global boundaries

    check the scene interface for configuring the global boundaries
    */
    class iGlobalBoundaryComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iGlobalBoundaryComponent(iGlobalBoundaryType type, const iaString &name = "global boundary")
            : iEntityComponent(name), _type(type)
        {
        }

        /*! global boundary type
         */
        iGlobalBoundaryType _type = iGlobalBoundaryType::None;
    };

    /*! motion interaction type
     */
    enum class iMotionInteractionType
    {
        /*! no action
         */
        None,

        /*! divert an prevent collision if possible
         */
        Divert
    };

    /*! defines instructions how to handle collisions or near collisions
     */
    class iMotionInteractionResolverComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iMotionInteractionResolverComponent(iMotionInteractionType type, const iaString &name = "motion interaction resolver")
            : iEntityComponent(name), _type(type)
        {
        }

        /*! motion reation type
         */
        iMotionInteractionType _type = iMotionInteractionType::None;
    };

    /*! render debug component
     */
    class iRenderDebugComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iRenderDebugComponent(const iaString &name = "render debug")
            : iEntityComponent(name)
        {
        }

        /*! render everything in wireframe mode
         */
        bool _renderWireframe = false;

        /*! display what ever spacial partitioning is currently used
        ie Quadtree, Octree etc.
        */
        bool _renderSpacePartitioning = false;

        /*! display bounds of scene objects
         */
        bool _renderBounds = false;
    };

    /*! I love parties <3
     */
    class iPartyComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iPartyComponent(uint32 partyID, const iaString &name = "party")
            : iEntityComponent(name), _partyID(partyID)
        {
        }

        /*! party id
         */
        uint32 _partyID = 0;
    };

    /*! animation component
     */
    class iAnimationComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iAnimationComponent(iAnimationControllerPtr animationController = nullptr, const iaString &name = "animation")
            : iEntityComponent(name), _animationController(animationController)
        {
        }

        /*! animation controller
         */
        iAnimationControllerPtr _animationController;
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