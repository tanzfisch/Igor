// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/traversal/iEntityBoundsTraverser.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iMeshRenderComponent.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iEntityBoundsTraverser::iEntityBoundsTraverser()
    {
        setIgnoreInactive(false);
    }

    void iEntityBoundsTraverser::preTraverse()
    {
    }

    bool iEntityBoundsTraverser::preOrderVisit(iEntityPtr entity)
    {
        auto transformComponent = entity->getComponent<iTransformComponent>();
        if (transformComponent == nullptr)
        {
            return true;
        }

        auto meshRenderComponent = entity->getComponent<iMeshRenderComponent>();
        if (meshRenderComponent != nullptr)
        {
            for (const auto &meshRef : meshRenderComponent->getMeshReferences())
            {
                iaMatrixd matrix = transformComponent->getWorldMatrix();
                matrix *= meshRef._offset;

                const auto &box = meshRef._mesh->getBoundingBox();
                iaSphered sphere;
                iaVector4d boxCenter(box._center);
                auto sphereCenter = matrix * boxCenter;
                sphere._center.set(sphereCenter._x, sphereCenter._y, sphereCenter._z);
                auto radius = matrix * box._halfWidths;
                sphere._radius = radius.length();

                if (_initialSphere)
                {
                    _sphere = sphere;
                    _initialSphere = false;
                }
                else
                {
                    _sphere.merge(sphere);
                }
            }
        }

        return true;
    }

    void iEntityBoundsTraverser::postOrderVisit(iEntityPtr entity)
    {
    }

    void iEntityBoundsTraverser::postTraverse()
    {
    }

    const iaSphered &iEntityBoundsTraverser::getSphere() const
    {
        return _sphere;
    }
} // namespace igor