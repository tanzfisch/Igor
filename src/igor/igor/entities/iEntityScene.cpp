// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntityScene.h>

#include <igor/entities/iEntity.h>

#include <igor/entities/systems/iSpriteRenderSystem.h>
#include <igor/entities/systems/iTransformHierarchySystem.h>
#include <igor/entities/systems/iBehaviourSystem.h>
#include <igor/entities/systems/iQuadtreeSystem.h>
#include <igor/entities/systems/iAnimationSystem.h>
#include <igor/renderer/iRenderer.h>

#include <utility>
#include <tuple>

#include <entt.h>

namespace igor
{

    // for debugging
    static void renderQuadtree(const iQuadtreed::NodePtr &node)
    {
        if (node == nullptr)
        {
            return;
        }

        iRenderer::getInstance().drawRectangle(node->_box._x, node->_box._y, node->_box._width, node->_box._height, iaColor4f::red);

        for (auto object : node->_objects)
        {
            iRenderer::getInstance().drawCircle(object->_circle._center._x, object->_circle._center._y, object->_circle._radius);
        }

        for (auto child : node->_children)
        {
            renderQuadtree(child);
        }
    }

    class iRegistry
    {
    public:
        /*! the entt registry
         */
        entt::registry _registry;
    };

    iEntityScene::iEntityScene()
    {
        _registry = new iRegistry();

        _systems.push_back(std::make_shared<iAnimationSystem>());
        _systems.push_back(std::make_shared<iBehaviourSystem>());

        _velocitySystem = std::make_shared<iVelocitySystem>();
        _systems.push_back(_velocitySystem);
        _systems.push_back(std::make_shared<iTransformHierarchySystem>());
        _systems.push_back(std::make_shared<iQuadtreeSystem>());

        _renderingSystems.push_back(std::make_shared<iSpriteRenderSystem>());
    }

    iEntityScene::~iEntityScene()
    {
        if (_quadtree != nullptr)
        {
            delete _quadtree;
        }

        delete _registry;
    }

    void iEntityScene::clear()
    {
        _systems.clear();
        _registry->_registry.clear();
    }

    void iEntityScene::setBounds(const iAABoxd &box)
    {
        _velocitySystem->setBounds(box);
    }

    const iAABoxd &iEntityScene::getBounds() const
    {
        return _velocitySystem->getBounds();
    }

    void iEntityScene::initializeQuadtree(const iaRectangled &box, const uint32 splitThreshold, const uint32 maxDepth)
    {
        con_assert(_quadtree == nullptr, "Quadtree already initialized");

        _quadtree = new iQuadtreed(box, splitThreshold, maxDepth);
    }

    iQuadtreed &iEntityScene::getQuadtree() const
    {
        con_assert(_quadtree != nullptr, "Quadtree not initialized");
        return *_quadtree;
    }

    bool iEntityScene::hasQuadtree() const
    {
        return _quadtree != nullptr;
    }

    void iEntityScene::onUpdate(const iaTime &time)
    {
        destroyEntities();

        for (iEntitySystemPtr &system : _systems)
        {
            system->update(time, shared_from_this());
        }
    }

    void iEntityScene::onRender(float32 clientWidth, float32 clientHeight)
    {
        // TODO sort by zindex
        auto &registry = _registry->_registry;

        auto cameraView = registry.view<iCameraComponent, iTransformComponent>();

        for (auto entityID : cameraView)
        {
            auto [camera, transform] = cameraView.get<iCameraComponent, iTransformComponent>(entityID);
            iRenderDebugComponent *debug = registry.try_get<iRenderDebugComponent>(entityID);

            iRenderer::getInstance().setViewport(camera._viewport._x * clientWidth,
                                                 camera._viewport._y * clientHeight,
                                                 camera._viewport._width * clientWidth,
                                                 camera._viewport._height * clientHeight);

            if (camera._clearColorActive)
            {
                iRenderer::getInstance().clearColorBuffer(camera._clearColor);
            }

            if (camera._clearDepthActive)
            {
                iRenderer::getInstance().clearDepthBuffer(camera._clearDepth);
            }

            if (camera._projection == iProjectionType::Perspective)
            {
                iRenderer::getInstance().setPerspective(camera._fieldOfView, camera._viewport._width / camera._viewport._height, camera._clipNear, camera._clipFar);
            }
            else
            {
                iRenderer::getInstance().setOrtho(camera._leftOrtho, camera._rightOrtho, camera._bottomOrtho, camera._topOrtho, camera._clipNear, camera._clipFar);
            }

            iRenderer::getInstance().setViewMatrixFromCam(transform._worldMatrix);

            if (debug != nullptr)
            {
                iRenderer::getInstance().setWireframeEnabled(debug->_renderWireframe);
            }

            for (auto &system : _renderingSystems)
            {
                system->render(shared_from_this());
            }

            if (debug != nullptr && debug->_renderSpacePartitioning && _quadtree != nullptr)
            {
                renderQuadtree(_quadtree->getRoot());
            }
        }

        iRenderer::getInstance().flush();
    }

