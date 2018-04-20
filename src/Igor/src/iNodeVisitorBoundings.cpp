// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeVisitorBoundings.h>
#include <iNode.h>
#include <iNodeMesh.h>
#include <iMesh.h>
#include <iNodeTransform.h>

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
        bool hasSphere = false;
        iSphered nodeSphere;

        if (node->getKind() == iNodeKind::Volume)
        {
            iNodeVolume* volume = static_cast<iNodeVolume*>(node);
            iaMatrixd matrix = volume->getWorldMatrix();
            nodeSphere = volume->getBoundingSphere();
            iaVector3d c(nodeSphere._center._x, nodeSphere._center._y, nodeSphere._center._z);
            c = matrix * c;
            nodeSphere._center.set(c._x, c._y, c._z);
            hasSphere = true;
        }
        else if (node->getType() == iNodeType::iNodeTransform)
        {
            iNodeTransform* transform = static_cast<iNodeTransform*>(node);
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