// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentSpriteRender.h"

#include <igor/entities/components/iSpriteRenderComponent.h>

UserControlComponentSpriteRender::UserControlComponentSpriteRender(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Sprite Renderer", parent)
{
}

void UserControlComponentSpriteRender::onInit()
{
    UserControlComponent::onInit();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr spriteLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    spriteLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    spriteLayout->setStretchIndex(1);
    iWidgetLabelPtr labelSprite = new iWidgetLabel(spriteLayout);
    labelSprite->setText("Sprite");
    labelSprite->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelSprite->setHorizontalAlignment(iHorizontalAlignment::Left);
    labelSprite->setVerticalAlignment(iVerticalAlignment::Top);

    _sprite = new iUserControlSprite(spriteLayout);
    _sprite->getChangeEvent().add(iChangeDelegate(this, &UserControlComponentSpriteRender::onValueChanged));

    iWidgetBoxLayoutPtr sizeLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    sizeLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    sizeLayout->setStretchIndex(1);
    iWidgetLabelPtr labelSize = new iWidgetLabel(sizeLayout);
    labelSize->setText("Size");
    labelSize->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelSize->setHorizontalAlignment(iHorizontalAlignment::Left);

    _size = new iUserControlVector(2, sizeLayout);
    _size->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _size->getChangeEvent().add(iChangeDelegate(this, &UserControlComponentSpriteRender::onValueChanged));

    iWidgetBoxLayoutPtr colorLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    colorLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    colorLayout->setStretchIndex(1);
    iWidgetLabelPtr labelColor = new iWidgetLabel(colorLayout);
    labelColor->setText("Color");
    labelColor->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelColor->setHorizontalAlignment(iHorizontalAlignment::Left);

    _color = new iUserControlColor(colorLayout);
    _color->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _color->getChangeEvent().add(iChangeDelegate(this, &UserControlComponentSpriteRender::onValueChanged));

    iWidgetBoxLayoutPtr zIndexLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    zIndexLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    zIndexLayout->setStretchIndex(1);
    iWidgetLabelPtr labelZIndex = new iWidgetLabel(zIndexLayout);
    labelZIndex->setText("zIndex");
    labelZIndex->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelZIndex->setHorizontalAlignment(iHorizontalAlignment::Left);

    _zIndex = new iWidgetLineTextEdit(zIndexLayout);
    _zIndex->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _zIndex->getChangeEvent().add(iChangeDelegate(this, &UserControlComponentSpriteRender::onValueChanged));

    iWidgetBoxLayoutPtr renderModeLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    renderModeLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    renderModeLayout->setStretchIndex(1);
    iWidgetLabelPtr labelRenderMode = new iWidgetLabel(renderModeLayout);
    labelRenderMode->setText("Mode");
    labelRenderMode->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelRenderMode->setHorizontalAlignment(iHorizontalAlignment::Left);

    _mode = new iWidgetSelectBox(renderModeLayout);
    _mode->addItem("Simple");
    _mode->addItem("Tiled");
    _mode->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _mode->getChangeEvent().add(iChangeDelegate(this, &UserControlComponentSpriteRender::onValueChanged));
    
    iWidgetBoxLayoutPtr frameIndexLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    frameIndexLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    frameIndexLayout->setStretchIndex(1);
    iWidgetLabelPtr labelFrameIndex = new iWidgetLabel(frameIndexLayout);
    labelFrameIndex->setText("Frame");
    labelFrameIndex->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelFrameIndex->setHorizontalAlignment(iHorizontalAlignment::Left);

    _frameIndex = new iWidgetLineTextEdit(frameIndexLayout);
    _frameIndex->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _frameIndex->getChangeEvent().add(iChangeDelegate(this, &UserControlComponentSpriteRender::onValueChanged));
}

void UserControlComponentSpriteRender::onValueChanged(iWidgetPtr source)
{
    onUpdateComponent();
}

void UserControlComponentSpriteRender::onUpdateUI()
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

    iSpriteRenderComponent *component = entity->getComponent<iSpriteRenderComponent>();
    if (component == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    if (component->getSprite() != nullptr)
    {
        _sprite->setID(component->getSprite()->getID());
    }
    else
    {
        _sprite->setID(iResourceID::getInvalid());
    }

    _size->setValue(0, component->getSize()._x);
    _size->setValue(1, component->getSize()._y);
    _color->setColor(component->getColor());
    _zIndex->setText(iaString::toString(component->getZIndex()));
    _mode->setSelection(static_cast<uint32>(component->getRenderMode()));
    _frameIndex->setText(iaString::toString(component->getFrameIndex()));

    _ignoreUpdate = false;
}

void UserControlComponentSpriteRender::onUpdateComponent()
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

    iSpriteRenderComponent *component = entity->getComponent<iSpriteRenderComponent>();
    if (component == nullptr)
    {
        return;
    }

    component->setSprite(iResourceManager::getInstance().getResource<iSprite>(_sprite->getID()));
    component->setSize(iaVector2d(_size->getValue(0), _size->getValue(1)));
    component->setColor(_color->getColor());
    component->setZIndex(iaString::toInt(_zIndex->getText()));
    component->setRenderMode( static_cast<iSpriteRenderComponent::iRenderMode>(_mode->getSelectedIndex()));
    component->setFrameIndex(iaString::toInt(_frameIndex->getText()));
}

void UserControlComponentSpriteRender::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");

    entity->destroyComponent<iSpriteRenderComponent>();
}
