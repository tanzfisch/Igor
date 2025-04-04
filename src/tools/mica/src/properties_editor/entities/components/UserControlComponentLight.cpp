// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentLight.h"

#include <igor/entities/components/iLightComponent.h>

UserControlComponentLight::UserControlComponentLight(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Sphere", parent)
{    
}

void UserControlComponentLight::init()
{
    UserControlComponent::init();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr lightTypeLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    lightTypeLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    lightTypeLayout->setStretchIndex(1);
    iWidgetLabelPtr labelType = new iWidgetLabel(lightTypeLayout);
    labelType->setText("Type");
    labelType->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelType->setHorizontalAlignment(iHorizontalAlignment::Left);

    _lightType = new iWidgetSelectBox(lightTypeLayout);
    _lightType->addItem("Directional", iLightType::Directional);
    _lightType->addItem("Point", iLightType::Point);
    _lightType->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _lightType->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentLight::onValueChanged));

    iWidgetBoxLayoutPtr ambientLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    ambientLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    ambientLayout->setStretchIndex(1);
    iWidgetLabelPtr labelAmbient = new iWidgetLabel(ambientLayout);
    labelAmbient->setText("Ambient");
    labelAmbient->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelAmbient->setHorizontalAlignment(iHorizontalAlignment::Left);

    _ambient = new iUserControlColor(ambientLayout);
    _ambient->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _ambient->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentLight::onValueChanged));

    iWidgetBoxLayoutPtr diffuseLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    diffuseLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    diffuseLayout->setStretchIndex(1);
    iWidgetLabelPtr labelDiffuse = new iWidgetLabel(diffuseLayout);
    labelDiffuse->setText("Diffuse");
    labelDiffuse->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelDiffuse->setHorizontalAlignment(iHorizontalAlignment::Left);

    _diffuse = new iUserControlColor(diffuseLayout);
    _diffuse->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _diffuse->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentLight::onValueChanged));    

    iWidgetBoxLayoutPtr specularLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    specularLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    specularLayout->setStretchIndex(1);
    iWidgetLabelPtr labelSpecular = new iWidgetLabel(specularLayout);
    labelSpecular->setText("Diffuse");
    labelSpecular->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelSpecular->setHorizontalAlignment(iHorizontalAlignment::Left);

    _specular = new iUserControlColor(specularLayout);
    _specular->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _specular->registerOnChangeEvent(iChangeDelegate(this, &UserControlComponentLight::onValueChanged));      
}

void UserControlComponentLight::onValueChanged(iWidgetPtr source)
{
    updateComponent();
}

void UserControlComponentLight::update()
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

    iLightComponent *component = entity->getComponent<iLightComponent>();
    if (component == nullptr)
    {
        return;
    }

    _ignoreUpdate = true;

    _lightType->setSelection(std::any(component->getType()));
    _ambient->setColor(component->getAmbient());
    _diffuse->setColor(component->getDiffuse());
    _specular->setColor(component->getSpecular());

    _ignoreUpdate = false;
}

void UserControlComponentLight::updateComponent()
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

    iLightComponent *component = entity->getComponent<iLightComponent>();
    if (component == nullptr)
    {
        return;
    }

    component->setType(std::any_cast<iLightType>(_lightType->getSelectedUserData()));
    const iaColor4f &ambient = _ambient->getColor();
    component->setAmbient(ambient._r, ambient._g, ambient._b);
    const iaColor4f &diffuse = _diffuse->getColor();
    component->setDiffuse(diffuse._r, diffuse._g, diffuse._b);
    const iaColor4f &specular = _specular->getColor();
    component->setSpecular(specular._r, specular._g, specular._b);
}
