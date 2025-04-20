// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlMaterial.h"

#include "../../MicaDefines.h"

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

    const auto ambient = _ambientColorChooser->getColor();
    const auto diffuse = _diffuseColorChooser->getColor();
    const auto specular = _specularColorChooser->getColor();
    const auto amissive = _emissiveColorChooser->getColor();

    material->setAmbient(iaColor3f(ambient._r, ambient._g, ambient._b));
    material->setDiffuse(iaColor3f(diffuse._r, diffuse._g, diffuse._b));
    material->setSpecular(iaColor3f(specular._r, specular._g, specular._b));
    material->setEmissive(iaColor3f(amissive._r, amissive._g, amissive._b));
    material->setShininess(_numberChooserShininess->getValue());

    for (int i = 0; i < 4; ++i)
    {
        auto id = _textureChooser[i]->getID();
        if (id.isValid())
        {
            material->setTexture(iResourceManager::getInstance().loadResource<iTexture>(id), i);
        }
        else
        {
            material->setTexture(nullptr, i);
        }
    }

    if(_shaderMaterialChooser->getID().isValid())
    {
        material->setShader(iResourceManager::getInstance().loadResource<iShader>(_shaderMaterialChooser->getID()));
    }
    else
    {
        material->setShader(nullptr);
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

    _sliderShininess->setValue(material->getShininess());
    _numberChooserShininess->setValue(material->getShininess());

    for (int i = 0; i < 4; ++i)
    {
        if (material->hasTextureUnit(i))
        {
            _textureChooser[i]->setID(material->getTexture(i)->getID());
        }
        else
        {
            _textureChooser[i]->setID(iResourceID::getInvalid());
        }
    }

    if (material->getShader() != nullptr)
    {
        _shaderMaterialChooser->setID(material->getShader()->getID());
    }
    else
    {
        _shaderMaterialChooser->setID(iResourceID::getInvalid());
    }

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
    lightGroupBox->setText("Material");
    lightGroupBox->setHeaderOnly();

    iWidgetBoxLayoutPtr materialLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, lightGroupBox);

    _ambientColorChooser = new iUserControlColor(materialLayout);
    _ambientColorChooser->setMode(iColorChooserMode::RGB);
    _ambientColorChooser->setText("Ambient");
    _ambientColorChooser->getChangeEvent().add(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    _diffuseColorChooser = new iUserControlColor(materialLayout);
    _diffuseColorChooser->setMode(iColorChooserMode::RGB);
    _diffuseColorChooser->setText("Diffuse");
    _diffuseColorChooser->getChangeEvent().add(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    _specularColorChooser = new iUserControlColor(materialLayout);
    _specularColorChooser->setMode(iColorChooserMode::RGB);
    _specularColorChooser->setText("Specular");
    _specularColorChooser->getChangeEvent().add(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    _emissiveColorChooser = new iUserControlColor(materialLayout);
    _emissiveColorChooser->setMode(iColorChooserMode::RGB);
    _emissiveColorChooser->setText("Emissive");
    _emissiveColorChooser->getChangeEvent().add(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));

    iWidgetLabel *labelShininess = new iWidgetLabel(materialLayout);
    labelShininess->setText("Shininess");
    labelShininess->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetBoxLayoutPtr shininessLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, materialLayout);

    _sliderShininess = new iWidgetSlider(shininessLayout);
    _sliderShininess->setHorizontalAlignment(iHorizontalAlignment::Left);
    _sliderShininess->setMinValue(0.0f);
    _sliderShininess->setMaxValue(1000.0f);
    _sliderShininess->setSteppingWheel(0.1f, 0.1f);
    _sliderShininess->setValue(0.0f);
    _sliderShininess->setMinWidth(220);
    _sliderShininess->getChangeEvent().add(iChangeDelegate(this, &UserControlMaterial::onDoUpdateShininess));

    _numberChooserShininess = new iWidgetNumberChooser(shininessLayout);
    _numberChooserShininess->setMinMaxNumber(0.0f, 1000.0f);
    _numberChooserShininess->setAfterPoint(2);
    _numberChooserShininess->setValue(0.0f);
    _numberChooserShininess->setMinWidth(80);
    _numberChooserShininess->setSteppingWheel(0.1f, 0.1f);
    _numberChooserShininess->setStepping(0.01f, 0.01f);
    _numberChooserShininess->getChangeEvent().add(iChangeDelegate(this, &UserControlMaterial::onDoUpdateShininess));

    iWidgetLabelPtr labelTextureUnit[4] = {nullptr, nullptr, nullptr, nullptr};
    for (int i = 0; i < 4; ++i)
    {
        iWidgetBoxLayoutPtr textureLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, materialLayout);
        labelTextureUnit[i] = new iWidgetLabel(textureLayout);
        labelTextureUnit[i]->setText(iaString("Texture ") + iaString::toString(i));
        labelTextureUnit[i]->setMinWidth(MICA_REGULARBUTTON_SIZE);
        labelTextureUnit[i]->setVerticalAlignment(iVerticalAlignment::Top);
        labelTextureUnit[i]->setHorizontalAlignment(iHorizontalAlignment::Left);

        _textureChooser[i] = new iUserControlTexture(textureLayout);
        _textureChooser[i]->getChangeEvent().add(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
    }

    iWidgetBoxLayoutPtr shaderMaterialLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, materialLayout);
    iWidgetLabelPtr shaderMaterialLabel = new iWidgetLabel(shaderMaterialLayout);
    shaderMaterialLabel->setVerticalAlignment(iVerticalAlignment::Top);
    shaderMaterialLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
    shaderMaterialLabel->setText("Shader");
    shaderMaterialLabel->setMinWidth(MICA_REGULARBUTTON_SIZE);
    _shaderMaterialChooser = new iUserControlShaderMaterial(shaderMaterialLayout);

    _shaderMaterialChooser->getChangeEvent().add(iChangeDelegate(this, &UserControlMaterial::onDoUpdateMaterial));
}

void UserControlMaterial::onDoUpdateShininess(const iWidgetPtr source)
{
    if (source == _sliderShininess)
    {
        _numberChooserShininess->setValue(_sliderShininess->getValue());
    }
    else
    {
        _sliderShininess->setValue(_numberChooserShininess->getValue());
    }

    updateResource();
}

void UserControlMaterial::onDoUpdateMaterial(const iWidgetPtr source)
{
    updateResource();
}
