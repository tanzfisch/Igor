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
         */
        iBehaviourComponent()
        {
        }

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance()
        {
            return new iBehaviourComponent();
        }

        /*! behaviors
         */
        std::vector<iBehaviourData> _behaviors;

    private:
        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override
        {
            iBehaviourComponent *component = new iBehaviourComponent();
            // TODO
            return component;
        }
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
        iGlobalBoundaryComponent(iGlobalBoundaryType type = iGlobalBoundaryType::None, const iaString &name = "global boundary")
            : _type(type)
        {
        }

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance()
        {
            return new iGlobalBoundaryComponent();
        }

        /*! global boundary type
         */
        iGlobalBoundaryType _type = iGlobalBoundaryType::None;

    private:
        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override
        {
            iGlobalBoundaryComponent *component = new iGlobalBoundaryComponent();
            component->_type = _type;
            return component;
        }
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
        iMotionInteractionResolverComponent(iMotionInteractionType type = iMotionInteractionType::None, const iaString &name = "motion interaction resolver")
            : _type(type)
        {
        }

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance()
        {
            return new iMotionInteractionResolverComponent();
        }

        /*! motion reation type
         */
        iMotionInteractionType _type = iMotionInteractionType::None;

    private:
        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override
        {
            iMotionInteractionResolverComponent *component = new iMotionInteractionResolverComponent();
            component->_type = _type;
            return component;
        }
    };

    /*! render debug component
     */
    class iRenderDebugComponent : public iEntityComponent
    {
    public:
        /*! ctor
         */
        iRenderDebugComponent()
        {
        }

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance()
        {
            return new iRenderDebugComponent();
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

    private:
        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override
        {
            iRenderDebugComponent *component = new iRenderDebugComponent();
            component->_renderWireframe = _renderWireframe;
            component->_renderSpacePartitioning = _renderSpacePartitioning;
            component->_renderBounds = _renderBounds;
            return component;
        }
    };

    /*! I love parties <3
     */
    class iPartyComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iPartyComponent(uint32 partyID = 0, const iaString &name = "party")
            : _partyID(partyID)
        {
        }

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance()
        {
            return new iPartyComponent();
        }

        /*! party id
         */
        uint32 _partyID = 0;

    private:
        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override
        {
            iPartyComponent *component = new iPartyComponent();
            component->_partyID = _partyID;
            return component;
        }
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
            : _animationController(animationController)
        {
        }

        /*! creates instance of this component type
         */
        static iEntityComponent *createInstance()
        {
            return new iAnimationComponent();
        }

        /*! animation controller
         */
        iAnimationControllerPtr _animationController;

    private:
        /*! \returns a copy of this component
         */
        iEntityComponentPtr getCopy() override
        {
            iAnimationComponent *component = new iAnimationComponent();
            component->_animationController = _animationController;
            return component;
        }
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
    // sky box
    // UI elements as component

    // constraints
    // aim at / look at / transform / rotate / scale
    // struct iMeshRenderComponent

}

#endif //  __IGOR_COMPONENTS__