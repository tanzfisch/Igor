#include "CameraArc.h"

CameraArc::CameraArc(iNodePtr node)
{
    // cam
    iNodeTransformPtr cameraCOI = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraCOI->setName("arc camera COI");
    _cameraCOI = cameraCOI->getID();

    iNodeTransformPtr cameraHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraHeading->setName("arc camera heading");
    _cameraHeading = cameraHeading->getID();

    iNodeTransformPtr cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraPitch->setName("arc camera pitch");
    _cameraPitch = cameraPitch->getID();

    iNodeTransformPtr cameraDistance = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraDistance->setName("arc camera translation");
    cameraDistance->translate(0, 0, _distance);
    _cameraDistance = cameraDistance->getID();

    iNodeCameraPtr camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    camera->setName("arc camera");
    _camera = camera->getID();

    node->insertNode(cameraCOI);
    cameraCOI->insertNode(cameraHeading);
    cameraHeading->insertNode(cameraPitch);
    cameraPitch->insertNode(cameraDistance);
    cameraDistance->insertNode(camera);
}

float64 CameraArc::getHeading() const
{
    return _heading;
}

void CameraArc::setHeading(float64 heading)
{
    _heading = heading;

    iNodeTransformPtr camHeading = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_cameraHeading));
    if (camHeading == nullptr)
    {
        return;
    }

    camHeading->identity();
    camHeading->rotate(_heading, iaAxis::Y);
}

iNodeID CameraArc::getCameraDistanceNode() const
{
    return _cameraDistance;
}

iNodeID CameraArc::getCameraNode() const
{
    return _camera;
}

float64 CameraArc::getPitch() const
{
    return _pitch;
}

void CameraArc::setPitch(float64 pitch)
{
    _pitch = pitch;

    iNodeTransformPtr camPitch = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_cameraPitch));
    if (camPitch == nullptr)
    {
        return;
    }

    camPitch->identity();
    camPitch->rotate(_pitch, iaAxis::X);
}

float64 CameraArc::getDistance() const
{
    return _distance;
}

void CameraArc::setDistance(float64 distance)
{
    _distance = distance;

    iNodeTransformPtr camDistance = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_cameraDistance));
    if (camDistance == nullptr)
    {
        return;
    }

    camDistance->identity();
    camDistance->translate(0, 0, _distance);
}

void CameraArc::setCenterOfInterest(const iaVector3d &coi)
{
    iNodeTransformPtr camCOI = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_cameraCOI));
    if (camCOI == nullptr)
    {
        return;
    }

    camCOI->identity();
    camCOI->translate(coi);
}

iaVector3d CameraArc::getCenterOfInterest() const
{
    iaVector3d result;

    iNodeTransformPtr camCOI = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_cameraCOI));
    if (camCOI == nullptr)
    {
        return result;
    }

    iaMatrixd matrix;
    camCOI->getMatrix(matrix);
    return matrix._pos;
}