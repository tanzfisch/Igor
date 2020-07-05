#include "MicaSceneLayer.h"

MicaSceneLayer::MicaSceneLayer(iWindow *window, int32 zIndex)
    : iLayer(window, "Scene", zIndex)
{
}

MicaSceneLayer::~MicaSceneLayer()
{
}

void MicaSceneLayer::onDeinit()
{
}

void MicaSceneLayer::onInit()
{
    _view.setName("Scene View");
    _view.setClearColor(iaColor4f(0.25f, 0.25f, 0.25f, 1.0f));
    _view.setPerspective(45.0f);
    _view.setClipPlanes(1.0f, 100000.f);
    _view.setScene(_workspace.getScene());
    getWindow()->addView(&_view, getZIndex());

    // cam
    _cameraCOI = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraCOI->setName("camera COI");
    _cameraHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraHeading->setName("camera heading");
    _cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraPitch->setName("camera pitch");
    _cameraTranslation = iNodeManager::getInstance().createNode<iNodeTransform>();
    _cameraTranslation->setName("camera translation");
    _cameraTranslation->translate(0, 0, 80);
    _camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    _camera->setName("camera");
    _workspace.getRoot()->insertNode(_cameraCOI);
    _cameraCOI->insertNode(_cameraHeading);
    _cameraHeading->insertNode(_cameraPitch);
    _cameraPitch->insertNode(_cameraTranslation);
    _cameraTranslation->insertNode(_camera);
    _view.setCurrentCamera(_camera->getID());

    // light
    _directionalLightRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
    _directionalLightRotate->setName("directional light rotate");

    _directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    _directionalLightTranslate->setName("directional light translate");
    _directionalLightTranslate->translate(10000, 10000, 0);

    _lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    _lightNode->setName("directional light");
    _lightNode->setAmbient(iaColor4f(0.5f, 0.5f, 0.5f, 1.0f));
    _lightNode->setDiffuse(iaColor4f(0.9f, 0.9f, 0.9f, 1.0f));
    _lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));

    _workspace.getRoot()->insertNode(_directionalLightRotate);
    _directionalLightRotate->insertNode(_directionalLightTranslate);
    _directionalLightTranslate->insertNode(_lightNode);
}

iNodePtr MicaSceneLayer::getNodeAt(int32 x, int32 y)
{
    //bool wasVisible = _defaultSkyBox->isVisible();
    //_defaultSkyBox->setVisible(false);

    uint64 nodeID = _view.pickcolorID(x, y);
    iNodePtr node = iNodeManager::getInstance().getNode(nodeID);

    //_defaultSkyBox->setVisible(wasVisible);

    return node;
}

void MicaSceneLayer::onPreDraw()
{
    updateCamDistanceTransform();
}

void MicaSceneLayer::rotateCamera(float64 heading, float64 pitch)
{
    _cameraPitch->rotate(pitch, iaAxis::X);
    _cameraHeading->rotate(heading, iaAxis::Y);
}

void MicaSceneLayer::updateCamDistanceTransform()
{
    _cameraTranslation->identity();
    _cameraTranslation->translate(0, 0, _camDistance);
}

void MicaSceneLayer::translateCOI(const iaVector3d &translate)
{
    _cameraCOI->translate(translate);
}

void MicaSceneLayer::getCamTransformation(iaMatrixd &matrix) const
{
    _cameraTranslation->getMatrix(matrix);
}

void MicaSceneLayer::getCamCOI(iaMatrixd &matrix) const
{
    _cameraCOI->getMatrix(matrix);
}

void MicaSceneLayer::frameOnSelection()
{
    auto selection = _workspace.getSelection();

    iNodeVisitorBoundings visitorBoundings;
    iSphered sphere;
    iSphered selectionSphere;

    for (auto nodeID : selection)
    {
        auto node = iNodeManager::getInstance().getNode(nodeID);
        if (node != nullptr)
        {
            visitorBoundings.traverseTree(node);
            visitorBoundings.getSphere(sphere);
            selectionSphere.merge(sphere);
        }
    }

    iaMatrixd coiMatrix;
    coiMatrix._pos = sphere._center;
    _cameraCOI->setMatrix(coiMatrix);

    if (sphere._radius > 0.0f)
    {
        _camDistance = sphere._radius * 4.0f;
    }
    else
    {
        _camDistance = 1.0f;
    }

    updateCamDistanceTransform();
}
