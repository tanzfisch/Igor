// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include "Settings.h"

#include "../MicaDefines.h"

Settings::Settings()
{
    setTitle("Settings");

    setResizeable(false);
    setMoveable(true);
    setAcceptESCToClose(true);

    init();
}

Settings::~Settings()
{
}

void Settings::open()
{
    readSettings();

    setHorizontalAlignment(iHorizontalAlignment::Center);
    setVerticalAlignment(iVerticalAlignment::Center);

    iDialog::open();
}

void Settings::init()
{
    iWidgetBoxLayoutPtr mainLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    mainLayout->setSpacing(5);
    mainLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    mainLayout->setVerticalAlignment(iVerticalAlignment::Stretch);

    iWidgetGroupBoxPtr settingsGroupBox = new iWidgetGroupBox(mainLayout);
    settingsGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    settingsGroupBox->setVerticalAlignment(iVerticalAlignment::Stretch);
    settingsGroupBox->setText("Mica");

    iWidgetBoxLayoutPtr settingsLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, settingsGroupBox);
    settingsLayout->setSpacing(5);
    settingsLayout->setStretchIndex(1);
    settingsLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    settingsLayout->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetBoxLayoutPtr scriptEditorLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, settingsLayout);
    scriptEditorLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    scriptEditorLayout->setStretchIndex(1);
    iWidgetLabelPtr labelScriptEditor = new iWidgetLabel(scriptEditorLayout);
    labelScriptEditor->setText("Script Editor");
    labelScriptEditor->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelScriptEditor->setHorizontalAlignment(iHorizontalAlignment::Left);

    _scriptEditor = new iWidgetLineTextEdit(scriptEditorLayout);
    _scriptEditor->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _scriptEditor->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _scriptEditor->setEnabled(true);

    iWidgetSpacerPtr spacer = new iWidgetSpacer(20, 100, false, settingsLayout);

    iWidgetBoxLayoutPtr buttonsLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, mainLayout);
    buttonsLayout->setStretchIndex(1);
    buttonsLayout->setHorizontalAlignment(iHorizontalAlignment::Right);
    buttonsLayout->setVerticalAlignment(iVerticalAlignment::Bottom);

    iWidgetButtonPtr cancelButton = new iWidgetButton(buttonsLayout);
    cancelButton->setText("Cancel");
    cancelButton->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    cancelButton->getClickEvent().add(iClickDelegate(this, &Settings::onCancel));

    iWidgetButtonPtr okButton = new iWidgetButton(buttonsLayout);
    okButton->setText("Ok");
    okButton->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    okButton->getClickEvent().add(iClickDelegate(this, &Settings::onOk));
}

void Settings::readSettings()
{
    if (iConfig::getInstance().hasValue("mica.scriptEditor"))
    {
        _scriptEditor->setText(iConfig::getInstance().getValue("mica.scriptEditor"));
    }
}

void Settings::writeSettings()
{
    iConfig::getInstance().setValue("mica.scriptEditor", _scriptEditor->getText());
}

void Settings::onOk(iWidgetPtr source)
{
    writeSettings();
    close();
}

void Settings::onCancel(iWidgetPtr source)
{
    close();
}