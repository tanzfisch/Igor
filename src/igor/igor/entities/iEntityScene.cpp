#include <igor/entities/iEntityScene.h>

#include <igor/entities/iEntity.h>

#include <igor/entities/systems/iSpriteRenderSystem.h>
#include <igor/entities/systems/iTransformHierarchySystem.h>
#include <igor/entities/systems/iBehaviourSystem.h>
#include <igor/entities/systems/iQuadtreeSystem.h>
#include <igor/renderer/iRenderer.h>

#include <utility>
#include <tuple>

namespace igor
{

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

        _velocitySystem = std::make_shared<iVelocitySystem>();
        _systems.push_back(_velocitySystem);
        _systems.push_back(std::make_shared<iTransformHierarchySystem>());
        _systems.push_back(std::make_shared<iQuadtreeSystem>());
        _systems.push_back(std::make_shared<iBehaviourSystem>());

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
        getRegistry().clear();
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

    void iEntityScene::onUpdate()
    {
        for (iEntitySystemPtr &system : _systems)
        {
            system->update(shared_from_this());
        }
    }

    void iEntityScene::onRender(float32 clientWidth, float32 clientHeight)
    {
        auto cameraView = _registry->_registry.view<iCameraComponent, iTransformComponent>();

        for (auto entityID : cameraView)
        {
            auto [camera, transform] = cameraView.get<iCameraComponent, iTransformComponent>(entityID);            

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

            for (iEntitySystemPtr &system : _renderingSystems)
            {
                system->update(shared_from_this());
            }            
        }

        iRenderer::getInstance().flush();
    }

    iEntity iEntityScene::createEntity(const iaString &name, bool active)
    {
        iEntityID entityID = getRegistry().create();
        getRegistry().emplace_or_replace<iBaseEntityComponent>(entityID, name);

        iEntity entity(entityID, shared_from_this());
        entity.setActive(active);

        return entity;
    }

    void iEntityScene::destroyEntity(iEntityID entityID)
    {
        /*! cleanup quadtree
         */
        iBody2DComponent *component = tryGetComponent<iBody2DComponent>(entityID);
        if (component != nullptr)
        {
            getQuadtree().remove(component->_object);
        }

        // cleanup hierarchy
        iHierarchyComponent *hierarchy = getRegistry().try_get<iHierarchyComponent>(entityID);
        if (hierarchy != nullptr &&
            getRegistry().valid(hierarchy->_parent))
        {
            iHierarchyComponent *parentHierarchy = getRegistry().try_get<iHierarchyComponent>(hierarchy->_parent);
            if (parentHierarchy != nullptr)
            {
                parentHierarchy->_childCount = std::max(0, parentHierarchy->_childCount - 1);
            }
        }

        getRegistry().destroy(entityID);
    }

    entt::registry &iEntityScene::getRegistry() const
    {
        return _registry->_registry;
    }

    template <typename... Components>
    std::type_index getTypeIndex()
    {
        return std::type_index(typeid(std::tuple<Components...>));
    }

    template <typename T>
    T &iEntityScene::addComponent(iEntityID entityID, const T &component)
    {
        T &result = getRegistry().emplace_or_replace<T>(entityID);
        result = component;

        return result;
    }

    template <>
    iBody2DComponent &iEntityScene::addComponent<iBody2DComponent>(iEntityID entityID, const iBody2DComponent &component)
    {
        iTransformComponent *transform = tryGetComponent<iTransformComponent>(entityID);
        if (transform == nullptr)
        {
            const iaVector2d center = getQuadtree().getRootBox().getCenter();
            transform = &(getRegistry().emplace_or_replace<iTransformComponent>(entityID, iaVector3d(center._x, center._y, 0.0)));
        }

        iBody2DComponent &result = getRegistry().emplace_or_replace<iBody2DComponent>(entityID);

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
            getRegistry().emplace_or_replace<iTransformComponent>(entityID);
        }

        iCameraComponent &result = getRegistry().emplace_or_replace<iCameraComponent>(entityID);
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

    template <typename T>
    T &iEntityScene::getComponent(iEntityID entityID)
    {
        return getRegistry().get<T>(entityID);
    }

    template iCircleCollision2DComponent &iEntityScene::getComponent<iCircleCollision2DComponent>(iEntityID entityID);
    template iBaseEntityComponent &iEntityScene::getComponent<iBaseEntityComponent>(iEntityID entityID);
    template iBehaviourComponent &iEntityScene::getComponent<iBehaviourComponent>(iEntityID entityID);
    template iActiveComponent &iEntityScene::getComponent<iActiveComponent>(iEntityID entityID);
    template iSpriteRendererComponent &iEntityScene::getComponent<iSpriteRendererComponent>(iEntityID entityID);
    template iTransformComponent &iEntityScene::getComponent<iTransformComponent>(iEntityID entityID);
    template iVelocityComponent &iEntityScene::getComponent<iVelocityComponent>(iEntityID entityID);
    template iCameraComponent &iEntityScene::getComponent<iCameraComponent>(iEntityID entityID);

    template <typename T>
    T *iEntityScene::tryGetComponent(iEntityID entityID)
    {
        return getRegistry().try_get<T>(entityID);
    }

    template iCircleCollision2DComponent *iEntityScene::tryGetComponent<iCircleCollision2DComponent>(iEntityID entityID);
    template iBaseEntityComponent *iEntityScene::tryGetComponent<iBaseEntityComponent>(iEntityID entityID);
    template iBehaviourComponent *iEntityScene::tryGetComponent<iBehaviourComponent>(iEntityID entityID);
    template iActiveComponent *iEntityScene::tryGetComponent<iActiveComponent>(iEntityID entityID);
    template iSpriteRendererComponent *iEntityScene::tryGetComponent<iSpriteRendererComponent>(iEntityID entityID);
    template iTransformComponent *iEntityScene::tryGetComponent<iTransformComponent>(iEntityID entityID);
    template iVelocityComponent *iEntityScene::tryGetComponent<iVelocityComponent>(iEntityID entityID);
    template iCameraComponent *iEntityScene::tryGetComponent<iCameraComponent>(iEntityID entityID);

    template <typename T>
    void iEntityScene::removeComponent(iEntityID entityID)
    {
        getRegistry().remove<T>(entityID);
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
        getRegistry().remove<iBody2DComponent>(entityID);
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

} // igor
