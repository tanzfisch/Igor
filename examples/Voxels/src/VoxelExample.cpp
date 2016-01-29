// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "VoxelExample.h"

#include <iaConsole.h>
using namespace IgorAux;

#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iNodeVisitorPrintTree.h>
#include <iTaskManager.h>
#include <iNodeSkyBox.h>
#include <iNodeLight.h>
#include <iNodeCamera.h>
#include <iNodeModel.h> 
#include <iNodeTransform.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeFactory.h>
#include <iMouse.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iTaskFlushModels.h>
#include <iTaskFlushTextures.h>
#include <iMaterialResourceFactory.h>
#include <iVoxelData.h>
#include <iMeshBuilder.h>
#include <iaVector3.h>
#include <iContouringCubes.h>
#include <iTextureResourceFactory.h>
#include <iPixmap.h>
#include <iStatistics.h>
#include <iTargetMaterial.h>
#include <iPerlinNoise.h>
#include <iNodeTransformControl.h>
#include <iNodeLODTrigger.h>
#include <iNodeLODSwitch.h>
#include <iOctree.h>
#include <iPhysicsManager.h>
using namespace Igor;

#include "Player.h"

VoxelExample::VoxelExample()
{
    init();
}

VoxelExample::~VoxelExample()
{
    deinit();
}

void VoxelExample::registerHandles()
{
    iKeyboard::getInstance().registerKeyDownDelegate(KeyDownDelegateExt(this, &VoxelExample::keyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(MouseMoveFullDelegate(this, &VoxelExample::mouseMoved));
    iMouse::getInstance().registerMouseKeyDownDelegate(MouseKeyDownDelegate(this, &VoxelExample::mouseDown));
    iMouse::getInstance().registerMouseWheelDelegate(MouseWheelDelegate(this, &VoxelExample::mouseWheel));

    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &VoxelExample::windowResized));
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &VoxelExample::windowClosed));

    iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(this, &VoxelExample::handle));
}

void VoxelExample::unregisterHandles()
{
    iApplication::getInstance().unregisterApplicationHandleDelegate(ApplicationHandleDelegate(this, &VoxelExample::handle));

    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &VoxelExample::windowResized));
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &VoxelExample::windowClosed));

    iMouse::getInstance().unregisterMouseKeyDownDelegate(MouseKeyDownDelegate(this, &VoxelExample::mouseDown));
    iMouse::getInstance().unregisterMouseWheelDelegate(MouseWheelDelegate(this, &VoxelExample::mouseWheel));

    iMouse::getInstance().unregisterMouseMoveFullDelegate(MouseMoveFullDelegate(this, &VoxelExample::mouseMoved));
    iKeyboard::getInstance().unregisterKeyDownDelegate(KeyDownDelegateExt(this, &VoxelExample::keyPressed));
}

void VoxelExample::initViews()
{
    _view.setClearColor(iaColor4f(0.97, 0.97, 1.0, 1));
    _view.setPerspective(60);
    _view.setClipPlanes(0.1f, 400.f);
    _view.registerRenderDelegate(RenderDelegate(this, &VoxelExample::render));

    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0, 1280, 768, 0);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &VoxelExample::renderOrtho));

    _window.setTitle("VoxelExample");
    _window.addView(&_view);
    _window.addView(&_viewOrtho);
    _window.setSize(1280, 768);
    _window.open();
}

void VoxelExample::initScene()
{
    _scene = iSceneFactory::getInstance().createScene();
    iPhysicsManager::getInstance().setScene(_scene);

    _view.setScene(_scene);

    // light
    _lightTranslate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _lightTranslate->translate(100, 100, 100);
    _lightRotate = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _lightNode = static_cast<iNodeLight*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeLight));
    _lightNode->setAmbient(iaColor4f(0.6f, 0.6f, 0.6f, 1.0f));
    _lightNode->setDiffuse(iaColor4f(0.9f, 0.7f, 0.6f, 1.0f));
    _lightNode->setSpecular(iaColor4f(1.0f, 0.9f, 0.87f, 1.0f));

    _lightRotate->insertNode(_lightTranslate);
    _lightTranslate->insertNode(_lightNode);
    _scene->getRoot()->insertNode(_lightRotate);

    _markerTranform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    iNodeTransform* offset = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    offset->translate(0, 1, 0);
    iNodeTransform* scale1 = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    scale1->scale(20, 0.5, 0.5);
    iNodeTransform* scale2 = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    scale2->scale(0.5, 20, 0.5);
    iNodeTransform* scale3 = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    scale3->scale(0.5, 0.5, 20);

    iNodeModel* marker1 = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    marker1->setModel("crate.ompf");
    iNodeModel* marker2 = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    marker2->setModel("crate.ompf");
    iNodeModel* marker3 = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
    marker3->setModel("crate.ompf");

    _scene->getRoot()->insertNode(_markerTranform);
    _markerTranform->insertNode(offset);
    offset->insertNode(scale1);
    offset->insertNode(scale2);
    offset->insertNode(scale3);
    scale1->insertNode(marker1);
    scale2->insertNode(marker2);
    scale3->insertNode(marker3);
}

void VoxelExample::initPlayer()
{
    _player = new Player(_scene);
}

void VoxelExample::init()
{
    con(" -- OpenGL 3D Test --" << endl);

    initViews();
    initScene();
    initPlayer();
    initVoxelData();

    // set up octree debug rendering
    _octreeMaterial = iMaterialResourceFactory::getInstance().createMaterial("Octree");
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->getRenderStateSet().setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    iMaterialResourceFactory::getInstance().getMaterial(_octreeMaterial)->getRenderStateSet().setRenderState(iRenderState::Wireframe, iRenderStateValue::On);

    // set up statistics
    _font = new iTextureFont("StandardFont.png");
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial("TextureAndBlending");
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    iStatistics::getInstance().setVerbosity(iRenderStatisticsVerbosity::None);

    // launch resource handlers
    iTaskManager::getInstance().addTask(new iTaskFlushModels(&_window));
    iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    registerHandles();
}

