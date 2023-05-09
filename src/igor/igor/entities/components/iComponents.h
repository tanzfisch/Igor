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
    static constexpr iEntityID IGOR_INVALID_ENTITY_ID = entt::null;

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

    enum class iSpriteRenderMode
    {
        Simple,
        Tiled
    };

    /*! sprite render component
     */
    struct iSpriteRendererComponent
    {
        /*! texture to render as sprite
         */
        iTexturePtr _texture; // TODO need sprite not just texture

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
    };

    // TODO introduce a render layer component so we only have to order by zIndex within a layer and not across all

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

        /*! the world matrix of this transform
         */
        iaMatrixd _worldMatrix;
    };

    /*! hierarchy component to create parent child relationships
     */
    struct iHierarchyComponent
    {
        /*! parent entity id
         */
        iEntityID _parent = IGOR_INVALID_ENTITY_ID;

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
    struct iBody2DComponent
    {
        /*! quadtree object
         */
        iQuadtreed::ObjectPtr _object;
    };

    /*! 2D collision component
     */
    struct iCircleCollision2DComponent
    {
        /*! the circles radius
         */
        float64 _radius;

        /*! the offset position
         */
        iaVector2d _offset;
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
    typedef iaDelegate<void, iEntity &, std::any &> iBehaviourDelegate;

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

    static constexpr uint32 IGOR_MAX_BEHAVIORS_PER_COMPONENT = 10;

    /*! behaviour component
     */
    struct iBehaviourComponent
    {
        /*! behaviors
         */
        std::array<iBehaviourData, IGOR_MAX_BEHAVIORS_PER_COMPONENT> _behaviour;
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
    struct iGlobalBoundaryComponent
    {
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
    struct iMotionInteractionResolverComponent
    {
        iMotionInteractionType _type = iMotionInteractionType::None;
    };

    /*! projection type definition
     */
    enum class iProjectionType
    {
        Perspective,
        Orthogonal
    };

    struct iCameraComponent
    {
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

    struct iRenderDebugComponent
    {
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
    struct iPartyComponent
    {
        uint32 _partyID = 0;
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