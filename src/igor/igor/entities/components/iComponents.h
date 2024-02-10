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

#ifndef IGOR_COMPONENTS_H
#define IGOR_COMPONENTS_H

#include <igor/entities/iEntityComponent.h>

#include <igor/resources/texture/iTexture.h>
#include <igor/resources/sprite/iSprite.h>
#include <igor/resources/animation/iAnimationController.h>
#include <igor/data/iQuadtree.h>

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
    typedef iEntity* iEntityPtr;

    /*! sprite render mode
    */
    enum class iSpriteRenderMode
    {
        Simple,
        Tiled
    };

    /*! sprite render component
     */
    class iSpriteRendererComponent : public iEntityComponent
    {

    public:
        /*! ctor

        \param name the name of this component
        */
        iSpriteRendererComponent(iSpritePtr sprite, const iaVector2d &size = iaVector2d(1.0, 1.0), const iaColor4f &color = iaColor4f::white, int32 zIndex = 0, iSpriteRenderMode renderMode = iSpriteRenderMode::Simple, const iaString &name = "sprite render")
            : iEntityComponent(name), _sprite(sprite), _size(size), _color(color), _zIndex(zIndex), _renderMode(renderMode)
        {
        }

        /*! sprite to render
         */
        iSpritePtr _sprite;

        /*! render size
         */
        iaVector2d _size = {1.0, 1.0};

        /*! color to render sprite with
         */
        iaColor4f _color = iaColor4f::white;

        /*! specifies the render order within a layer
         */
        int32 _zIndex = 0;

        /*! sprite render mode
         */
        iSpriteRenderMode _renderMode = iSpriteRenderMode::Simple;

        /*! index of the sprite frame to render
         */
        uint32 _frameIndex = 0;
    };

    // TODO introduce a render layer component so we only have to order by zIndex within a layer and not across all

    /*! transform component representing position, orientation and scale of given entity

    3d variant
    */
    class IGOR_API iTransformComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iTransformComponent(const iaVector3d &position = iaVector3d(), const iaVector3d &orientation = iaVector3d(), const iaVector3d &scale = iaVector3d(1.0, 1.0, 1.0), const iaString &name = "transform")
            : iEntityComponent(name), _position(position), _orientation(orientation), _scale(scale)
        {
        }

        /*! position
         */
        iaVector3d _position;

        /*! euler angles in rad
         */
        iaVector3d _orientation;

        /*! scale
         */
        iaVector3d _scale = {1.0, 1.0, 1.0};

        /*! the world matrix of this transform
         */
        iaMatrixd _worldMatrix;
    };

    /*! hierarchy component to create parent child relationships
     */
    class iHierarchyComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iHierarchyComponent(iEntityID parent, const iaString &name = "hierarchy")
            : iEntityComponent(name), _parent(parent)
        {
        }

        /*! parent entity id
         */
        iEntityID _parent;

        /*! counting how many child this entity has
         */
        int32 _childCount = 0;

        /*! the higher the number the more generations are below it

        this is used for sorting entities by hierarchy
        */
        int32 _generation = 0;
    };

    /*! 2D body component
     */
    class iBody2DComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iBody2DComponent(iQuadtreed::ObjectPtr object = nullptr, const iaString &name = "body 2d")
            : iEntityComponent(name), _object(object)
        {
        }

        /*! quadtree object
         */
        iQuadtreed::ObjectPtr _object;
    };

    /*! 2D collision component
     */
    class iCircleCollision2DComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iCircleCollision2DComponent(float64 radius, const iaVector2d &offset = iaVector2d(), const iaString &name = "collision 2d")
            : iEntityComponent(name), _radius(radius), _offset(offset)
        {
        }

        /*! the circles radius
         */
        float64 _radius;

        /*! the offset position
         */
        iaVector2d _offset;
    };

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

    /*! projection type definition
     */
    enum class iProjectionType
    {
        Perspective,
        Orthogonal
    };

    /*! camera component
     */
    class iCameraComponent : public iEntityComponent
    {
    public:
        /*! ctor

        \param name the name of this component
        */
        iCameraComponent(const iaString &name = "camera")
            : iEntityComponent(name)
        {
        }

        /*! viewport
         */
        iaRectangled _viewport = {0.0, 0.0, 1.0, 1.0};

        /*! projection type
         */
        iProjectionType _projection = iProjectionType::Perspective;

        /*! field of view
         */
        float64 _fieldOfView = 45.0;

        /*! near clip plane
         */
        float64 _clipNear = 1.0;

        /*! far clip plane
         */
        float64 _clipFar = 10000.0;

        /*! if true clear the color buffer
         */
        bool _clearColorActive = true;

        /*! if true clear the depth buffer
         */
        bool _clearDepthActive = true;

        /*! the clear color
         */
        iaColor4f _clearColor = iaColor4f::gray;

        /*! clear depth value
         */
        float64 _clearDepth = 1.0;

        /*! left value used for orthogonal projection
         */
        float64 _leftOrtho = -1.0;

        /*! right value used for orthogonal projection
         */
        float64 _rightOrtho = 1.0;

        /*! top value used for orthogonal projection
         */
        float64 _topOrtho = 1.0;

        /*! bottom value used for orthogonal projection
         */
        float64 _bottomOrtho = -1.0;

        /*! z index aka order in which cameras will be rendered
         */
        int32 _zIndex = 0;
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