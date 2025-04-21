// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/prefab/loader/iPrefabIO.h>

#include <igor/resources/iResourceManager.h>
#include <igor/entities/iEntitySystemModule.h>

#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iCameraComponent.h>
#include <igor/entities/components/iCircleComponent.h>
#include <igor/entities/components/iLightComponent.h>
#include <igor/entities/components/iMeshRenderComponent.h>
#include <igor/entities/components/iOctreeComponent.h>
#include <igor/entities/components/iQuadtreeComponent.h>
#include <igor/entities/components/iSphereComponent.h>
#include <igor/entities/components/iSpriteRenderComponent.h>
#include <igor/entities/components/iMeshReferenceComponent.h>

#include <igor/entities/components/iComponents.h>

#include <fstream>

namespace igor
{
    static void readTransform(iEntityPtr entity, const json &componentJson)
    {
        iTransformComponent *component = new iTransformComponent();
        entity->addComponent(component);

        component->setPosition(componentJson["position"].get<iaVector3d>());
        component->setOrientation(componentJson["orientation"].get<iaVector3d>());
        component->setScale(componentJson["scale"].get<iaVector3d>());
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
        entity->addComponent(component);

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
        iSphereComponent *component = new iSphereComponent();
        entity->addComponent(component);

        component->setRadius(componentJson["radius"].get<float64>());
        component->setOffset(componentJson["offset"].get<iaVector3d>());
    }

    static void writeSphere(json &componentJson, iSphereComponent *component)
    {
        componentJson["radius"] = component->getRadius();
        componentJson["offset"] = component->getOffset();
    }

    static void readOctree(iEntityPtr entity, const json &componentJson)
    {
        entity->addComponent(new iOctreeComponent());
    }

    static void readMeshRender(iEntityPtr entity, const json &componentJson)
    {
        entity->addComponent(new iMeshRenderComponent());
    }

    static void readMeshReference(iEntityPtr entity, const json &componentJson)
    {
        iMeshReferenceComponent *component = new iMeshReferenceComponent();
        entity->addComponent(component);

        component->setModel(iResourceManager::getInstance().requestResource<iModel>(componentJson["model"].get<iaUUID>()));

        std::vector<iaString> paths;
        if (componentJson.contains("meshPaths"))
        {
            json meshPaths = componentJson["meshPaths"];
            for (const auto &meshPath : meshPaths)
            {
                paths.emplace_back(meshPath.get<iaString>());
            }
        }

        component->setMeshPaths(paths);
    }

    static void writeMeshReference(json &componentJson, iMeshReferenceComponentPtr component)
    {
        componentJson["model"] = component->getModel()->getID();

        json pathsJson = json::array();
        for (const auto &path : component->getMeshPaths())
        {
            pathsJson.push_back(path);
        }

        if (!pathsJson.empty())
        {
            componentJson["meshPaths"] = pathsJson;
        }
    }

    static void readLight(iEntityPtr entity, const json &componentJson)
    {
        iLightComponentPtr component = new iLightComponent();
        entity->addComponent(component);

        component->setAmbient(componentJson["ambient"].get<iaColor3f>());
        component->setDiffuse(componentJson["diffuse"].get<iaColor3f>());
        component->setSpecular(componentJson["specular"].get<iaColor3f>());
        component->setType(static_cast<iLightType>(componentJson["type"].get<int>()));
    }

    static void writeLight(json &componentJson, iLightComponentPtr component)
    {
        componentJson["ambient"] = component->getAmbient();
        componentJson["diffuse"] = component->getDiffuse();
        componentJson["specular"] = component->getSpecular();
        componentJson["type"] = (int)component->getType();
    }

    void iPrefabIO::connectEntity(iEntityScenePtr scene, const json &entityJson)
    {
        if (!entityJson.contains("parent"))
        {
            return;
        }

        const iEntityID entityID = entityJson["id"].get<iaUUID>();
        iEntityPtr entity = scene->getEntity(entityID);

        auto parentID = entityJson["parent"].get<iaUUID>();
        entity->setParent(parentID);
    }

