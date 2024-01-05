// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlMaterial.h"

UserControlMaterial::UserControlMaterial(iResourceID resourceID, const iWidgetPtr parent)
    : UserControlResource(resourceID, parent)
{
}

void UserControlMaterial::updateResource()
{
    iMaterialPtr material = iResourceManager::getInstance().getResource<iMaterial>(getResourceID());

    if (_ignoreUpdate ||
        material == nullptr)
    {
        return;
    }

    iResourceManager::getInstance().saveResource(getResourceID());
}

void UserControlMaterial::update()
{
    UserControlResource::update();

    iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>(getResourceID());

    _ignoreUpdate = true;

    _ambientColorChooser->setColor(material->getAmbient());
    _diffuseColorChooser->setColor(material->getDiffuse());
    _specularColorChooser->setColor(material->getSpecular());
    _emissiveColorChooser->setColor(material->getEmissive());

    _ignoreUpdate = false;
}

void UserControlMaterial::init()
{
    UserControlResource::init();

    iWidgetBoxLayoutPtr mainLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, getLayout());
    mainLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    mainLayout->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetGroupBox *lightGroupBox = new iWidgetGroupBox(mainLayout);
    lightGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    lightGroupBox->setText("Lighting");
    lightGroupBox->setHeaderOnly();

    iWidgetBoxLayoutPtr lightLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, lightGroupBox);

    _ambientColorChooser = new iUserControlColorChooser(lightLayout);
    _ambientColorChooser->setMode(iColorChooserMode::RGB);
    _ambientColorChooser->setText("Ambient");
    //_ambientColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlMaterial::onAmbientChange));

    _diffuseColorChooser = new iUserControlColorChooser(lightLayout);
    _diffuseColorChooser->setMode(iColorChooserMode::RGB);
    _diffuseColorChooser->setText("Diffuse");
    //_diffuseColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlMaterial::onDiffuseChange));

    _specularColorChooser = new iUserControlColorChooser(lightLayout);
    _specularColorChooser->setMode(iColorChooserMode::RGB);
    _specularColorChooser->setText("Specular");
    //_specularColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlMaterial::onSpecularChange));

    _emissiveColorChooser = new iUserControlColorChooser(lightLayout);
    _emissiveColorChooser->setMode(iColorChooserMode::RGB);
    _emissiveColorChooser->setText("Emissive");
    //_emissiveColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlMaterial::onEmissiveChange));

}

