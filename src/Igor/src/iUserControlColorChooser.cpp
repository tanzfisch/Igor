// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iUserControlColorChooser.h>

#include <iWidgetManager.h>
#include <iWidgetGrid.h>
#include <iWidgetTextEdit.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetSpacer.h>
#include <iWidgetColorView.h>
#include <iWidgetSlider.h>
#include <iWidgetNumberChooser.h>

namespace Igor
{

    iUserControlColorChooser::iUserControlColorChooser(iColorChooserMode mode)
    {
        _mode = mode;
        if (_mode == iColorChooserMode::RGBA)
        {
            _components = 4;
        }
        else
        {
            _components = 3;
        }

        initGUI();
    }

    iUserControlColorChooser::~iUserControlColorChooser()
    {
        deinitGUI();
    }

    void iUserControlColorChooser::onTextEditChange(iWidget* source)
    {
    }

    void iUserControlColorChooser::onExpandButtonPressed(iWidget* source)
    {
        setExpanded(!_expanded);
    }

    void iUserControlColorChooser::initGUI()
    {
        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _grid->appendRows(1);
        _grid->setHorrizontalAlignment(iHorrizontalAlignment::Left);
        _grid->setVerticalAlignment(iVerticalAlignment::Top);

        _headlineGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _headlineGrid->appendCollumns(2);
        _headlineGrid->setHorrizontalAlignment(iHorrizontalAlignment::Left);
        _headlineGrid->setVerticalAlignment(iVerticalAlignment::Top);

        _label = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
        _label->setText(_text);
        _label->setHorrizontalAlignment(iHorrizontalAlignment::Left);
        _label->setVerticalAlignment(iVerticalAlignment::Top);

        _expandButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
        _expandButton->setTexture("icons\\plus.png");
        _expandButton->setText("");
        _expandButton->setWidth(12);
        _expandButton->setHeight(12);
        _expandButton->registerOnClickEvent(iClickDelegate(this, &iUserControlColorChooser::onExpandButtonPressed));

        _valueChooserH = createTextBox(iChangeDelegate(this, &iUserControlColorChooser::onTextChangedH));
        _valueChooserS = createTextBox(iChangeDelegate(this, &iUserControlColorChooser::onTextChangedS));
        _valueChooserV = createTextBox(iChangeDelegate(this, &iUserControlColorChooser::onTextChangedV));
        _valueChooserR = createTextBox(iChangeDelegate(this, &iUserControlColorChooser::onTextChangedR));
        _valueChooserG = createTextBox(iChangeDelegate(this, &iUserControlColorChooser::onTextChangedG));
        _valueChooserB = createTextBox(iChangeDelegate(this, &iUserControlColorChooser::onTextChangedB));
        _valueChooserA = createTextBox(iChangeDelegate(this, &iUserControlColorChooser::onTextChangedA));

        _labelH = createLabel("H");
        _labelS = createLabel("S");
        _labelV = createLabel("V");
        _labelR = createLabel("R");
        _labelG = createLabel("G");
        _labelB = createLabel("B");
        _labelA = createLabel("A");

        _headlineGrid->addWidget(_expandButton, 0, 0);
        _headlineGrid->addWidget(_label, 1, 0);

        _grid->addWidget(_headlineGrid, 0, 0);
        
        initExpanded();
        initCollapsed();

        setExpanded(_expanded);
    }

    void iUserControlColorChooser::setExpanded(bool expanded)
    {
        _expanded = expanded;

        if (_expanded)
        {
            _collapsedGrid->setActive(false);
            _expandedGrid->setActive(true);
            _grid->addWidget(_expandedGrid, 0, 1);
            _expandButton->setTexture("icons\\minus.png");
        }
        else
        {
            _expandedGrid->setActive(false);
            _collapsedGrid->setActive(true);
            _grid->addWidget(_collapsedGrid, 0, 1);
            _expandButton->setTexture("icons\\plus.png");
        }
    }

