// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/traversal/iNodeVisitorBoundings.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/scene/nodes/iNodeTransform.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iNodeVisitorBoundings::iNodeVisitorBoundings()
    {
        setTraverseInactiveChildren();
    }

    bool iNodeVisitorBoundings::preOrderVisit(iNodePtr node, iNodePtr nextSibling)
    {
        bool hasSphere = false;
        iaSphered nodeSphere;

        if (node->getKind() == iNodeKind::Volume)
        {
            iNodeVolume *volume = static_cast<iNodeVolume *>(node);
            iaMatrixd matrix = volume->getWorldMatrix();
            nodeSphere = volume->getBoundingSphere();
            iaVector3d c(nodeSphere._center._x, nodeSphere._center._y, nodeSphere._center._z);
            c = matrix * c;
            nodeSphere._center.set(c._x, c._y, c._z);
            hasSphere = true;
        }
        else if (node->getType() == iNodeType::iNodeTransform)
        {
            iNodeTransform *transform = static_cast<iNodeTransform *>(node);
            iaMatrixd matrix;
            transform->calcWorldTransformation(matrix);
            nodeSphere._center = matrix._pos;
            hasSphere = true;
        }

        if (hasSphere)
        {
            if (_initialSphere)
            {
                _sphere = nodeSphere;
                _initialSphere = false;
            }
            else
            {
                _sphere.merge(nodeSphere);
            }
        }

        return true;
    }

    void iNodeVisitorBoundings::postOrderVisit(iNodePtr node)
    {
    }

    void iNodeVisitorBoundings::preTraverse()
    {
    }

    void iNodeVisitorBoundings::postTraverse()
    {
    }

    void iNodeVisitorBoundings::getSphere(iaSphered &sphere)
    {
        sphere = _sphere;
    }
} // namespace igor