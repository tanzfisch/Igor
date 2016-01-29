#include "GameScene.h"

#include "GameControl.h"

#include <iNodeModel.h>
#include <iModel.h>
#include <iNodeFactory.h>
#include <iSceneFactory.h>
#include <iApplication.h>
#include <iRenderer.h>
#include <iRenderStateSet.h>
#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iNodeSkyBox.h>
#include <iEntityFactory.h>
#include <iEntity.h>
#include <iPhysicsBody.h>
#include <iMaterialResourceFactory.h>
using namespace Igor;

GameScene::GameScene(GameControl* gameControl)
: _gameControl(gameControl)
{
    init();
}

GameScene::~GameScene()
{
    deinit();
}

void GameScene::init()
{
    _scene = iSceneFactory::getInstance().createScene();

    _cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _cameraDistance = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));

    _cameraHeading->insertNode(_cameraPitch);
    _cameraPitch->insertNode(_cameraDistance);
    _cameraDistance->insertNode(_camera);

    _cameraPitch->rotate(-45.0f / 180.0f * M_PI, iaAxis::X);
    _cameraHeading->rotate(45.0f / 180.0f * M_PI, iaAxis::Y);
    _cameraDistance->translate(0, 0, 7);

    initTiles();
    initMoveables();

    _moveables[_gameControl->getPlayer()]->insertNode(_cameraHeading);
    _camera->makeCurrent();

    _directionalLightRotate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _directionalLightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _directionalLightTranslate->translate(100, 100, 100);
    _directionalLightRotate->insertNode(_directionalLightTranslate);
    _lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    _lightNode->setAmbient(iaColor4f(0.6f, 0.6f, 0.6f, 1.0f));
    _lightNode->setDiffuse(iaColor4f(0.8f, 0.8f, 0.8f, 1.0f));
    _lightNode->setSpecular(iaColor4f(0.9f, 0.9f, 0.9f, 1.0f));

    _directionalLightTranslate->insertNode(_lightNode);
    _scene->getRoot()->insertNode(_directionalLightRotate);

    int32 materialSkyBox = iMaterialResourceFactory::getInstance().createMaterial("SkyBox");
    iMaterialResourceFactory::getInstance().getMaterial(materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(materialSkyBox)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterialGroup(materialSkyBox)->setOrder(10);

    iNodeSkyBox* skyBoxNode = static_cast<iNodeSkyBox*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSkyBox));
    skyBoxNode->setTextures(
        "skybox_default/front.png",
        "skybox_default/back.png",
        "skybox_default/left.png",
        "skybox_default/right.png",
        "skybox_default/top.png",
        "skybox_default/bottom.png");
    skyBoxNode->setTextureScale(10);
    skyBoxNode->setMaterial(materialSkyBox);
    _scene->getRoot()->insertNode(skyBoxNode);

    iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(this, &GameScene::onHandle));
}

void GameScene::deinit()
{
    iSceneFactory::getInstance().destroyScene(_scene);

    iApplication::getInstance().unregisterApplicationHandleDelegate(ApplicationHandleDelegate(this, &GameScene::onHandle));
}

void GameScene::initTiles()
{
    auto tiles = _gameControl->getTiles();
    auto width = tiles->getWidth();
    auto height = tiles->getHeight();

    for (int z = 0; z < height; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            uint32 value = tiles->getTile(x, z);
            iNodeTransform* tilePosition = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
            tilePosition->translate(x, 0, z);
            iNodeModel* tile = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));

            if (0 == value)
            {
                tile->setModel("grass.ompf");
            }
            else
            {
                tile->setModel("cube.ompf");
            }

            tilePosition->insertNode(tile);
            _scene->getRoot()->insertNode(tilePosition);
        }
    }
}

void GameScene::initMoveables()
{
    auto entities = iEntityFactory::getInstance().getEntities();
    auto entityIter = entities.begin();
    while (entities.end() != entityIter)
    {
        iEntity* entity = (*entityIter).second;

/*        if (entity->getBody()->getBodyType() == PhysicsBodyType::Player)
        {
            iNodeTransform* moveablePosition = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
            moveablePosition->translate(entity->getPosition());
            iNodeTransform* moveableOrientation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
            iNodeTransform* moveableOffset = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
            moveableOffset->rotate(M_PI * 0.5f, iaAxis::Y);
            moveableOffset->translate(0, 0, -0.4);
            iNodeModel* movable = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
            movable->setModel("cat.ompf");

            moveablePosition->insertNode(moveableOrientation);
            moveableOrientation->insertNode(moveableOffset);
            moveableOffset->insertNode(movable);
            _scene->getRoot()->insertNode(moveablePosition);

            _moveables[entity->getID()] = moveablePosition;
        }
        else
        {
            iNodeTransform* moveablePosition = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
            moveablePosition->translate(entity->getPosition());
            iNodeModel* movable = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
            movable->setModel("crate.ompf");

            moveablePosition->insertNode(movable);
            _scene->getRoot()->insertNode(moveablePosition);

            _moveables[entity->getID()] = moveablePosition;
        }*/

        ++entityIter;
    }
}

void GameScene::updateMoveables()
{
    auto entities = iEntityFactory::getInstance().getEntities();
    auto entityIter = entities.begin();
    while (entities.end() != entityIter)
    {
        iEntity* entity = (*entityIter).second;
/*        if (entity->getBody()->getBodyType() == PhysicsBodyType::Player)
        {
            iaMatrixf position;
            position._pos = entity->getPosition();
            _moveables[entity->getID()]->setMatrix(position);
            auto child = _moveables[entity->getID()]->getChildren().begin();
            iNodeTransform* rotationNode = static_cast<iNodeTransform*>((*child));

            iaMatrixf rotation;
            rotation.rotate(2.0f * M_PI * (static_cast<float32>(entity->getInt(_directionIndex)) / 8.0f), iaAxis::Y);
            rotationNode->setMatrix(rotation);
        }
        else
        {
            iaMatrixf matrix;
            entity->getMatrix(matrix);
            _moveables[entity->getID()]->setMatrix(matrix);
        }*/

        ++entityIter;
    }
}

void GameScene::onHandle()
{
    updateMoveables();

    _scene->handle();
}

iScene* GameScene::getScene()
{
    return _scene;
}
