// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include "Evaluation.h"

#include <iaux/system/iaConsole.h>
#include <iaux/data/iaString.h>
#include <iaux/math/iaBSpline.h>
#include <iaux/data/iaConvert.h>
using namespace iaux;

#include <igor/resources/material/iMaterial.h>
#include <igor/scene/traversal/iNodeVisitorPrintTree.h>
#include <igor/threading/iTaskManager.h>
#include <igor/scene/nodes/iNodeSkyBox.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/graphics/iRenderer.h>
#include <igor/system/iApplication.h>
#include <igor/scene/iSceneFactory.h>
#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/system/iMouse.h>
#include <igor/system/iKeyboard.h>
#include <igor/system/iTimer.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/threading/tasks/iTaskFlushModels.h>
#include <igor/threading/tasks/iTaskFlushTextures.h>
#include <iaux/data/iaString.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/scene/nodes/iNodeSwitch.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>
#include <igor/scene/nodes/iNodeLODTrigger.h>
#include <igor/scene/traversal/iNodeVisitorRenderColorID.h>
#include <igor/evaluation/iEvaluationManager.h>
#include <igor/evaluation/iEvaluationTransform.h>
#include <igor/evaluation/iEvaluationScript.h>
using namespace igor;

Evaluation::Evaluation()
    : ExampleBase("Evaluation")
{
}

void Evaluation::init()
{
    // place some camera
    iNodeTransform *cameraTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraTransform->setName("camera transform");
    cameraTransform->translate(0, 0, 10);
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    camera->setName("camera");
    cameraTransform->insertNode(camera);
    getScene()->getRoot()->insertNode(cameraTransform);
    getView().setCurrentCamera(camera->getID());

    // transform node for the lights orientation
    iNodeTransform *directionalLightRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
    // transform node for the lights distance to the origin
    iNodeTransform *directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    directionalLightTranslate->translate(100, 100, 100);
    // the light node
    iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor4f(0.3f, 0.3f, 0.3f, 1.0f));
    lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
    // insert light to scene
    getScene()->getRoot()->insertNode(directionalLightRotate);
    directionalLightRotate->insertNode(directionalLightTranslate);
    directionalLightTranslate->insertNode(lightNode);

    // create a cat model
    iNodeTransformPtr catTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    catTransform->setName("cat transform");
    catTransform->translate(0, 0, 0);
    iNodeModel *catModel = iNodeManager::getInstance().createNode<iNodeModel>();
    // Node model names can be altered but they also are generated based on the file name
    catModel->setModel("cat.ompf");
    // building the created nodes together and insert them in the scene
    getScene()->getRoot()->insertNode(catTransform);
    catTransform->insertNode(catModel);

    // animate the cat model using evaluation
    iEvaluationTransform *transformEvaluation = iEvaluationManager::getInstance().createEvaluation<iEvaluationTransform>(catTransform->getID());

    // create some key frames using a bspline
    iaBSplinef bspline;
    bspline.addSupportPoint(iaVector3f(-4, 0, 0));
    bspline.addSupportPoint(iaVector3f(-2, 5, 0));
    bspline.addSupportPoint(iaVector3f(2, -5, 0));
    bspline.addSupportPoint(iaVector3f(4, 0, 0));

    std::vector<iaVector3f> points;
    bspline.getPoints(points, 20);

    for (auto point : points)
    {
        iaMatrixd matrix;
        matrix.translate(point._x, point._y, point._z);
        transformEvaluation->addKeyframe(matrix);
    }

    // keep repeating the animation
    transformEvaluation->setLooped();

    // set smooth easing function
    transformEvaluation->setEasingFunction(Easing::EasingFunction::InOutCubic);

    // set duration aka period to 4 seconds
    transformEvaluation->setDuration(iaTime::fromSeconds(4.0));
    // start now
    transformEvaluation->setStart(iTimer::getInstance().getFrameTime());

    // add a script evaluation on top
    iEvaluationScript *scriptEvaluation = iEvaluationManager::getInstance().createEvaluation<iEvaluationScript>(catTransform->getID());
    scriptEvaluation->setLooped();
    scriptEvaluation->setDuration(iaTime::fromSeconds(4.0));
    scriptEvaluation->setStart(iTimer::getInstance().getFrameTime());
    scriptEvaluation->setScript(iEvaluationScriptDelegate(this, &Evaluation::evalScript));
    scriptEvaluation->setEasingFunction(Easing::EasingFunction::OutElastic);
}

void Evaluation::evalScript(iNodeTransformPtr transform, float64 t)
{
    transform->rotate(t, iaAxis::Y);
}