    iEntity iEntityScene::createEntity(const iaString &name, bool active)
    {
        iEntityID entityID = static_cast<iEntityID>(_registry->_registry.create());
        _registry->_registry.emplace_or_replace<iBaseEntityComponent>(static_cast<entt::entity>(entityID), name);

        iEntity entity(entityID, shared_from_this());
        entity.setActive(active);

        return entity;
    }

    void iEntityScene::destroyEntity(iEntityID entityID)
    {
        _deleteQueue.push_back(entityID);
    }

    void iEntityScene::destroyEntity(const iEntity &entity)
    {
        _deleteQueue.push_back(entity.getID());
    }

    void iEntityScene::destroyEntities()
    {
        for (auto entityID : _deleteQueue)
        {
            if (!_registry->_registry.valid(static_cast<entt::entity>(entityID)))
            {
                continue;
            }

            /*! cleanup quadtree
             */
            iBody2DComponent *component = _registry->_registry.try_get<iBody2DComponent>(static_cast<entt::entity>(entityID));
            if (component != nullptr)
            {
                getQuadtree().remove(component->_object);
            }

            // cleanup hierarchy
            iHierarchyComponent *hierarchy = _registry->_registry.try_get<iHierarchyComponent>(static_cast<entt::entity>(entityID));
            if (hierarchy != nullptr &&
                _registry->_registry.valid(static_cast<entt::entity>(hierarchy->_parent)))
            {
                iHierarchyComponent *parentHierarchy = _registry->_registry.try_get<iHierarchyComponent>(static_cast<entt::entity>(hierarchy->_parent));
                if (parentHierarchy != nullptr)
                {
                    parentHierarchy->_childCount = std::max(0, parentHierarchy->_childCount - 1);
                }
            }

            _registry->_registry.destroy(static_cast<entt::entity>(entityID));
        }

        _deleteQueue.clear();
    }

    void *iEntityScene::getRegistry() const
    {
        return &_registry->_registry;
    }