    void iPrefabIO::readEntity(iEntityScenePtr scene, const json &entityJson)
    {
        using ReadComponentFunc = std::function<void(iEntityPtr, const json &)>;
        const std::unordered_map<iaString, ReadComponentFunc> func = {
            {"transform", readTransform},
            {"camera", readCamera},
            {"sphere", readSphere},
            {"octree", readOctree},
            {"meshRender", readMeshRender},
            {"light", readLight},
            {"meshReference", readMeshReference}};

        const iaString entityName = entityJson["name"].get<iaString>();
        const iEntityID entityID = entityJson["id"].get<iaUUID>();
        const bool active = entityJson["active"].get<bool>();

        iEntityPtr entity = scene->createEntity(entityName, entityID);
        entity->setActive(active);

        if (!entityJson.contains("components"))
        {
            return;
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
    }

    bool iPrefabIO::read(const iaString &filename, const iPrefabPtr &prefab)
    {
        try // for catching json exceptions
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
            auto scene = iEntitySystemModule::getInstance().createScene(sceneName, sceneID, true);

            prefab->_sceneID = scene->getID();

            json entitiesJson = entityScene["entities"];
            for (const auto &entityJson : entitiesJson)
            {
                readEntity(scene, entityJson);
            }
            for (const auto &entityJson : entitiesJson)
            {
                connectEntity(scene, entityJson);
            }
        }
        catch (const std::exception &e)
        {
            con_err("Caught an exception: " << e.what());
            return false;
        }
        catch (...)
        {
            con_err("Caught an unknown exception.");
            return false;
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

        json componentsJson = json::array();
        auto componentTypes = entity->getComponentTypes();
        std::sort(componentTypes.begin(), componentTypes.end(), [](const std::type_index& a, const std::type_index& b) {
            return a.name() < b.name();
        });

        for (const auto &typeIndex : componentTypes)
        {
            const auto component = entity->getComponent(typeIndex);

            iTransformComponent *transform = dynamic_cast<iTransformComponent *>(component);
            if (transform != nullptr)
            {
                json componentJson;
                componentJson["componentType"] = "transform";
                writeTransform(componentJson, transform);
                componentsJson.push_back(componentJson);
                continue;
            }

            iCameraComponent *camera = dynamic_cast<iCameraComponent *>(component);
            if (camera != nullptr)
            {
                json componentJson;
                componentJson["componentType"] = "camera";
                writeCamera(componentJson, camera);
                componentsJson.push_back(componentJson);
                continue;
            }

            iSphereComponent *sphereCollision = dynamic_cast<iSphereComponent *>(component);
            if (sphereCollision != nullptr)
            {
                json componentJson;
                componentJson["componentType"] = "sphere";
                writeSphere(componentJson, sphereCollision);
                componentsJson.push_back(componentJson);
                continue;
            }

            iOctreeComponent *octree = dynamic_cast<iOctreeComponent *>(component);
            if (octree != nullptr)
            {
                json componentJson;
                componentJson["componentType"] = "octree";
                // nothing else to write here
                componentsJson.push_back(componentJson);
                continue;
            }

            iMeshRenderComponent *meshRender = dynamic_cast<iMeshRenderComponent *>(component);
            if (meshRender != nullptr)
            {
                json componentJson;
                componentJson["componentType"] = "meshRender";
                // nothing else to write here
                componentsJson.push_back(componentJson);
                continue;
            }

            iMeshReferenceComponent *meshReference = dynamic_cast<iMeshReferenceComponent *>(component);
            if (meshReference != nullptr)
            {
                json componentJson;
                componentJson["componentType"] = "meshReference";
                writeMeshReference(componentJson, meshReference);
                componentsJson.push_back(componentJson);
                continue;
            }

            iLightComponent *light = dynamic_cast<iLightComponent *>(component);
            if (light != nullptr)
            {
                json componentJson;
                componentJson["componentType"] = "light";
                writeLight(componentJson, light);
                componentsJson.push_back(componentJson);
                continue;
            }

            con_err("unknown component type for id: " << component->getID());
        }

        if (!componentsJson.empty())
        {
            entityJson["components"] = componentsJson;
        }
    }

    bool iPrefabIO::write(const iaString &filename, const iPrefabPtr &prefab)
    {
        try // for catching json exceptions
        {
            char temp[2048];
            filename.getData(temp, 2048);

            std::ofstream stream;
            stream.open(temp);

            auto scene = iEntitySystemModule::getInstance().getScene(prefab->getSceneID());

            json entitiesJson = json::array();
            for (const auto entity : scene->getEntities())
            {
                json entityJson;
                writeEntity(entityJson, entity);
                entitiesJson.push_back(entityJson);
            }

            json sceneJson = {
                {"name", scene->getName()},
                {"id", scene->getID()},
                {"entities", entitiesJson}};

            json dataJson;
            dataJson["entityScene"] = sceneJson;

            stream << dataJson.dump(4);
        }
        catch (const std::exception &e)
        {
            con_err("Caught an exception: " << e.what());
        }
        catch (...)
        {
            con_err("Caught an unknown exception.");
        }

        return true;
    }

} // namespace igor
