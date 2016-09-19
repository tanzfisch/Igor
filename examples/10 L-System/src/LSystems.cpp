#include "LSystems.h"

#include <iMaterial.h>
#include <iMaterialGroup.h>
#include <iNodeVisitorPrintTree.h>
#include <iTaskManager.h>
#include <iNodeCamera.h>
#include <iNodeModel.h> 
#include <iNodeTransform.h>
#include <iRenderer.h>
#include <iApplication.h>
#include <iSceneFactory.h>
#include <iScene.h>
#include <iNodeFactory.h>
#include <iMouse.h>
#include <iKeyboard.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iNodeLight.h>
#include <iModelResourceFactory.h>
#include <iTaskFlushModels.h>
#include <iTaskFlushTextures.h>
#include <iMaterialResourceFactory.h>
#include <iStatistics.h>
#include <iNodeSwitch.h>
#include <iNodeLODSwitch.h>
#include <iNodeLODTrigger.h>
#include <iGradient.h>
using namespace Igor;

#include <iaConsole.h>
#include <iaString.h>
using namespace IgorAux;

LSystems::LSystems()
{
    init();
}

LSystems::~LSystems()
{
    deinit();
}

void LSystems::init()
{
    con(" -- 3D Example --" << endl);

    // setup window
    _window.setTitle("Igor - LSystems");
    _window.setSize(1280, 758);
    _window.setCentered();
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &LSystems::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &LSystems::onWindowResized));

    // setup perspective view
    _view.setClearColor(iaColor4f(0.25f, 0.25f, 0.25f, 1));
    _view.setPerspective(45);
    _view.setClipPlanes(0.1f, 10000.f);
    _view.registerRenderDelegate(RenderDelegate(this, &LSystems::onRender));
    _window.addView(&_view);

    // setup orthogonal view
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &LSystems::onRenderOrtho));
    _window.addView(&_viewOrtho);

    _window.open();

    // init scene
    _scene = iSceneFactory::getInstance().createScene();
    // bind scene to perspective view
    _view.setScene(_scene);

    // setup camera
    // we want a camera which can be rotated arround the origin
    // we will acchive that with 3 transform nodes
    // one is for the heading
    iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    // give the transform node a name. naming is optional and ist jus for helping to debug. 
    // Names do not have to be unique but since it is possible to find nodes by name they better are
    cameraHeading->setName("camera heading");
    cameraHeading->rotate(0.1, iaAxis::Y);
    _cameraHeading = cameraHeading->getID();
    // one is for the pitch
    iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    cameraPitch->setName("camera pitch");
    _cameraPitch = cameraPitch->getID();
    // and one is for translation or distance from the origin
    iNodeTransform* cameraTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    cameraTranslation->setName("camera translation");
    // translate away from origin
    cameraTranslation->translate(0, 0, 100);
    _cameraTranslation = cameraTranslation->getID();
    // from all nodes that we want to control later we save the node ID
    // and last but not least we create a camera node
    iNodeCamera* camera = static_cast<iNodeCamera*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeCamera));
    camera->setName("camera");
    // and build everything together
    // first we add the heading to the root node
    _scene->getRoot()->insertNode(cameraHeading);
    // than the pitch to the heading node
    cameraHeading->insertNode(cameraPitch);
    // then the translation to the pitch node
    cameraPitch->insertNode(cameraTranslation);
    // and than we add the camera to the translation
    cameraTranslation->insertNode(camera);
    // and finally we set the camera active. for this to work a camera must be part of a scene 
    // wich we achived by adding all those nodes on to an other starting with the root node
    camera->makeCurrent();

    // TODO
    /*_lSystem.addRule('F', "F[+F]F[-F]F");
    _lSystemResult = _lSystem.generate("F", 6);*/

    /*
    _lSystem.addRule('X', "F[+X]F[-X]+X");
    _lSystem.addRule('F', "FF");
    _lSystemResult = _lSystem.generate("X", 6);
    */

    vector<pair<float32, iaString>> weightedRule1;
    weightedRule1.push_back(pair<float32, iaString>(0.4, "F[+LX]F[-RX]+RX"));
    weightedRule1.push_back(pair<float32, iaString>(0.4, "F[+RX]F[-LX]"));
    weightedRule1.push_back(pair<float32, iaString>(0.1, "F[-Rx]+LX"));
    weightedRule1.push_back(pair<float32, iaString>(0.1, "F[-Rx]+Lx"));
    _lSystem.addRule('X', weightedRule1);

    vector<pair<float32, iaString>> weightedRule2;
    weightedRule2.push_back(pair<float32, iaString>(0.4, "FF"));
    weightedRule2.push_back(pair<float32, iaString>(0.15, "FRFL"));
    weightedRule2.push_back(pair<float32, iaString>(0.15, "FLFR"));
    weightedRule2.push_back(pair<float32, iaString>(0.15, "F+F-"));
    weightedRule2.push_back(pair<float32, iaString>(0.15, "F-F+"));
    _lSystem.addRule('F', weightedRule2);

    _lSystemMaterialID = iMaterialResourceFactory::getInstance().createMaterial();
    //iMaterialResourceFactory::getInstance().getMaterial(materialID)->getRenderStateSet()

    // init render statistics
    _font = new iTextureFont("StandardFont.png");
    iStatistics::getInstance().setVerbosity(iRenderStatisticsVerbosity::FPSAndMetrics);

    _taskFlushTexturesID = iTaskManager::getInstance().addTask(new iTaskFlushTextures(&_window));

    // register some callbacks
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &LSystems::onKeyPressed));
    iMouse::getInstance().registerMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &LSystems::onMouseMoved));
    iMouse::getInstance().registerMouseWheelDelegate(iMouseWheelDelegate(this, &LSystems::onMouseWheel));
}