    void *iEntityScene::addComponent(iEntityID entityID, const void *component, const std::type_info &typeInfo)
    {
        if (typeInfo == typeid(iDeleteComponent))
        {
            const iDeleteComponent &typedComponent = *static_cast<const iDeleteComponent *>(component);
            iDeleteComponent &result = _registry->_registry.emplace_or_replace<iDeleteComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iSpriteRendererComponent))
        {
            const iSpriteRendererComponent &typedComponent = *static_cast<const iSpriteRendererComponent *>(component);
            iSpriteRendererComponent &result = _registry->_registry.emplace_or_replace<iSpriteRendererComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iTransformComponent))
        {
            const iTransformComponent &typedComponent = *static_cast<const iTransformComponent *>(component);
            iTransformComponent &result = _registry->_registry.emplace_or_replace<iTransformComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iHierarchyComponent))
        {
            const iHierarchyComponent &typedComponent = *static_cast<const iHierarchyComponent *>(component);
            iHierarchyComponent &result = _registry->_registry.emplace_or_replace<iHierarchyComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iBody2DComponent))
        {
            iTransformComponent *transform = _registry->_registry.try_get<iTransformComponent>(static_cast<entt::entity>(entityID));
            if (transform == nullptr)
            {
                const iaVector2d center = getQuadtree().getRootBox().getCenter();
                transform = &(_registry->_registry.emplace_or_replace<iTransformComponent>(static_cast<entt::entity>(entityID), iaVector3d(center._x, center._y, 0.0)));
            }

            const iBody2DComponent &typedComponent = *static_cast<const iBody2DComponent *>(component);
            iBody2DComponent &result = _registry->_registry.emplace_or_replace<iBody2DComponent>(static_cast<entt::entity>(entityID));
            result._object = std::make_shared<iQuadtreed::Object>(iaCircled(transform->_position._x, transform->_position._y, 1.0), entityID);
            getQuadtree().insert(result._object);

            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iCircleCollision2DComponent))
        {
            const iCircleCollision2DComponent &typedComponent = *static_cast<const iCircleCollision2DComponent *>(component);
            iCircleCollision2DComponent &result = _registry->_registry.emplace_or_replace<iCircleCollision2DComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iVelocityComponent))
        {
            const iVelocityComponent &typedComponent = *static_cast<const iVelocityComponent *>(component);
            iVelocityComponent &result = _registry->_registry.emplace_or_replace<iVelocityComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iBehaviourComponent))
        {
            const iBehaviourComponent &typedComponent = *static_cast<const iBehaviourComponent *>(component);
            iBehaviourComponent &result = _registry->_registry.emplace_or_replace<iBehaviourComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iGlobalBoundaryComponent))
        {
            const iGlobalBoundaryComponent &typedComponent = *static_cast<const iGlobalBoundaryComponent *>(component);
            iGlobalBoundaryComponent &result = _registry->_registry.emplace_or_replace<iGlobalBoundaryComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iMotionInteractionResolverComponent))
        {
            const iMotionInteractionResolverComponent &typedComponent = *static_cast<const iMotionInteractionResolverComponent *>(component);
            iMotionInteractionResolverComponent &result = _registry->_registry.emplace_or_replace<iMotionInteractionResolverComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iCameraComponent))
        {
            iTransformComponent *transform = tryGetComponent<iTransformComponent>(entityID);
            if (transform == nullptr)
            {
                _registry->_registry.emplace_or_replace<iTransformComponent>(static_cast<entt::entity>(entityID));
            }

            const iCameraComponent &typedComponent = *static_cast<const iCameraComponent *>(component);
            iCameraComponent &result = _registry->_registry.emplace_or_replace<iCameraComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iMotionInteractionResolverComponent))
        {
            const iMotionInteractionResolverComponent &typedComponent = *static_cast<const iMotionInteractionResolverComponent *>(component);
            iMotionInteractionResolverComponent &result = _registry->_registry.emplace_or_replace<iMotionInteractionResolverComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iRenderDebugComponent))
        {
            const iRenderDebugComponent &typedComponent = *static_cast<const iRenderDebugComponent *>(component);
            iRenderDebugComponent &result = _registry->_registry.emplace_or_replace<iRenderDebugComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iPartyComponent))
        {
            const iPartyComponent &typedComponent = *static_cast<const iPartyComponent *>(component);
            iPartyComponent &result = _registry->_registry.emplace_or_replace<iPartyComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iAnimationComponent))
        {
            iTransformComponent *transform = tryGetComponent<iTransformComponent>(entityID);
            if (transform == nullptr)
            {
                _registry->_registry.emplace_or_replace<iTransformComponent>(static_cast<entt::entity>(entityID));
            }

            const iAnimationComponent &typedComponent = *static_cast<const iAnimationComponent *>(component);
            iAnimationComponent &result = _registry->_registry.emplace_or_replace<iAnimationComponent>(static_cast<entt::entity>(entityID));
            result = typedComponent;
            return static_cast<void *>(&result);
        }

        con_crit("unsupported type " << typeInfo.name());
        return nullptr;
    }

    void *iEntityScene::getComponent(iEntityID entityID, const std::type_info &typeInfo)
    {
        if (typeInfo == typeid(iDeleteComponent))
        {
            iDeleteComponent &result = _registry->_registry.get<iDeleteComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iSpriteRendererComponent))
        {
            iSpriteRendererComponent &result = _registry->_registry.get<iSpriteRendererComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iTransformComponent))
        {
            iTransformComponent &result = _registry->_registry.get<iTransformComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iHierarchyComponent))
        {
            iHierarchyComponent &result = _registry->_registry.get<iHierarchyComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iBody2DComponent))
        {
            iBody2DComponent &result = _registry->_registry.get<iBody2DComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iCircleCollision2DComponent))
        {
            iCircleCollision2DComponent &result = _registry->_registry.get<iCircleCollision2DComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iVelocityComponent))
        {
            iVelocityComponent &result = _registry->_registry.get<iVelocityComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iBehaviourComponent))
        {
            iBehaviourComponent &result = _registry->_registry.get<iBehaviourComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iGlobalBoundaryComponent))
        {
            iGlobalBoundaryComponent &result = _registry->_registry.get<iGlobalBoundaryComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iMotionInteractionResolverComponent))
        {
            iMotionInteractionResolverComponent &result = _registry->_registry.get<iMotionInteractionResolverComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iCameraComponent))
        {
            iCameraComponent &result = _registry->_registry.get<iCameraComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iMotionInteractionResolverComponent))
        {
            iMotionInteractionResolverComponent &result = _registry->_registry.get<iMotionInteractionResolverComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iRenderDebugComponent))
        {
            iRenderDebugComponent &result = _registry->_registry.get<iRenderDebugComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iPartyComponent))
        {
            iPartyComponent &result = _registry->_registry.get<iPartyComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }
        else if (typeInfo == typeid(iAnimationComponent))
        {
            iAnimationComponent &result = _registry->_registry.get<iAnimationComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(&result);
        }

        con_crit("unsupported type " << typeInfo.name());
        return nullptr;
    }

    void *iEntityScene::tryGetComponent(iEntityID entityID, const std::type_info &typeInfo)
    {
        if (typeInfo == typeid(iDeleteComponent))
        {
            iDeleteComponent *result = _registry->_registry.try_get<iDeleteComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iSpriteRendererComponent))
        {
            iSpriteRendererComponent *result = _registry->_registry.try_get<iSpriteRendererComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iTransformComponent))
        {
            iTransformComponent *result = _registry->_registry.try_get<iTransformComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iHierarchyComponent))
        {
            iHierarchyComponent *result = _registry->_registry.try_get<iHierarchyComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iBody2DComponent))
        {
            iBody2DComponent *result = _registry->_registry.try_get<iBody2DComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iCircleCollision2DComponent))
        {
            iCircleCollision2DComponent *result = _registry->_registry.try_get<iCircleCollision2DComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iVelocityComponent))
        {
            iVelocityComponent *result = _registry->_registry.try_get<iVelocityComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iBehaviourComponent))
        {
            iBehaviourComponent *result = _registry->_registry.try_get<iBehaviourComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iGlobalBoundaryComponent))
        {
            iGlobalBoundaryComponent *result = _registry->_registry.try_get<iGlobalBoundaryComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iMotionInteractionResolverComponent))
        {
            iMotionInteractionResolverComponent *result = _registry->_registry.try_get<iMotionInteractionResolverComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iCameraComponent))
        {
            iCameraComponent *result = _registry->_registry.try_get<iCameraComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iMotionInteractionResolverComponent))
        {
            iMotionInteractionResolverComponent *result = _registry->_registry.try_get<iMotionInteractionResolverComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iRenderDebugComponent))
        {
            iRenderDebugComponent *result = _registry->_registry.try_get<iRenderDebugComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iPartyComponent))
        {
            iPartyComponent *result = _registry->_registry.try_get<iPartyComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }
        else if (typeInfo == typeid(iAnimationComponent))
        {
            iAnimationComponent *result = _registry->_registry.try_get<iAnimationComponent>(static_cast<entt::entity>(entityID));
            return static_cast<void *>(result);
        }

        con_crit("unsupported type " << typeInfo.name());
        return nullptr;
    }

    void iEntityScene::removeComponent(iEntityID entityID, const std::type_info &typeInfo)
    {
        if (typeInfo == typeid(iDeleteComponent))
        {
            _registry->_registry.remove<iDeleteComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iSpriteRendererComponent))
        {
            _registry->_registry.remove<iSpriteRendererComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iTransformComponent))
        {
            _registry->_registry.remove<iTransformComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iHierarchyComponent))
        {
            _registry->_registry.remove<iHierarchyComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iBody2DComponent))
        {
            iBody2DComponent *component = _registry->_registry.try_get<iBody2DComponent>(static_cast<entt::entity>(entityID));
            if (component != nullptr)
            {
                getQuadtree().remove(component->_object);
                _registry->_registry.remove<iBody2DComponent>(static_cast<entt::entity>(entityID));
            }
        }
        else if (typeInfo == typeid(iCircleCollision2DComponent))
        {
            _registry->_registry.remove<iCircleCollision2DComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iVelocityComponent))
        {
            _registry->_registry.remove<iVelocityComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iBehaviourComponent))
        {
            _registry->_registry.remove<iBehaviourComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iGlobalBoundaryComponent))
        {
            _registry->_registry.remove<iGlobalBoundaryComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iMotionInteractionResolverComponent))
        {
            _registry->_registry.remove<iMotionInteractionResolverComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iCameraComponent))
        {
            _registry->_registry.remove<iCameraComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iMotionInteractionResolverComponent))
        {
            _registry->_registry.remove<iMotionInteractionResolverComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iRenderDebugComponent))
        {
            _registry->_registry.remove<iRenderDebugComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iPartyComponent))
        {
            _registry->_registry.remove<iPartyComponent>(static_cast<entt::entity>(entityID));
        }
        else if (typeInfo == typeid(iAnimationComponent))
        {
            _registry->_registry.remove<iAnimationComponent>(static_cast<entt::entity>(entityID));
        }

        return;
    }

} // igor
