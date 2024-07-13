// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/prefab/loader/iPrefabIO.h>

#include <igor/resources/iResourceManager.h>
#include <igor/entities/iEntitySystemModule.h>

#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iCameraComponent.h>
#include <igor/entities/components/iCircleCollision2DComponent.h>
#include <igor/entities/components/iLightComponent.h>
#include <igor/entities/components/iMeshRenderComponent.h>
#include <igor/entities/components/iOctreeComponent.h>
#include <igor/entities/components/iQuadtreeComponent.h>
#include <igor/entities/components/iSphereCollision3DComponent.h>
#include <igor/entities/components/iSpriteRenderComponent.h>

#include <igor/entities/components/iComponents.h>

#include <fstream>

namespace igor
{
    using ReadComponentFunc = std::function<void(iEntityPtr, const json &)>;

    static void readTransform(iEntityPtr entity, const json &componentJson)
    {
        iTransformComponent *component = new iTransformComponent();        

        component->setPosition(componentJson["position"].get<iaVector3d>());
        component->setOrientation(componentJson["orientation"].get<iaVector3d>());
        component->setScale(componentJson["scale"].get<iaVector3d>());

        entity->addComponent(component);
    }

    static void writeTransform(json &componentJson, iTransformComponent *component)
    {
        componentJson["position"] = component->getPosition();
        componentJson["orientation"] = component->getOrientation();
        componentJson["scale"] = component->getScale();
    }

    static void readCamera(iEntityPtr entity, const json &componentJson)
    {
        iCameraComponent *component = new iCameraComponent();        

        component->setClearColor(componentJson["clearColor"].get<iaColor4f>());
        component->setClearColorActive(componentJson["clearColorActive"].get<bool>());
        component->setClearDepth(componentJson["clearDepth"].get<float32>());
        component->setClearDepthActive(componentJson["clearDepthActive"].get<bool>());
        component->setViewportRelative(componentJson["viewport"].get<iaRectangled>());
        component->setClipPlanes(componentJson["clipNear"].get<float32>(), componentJson["clipFar"].get<float32>());
        const bool perspective = componentJson["perspective"].get<bool>();
        if (perspective)
        {
            component->setPerspective(componentJson["fov"].get<float32>());
        }
        else
        {
            component->setOrthogonal(
                componentJson["orthoLeft"].get<float32>(),
                componentJson["orthoRight"].get<float32>(),
                componentJson["orthoTop"].get<float32>(),
                componentJson["orthoBottom"].get<float32>());
        }
        
        entity->addComponent(component);
    }

    static void writeCamera(json &componentJson, iCameraComponent *component)
    {
        componentJson["clearColor"] = component->getClearColor();
        componentJson["clearColorActive"] = component->isClearColorActive();
        componentJson["clearDepth"] = component->getClearDepth();
        componentJson["clearDepthActive"] = component->isClearDepthActive();
        componentJson["viewport"] = component->getViewport();
        componentJson["perspective"] = component->getProjectionType() == iProjectionType::Perspective;
        componentJson["fov"] = component->getFieldOfView();
        componentJson["clipNear"] = component->getNearClipPlane();
        componentJson["clipFar"] = component->getFarClipPlane();
        componentJson["orthoLeft"] = component->getLeftOrtho();
        componentJson["orthoRight"] = component->getRightOrtho();
        componentJson["orthoTop"] = component->getTopOrtho();
        componentJson["orthoBottom"] = component->getBottomOrtho();
    }

    static void readSphere(iEntityPtr entity, const json &componentJson)
    {
    }

    static void readOctree(iEntityPtr entity, const json &componentJson)
    {
    }
    static void readMeshRender(iEntityPtr entity, const json &componentJson)
    {
    }

    iEntityPtr iPrefabIO::readEntity(iEntityScenePtr scene, const json &entityJson)
    {
        std::unordered_map<iaString, ReadComponentFunc> func = {
            {"transform", readTransform},
            {"camera", readCamera},
            {"sphere", readSphere},
            {"octree", readOctree},
            {"meshRender", readMeshRender}};

        const iaString entityName = entityJson["name"].get<iaString>();
        const iEntityID entityID = entityJson["id"].get<iaUUID>();
        const bool active = entityJson["active"].get<bool>();

        iEntityPtr entity = scene->createEntity(entityName, entityID);
        entity->setActive(active);

        if (!entityJson.contains("components"))
        {
            return entity;
        }

        json componentsJson = entityJson["components"];
        for (const auto &componentJson : componentsJson)
        {
            const iaString type = componentJson["componentType"].get<iaString>();
            auto iter = func.find(type);
            if (iter == func.end())
            {
                con_err("unsupported component type \"" << type << "\"");
                continue;
            }

            iter->second(entity, componentJson);
        }

        return entity;
    }