void LSystems::deinit()
{
    // unregister some callbacks
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &LSystems::onKeyPressed));
    iMouse::getInstance().unregisterMouseMoveFullDelegate(iMouseMoveFullDelegate(this, &LSystems::onMouseMoved));
    iMouse::getInstance().unregisterMouseWheelDelegate(iMouseWheelDelegate(this, &LSystems::onMouseWheel));
    _window.unregisterWindowCloseDelegate(WindowCloseDelegate(this, &LSystems::onWindowClosed));
    _window.unregisterWindowResizeDelegate(WindowResizeDelegate(this, &LSystems::onWindowResized));
    _view.unregisterRenderDelegate(RenderDelegate(this, &LSystems::onRender));
    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &LSystems::onRenderOrtho));

    // deinit statistics
    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }

    iTaskManager::getInstance().getTask(_taskFlushTexturesID)->abort();

    iSceneFactory::getInstance().destroyScene(_scene);
    _scene = nullptr;

    if (_window.isOpen())
    {
        _window.close();
        _window.removeView(&_view);
        _window.removeView(&_viewOrtho);
    }

    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }
}

void LSystems::onRender()
{
    const float32 lenght = 0.5;
    const float32 angle = 0.4485;

    srand(123);

    for (int x = 0; x < 10; ++x)
    {
        iaMatrixf matrix;
        matrix.translate(-5, -30, 0);
        matrix.translate(rand()%10, 0, -rand()%10);
        
        iRenderer::getInstance().setModelMatrix(matrix);
        iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_lSystemMaterialID));
        iRenderer::getInstance().setLineWidth(2);

        _lSystemResult = _lSystem.generate("X", 6);

        vector<pair<iaVector3f, iaVector3f>> stack;
        iaVector3f pos;
        iaVector3f dir(0, lenght, 0);

        iaMatrixf rotateRight;
        rotateRight.rotate(angle, iaAxis::Z);

        iaMatrixf rotateLeft;
        rotateLeft.rotate(-angle, iaAxis::Z);

        iaMatrixf rotateRightLess;
        rotateRightLess.rotate(angle, iaAxis::X);

        iaMatrixf rotateLeftLess;
        rotateLeftLess.rotate(-angle, iaAxis::X);

        for (int i = 0; i < _lSystemResult.getSize(); ++i)
        {
            float32 variation = 1.2 - (((rand() % 100) / 100.0f) * 0.4);

            switch (_lSystemResult[i])
            {
            case 'F':
                iRenderer::getInstance().setColor(iaColor4f(0, 0.9, 0, 1));
                iRenderer::getInstance().drawLine(pos, pos + dir);
                pos += dir * variation;
                break;

            case 'x':
                iRenderer::getInstance().setColor(iaColor4f(0.6 * variation, 0.2, 0, 1));
                iRenderer::getInstance().setPointSize(5 * variation);
                iRenderer::getInstance().drawPoint(pos);
                break;

            case 'X':
                iRenderer::getInstance().setColor(iaColor4f(0.9 * variation, 0, 0, 1));
                iRenderer::getInstance().setPointSize(10 * variation);
                iRenderer::getInstance().drawPoint(pos);
                break;

            case 'R':
            {
                iaMatrixf matrix;
                matrix.rotate(angle * variation, iaAxis::X);
                dir = matrix * dir;
            }
            break;

            case 'L':
            {
                iaMatrixf matrix;
                matrix.rotate(-angle * variation, iaAxis::X);
                dir = matrix * dir;
            }
            break;

            case '+':
            {
                iaMatrixf matrix;
                matrix.rotate(angle * variation, iaAxis::Z);
                dir = matrix * dir;
            }
            break;

            case '-':
            {
                iaMatrixf matrix;
                matrix.rotate(-angle * variation, iaAxis::Z);
                dir = matrix * dir;
            }
            break;

            case '[':
                stack.push_back(pair<iaVector3f, iaVector3f>(pos, dir));
                break;

            case ']':
                pos = stack.back().first;
                dir = stack.back().second;
                stack.pop_back();
                break;
            }
        }
    }
}

void LSystems::onMouseWheel(int32 d)
{
    iNodeTransform* camTranslation = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_cameraTranslation));
    if (camTranslation != nullptr)
    {
        if (d < 0)
        {
            camTranslation->translate(0, 0, 10);
        }
        else
        {
            camTranslation->translate(0, 0, -10);
        }
    }
}

void LSystems::onMouseMoved(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* _window)
{
    if (iMouse::getInstance().getLeftButton())
    {
        iNodeTransform* cameraPitch = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_cameraPitch));
        iNodeTransform* cameraHeading = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_cameraHeading));

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraHeading->rotate((x1 - x2) * 0.005f, iaAxis::Y);
            iMouse::getInstance().setCenter(true);
        }
    }
}

void LSystems::onWindowClosed()
{
    iApplication::getInstance().stop();
}

void LSystems::onWindowResized(int32 clientWidth, int32 clientHeight)
{
    _viewOrtho.setOrthogonal(0, clientWidth, clientHeight, 0);
}

void LSystems::onKeyPressed(iKeyCode key)
{
    if (key == iKeyCode::ESC)
    {
        iApplication::getInstance().stop();
    }

    if (key == iKeyCode::F1)
    {
        iNodeVisitorPrintTree printTree;
        if (_scene != nullptr)
        {
            printTree.printToConsole(_scene->getRoot());
        }
    }
}

void LSystems::onRenderOrtho()
{
    if (_font != nullptr)
    {
        iStatistics::getInstance().drawStatistics(&_window, _font, iaColor4f(1, 1, 1, 1));
    }
}

void LSystems::run()
{
    iApplication::getInstance().run();
}
