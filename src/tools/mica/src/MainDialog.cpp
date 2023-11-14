// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "MainDialog.h"

MainDialog::MainDialog()
{
    initGUI();
}

MainDialog::~MainDialog()
{
    deinitGUI();
}

// this really should not be in the MainDialog but in the UILayer in the "main" menu that does not exists yet
iWidgetMenuBarPtr MainDialog::createMenu()
{
    iWidgetMenuBarPtr menuBar = new iWidgetMenuBar();

    iWidgetMenuPtr fileMenu = new iWidgetMenu("File");
/*    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onCreateProject), "Create Project", "Create a new project");
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onLoadProject), "Load Project", "Loading an existing project", "igor_icon_load");
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onSaveProject), "Save Project", "Saving the current project", "igor_icon_save");
    fileMenu->addSeparator();
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onLoadFile), "Load File", "Loading file", "igor_icon_load");
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onSaveFile), "Save File", "Saving file", "igor_icon_save");*/
    fileMenu->addSeparator();
    fileMenu->addAction("igor:exit");
    menuBar->addMenu(fileMenu);

    iWidgetMenuPtr editMenu = new iWidgetMenu("Edit");
/*    editMenu->addCallback(iClickDelegate(this, &MainDialog::onCut), "Cut", "Cut selection", "igor_icon_cut");
    editMenu->addCallback(iClickDelegate(this, &MainDialog::onCopy), "Copy", "Copy selection", "igor_icon_copy");
    editMenu->addCallback(iClickDelegate(this, &MainDialog::onPaste), "Paste", "Paste from clipboard", "igor_icon_paste");
    editMenu->addSeparator();
    editMenu->addCallback(iClickDelegate(this, &MainDialog::onDelete), "Delete", "Delete selection", "igor_icon_delete");*/
    menuBar->addMenu(editMenu);

    return menuBar;
}

void MainDialog::initGUI()
{
    setZValue(10000);
    setDockable(false);
    setMoveable(true);
    setHeaderEnabled(true);
    setHorizontalAlignment(iHorizontalAlignment::Center);
    setVerticalAlignment(iVerticalAlignment::Center);
    setMinWidth(400);
    setMinHeight(400);

    iWidgetBoxLayoutPtr vbox = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    vbox->setStretchIndex(1);
    vbox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    vbox->setVerticalAlignment(iVerticalAlignment::Stretch);
    vbox->addWidget(createMenu());
    vbox->addWidget(new iWidgetDockingLayout());
}

void MainDialog::deinitGUI()
{
}