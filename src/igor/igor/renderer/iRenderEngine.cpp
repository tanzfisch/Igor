// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iRenderEngine.h>

#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iCameraComponent.h>
#include <igor/entities/components/iMeshRenderComponent.h>

namespace igor
{

    void iRenderEngine::setCamera(iEntityPtr camera)
    {
        con_assert(camera != nullptr, "zero pointer");
        con_assert(camera->getScene() == _scene, "incompatible scene");

        auto cameraComponent = camera->getComponent<iCameraComponent>();
        auto transform = camera->getComponent<iTransformComponent>();
        con_assert(cameraComponent != nullptr && transform != nullptr, "missing components");

        _cameraID = camera->getID();
    }

    void iRenderEngine::addMesh(iEntityPtr mesh)
    {
        con_assert(mesh != nullptr, "zero pointer");
        con_assert(mesh->getScene() == _scene, "incompatible scene");

        auto transform = mesh->getComponent<iTransformComponent>();
        auto meshRender = mesh->getComponent<iMeshRenderComponent>();
        con_assert(meshRender != nullptr && transform != nullptr, "missing components");

        // store in instancing buffers
    }

    void iRenderEngine::render()
    {

    }

} // namespace igor