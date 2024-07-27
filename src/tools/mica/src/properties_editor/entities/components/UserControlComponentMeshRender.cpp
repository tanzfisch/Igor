// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
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

    iWidgetBoxLayoutPtr indexCountLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    indexCountLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    indexCountLayout->setStretchIndex(1);
    iWidgetLabelPtr labelIndices = new iWidgetLabel(indexCountLayout);
    labelIndices->setText("Indices");
    labelIndices->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelIndices->setHorizontalAlignment(iHorizontalAlignment::Left);

    _labelIndexCount = new iWidgetLabel(indexCountLayout);
    _labelIndexCount->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetBoxLayoutPtr texUnitCountLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    texUnitCountLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    texUnitCountLayout->setStretchIndex(1);
    iWidgetLabelPtr labelTexUnits = new iWidgetLabel(texUnitCountLayout);
    labelTexUnits->setText("TexUnits");
    labelTexUnits->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelTexUnits->setHorizontalAlignment(iHorizontalAlignment::Left);    

    _labelTexUnitCount = new iWidgetLabel(texUnitCountLayout);
    _labelTexUnitCount->setHorizontalAlignment(iHorizontalAlignment::Left);
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

    const auto mesh = component->getMesh();
    if (mesh != nullptr)
    {
        _labelIndexCount->setText(iaString::toString(mesh->getIndexCount()));
        _labelVertexCount->setText(iaString::toString(mesh->getVertexCount()));
        _labelTrianglesCount->setText(iaString::toString(mesh->getTrianglesCount()));
        _labelTexUnitCount->setText(iaString::toString(mesh->getTextureUnitCount()));
    }
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
