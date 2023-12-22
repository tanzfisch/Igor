// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlLight.h"

UserControlLight::UserControlLight(iNodeID nodeID, const iWidgetPtr parent)
    : UserControlNode(nodeID, parent)
{
}

void UserControlLight::update()
{
    UserControlNode::update();

    iNodeLight *lightNode = static_cast<iNodeLight *>(iNodeManager::getInstance().getNode(getNodeID()));

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
    iNodeLight *lightNode = static_cast<iNodeLight *>(iNodeManager::getInstance().getNode(getNodeID()));

    if (lightNode != nullptr)
    {
        lightNode->setAmbient(_ambient);
        lightNode->setDiffuse(_diffuse);
        lightNode->setSpecular(_specular);
    }
}

void UserControlLight::onAmbientChange(const iaColor4f &color)
{
    _ambient.set(color._r, color._g, color._b);
    updateNode();
}

void UserControlLight::onDiffuseChange(const iaColor4f &color)
{
    _diffuse.set(color._r, color._g, color._b);
    updateNode();
}

void UserControlLight::onSpecularChange(const iaColor4f &color)
{
    _specular.set(color._r, color._g, color._b);
    updateNode();
}

void UserControlLight::init()
{
    UserControlNode::init();

    _grid = new iWidgetGridLayout(getLayout());
    _grid->appendRows(2);
    _grid->setBorder(2);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Right);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);

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
