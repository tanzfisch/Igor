// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlColorChooser.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/widgets/iWidgetTextEdit.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetColor.h>
#include <igor/ui/widgets/iWidgetSlider.h>
#include <igor/ui/widgets/iWidgetNumberChooser.h>

namespace igor
{

    iUserControlColorChooser::iUserControlColorChooser(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlColorChooser, parent)
    {
        initGUI();
    }

    iUserControlColorChooser::~iUserControlColorChooser()
    {
        deinitGUI();
    }

    void iUserControlColorChooser::block(bool blockEvents)
    {
        iWidget::block(blockEvents);

        // update own events
        _colorChanged.block(isBlocked());
    }

    void iUserControlColorChooser::setMode(iColorChooserMode mode)
    {
        if (_mode != mode)
        {
            _mode = mode;
            updateMode();
        }
    }

    iColorChooserMode iUserControlColorChooser::getMode()
    {
        return _mode;
    }

    void iUserControlColorChooser::updateMode()
    {
        if (_mode == iColorChooserMode::RGBA)
        {
            _valueChooserA->setActive(true);
            _valueChooserA->setVisible(true);
            _labelA->setActive(true);
            _labelA->setVisible(true);

            _valueChooserAExpanded->setActive(true);
            _valueChooserAExpanded->setVisible(true);
            _labelAExpanded->setActive(true);
            _labelAExpanded->setVisible(true);
            _sliderA->setActive(true);
            _sliderA->setVisible(true);

            _expandedSliderGrid->addWidget(_labelAExpanded, 0, 6);
            _expandedSliderGrid->addWidget(_sliderA, 1, 6);
            _expandedSliderGrid->addWidget(_valueChooserAExpanded, 2, 6);
        }
        else
        {
            _valueChooserA->setActive(false);
            _valueChooserA->setVisible(false);
            _labelA->setActive(false);
            _labelA->setVisible(false);

            _valueChooserAExpanded->setActive(false);
            _valueChooserAExpanded->setVisible(false);
            _labelAExpanded->setActive(false);
            _labelAExpanded->setVisible(false);
            _sliderA->setActive(false);
            _sliderA->setVisible(false);

            _expandedSliderGrid->removeWidget(_valueChooserAExpanded);
            _expandedSliderGrid->removeWidget(_labelAExpanded);
            _expandedSliderGrid->removeWidget(_sliderA);
        }
    }

    void iUserControlColorChooser::setExpand(bool expand)
    {
        setExpanded(expand);
    }

    bool iUserControlColorChooser::getExpand() const
    {
        return _expanded;
    }

    void iUserControlColorChooser::setHeadlineVisible(bool headlineVisible)
    {
        _showHeadline = headlineVisible;

        if (_expandButton != nullptr)
        {
            _expandButton->setVisible(_showHeadline);
            _expandButton->setActive(_showHeadline);

            if (_showHeadline)
            {
                if (!_headlineGrid->hasParent())
                {
                    _grid->addWidget(_headlineGrid, 0, 0);
                }
            }
            else
            {
                if (_headlineGrid->hasParent())
                {
                    _grid->removeWidget(_headlineGrid);
                }
            }
        }
    }

    bool iUserControlColorChooser::isHeadlineVisible() const
    {
        return _showHeadline;
    }

