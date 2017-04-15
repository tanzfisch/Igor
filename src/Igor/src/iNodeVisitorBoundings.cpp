// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeVisitorBoundings.h>
#include <iNode.h>
#include <iNodeMesh.h>
#include <iMesh.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iNodeVisitorBoundings::iNodeVisitorBoundings()
    {
        setTraverseInactiveChildren();
    }

    bool iNodeVisitorBoundings::preOrderVisit(iNode* node)
    {
        if (iNodeType::iNodeMesh == node->getType())
        {
            iNodeMesh* mesh = static_cast<iNodeMesh*>(node);
            iaMatrixd matrix = mesh->getWorldMatrix();
            iSphered nodeSphere;
            nodeSphere = mesh->getBoundingSphere();
            iaVector3d c(nodeSphere._center._x, nodeSphere._center._y, nodeSphere._center._z);
            c = matrix * c;
            nodeSphere._center.set(c._x, c._y, c._z);

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

    void iNodeVisitorBoundings::postOrderVisit(iNode* node)
    {
    }

    void iNodeVisitorBoundings::preTraverse()
    {
    }

    void iNodeVisitorBoundings::postTraverse()
    {
        
    }

    void iNodeVisitorBoundings::getSphere(iSphered& sphere)
    {
        sphere = _sphere;
    }
}