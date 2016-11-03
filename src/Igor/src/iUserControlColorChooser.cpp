// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
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
        if (mode == iColorChooserMode::RGBA)
        {
            _components = 4;
        }
        else if (mode == iColorChooserMode::RGB)
        {
            _components = 3;
        }
        else
        {
            con_err("invalid mode");
        }

        initGUI();
    }

    iUserControlColorChooser::~iUserControlColorChooser()
    {
        deinitGUI();
    }

    void iUserControlColorChooser::onExpandButtonPressed(iWidget* source)
    {
        setExpanded(!_expanded);
    }

    void iUserControlColorChooser::initGUI()
    {
        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _allWidgets.push_back(_grid);
        _grid->appendRows(1);
        _grid->setHorizontalAlignment(iHorizontalAlignment::Left);
        _grid->setVerticalAlignment(iVerticalAlignment::Top);

        _headlineGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _allWidgets.push_back(_headlineGrid);
        _headlineGrid->appendCollumns(2);
        _headlineGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
        _headlineGrid->setVerticalAlignment(iVerticalAlignment::Top);

        _titleLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
        _allWidgets.push_back(_titleLabel);
        _titleLabel->setText(_text);
        _titleLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
        _titleLabel->setVerticalAlignment(iVerticalAlignment::Top);

        _expandButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
        _allWidgets.push_back(_expandButton);
        _expandButton->setTexture("icons\\plus.png");
        _expandButton->setText("");
        _expandButton->setWidth(12);
        _expandButton->setHeight(12);
        _expandButton->registerOnClickEvent(iClickDelegate(this, &iUserControlColorChooser::onExpandButtonPressed));

        _valueChooserH = createNumberChooser(iChangeDelegate(this, &iUserControlColorChooser::onValueChangedH));
        _valueChooserS = createNumberChooser(iChangeDelegate(this, &iUserControlColorChooser::onValueChangedS));
        _valueChooserV = createNumberChooser(iChangeDelegate(this, &iUserControlColorChooser::onValueChangedV));
        _valueChooserR = createNumberChooser(iChangeDelegate(this, &iUserControlColorChooser::onValueChangedR));
        _valueChooserG = createNumberChooser(iChangeDelegate(this, &iUserControlColorChooser::onValueChangedG));
        _valueChooserB = createNumberChooser(iChangeDelegate(this, &iUserControlColorChooser::onValueChangedB));
        _valueChooserA = createNumberChooser(iChangeDelegate(this, &iUserControlColorChooser::onValueChangedA));
        _valueChooserRExpanded = createNumberChooser(iChangeDelegate(this, &iUserControlColorChooser::onValueChangedRExpanded));
        _valueChooserGExpanded = createNumberChooser(iChangeDelegate(this, &iUserControlColorChooser::onValueChangedGExpanded));
        _valueChooserBExpanded = createNumberChooser(iChangeDelegate(this, &iUserControlColorChooser::onValueChangedBExpanded));
        _valueChooserAExpanded = createNumberChooser(iChangeDelegate(this, &iUserControlColorChooser::onValueChangedAExpanded));

        _labelH = createLabel("H");
        _labelS = createLabel("S");
        _labelV = createLabel("V");
        _labelR = createLabel("R");
        _labelG = createLabel("G");
        _labelB = createLabel("B");
        _labelA = createLabel("A");

        _headlineGrid->addWidget(_expandButton, 0, 0);
        _headlineGrid->addWidget(_titleLabel, 1, 0);

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
            _expandedGrid->setVisible(true);
            _grid->addWidget(_expandedGrid, 0, 1);
            _expandButton->setTexture("icons\\minus.png");
        }
        else
        {
            _expandedGrid->setActive(false);
            _collapsedGrid->setActive(true);
            _collapsedGrid->setVisible(true);
            _grid->addWidget(_collapsedGrid, 0, 1);
            _expandButton->setTexture("icons\\plus.png");
        }
    }

    void iUserControlColorChooser::onValueChangedH(iWidget* source)
    {
        _sliderH->setValue(_valueChooserH->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedS(iWidget* source)
    {
        _sliderS->setValue(_valueChooserS->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedV(iWidget* source)
    {
        _sliderV->setValue(_valueChooserV->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedR(iWidget* source)
    {
        _sliderR->setValue(_valueChooserR->getValue());
        _valueChooserRExpanded->setValue(_valueChooserR->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedG(iWidget* source)
    {
        _sliderG->setValue(_valueChooserG->getValue());
        _valueChooserGExpanded->setValue(_valueChooserG->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedB(iWidget* source)
    {
        _sliderB->setValue(_valueChooserB->getValue());
        _valueChooserBExpanded->setValue(_valueChooserB->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedA(iWidget* source)
    {
        _sliderA->setValue(_valueChooserA->getValue());
        _valueChooserAExpanded->setValue(_valueChooserA->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedRExpanded(iWidget* source)
    {
        _sliderR->setValue(_valueChooserRExpanded->getValue());
        _valueChooserR->setValue(_valueChooserRExpanded->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedGExpanded(iWidget* source)
    {
        _sliderG->setValue(_valueChooserGExpanded->getValue());
        _valueChooserG->setValue(_valueChooserGExpanded->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedBExpanded(iWidget* source)
    {
        _sliderB->setValue(_valueChooserBExpanded->getValue());
        _valueChooserB->setValue(_valueChooserBExpanded->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedAExpanded(iWidget* source)
    {
        _sliderA->setValue(_valueChooserAExpanded->getValue());
        _valueChooserA->setValue(_valueChooserAExpanded->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedH(iWidget* source)
    {
        _valueChooserH->setValue(_sliderH->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedS(iWidget* source)
    {
        _valueChooserS->setValue(_sliderS->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedV(iWidget* source)
    {
        _valueChooserV->setValue(_sliderV->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedR(iWidget* source)
    {
        _valueChooserR->setValue(_sliderR->getValue());
        _valueChooserRExpanded->setValue(_sliderR->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedG(iWidget* source)
    {
        _valueChooserG->setValue(_sliderG->getValue());
        _valueChooserGExpanded->setValue(_sliderR->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedB(iWidget* source)
    {
        _valueChooserB->setValue(_sliderB->getValue());
        _valueChooserBExpanded->setValue(_sliderR->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedA(iWidget* source)
    {
        _valueChooserA->setValue(_sliderA->getValue());
        _valueChooserAExpanded->setValue(_sliderR->getValue());
        updateColorHSV();
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

        _valueChooserRExpanded->setValue(_colorRGBA._r * 255.0f);
        _valueChooserGExpanded->setValue(_colorRGBA._g * 255.0f);
        _valueChooserBExpanded->setValue(_colorRGBA._b * 255.0f);
        _valueChooserAExpanded->setValue(_colorRGBA._a * 255.0f);

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

    void iUserControlColorChooser::updateColorHSV()
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

    void iUserControlColorChooser::updateColorRGB()
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
        _allWidgets.push_back(result);
        result->setText(text);
        return result;
    }

    iWidgetNumberChooser* iUserControlColorChooser::createNumberChooser(iChangeDelegate changeDelegate)
    {
        iWidgetNumberChooser* result = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget(iWidgetType::NumberChooser));
        _allWidgets.push_back(result);
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
        _allWidgets.push_back(result);
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
        _allWidgets.push_back(_expandedGrid);
        _expandedGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
        _expandedGrid->setVerticalAlignment(iVerticalAlignment::Top);
        _expandedGrid->setBorder(0);
        _expandedGrid->setCellSpacing(4);
        _expandedGrid->appendRows(1);

        _expandedSliderGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid)); 
        _allWidgets.push_back(_expandedSliderGrid);
        _expandedSliderGrid->appendCollumns(2);
        _expandedSliderGrid->appendRows(_components + 3);
        _expandedSliderGrid->setBorder(0);
        _expandedSliderGrid->setCellSpacing(4);
        _expandedSliderGrid->setHorizontalAlignment(iHorizontalAlignment::Left);

        _sliderH = createSlider("special\\gradientH.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedH));
        _sliderS = createSlider("special\\gradientS.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedS));
        _sliderV = createSlider("special\\gradientV.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedV));
        _sliderR = createSlider("special\\gradientR.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedR));
        _sliderG = createSlider("special\\gradientG.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedG));
        _sliderB = createSlider("special\\gradientB.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedB));
        _sliderA = createSlider("special\\gradientA.png", iChangeDelegate(this, &iUserControlColorChooser::onSliderChangedA));
        _sliderA->setBackgroundTexture("special\\checker.png");

        _colorViewExpanded = static_cast<iWidgetColorView*>(iWidgetManager::getInstance().createWidget(iWidgetType::ColorView));
        _allWidgets.push_back(_colorViewExpanded);
        _colorViewExpanded->setWidth(54);
        _colorViewExpanded->setHorizontalAlignment(iHorizontalAlignment::Right);
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
        _expandedSliderGrid->addWidget(_valueChooserRExpanded, 2, 3);
        _expandedSliderGrid->addWidget(_valueChooserGExpanded, 2, 4);
        _expandedSliderGrid->addWidget(_valueChooserBExpanded, 2, 5);

        if (_components == 4)
        {
            _expandedSliderGrid->addWidget(_valueChooserAExpanded, 2, 6);
        }

        _expandedGrid->addWidget(_expandedSliderGrid, 0, 0);
        _expandedGrid->addWidget(_colorViewExpanded, 0, 1);
    }

    void iUserControlColorChooser::initCollapsed()
    {
        _collapsedGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _allWidgets.push_back(_collapsedGrid);
        _collapsedGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
        _collapsedGrid->setVerticalAlignment(iVerticalAlignment::Top);
        _collapsedGrid->appendCollumns(_components == 4 ? 8 : 7);
        _collapsedGrid->setCellSpacing(4);

        _collapsedSpacer = static_cast<iWidgetSpacer*>(iWidgetManager::getInstance().createWidget(iWidgetType::Spacer));
        _allWidgets.push_back(_collapsedSpacer);
        _collapsedSpacer->setWidth(65);
        _collapsedSpacer->setHeight(0);

        _colorViewCollapsed = static_cast<iWidgetColorView*>(iWidgetManager::getInstance().createWidget(iWidgetType::ColorView));
        _allWidgets.push_back(_colorViewCollapsed);
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
        for (auto widget : _allWidgets)
        {
            iWidgetManager::getInstance().destroyWidget(widget);
        }

        _allWidgets.clear();

        _grid = nullptr;
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

        if (_titleLabel != nullptr)
        {
            _titleLabel->setText(_text);
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