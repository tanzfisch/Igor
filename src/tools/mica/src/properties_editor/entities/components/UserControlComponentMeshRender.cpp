// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentMeshRender.h"

#include <igor/entities/components/iMeshRenderComponent.h>

UserControlComponentMeshRender::UserControlComponentMeshRender(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Mesh Renderer", parent)
{
}

void UserControlComponentMeshRender::init()
{
    UserControlComponent::init();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr meshCountLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    meshCountLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    meshCountLayout->setStretchIndex(1);
    iWidgetLabelPtr labelMesh = new iWidgetLabel(meshCountLayout);
    labelMesh->setText("Meshes");
    labelMesh->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelMesh->setHorizontalAlignment(iHorizontalAlignment::Left);

    _labelMeshCount = new iWidgetLabel(meshCountLayout);
    _labelMeshCount->setHorizontalAlignment(iHorizontalAlignment::Left);    

    iWidgetBoxLayoutPtr trianglesCountLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    trianglesCountLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    trianglesCountLayout->setStretchIndex(1);
    iWidgetLabelPtr labelIndex = new iWidgetLabel(trianglesCountLayout);
    labelIndex->setText("Triangles");
    labelIndex->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelIndex->setHorizontalAlignment(iHorizontalAlignment::Left);

    _labelTrianglesCount = new iWidgetLabel(trianglesCountLayout);
    _labelTrianglesCount->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetBoxLayoutPtr vertexCountLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    vertexCountLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    vertexCountLayout->setStretchIndex(1);
    iWidgetLabelPtr labelVertex = new iWidgetLabel(vertexCountLayout);
    labelVertex->setText("Vertices");
    labelVertex->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVertex->setHorizontalAlignment(iHorizontalAlignment::Left);

    _labelVertexCount = new iWidgetLabel(vertexCountLayout);
    _labelVertexCount->setHorizontalAlignment(iHorizontalAlignment::Left);
}

void UserControlComponentMeshRender::onValueChanged(iWidgetPtr source)
{
    updateComponent();
}

void UserControlComponentMeshRender::update()
{
    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(_sceneID);
    if (scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(_entityID);
    if (entity == nullptr)
    {
        return;
    }

    iMeshRenderComponent *component = entity->getComponent<iMeshRenderComponent>();
    if (component == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    const auto meshReferences = component->getMeshReferences();
    const uint32 meshCount = meshReferences.size();
    uint32 vertexCount = 0;
    uint32 trianglesCount = 0;

    for (const auto &ref : meshReferences)
    {
        vertexCount += ref._mesh->getVertexCount();
        trianglesCount += ref._mesh->getTrianglesCount();
    
    _labelMeshCount->setText(iaString::toString(meshCount));}
    _labelVertexCount->setText(iaString::toString(vertexCount));
    _labelTrianglesCount->setText(iaString::toString(trianglesCount));

    _ignoreUpdate = false;
}

void UserControlComponentMeshRender::updateComponent()
{
    if (_ignoreUpdate)
    {
        return;
    }

    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(_sceneID);
    if (scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(_entityID);
    if (entity == nullptr)
    {
        return;
    }

    iMeshReferenceComponent *component = entity->getComponent<iMeshReferenceComponent>();
    if (component == nullptr)
    {
        return;
    }

    // TODO nothing to do here right?
}

void UserControlComponentMeshRender::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");
    
    entity->destroyComponent<iMeshReferenceComponent>();
}