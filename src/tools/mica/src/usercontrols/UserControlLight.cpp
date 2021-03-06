// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlLight.h"

UserControlLight::UserControlLight()
{
    initGUI();
}

void UserControlLight::updateGUI()
{
    iNodeLight *lightNode = static_cast<iNodeLight *>(iNodeManager::getInstance().getNode(_lightNodeId));

    if (lightNode != nullptr)
    {
        _ambient = lightNode->getAmbient();
        _diffuse = lightNode->getDiffuse();
        _specular = lightNode->getSpecular();

        _ambientColorChooser->setColor(_ambient);
        _diffuseColorChooser->setColor(_diffuse);
        _specularColorChooser->setColor(_specular);
    }
}

void UserControlLight::updateNode()
{
    iNodeLight *lightNode = static_cast<iNodeLight *>(iNodeManager::getInstance().getNode(_lightNodeId));

    if (lightNode != nullptr)
    {
        lightNode->setAmbient(_ambient);
        lightNode->setDiffuse(_diffuse);
        lightNode->setSpecular(_specular);
    }
}

void UserControlLight::onAmbientChange(const iaColor4f &color)
{
    _ambient = color;
    updateNode();
}

void UserControlLight::onDiffuseChange(const iaColor4f &color)
{
    _diffuse = color;
    updateNode();
}

void UserControlLight::onSpecularChange(const iaColor4f &color)
{
    _specular = color;
    updateNode();
}

void UserControlLight::setNode(uint32 id)
{
    _lightNodeId = id;
    updateGUI();
}

uint32 UserControlLight::getNode()
{
    return _lightNodeId;
}

void UserControlLight::initGUI()
{
    _grid = new iWidgetGrid();
    _grid->appendRows(2);
    _grid->setBorder(2);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Right);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    addWidget(_grid);

    _ambientColorChooser = new iUserControlColorChooser();
    _ambientColorChooser->setMode(iColorChooserMode::RGB);
    _ambientColorChooser->setText("Ambient");
    _ambientColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlLight::onAmbientChange));

    _diffuseColorChooser = new iUserControlColorChooser();
    _diffuseColorChooser->setMode(iColorChooserMode::RGB);
    _diffuseColorChooser->setText("Diffuse");
    _diffuseColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlLight::onDiffuseChange));

    _specularColorChooser = new iUserControlColorChooser();
    _specularColorChooser->setMode(iColorChooserMode::RGB);
    _specularColorChooser->setText("Specular");
    _specularColorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &UserControlLight::onSpecularChange));

    _grid->addWidget(_ambientColorChooser, 0, 0);
    _grid->addWidget(_diffuseColorChooser, 0, 1);
    _grid->addWidget(_specularColorChooser, 0, 2);
}