void VoxelExample::deinit()
{
    unregisterHandles();

    iSceneFactory::getInstance().destroyScene(_scene);

    _view.unregisterRenderDelegate(RenderDelegate(this, &VoxelExample::render));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &VoxelExample::renderOrtho));

    _window.close();
    _window.removeView(&_view);
    _window.removeView(&_viewOrtho);

    if (_font)
    {
        delete _font;
    }
}

void VoxelExample::mouseWheel(int d)
{
    if (iKeyboard::getInstance().getKey(iKeyCode::LShift))
    {
        if (d > 0)
        {
            _toolDensity += 10;
            if (_toolDensity > 255)
            {
                _toolDensity = 255;
            }
        }
        else
        {
            _toolDensity -= 10;

            if (_toolDensity < 1)
            {
                _toolDensity = 1;
            }
        }

        con_endl("tool density " << _toolDensity);
    }
    else
    {
        if (d > 0)
        {
            _toolSize += 1;
            if (_toolSize > 10)
            {
                _toolSize = 10;
            }
        }
        else
        {
            _toolSize -= 1;

            if (_toolSize < 1)
            {
                _toolSize = 1;
            }
        }

        con_endl("tool size " << _toolSize);
    }
}

void VoxelExample::mouseDown(iKeyCode key)
{
    if (key == iKeyCode::MouseRight)
    {
        uint8 density = static_cast<uint8>(_toolDensity);
        if (iKeyboard::getInstance().getKey(iKeyCode::LShift))
        {
            density = 0;
        }

        iaMatrixf matrix;
        _markerTranform->getMatrix(matrix);
        iaVector3I markerPosition(matrix._pos._x, matrix._pos._y, matrix._pos._z);
        
        int64 halfToolSize = _toolSize / 2;

        for (int x = -halfToolSize; x < halfToolSize + 1; ++x)
        {
            for (int y = -halfToolSize; y < halfToolSize + 1; ++y)
            {
                for (int z = -halfToolSize; z < halfToolSize + 1; ++z)
                {
                    iaVector3I pos(x, y, z);
                    _voxelTerrainGenerator->setVoxelDensity(markerPosition + pos, density);
                }
            }
        }

        iaVector3I tilePosition = markerPosition / 32;

        for (int x = -1; x < 2; ++x)
        {
            for (int y = -1; y < 2; ++y)
            {
                for (int z = -1; z < 2; ++z)
                {
                    iaVector3I pos(x, y, z);
                    _voxelTerrainGenerator->refreshTile(tilePosition + pos);
                }
            }
        }
    }
}

void VoxelExample::mouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
{
    if (iMouse::getInstance().getMiddleButton())
    {
        float32 dx = static_cast<float32>(x1 - x2) * 0.005f;
        float32 dy = static_cast<float32>(y1 - y2) * 0.005f;
        _lightRotate->rotate(dx, iaAxis::Y);
        _lightRotate->rotate(dy, iaAxis::X);

        iMouse::getInstance().setCenter(true);
    }
}

void VoxelExample::windowClosed()
{
    iApplication::getInstance().stop();
}

void VoxelExample::windowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0, clientWidth, clientHeight, 0);
}

void VoxelExample::keyPressed(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        break;

    case iKeyCode::F2:
        _renderOctree = !_renderOctree;
        break;

    case iKeyCode::F3:
    {
        iRenderStatisticsVerbosity level = iStatistics::getInstance().getVerbosity();

        if (level == iRenderStatisticsVerbosity::All)
        {
            level = iRenderStatisticsVerbosity::None;
        }
        else
        {
            int value = static_cast<int>(level);
            value++;
            level = static_cast<iRenderStatisticsVerbosity>(value);
        }

        iStatistics::getInstance().setVerbosity(level);
    }
    break;

    case iKeyCode::P:
        _player->printPosition();
        break;

    case iKeyCode::Space:
    {
        
    }
    break;
    }
}

void VoxelExample::updateMarkerPosition()
{
   iaMatrixf matrix;
    _player->getMatrix(matrix);

    iaVector3I dir(matrix._depth._x * 100.0f, matrix._depth._y * 100.0f, matrix._depth._z * 100.0f);
    dir.negate();
    iaVector3I from(matrix._pos._x, matrix._pos._y, matrix._pos._z);
    iaVector3I to(from);
    to += dir;

    iaVector3I result = _voxelTerrainGenerator->castRay(from, to);
    matrix.identity();
    matrix.translate(result._x, result._y, result._z);
    _markerTranform->setMatrix(matrix);
}

void VoxelExample::initVoxelData()
{
    _voxelTerrainGenerator = new VoxelTerrainGenerator(_scene);
    _voxelTerrainGenerator->setLODTrigger(_player->getLODTrigger());
}

void VoxelExample::handle()
{
    updateMarkerPosition();
}

void VoxelExample::render()
{
    if (_renderOctree)
    {
        iaMatrixf model;
        iRenderer::getInstance().setModelMatrix(model);

        iMaterialResourceFactory::getInstance().setMaterial(_octreeMaterial);
        iRenderer::getInstance().setColor(0, 1.0f, 0, 0.5f);
        _scene->getOctree()->draw();
    }
}

void VoxelExample::renderOrtho()
{
    iStatistics::getInstance().drawStatistics(&_window, _font, iaColor4f(1.0f, 0.5f, 0.0f, 1.0f));
}

void VoxelExample::run()
{
    iApplication::getInstance().run();
}

