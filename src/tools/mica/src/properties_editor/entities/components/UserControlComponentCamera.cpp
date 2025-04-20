// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentCamera.h"

#include <igor/entities/components/iCameraComponent.h>

UserControlComponentCamera::UserControlComponentCamera(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Camera", parent)
{
}

void UserControlComponentCamera::init()
{
    UserControlComponent::init();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr viewportLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    viewportLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    viewportLayout->setStretchIndex(1);
    iWidgetLabelPtr labelViewport = new iWidgetLabel(viewportLayout);
    labelViewport->setText("Viewport");
    labelViewport->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelViewport->setHorizontalAlignment(iHorizontalAlignment::Left);

    _viewport = new iUserControlVector(4, viewportLayout);
    _viewport->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _viewport->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentCamera::onValueChanged));

    iWidgetBoxLayoutPtr projectionLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    projectionLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    projectionLayout->setStretchIndex(1);
    iWidgetLabelPtr labelPerspective = new iWidgetLabel(projectionLayout);
    labelPerspective->setText("Perspec.");
    labelPerspective->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelPerspective->setHorizontalAlignment(iHorizontalAlignment::Left);

    _perspective = new iWidgetCheckBox(projectionLayout);
    _perspective->setHorizontalAlignment(iHorizontalAlignment::Left);
    _perspective->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentCamera::onProjectionChanged));

    _perspectiveLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, _layout);
    _perspectiveLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr fovLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _perspectiveLayout);
    fovLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    fovLayout->setStretchIndex(1);
    iWidgetLabelPtr labelFOV = new iWidgetLabel(fovLayout);
    labelFOV->setText("FOV");
    labelFOV->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelFOV->setHorizontalAlignment(iHorizontalAlignment::Left);

    _fov = new iWidgetLineTextEdit(fovLayout);
    _fov->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _fov->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentCamera::onValueChanged));

    _orthogonalLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, _layout);
    _orthogonalLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);

    for (int i = 0; i < 4; ++i)
    {
        iWidgetBoxLayoutPtr orthoLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _orthogonalLayout);
        orthoLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        orthoLayout->setStretchIndex(1);
        iWidgetLabelPtr labelOrtho = new iWidgetLabel(orthoLayout);
        switch (i)
        {
        case 0:
            labelOrtho->setText("Left");
            break;
        case 1:
            labelOrtho->setText("Right");
            break;
        case 2:
            labelOrtho->setText("Top");
            break;
        case 3:
            labelOrtho->setText("Bottom");
            break;
        }
        labelOrtho->setMinWidth(MICA_REGULARBUTTON_SIZE);
        labelOrtho->setHorizontalAlignment(iHorizontalAlignment::Left);

        _ortho[i] = new iWidgetLineTextEdit(orthoLayout);
        _ortho[i]->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        _ortho[i]->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentCamera::onValueChanged));
    }

    iWidgetBoxLayoutPtr clipPlanesLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    clipPlanesLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    clipPlanesLayout->setStretchIndex(1);
    iWidgetLabelPtr labelClipPlanes = new iWidgetLabel(clipPlanesLayout);
    labelClipPlanes->setText("Near/Far");
    labelClipPlanes->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelClipPlanes->setHorizontalAlignment(iHorizontalAlignment::Left);

    _clipNearFar = new iUserControlVector(2, clipPlanesLayout);
    _clipNearFar->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _clipNearFar->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentCamera::onValueChanged));    

    iWidgetBoxLayoutPtr activeClearColorLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    activeClearColorLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    activeClearColorLayout->setStretchIndex(1);
    iWidgetLabelPtr labelActiveClearColor = new iWidgetLabel(activeClearColorLayout);
    labelActiveClearColor->setText("Cl. Color");
    labelActiveClearColor->setTooltip("Clear color");
    labelActiveClearColor->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelActiveClearColor->setHorizontalAlignment(iHorizontalAlignment::Left);

    _activeClearColor = new iWidgetCheckBox(activeClearColorLayout);
    _activeClearColor->setHorizontalAlignment(iHorizontalAlignment::Left);
    _activeClearColor->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentCamera::onValueChanged));

    iWidgetBoxLayoutPtr clearColorLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    clearColorLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    clearColorLayout->setStretchIndex(1);
    new iWidgetSpacer(MICA_REGULARBUTTON_SIZE, 0, false, clearColorLayout);

    _clearColor = new iUserControlColor(clearColorLayout);
    _clearColor->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _clearColor->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentCamera::onValueChanged));

    iWidgetBoxLayoutPtr activeClearDepthLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    activeClearDepthLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    activeClearDepthLayout->setStretchIndex(1);
    iWidgetLabelPtr labelActiveClearDepth = new iWidgetLabel(activeClearDepthLayout);
    labelActiveClearDepth->setText("Cl. Depth");
    labelActiveClearDepth->setTooltip("Clear depth");
    labelActiveClearDepth->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelActiveClearDepth->setHorizontalAlignment(iHorizontalAlignment::Left);

    _activeClearDepth = new iWidgetCheckBox(activeClearDepthLayout);
    _activeClearDepth->setHorizontalAlignment(iHorizontalAlignment::Left);
    _activeClearDepth->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentCamera::onValueChanged));

    iWidgetBoxLayoutPtr clearDepthLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    clearDepthLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    clearDepthLayout->setStretchIndex(1);
    new iWidgetSpacer(MICA_REGULARBUTTON_SIZE, 0, false, clearDepthLayout);

    _clearDepth = new iWidgetLineTextEdit(clearDepthLayout);
    _clearDepth->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _clearDepth->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentCamera::onValueChanged));
}