    void iUserControlColorChooser::onTextChangedH(iWidget* source)
    {
        _sliderH->setValue(_valueChooserH->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onTextChangedS(iWidget* source)
    {
        _sliderS->setValue(_valueChooserS->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onTextChangedV(iWidget* source)
    {
        _sliderV->setValue(_valueChooserV->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onTextChangedR(iWidget* source)
    {
        _sliderR->setValue(_valueChooserR->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onTextChangedG(iWidget* source)
    {
        _sliderG->setValue(_valueChooserG->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onTextChangedB(iWidget* source)
    {
        _sliderB->setValue(_valueChooserB->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onTextChangedA(iWidget* source)
    {
        _sliderA->setValue(_valueChooserA->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedH(iWidget* source)
    {
        _valueChooserH->setValue(_sliderH->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedS(iWidget* source)
    {
        _valueChooserS->setValue(_sliderS->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedV(iWidget* source)
    {
        _valueChooserV->setValue(_sliderV->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedR(iWidget* source)
    {
        _valueChooserR->setValue(_sliderR->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedG(iWidget* source)
    {
        _valueChooserG->setValue(_sliderG->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedB(iWidget* source)
    {
        _valueChooserB->setValue(_sliderB->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedA(iWidget* source)
    {
        _valueChooserA->setValue(_sliderA->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::updateWidgets()
    {
        _valueChooserH->setValue(_colorHSV._r * 255.0f);
        _valueChooserS->setValue(_colorHSV._g * 255.0f);
        _valueChooserV->setValue(_colorHSV._b * 255.0f);

        _valueChooserR->setValue(_colorRGBA._r * 255.0f);
        _valueChooserG->setValue(_colorRGBA._g * 255.0f);
        _valueChooserB->setValue(_colorRGBA._b * 255.0f);
        _valueChooserA->setValue(_colorRGBA._a * 255.0f);

        _sliderH->setValue(_colorHSV._r * 255.0f);
        _sliderS->setValue(_colorHSV._g * 255.0f);
        _sliderV->setValue(_colorHSV._b * 255.0f);

        _sliderR->setValue(_colorRGBA._r * 255.0f);
        _sliderG->setValue(_colorRGBA._g * 255.0f);
        _sliderB->setValue(_colorRGBA._b * 255.0f);
        _sliderA->setValue(_colorRGBA._a * 255.0f);

        updateColorViews();

        _colorChanged(_colorRGBA);
    }

    void iUserControlColorChooser::updateColorRGB()
    {
        _colorRGBA._r = _valueChooserR->getValue() / 255.0f;
        _colorRGBA._g = _valueChooserG->getValue() / 255.0f;
        _colorRGBA._b = _valueChooserB->getValue() / 255.0f;

        if (_components == 4)
        {
            _colorRGBA._a = _valueChooserA->getValue() / 255.0f;
        }
        else
        {
            _colorRGBA._a = 1.0f;
        }

        _colorHSV = iaColor4f::convertRGBtoHSV(_colorRGBA);
    }

    void iUserControlColorChooser::updateColorHSV()
    {
        _colorHSV._r = _valueChooserH->getValue() / 255.0f;
        _colorHSV._g = _valueChooserS->getValue() / 255.0f;
        _colorHSV._b = _valueChooserV->getValue() / 255.0f;
        _colorHSV._a = 1.0f;

        _colorRGBA = iaColor4f::convertHSVtoRGB(_colorHSV);
    }

    void iUserControlColorChooser::updateColorViews()
    {
        _colorViewCollapsed->setColor(_colorRGBA);
        _colorViewExpanded->setColor(_colorRGBA);
    }

    iWidgetLabel* iUserControlColorChooser::createLabel(const iaString& text)
    {
        iWidgetLabel* result = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
        result->setText(text);
        return result;
    }

    iWidgetNumberChooser* iUserControlColorChooser::createTextBox(iChangeDelegate changeDelegate)
    {
        iWidgetNumberChooser* result = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget(iWidgetType::NumberChooser));
        result->setMinMaxNumber(0, 255);
        result->setValue(123);
        result->setWidth(35);
        result->setSteppingWheel(5.0f, 5.0f);
        result->setStepping(1.0f, 1.0f);
        result->registerOnChangeEvent(changeDelegate);
        return result;
    }

    iWidgetSlider* iUserControlColorChooser::createSlider(iaString textureFileName, iChangeDelegate changeDelegate)
    {
        iWidgetSlider* result = static_cast<iWidgetSlider*>(iWidgetManager::getInstance().createWidget(iWidgetType::Slider));
        result->setWidth(260);
        result->setMinValue(0.0f);
        result->setMaxValue(255.0f);
        result->setSteppingWheel(5.0f, 5.0f);
        result->setValue(128.0f);
        result->setHeight(21);
        result->setTexture(textureFileName);
        result->registerOnChangeEvent(changeDelegate);
        return result;
    }

    void iUserControlColorChooser::initExpanded()
    {
        _expandedGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _expandedGrid->setHorrizontalAlignment(iHorrizontalAlignment::Left);
        _expandedGrid->setVerticalAlignment(iVerticalAlignment::Top);
        _expandedGrid->setBorder(0);
        _expandedGrid->setCellSpacing(4);
        _expandedGrid->appendRows(1);

        _expandedSliderGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _expandedSliderGrid->appendCollumns(2);
        _expandedSliderGrid->appendRows(_components + 3);
        _expandedSliderGrid->setBorder(0);
        _expandedSliderGrid->setCellSpacing(4);
        _expandedSliderGrid->setHorrizontalAlignment(iHorrizontalAlignment::Left);

        _sliderH = createSlider("special\\gradientH.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedH));
        _sliderS = createSlider("special\\gradientS.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedS));
        _sliderV = createSlider("special\\gradientV.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedV));
        _sliderR = createSlider("special\\gradientR.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedR));
        _sliderG = createSlider("special\\gradientG.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedG));
        _sliderB = createSlider("special\\gradientB.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedB));
        _sliderA = createSlider("special\\gradientA.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedA));
        _sliderA->setBackgroundTexture("special\\checker.png");

        _colorViewExpanded = static_cast<iWidgetColorView*>(iWidgetManager::getInstance().createWidget(iWidgetType::ColorView));
        _colorViewExpanded->setWidth(54);
        _colorViewExpanded->setHorrizontalAlignment(iHorrizontalAlignment::Right);
        _colorViewExpanded->setHeight(21);
        
        _expandedSliderGrid->addWidget(_labelH, 0, 0);
        _expandedSliderGrid->addWidget(_labelS, 0, 1);
        _expandedSliderGrid->addWidget(_labelV, 0, 2);
        _expandedSliderGrid->addWidget(_labelR, 0, 3);
        _expandedSliderGrid->addWidget(_labelG, 0, 4);
        _expandedSliderGrid->addWidget(_labelB, 0, 5);

        if (_components == 4)
        {
            _expandedSliderGrid->addWidget(_labelA, 0, 6);
        }

        _expandedSliderGrid->addWidget(_sliderH, 1, 0);
        _expandedSliderGrid->addWidget(_sliderS, 1, 1);
        _expandedSliderGrid->addWidget(_sliderV, 1, 2);
        _expandedSliderGrid->addWidget(_sliderR, 1, 3);
        _expandedSliderGrid->addWidget(_sliderG, 1, 4);
        _expandedSliderGrid->addWidget(_sliderB, 1, 5);

        if (_components == 4)
        {
            _expandedSliderGrid->addWidget(_sliderA, 1, 6);
        }

        _expandedSliderGrid->addWidget(_valueChooserH, 2, 0);
        _expandedSliderGrid->addWidget(_valueChooserS, 2, 1);
        _expandedSliderGrid->addWidget(_valueChooserV, 2, 2);
        _expandedSliderGrid->addWidget(_valueChooserR, 2, 3);
        _expandedSliderGrid->addWidget(_valueChooserG, 2, 4);
        _expandedSliderGrid->addWidget(_valueChooserB, 2, 5);

        if (_components == 4)
        {
            _expandedSliderGrid->addWidget(_valueChooserA, 2, 6);
        }

        _expandedGrid->addWidget(_expandedSliderGrid, 0, 0);
        _expandedGrid->addWidget(_colorViewExpanded, 0, 1);
    }

    void iUserControlColorChooser::initCollapsed()
    {
        _collapsedGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _collapsedGrid->setHorrizontalAlignment(iHorrizontalAlignment::Left);
        _collapsedGrid->setVerticalAlignment(iVerticalAlignment::Top);
        _collapsedGrid->appendCollumns(_components == 4 ? 8 : 7);
        _collapsedGrid->setCellSpacing(4);

        _collapsedSpacer = static_cast<iWidgetSpacer*>(iWidgetManager::getInstance().createWidget(iWidgetType::Spacer));
        _collapsedSpacer->setWidth(65);
        _collapsedSpacer->setHeight(0);

        _colorViewCollapsed = static_cast<iWidgetColorView*>(iWidgetManager::getInstance().createWidget(iWidgetType::ColorView));
        _colorViewCollapsed->setWidth(54);
        _colorViewCollapsed->setHeight(21);
        
        _collapsedGrid->addWidget(_labelR, 0, 0);
        _collapsedGrid->addWidget(_valueChooserR, 1, 0);
        _collapsedGrid->addWidget(_labelG, 2, 0);
        _collapsedGrid->addWidget(_valueChooserG, 3, 0);
        _collapsedGrid->addWidget(_labelB, 4, 0);
        _collapsedGrid->addWidget(_valueChooserB, 5, 0);

        if (_components == 4)
        {
            _collapsedGrid->addWidget(_labelA, 6, 0);
            _collapsedGrid->addWidget(_valueChooserA, 7, 0);
            _collapsedGrid->addWidget(_colorViewCollapsed, 8, 0);
        }
        else
        {
            _collapsedGrid->addWidget(_collapsedSpacer, 6, 0);
            _collapsedGrid->addWidget(_colorViewCollapsed, 7, 0);
        }
    }

    void iUserControlColorChooser::deinitGUI()
    {
        if (_colorViewExpanded != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_colorViewExpanded);
        }

        if (_colorViewCollapsed != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_colorViewCollapsed);
        }

        if (_grid != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_grid);
        }

        if (_headlineGrid != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_headlineGrid);
        }

        if (_label != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_label);
        }

        if (_expandButton != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_expandButton);
        }

        if (_valueChooserH != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_valueChooserH);
        }

        if (_valueChooserS != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_valueChooserS);
        }

        if (_valueChooserV != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_valueChooserV);
        }

        if (_valueChooserR != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_valueChooserR);
        }

        if (_valueChooserG != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_valueChooserG);
        }

        if (_valueChooserB != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_valueChooserB);
        }

        if (_valueChooserA != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_valueChooserA);
        }

        if (_labelH != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_labelH);
        }

        if (_labelS != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_labelS);
        }

        if (_labelV != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_labelV);
        }

        if (_labelR != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_labelR);
        }

        if (_labelG != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_labelG);
        }

        if (_labelB != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_labelB);
        }

        if (_labelA != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_labelA);
        }

        if (_sliderH != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_sliderH);
        }

        if (_sliderS != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_sliderS);
        }

        if (_sliderV != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_sliderV);
        }

        if (_sliderR != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_sliderR);
        }

        if (_sliderG != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_sliderG);
        }

        if (_sliderB != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_sliderB);
        }

        if (_sliderA != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_sliderA);
        }
    }

    void iUserControlColorChooser::registerOnColorChangedEvent(iColorChangedDelegate colorChangedDelegate)
    {
        _colorChanged.append(colorChangedDelegate);
    }

    void iUserControlColorChooser::unregisterOnColorChangedEvent(iColorChangedDelegate colorChangedDelegate)
    {
        _colorChanged.remove(colorChangedDelegate);
    }

    void iUserControlColorChooser::setText(const iaString& text)
    {
        _text = text;

        if (_label != nullptr)
        {
            _label->setText(_text);
        }
    }

    const iaString& iUserControlColorChooser::getText() const
    {
        return _text;
    }

    iWidget* iUserControlColorChooser::getWidget()
    {
        return _grid;
    }

    const iaColor4f& iUserControlColorChooser::getColor() const
    {
        return _colorRGBA;
    }
    
    void iUserControlColorChooser::setColor(const iaColor4f& color)
    {
        _colorRGBA = color;
        _colorHSV = iaColor4f::convertRGBtoHSV(_colorRGBA);
        updateWidgets();
    }

    

}