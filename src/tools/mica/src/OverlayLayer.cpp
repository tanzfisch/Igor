#include "OverlayLayer.h"

OverlayLayer::OverlayLayer(iWindow *window, int32 zIndex, WorkspacePtr workspace)
    : iLayer(window, "Overlay", zIndex), _workspace(workspace)
{
}

OverlayLayer::~OverlayLayer()
{
}

void OverlayLayer::onInit()
{
    // init 3D user controls
    _view.setName("Overlay");
    _view.setClearColor(false);
    _view.setClearDepth(true);
    _view.setPerspective(45.0f);
    _view.setClipPlanes(1.0f, 100000.f);
    _view.registerRenderDelegate(iDrawDelegate(this, &OverlayLayer::render));

    getWindow()->addView(&_view, getZIndex());

    _scene = iSceneFactory::getInstance().createScene();
    _scene->setName("Overlay");
    _view.setScene(_scene);

    _materialOrientationPlane = iMaterialResourceFactory::getInstance().createMaterial("OrientationPlane");
    auto oriPlaneMaterial = iMaterialResourceFactory::getInstance().getMaterial(_materialOrientationPlane);
    oriPlaneMaterial->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    oriPlaneMaterial->setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
    oriPlaneMaterial->setOrder(iMaterial::RENDER_ORDER_MAX);

    _materialCelShading = iMaterialResourceFactory::getInstance().createMaterial("celShadingMaterial");
    auto celShadingMaterial = iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading);
    celShadingMaterial->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
    celShadingMaterial->addShaderSource("Mica/yellow.frag", iShaderObjectType::Fragment);
    celShadingMaterial->compileShader();
    celShadingMaterial->setRenderState(iRenderState::Wireframe, iRenderStateValue::On);
    celShadingMaterial->setRenderState(iRenderState::CullFace, iRenderStateValue::On);
    celShadingMaterial->setRenderState(iRenderState::CullFaceFunc, iRenderStateValue::Front);

    // create materials
    _materialBoundingBox = iMaterialResourceFactory::getInstance().createMaterial("materialBoundingBox");

    // _manipulator = new Manipulator(&_window, &_view, _sceneWidget3D);
}

void OverlayLayer::onDeinit()
{
    _view.unregisterRenderDelegate(iDrawDelegate(this, &OverlayLayer::render));
}

void OverlayLayer::render()
{
    renderSelection();
    renderOrientationPlane();
}

void OverlayLayer::renderSelection()
{
    for (auto nodeID : _workspace->getSelection())
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node == nullptr)
        {
            continue;
        }

        if (node->getKind() != iNodeKind::Renderable &&
            node->getKind() != iNodeKind::Volume)
        {
            continue;
        }

        iNodeRender *renderNode = static_cast<iNodeRender *>(node);
        iaMatrixd matrix = renderNode->getWorldMatrix();
        iRenderer::getInstance().setModelMatrix(matrix);

        if (node->getType() == iNodeType::iNodeMesh)
        {
            iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading));

            iNodeMesh *meshNode = static_cast<iNodeMesh *>(node);
            std::shared_ptr<iMeshBuffers> buffers = meshNode->getMeshBuffers();
            iRenderer::getInstance().setLineWidth(4);
            iRenderer::getInstance().drawMesh(buffers);
        }
        else
        {
            if (node->getKind() == iNodeKind::Volume)
            {
                iNodeVolume *renderVolume = static_cast<iNodeVolume *>(node);
                iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_materialBoundingBox));

                iAABoxd box = renderVolume->getBoundingBox();

                iRenderer::getInstance().setColor(1, 1, 0, 1);
                iRenderer::getInstance().drawBBox(box);
            }
        }
    }
}

void OverlayLayer::renderOrientationPlane()
{
    iaMatrixd identity;
    iRenderer::getInstance().setModelMatrix(identity);

    iRenderer::getInstance().setMaterial(_materialOrientationPlane);
    iRenderer::getInstance().setLineWidth(1);

    for (int i = -20; i < 21; ++i)
    {
        if (i % 2 == 0)
        {
            iRenderer::getInstance().setColor(1.0f, 1.0f, 1.0f, 0.5f);
        }
        else
        {
            iRenderer::getInstance().setColor(1.0f, 1.0f, 1.0f, 0.25f);
        }

        iRenderer::getInstance().drawLine(iaVector3f(-20.0f, 0.0f, i), iaVector3f(20.0f, 0.0f, i));
        iRenderer::getInstance().drawLine(iaVector3f(i, 0.0f, 20.0f), iaVector3f(i, 0.0f, -20.0f));
    }

    iRenderer::getInstance().setColor(1.0f, 0.0f, 0.0f, 1.0f);
    iRenderer::getInstance().drawLine(iaVector3f(0.0f, 0.0f, 0.0f), iaVector3f(20.0f, 0.0f, 0.0f));
    iRenderer::getInstance().setColor(0.0f, 0.0f, 1.0f, 1.0f);
    iRenderer::getInstance().drawLine(iaVector3f(0.0f, 0.0f, 0.0f), iaVector3f(0.0f, 0.0f, 20.0f));
}
