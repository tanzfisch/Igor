// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
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
            iaMatrixf matrix = mesh->getWorldMatrix();
            iSpheref nodeSphere;
            nodeSphere = mesh->getBoundingSphere();
            nodeSphere._center = matrix * nodeSphere._center;

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

    void iNodeVisitorBoundings::getSphere(iSpheref& sphere)
    {
        sphere = _sphere;
    }
}