    void iUserControlColorChooser::initGUI()
    {
        _grid = new iWidgetGrid(this);
        _grid->appendRows(1);
        _grid->setHorizontalAlignment(iHorizontalAlignment::Left);
        _grid->setVerticalAlignment(iVerticalAlignment::Top);

        _headlineGrid = new iWidgetGrid();
        _headlineGrid->appendCollumns(2);
        _headlineGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
        _headlineGrid->setVerticalAlignment(iVerticalAlignment::Top);

        _titleLabel = new iWidgetLabel();
        _titleLabel->setText(_text);
        _titleLabel->setHeight(0);
        _titleLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
        _titleLabel->setVerticalAlignment(iVerticalAlignment::Top);

        _expandButton = new iWidgetButton();
        _expandButton->setTexture("icons/plus.png");
        _expandButton->setText("");
        _expandButton->setWidth(12);
        _expandButton->setHeight(12);
        _expandButton->registerOnClickEvent(iClickDelegate(this, &iUserControlColorChooser::onExpandButtonPressed));
        _expandButton->setVisible(_showHeadline);
        _expandButton->setActive(_showHeadline);

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
        _labelRExpanded = createLabel("R");
        _labelGExpanded = createLabel("G");
        _labelBExpanded = createLabel("B");
        _labelAExpanded = createLabel("A");

        _headlineGrid->addWidget(_expandButton, 0, 0);
        _headlineGrid->addWidget(_titleLabel, 1, 0);

        if (_showHeadline)
        {
            _grid->addWidget(_headlineGrid, 0, 0);
        }

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
            _expandButton->setTexture("icons/minus.png");
        }
        else
        {
            _expandedGrid->setActive(false);
            _collapsedGrid->setActive(true);
            _collapsedGrid->setVisible(true);
            _grid->addWidget(_collapsedGrid, 0, 1);
            _expandButton->setTexture("icons/plus.png");
        }
    }

    __IGOR_DISABLE_WARNING__(4100)
    void iUserControlColorChooser::onExpandButtonPressed(const iWidgetPtr source)
    {
        setExpanded(!_expanded);
    }

    void iUserControlColorChooser::onValueChangedH(const iWidgetPtr source)
    {
        _sliderH->setValue(_valueChooserH->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedS(const iWidgetPtr source)
    {
        _sliderS->setValue(_valueChooserS->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedV(const iWidgetPtr source)
    {
        _sliderV->setValue(_valueChooserV->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedR(const iWidgetPtr source)
    {
        _sliderR->setValue(_valueChooserR->getValue());
        _valueChooserRExpanded->setValue(_valueChooserR->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedG(const iWidgetPtr source)
    {
        _sliderG->setValue(_valueChooserG->getValue());
        _valueChooserGExpanded->setValue(_valueChooserG->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedB(const iWidgetPtr source)
    {
        _sliderB->setValue(_valueChooserB->getValue());
        _valueChooserBExpanded->setValue(_valueChooserB->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedA(const iWidgetPtr source)
    {
        _sliderA->setValue(_valueChooserA->getValue());
        _valueChooserAExpanded->setValue(_valueChooserA->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedRExpanded(const iWidgetPtr source)
    {
        _sliderR->setValue(_valueChooserRExpanded->getValue());
        _valueChooserR->setValue(_valueChooserRExpanded->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedGExpanded(const iWidgetPtr source)
    {
        _sliderG->setValue(_valueChooserGExpanded->getValue());
        _valueChooserG->setValue(_valueChooserGExpanded->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedBExpanded(const iWidgetPtr source)
    {
        _sliderB->setValue(_valueChooserBExpanded->getValue());
        _valueChooserB->setValue(_valueChooserBExpanded->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onValueChangedAExpanded(const iWidgetPtr source)
    {
        _sliderA->setValue(_valueChooserAExpanded->getValue());
        _valueChooserA->setValue(_valueChooserAExpanded->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedH(const iWidgetPtr source)
    {
        _valueChooserH->setValue(_sliderH->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedS(const iWidgetPtr source)
    {
        _valueChooserS->setValue(_sliderS->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedV(const iWidgetPtr source)
    {
        _valueChooserV->setValue(_sliderV->getValue());
        updateColorRGB();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedR(const iWidgetPtr source)
    {
        _valueChooserR->setValue(_sliderR->getValue());
        _valueChooserRExpanded->setValue(_sliderR->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedG(const iWidgetPtr source)
    {
        _valueChooserG->setValue(_sliderG->getValue());
        _valueChooserGExpanded->setValue(_sliderR->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedB(const iWidgetPtr source)
    {
        _valueChooserB->setValue(_sliderB->getValue());
        _valueChooserBExpanded->setValue(_sliderR->getValue());
        updateColorHSV();
        updateWidgets();
    }

    void iUserControlColorChooser::onSliderChangedA(const iWidgetPtr source)
    {
        _valueChooserA->setValue(_sliderA->getValue());
        _valueChooserAExpanded->setValue(_sliderR->getValue());
        updateColorHSV();
        updateWidgets();
    }
    __IGOR_ENABLE_WARNING__(4100);

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

        if (_mode == iColorChooserMode::RGBA)
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

        if (_mode == iColorChooserMode::RGBA)
        {
            _colorHSV._a = _valueChooserA->getValue() / 255.0f;
        }
        else
        {
            _colorRGBA._a = 1.0f;
        }

        _colorRGBA = iaColor4f::convertHSVtoRGB(_colorHSV);
    }

    void iUserControlColorChooser::updateColorViews()
    {
        _colorViewCollapsed->setColor(_colorRGBA);
        _colorViewExpanded->setColor(_colorRGBA);
    }

    iWidgetLabel *iUserControlColorChooser::createLabel(const iaString &text)
    {
        iWidgetLabel *result = new iWidgetLabel();
        result->setText(text);
        return result;
    }

    iWidgetNumberChooser *iUserControlColorChooser::createNumberChooser(iChangeDelegate changeDelegate)
    {
        iWidgetNumberChooser *result = new iWidgetNumberChooser();
        result->setMinMaxNumber(0, 255);
        result->setValue(123);
        result->setWidth(35);
        result->setSteppingWheel(5.0f, 5.0f);
        result->setStepping(1.0f, 1.0f);
        result->registerOnChangeEvent(changeDelegate);
        return result;
    }

    iWidgetSlider *iUserControlColorChooser::createSlider(iaString textureFileName, iChangeDelegate changeDelegate)
    {
        iWidgetSlider *result = new iWidgetSlider();
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
        _expandedGrid = new iWidgetGrid();
        _expandedGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
        _expandedGrid->setVerticalAlignment(iVerticalAlignment::Top);
        _expandedGrid->setBorder(0);
        _expandedGrid->setCellSpacing(4);
        _expandedGrid->appendRows(1);

        _expandedSliderGrid = new iWidgetGrid();
        _expandedSliderGrid->appendCollumns(2);
        _expandedSliderGrid->appendRows(7);
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

        _colorViewExpanded = new iWidgetColor();
        _colorViewExpanded->setWidth(54);
        _colorViewExpanded->setHorizontalAlignment(iHorizontalAlignment::Right);
        _colorViewExpanded->setHeight(21);

        _expandedSliderGrid->addWidget(_labelH, 0, 0);
        _expandedSliderGrid->addWidget(_labelS, 0, 1);
        _expandedSliderGrid->addWidget(_labelV, 0, 2);
        _expandedSliderGrid->addWidget(_labelRExpanded, 0, 3);
        _expandedSliderGrid->addWidget(_labelGExpanded, 0, 4);
        _expandedSliderGrid->addWidget(_labelBExpanded, 0, 5);
        _expandedSliderGrid->addWidget(_labelAExpanded, 0, 6);

        _expandedSliderGrid->addWidget(_sliderH, 1, 0);
        _expandedSliderGrid->addWidget(_sliderS, 1, 1);
        _expandedSliderGrid->addWidget(_sliderV, 1, 2);
        _expandedSliderGrid->addWidget(_sliderR, 1, 3);
        _expandedSliderGrid->addWidget(_sliderG, 1, 4);
        _expandedSliderGrid->addWidget(_sliderB, 1, 5);
        _expandedSliderGrid->addWidget(_sliderA, 1, 6);

        _expandedSliderGrid->addWidget(_valueChooserH, 2, 0);
        _expandedSliderGrid->addWidget(_valueChooserS, 2, 1);
        _expandedSliderGrid->addWidget(_valueChooserV, 2, 2);
        _expandedSliderGrid->addWidget(_valueChooserRExpanded, 2, 3);
        _expandedSliderGrid->addWidget(_valueChooserGExpanded, 2, 4);
        _expandedSliderGrid->addWidget(_valueChooserBExpanded, 2, 5);
        _expandedSliderGrid->addWidget(_valueChooserAExpanded, 2, 6);

        _expandedGrid->addWidget(_expandedSliderGrid, 0, 0);
        _expandedGrid->addWidget(_colorViewExpanded, 0, 1);
    }

    void iUserControlColorChooser::initCollapsed()
    {
        _collapsedGrid = new iWidgetGrid();
        _collapsedGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
        _collapsedGrid->setVerticalAlignment(iVerticalAlignment::Top);
        _collapsedGrid->appendCollumns(8);
        _collapsedGrid->setCellSpacing(4);

        _colorViewCollapsed = new iWidgetColor();
        _colorViewCollapsed->setWidth(54);
        _colorViewCollapsed->setHeight(21);

        _collapsedGrid->addWidget(_labelR, 0, 0);
        _collapsedGrid->addWidget(_valueChooserR, 1, 0);
        _collapsedGrid->addWidget(_labelG, 2, 0);
        _collapsedGrid->addWidget(_valueChooserG, 3, 0);
        _collapsedGrid->addWidget(_labelB, 4, 0);
        _collapsedGrid->addWidget(_valueChooserB, 5, 0);

        _collapsedGrid->addWidget(_labelA, 6, 0);
        _collapsedGrid->addWidget(_valueChooserA, 7, 0);
        _collapsedGrid->addWidget(_colorViewCollapsed, 8, 0);
    }

    void iUserControlColorChooser::deinitGUI()
    {
        // not elegant but robust
#define KILL_ROGUE(widget)              \
    if (widget && !widget->hasParent()) \
    {                                   \
        delete widget;                  \
        widget = nullptr;               \
    }

        KILL_ROGUE(_headlineGrid);

        KILL_ROGUE(_valueChooserH);
        KILL_ROGUE(_valueChooserS);
        KILL_ROGUE(_valueChooserV);
        KILL_ROGUE(_valueChooserR);
        KILL_ROGUE(_valueChooserG);
        KILL_ROGUE(_valueChooserB);
        KILL_ROGUE(_valueChooserA);
        KILL_ROGUE(_valueChooserRExpanded);
        KILL_ROGUE(_valueChooserGExpanded);
        KILL_ROGUE(_valueChooserBExpanded);
        KILL_ROGUE(_valueChooserAExpanded);

        KILL_ROGUE(_labelH);
        KILL_ROGUE(_labelS);
        KILL_ROGUE(_labelV);
        KILL_ROGUE(_labelR);
        KILL_ROGUE(_labelG);
        KILL_ROGUE(_labelB);
        KILL_ROGUE(_labelA);
        KILL_ROGUE(_labelRExpanded);
        KILL_ROGUE(_labelGExpanded);
        KILL_ROGUE(_labelBExpanded);
        KILL_ROGUE(_labelAExpanded);

        KILL_ROGUE(_expandedSliderGrid);
        KILL_ROGUE(_collapsedGrid);

        KILL_ROGUE(_sliderH);
        KILL_ROGUE(_sliderS);
        KILL_ROGUE(_sliderV);
        KILL_ROGUE(_sliderR);
        KILL_ROGUE(_sliderG);
        KILL_ROGUE(_sliderB);
        KILL_ROGUE(_sliderA);

        KILL_ROGUE(_expandedGrid);
    }

    void iUserControlColorChooser::registerOnColorChangedEvent(iColorChangedDelegate colorChangedDelegate)
    {
        _colorChanged.append(colorChangedDelegate);
    }

    void iUserControlColorChooser::unregisterOnColorChangedEvent(iColorChangedDelegate colorChangedDelegate)
    {
        _colorChanged.remove(colorChangedDelegate);
    }

    void iUserControlColorChooser::setText(const iaString &text)
    {
        _text = text;

        if (_titleLabel != nullptr)
        {
            _titleLabel->setText(_text);
        }
    }

    const iaString &iUserControlColorChooser::getText() const
    {
        return _text;
    }

    const iaColor4f &iUserControlColorChooser::getColor() const
    {
        return _colorRGBA;
    }

    void iUserControlColorChooser::setColor(const iaColor4f &color)
    {
        _colorRGBA = color;
        _colorHSV = iaColor4f::convertRGBtoHSV(_colorRGBA);
        updateWidgets();
    }

} // namespace igor