void UserControlComponentCamera::onProjectionChanged(iWidgetPtr source)
{
    updateComponent();

    if (_perspective->isChecked())
    {
        _perspectiveLayout->setVisible(true);
        _orthogonalLayout->setVisible(false);
    }
    else
    {
        _perspectiveLayout->setVisible(false);
        _orthogonalLayout->setVisible(true);
    }
}

void UserControlComponentCamera::onValueChanged(iWidgetPtr source)
{
    updateComponent();
}

void UserControlComponentCamera::update()
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

    iCameraComponent *component = entity->getComponent<iCameraComponent>();
    if (component == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    auto rect = component->getViewport();
    _viewport->setValue(0, rect._x);
    _viewport->setValue(1, rect._y);
    _viewport->setValue(2, rect._width);
    _viewport->setValue(3, rect._height);

    _perspective->setChecked(component->getProjectionType() == iProjectionType::Perspective);

    _fov->setText(iaString::toString(component->getFieldOfView(), 4));

    _ortho[0]->setText(iaString::toString(component->getLeftOrtho(), 4));
    _ortho[1]->setText(iaString::toString(component->getRightOrtho(), 4));
    _ortho[2]->setText(iaString::toString(component->getTopOrtho(), 4));
    _ortho[3]->setText(iaString::toString(component->getBottomOrtho(), 4));

    if (_perspective->isChecked())
    {
        _perspectiveLayout->setVisible(true);
        _orthogonalLayout->setVisible(false);
    }
    else
    {
        _perspectiveLayout->setVisible(false);
        _orthogonalLayout->setVisible(true);
    }

    _clipNearFar->setValue(0, component->getNearClipPlane());
    _clipNearFar->setValue(1, component->getFarClipPlane());

    _activeClearColor->setChecked(component->isClearColorActive());
    _clearColor->setColor(component->getClearColor());

    _activeClearDepth->setChecked(component->isClearDepthActive());
    _clearDepth->setText(iaString::toString(component->getClearDepth(), 4));

    _ignoreUpdate = false;
}

void UserControlComponentCamera::updateComponent()
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

    iCameraComponent *component = entity->getComponent<iCameraComponent>();
    if (component == nullptr)
    {
        return;
    }

    iaRectangled rect(_viewport->getValue(0), _viewport->getValue(1), _viewport->getValue(2), _viewport->getValue(3));
    component->setViewportRelative(rect);

    if (_perspective->isChecked())
    {
        component->setPerspective(iaString::toFloat(_fov->getText()));
    }
    else
    {
        component->setOrthogonal(iaString::toFloat(_ortho[0]->getText()),
                                 iaString::toFloat(_ortho[1]->getText()),
                                 iaString::toFloat(_ortho[2]->getText()),
                                 iaString::toFloat(_ortho[3]->getText()));
    }

    component->setClipPlanes(_clipNearFar->getValue(0), _clipNearFar->getValue(1));

    component->setClearColor(_clearColor->getColor());
    component->setClearColorActive(_activeClearColor->isChecked());

    component->setClearDepth(iaString::toFloat(_clearDepth->getText()));
    component->setClearDepthActive(_activeClearDepth->isChecked());
}

void UserControlComponentCamera::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");
    
    entity->destroyComponent<iCameraComponent>();
}