    bool iPrefabIO::read(const iaString &filename, const iPrefabPtr &prefab)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ifstream file(temp);
        json data = json::parse(file);

        if (!data.contains("entityScene"))
        {
            con_err("unexpected data format");
            return false;
        }

        json entityScene = data["entityScene"];
        const iaString sceneName = entityScene["name"].get<iaString>();
        const iEntitySceneID sceneID = entityScene["id"].get<iaUUID>();
        auto scene = iEntitySystemModule::getInstance().createScene(sceneName, true, sceneID);
        prefab->_sceneID = scene->getID();

        json entitiesJson = data["entities"];
        for (const auto &entityJson : entitiesJson)
        {
            readEntity(scene, entityJson);
        }

        return true;
    }

    void iPrefabIO::writeEntity(json &entityJson, iEntityPtr entity)
    {
        entityJson["name"] = entity->getName();
        entityJson["id"] = entity->getID();
        entityJson["active"] = entity->isActive();
        if (entity->hasParent())
        {
            entityJson["parent"] = entity->getParent()->getID();
        }

        json childrenJson = json::array();
        const auto &activeChildren = entity->getChildren();
        for (const auto child : activeChildren)
        {
            childrenJson.push_back(child->getID());
        }

        const auto &inactiveChildren = entity->getInactiveChildren();
        for (const auto child : inactiveChildren)
        {
            childrenJson.push_back(child->getID());
        }

        if (!childrenJson.empty())
        {
            entityJson["children"] = childrenJson;
        }

        json componentsJson = json::array();
        for (const auto &pair : entity->_components)
        {
            iTransformComponent *transform = dynamic_cast<iTransformComponent *>(pair.second);
            if (transform != nullptr)
            {
                json componentJson;
                componentJson["componentType"] = "transform";
                writeTransform(componentJson, transform);
                componentsJson.push_back(componentJson);
                continue;
            }

            iCameraComponent *camera = dynamic_cast<iCameraComponent *>(pair.second);
            if (camera != nullptr)
            {
                json componentJson;
                componentJson["componentType"] = "camera";
                writeCamera(componentJson, camera);
                componentsJson.push_back(componentJson);
                continue;
            }

            iSphereCollision3DComponent *sphereCollision = dynamic_cast<iSphereCollision3DComponent *>(pair.second);
            if (sphereCollision != nullptr)
            {
                json componentJson;
                componentJson["componentType"] = "sphere";
                componentJson["component"] = *sphereCollision;
                componentsJson.push_back(componentJson);
                continue;
            }

            iOctreeComponent *octree = dynamic_cast<iOctreeComponent *>(pair.second);
            if (octree != nullptr)
            {
                json componentJson;
                componentJson["componentType"] = "octree";
                componentJson["component"] = *octree;
                componentsJson.push_back(componentJson);
                continue;
            }

            iMeshRenderComponent *meshRender = dynamic_cast<iMeshRenderComponent *>(pair.second);
            if (meshRender != nullptr)
            {
                json componentJson;
                componentJson["componentType"] = "meshRender";
                componentJson["component"] = *meshRender;
                componentsJson.push_back(componentJson);
                continue;
            }

            con_err("unknown component type");
        }

        if (!componentsJson.empty())
        {
            entityJson["components"] = componentsJson;
        }
    }

    bool iPrefabIO::write(const iaString &filename, const iPrefabPtr &prefab)
    {
        char temp[2048];
        filename.getData(temp, 2048);

        std::ofstream stream;
        stream.open(temp);

        auto scene = iEntitySystemModule::getInstance().getScene(prefab->getSceneID());

        json entitiesJson = json::array();
        for (const auto &pair : scene->_entities)
        {
            json entityJson;
            writeEntity(entityJson, pair.second);
            entitiesJson.push_back(entityJson);
        }

        // TODO octree and quadtree

        json sceneJson = {
            {"name", scene->getName()},
            {"sceneID", scene->getID()},
            {"entities", entitiesJson}};

        json dataJson;
        dataJson["entityScene"] = sceneJson;

        stream << dataJson.dump(4);

        return true;
    }

} // namespace igor
