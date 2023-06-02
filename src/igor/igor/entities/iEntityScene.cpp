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

    template <typename... Components>
    std::type_index getTypeIndex()
    {
        return std::type_index(typeid(std::tuple<Components...>));
    }

    template <typename T>
    T &iEntityScene::addComponent(iEntityID entityID, const T &component)
    {
        T &result = _registry->_registry.emplace_or_replace<T>(static_cast<entt::entity>(entityID));
        result = component;

        return result;
    }

    template <>
    iBody2DComponent &iEntityScene::addComponent<iBody2DComponent>(iEntityID entityID, const iBody2DComponent &component)
    {
        iTransformComponent *transform = _registry->_registry.try_get<iTransformComponent>(static_cast<entt::entity>(entityID));
        if (transform == nullptr)
        {
            const iaVector2d center = getQuadtree().getRootBox().getCenter();
            transform = &(_registry->_registry.emplace_or_replace<iTransformComponent>(static_cast<entt::entity>(entityID), iaVector3d(center._x, center._y, 0.0)));
        }

        iBody2DComponent &result = _registry->_registry.emplace_or_replace<iBody2DComponent>(static_cast<entt::entity>(entityID));

        result._object = std::make_shared<iQuadtreed::Object>(iaCircled(transform->_position._x, transform->_position._y, 1.0), entityID);
        getQuadtree().insert(result._object);

        return result;
    }

    template <>
    iCameraComponent &iEntityScene::addComponent<iCameraComponent>(iEntityID entityID, const iCameraComponent &component)
    {
        iTransformComponent *transform = tryGetComponent<iTransformComponent>(entityID);
        if (transform == nullptr)
        {
            _registry->_registry.emplace_or_replace<iTransformComponent>(static_cast<entt::entity>(entityID));
        }

        iCameraComponent &result = _registry->_registry.emplace_or_replace<iCameraComponent>(static_cast<entt::entity>(entityID));
        result = component;

        return result;
    }

    template <>
    iAnimationComponent &iEntityScene::addComponent<iAnimationComponent>(iEntityID entityID, const iAnimationComponent &component)
    {
        iTransformComponent *transform = tryGetComponent<iTransformComponent>(entityID);
        if (transform == nullptr)
        {
            _registry->_registry.emplace_or_replace<iTransformComponent>(static_cast<entt::entity>(entityID));
        }

        iAnimationComponent &result = _registry->_registry.emplace_or_replace<iAnimationComponent>(static_cast<entt::entity>(entityID));
        result = component;

        return result;
    }    

    // the following types are left out on purpose
    // iBaseEntityComponent, iActiveComponent
    template iCircleCollision2DComponent &iEntityScene::addComponent<iCircleCollision2DComponent>(iEntityID entityID, const iCircleCollision2DComponent &component);
    template iBehaviourComponent &iEntityScene::addComponent<iBehaviourComponent>(iEntityID entityID, const iBehaviourComponent &component);
    template iSpriteRendererComponent &iEntityScene::addComponent<iSpriteRendererComponent>(iEntityID entityID, const iSpriteRendererComponent &component);
    template iTransformComponent &iEntityScene::addComponent<iTransformComponent>(iEntityID entityID, const iTransformComponent &component);
    template iVelocityComponent &iEntityScene::addComponent<iVelocityComponent>(iEntityID entityID, const iVelocityComponent &component);
    template iCameraComponent &iEntityScene::addComponent<iCameraComponent>(iEntityID entityID, const iCameraComponent &component);
    template iRenderDebugComponent &iEntityScene::addComponent<iRenderDebugComponent>(iEntityID entityID, const iRenderDebugComponent &component);
    template iPartyComponent &iEntityScene::addComponent<iPartyComponent>(iEntityID entityID, const iPartyComponent &component);
    template iGlobalBoundaryComponent &iEntityScene::addComponent<iGlobalBoundaryComponent>(iEntityID entityID, const iGlobalBoundaryComponent &component);
    template iAnimationComponent &iEntityScene::addComponent<iAnimationComponent>(iEntityID entityID, const iAnimationComponent &component);

    template <typename T>
    T &iEntityScene::getComponent(iEntityID entityID)
    {
        return _registry->_registry.get<T>(static_cast<entt::entity>(entityID));
    }

    template iCircleCollision2DComponent &iEntityScene::getComponent<iCircleCollision2DComponent>(iEntityID entityID);
    template iBaseEntityComponent &iEntityScene::getComponent<iBaseEntityComponent>(iEntityID entityID);
    template iBehaviourComponent &iEntityScene::getComponent<iBehaviourComponent>(iEntityID entityID);
    template iActiveComponent &iEntityScene::getComponent<iActiveComponent>(iEntityID entityID);
    template iSpriteRendererComponent &iEntityScene::getComponent<iSpriteRendererComponent>(iEntityID entityID);
    template iTransformComponent &iEntityScene::getComponent<iTransformComponent>(iEntityID entityID);
    template iVelocityComponent &iEntityScene::getComponent<iVelocityComponent>(iEntityID entityID);
    template iCameraComponent &iEntityScene::getComponent<iCameraComponent>(iEntityID entityID);
    template iRenderDebugComponent &iEntityScene::getComponent<iRenderDebugComponent>(iEntityID entityID);
    template iPartyComponent &iEntityScene::getComponent<iPartyComponent>(iEntityID entityID);
    template iBody2DComponent &iEntityScene::getComponent<iBody2DComponent>(iEntityID entityID);
    template iGlobalBoundaryComponent &iEntityScene::getComponent<iGlobalBoundaryComponent>(iEntityID entityID);
    template iAnimationComponent &iEntityScene::getComponent<iAnimationComponent>(iEntityID entityID);

    template <typename T>
    T *iEntityScene::tryGetComponent(iEntityID entityID)
    {
        return _registry->_registry.try_get<T>(static_cast<entt::entity>(entityID));
    }

    template iCircleCollision2DComponent *iEntityScene::tryGetComponent<iCircleCollision2DComponent>(iEntityID entityID);
    template iBaseEntityComponent *iEntityScene::tryGetComponent<iBaseEntityComponent>(iEntityID entityID);
    template iBehaviourComponent *iEntityScene::tryGetComponent<iBehaviourComponent>(iEntityID entityID);
    template iActiveComponent *iEntityScene::tryGetComponent<iActiveComponent>(iEntityID entityID);
    template iSpriteRendererComponent *iEntityScene::tryGetComponent<iSpriteRendererComponent>(iEntityID entityID);
    template iTransformComponent *iEntityScene::tryGetComponent<iTransformComponent>(iEntityID entityID);
    template iVelocityComponent *iEntityScene::tryGetComponent<iVelocityComponent>(iEntityID entityID);
    template iCameraComponent *iEntityScene::tryGetComponent<iCameraComponent>(iEntityID entityID);
    template iRenderDebugComponent *iEntityScene::tryGetComponent<iRenderDebugComponent>(iEntityID entityID);
    template iPartyComponent *iEntityScene::tryGetComponent<iPartyComponent>(iEntityID entityID);
    template iBody2DComponent *iEntityScene::tryGetComponent<iBody2DComponent>(iEntityID entityID);
    template iGlobalBoundaryComponent *iEntityScene::tryGetComponent<iGlobalBoundaryComponent>(iEntityID entityID);
    template iAnimationComponent *iEntityScene::tryGetComponent<iAnimationComponent>(iEntityID entityID);

    template <typename T>
    void iEntityScene::removeComponent(iEntityID entityID)
    {
        _registry->_registry.remove<T>(static_cast<entt::entity>(entityID));
    }

    template <>
    void iEntityScene::removeComponent<iBody2DComponent>(iEntityID entityID)
    {
        iBody2DComponent *component = tryGetComponent<iBody2DComponent>(entityID);
        if (component == nullptr)
        {
            return;
        }

        getQuadtree().remove(component->_object);
        _registry->_registry.remove<iBody2DComponent>(static_cast<entt::entity>(entityID));
    }

    // the following types are left out on purpose
    // iBaseEntityComponent
    template void iEntityScene::removeComponent<iCircleCollision2DComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iBehaviourComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iActiveComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iSpriteRendererComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iTransformComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iVelocityComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iCameraComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iRenderDebugComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iPartyComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iGlobalBoundaryComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iAnimationComponent>(iEntityID entityID);
    

} // igor
