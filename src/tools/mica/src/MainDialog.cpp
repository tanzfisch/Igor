// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "MainDialog.h"

MainDialog::MainDialog()
{
    initGUI();
}

void MainDialog::initGUI()
{
    setTitle("Mica");
    setZValue(10000);
    setDockable(false);
    setMoveable(false);
    setHeaderEnabled(false);
    setHorizontalAlignment(iHorizontalAlignment::Stretch);
    setVerticalAlignment(iVerticalAlignment::Stretch);

    iWidgetBoxLayoutPtr vbox = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    vbox->setStretchIndex(1);
    vbox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    vbox->setVerticalAlignment(iVerticalAlignment::Stretch);
    vbox->addWidget(createMenu());
    vbox->addWidget(new iWidgetDockingLayout());
}

iWidgetMenuBarPtr MainDialog::createMenu()
{
    iWidgetMenuBarPtr menuBar = new iWidgetMenuBar();

    iWidgetMenuPtr fileMenu = new iWidgetMenu("File");
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onCreateProject), "Create Project", "Create a new project");
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onLoadProject), "Load Project", "Loading an existing project", "igor_icon_load");
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onSaveProject), "Save Project", "Saving the current project", "igor_icon_save");
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onCloseProject), "Close Project", "Closing the current project");
    fileMenu->addSeparator();
    fileMenu->addAction("igor:exit");
    menuBar->addMenu(fileMenu);

    iWidgetMenuPtr editMenu = new iWidgetMenu("Edit");
    // TODO
    menuBar->addMenu(editMenu);

    iWidgetMenuPtr projectMenu = new iWidgetMenu("Project");
    projectMenu->addCallback(iClickDelegate(this, &MainDialog::onPrintProjectTree), "Log Project Tree", "Logs the current project tree to the console");
    menuBar->addMenu(projectMenu);

    return menuBar;
}

CreateProjectEvent &MainDialog::getCreateProjectEvent()
{
    return _createProject;
}

LoadProjectEvent &MainDialog::getLoadProjectEvent()
{
    return _loadProject;
}

SaveProjectEvent &MainDialog::getSaveProjectEvent()
{
    return _saveProject;
}

CloseProjectEvent &MainDialog::getCloseProjectEvent()
{
    return _closeProject;
}

void MainDialog::onCreateProject(const iWidgetPtr source)
{
    _createProject();
}

void MainDialog::onLoadProject(const iWidgetPtr source)
{
    _loadProject();
}

void MainDialog::onSaveProject(const iWidgetPtr source)
{
    _saveProject();
}

void MainDialog::onCloseProject(const iWidgetPtr source)
{
    _closeProject();
}

void MainDialog::onPrintProjectTree(const iWidgetPtr source)
{
    iEntityPrintTraverser print(true);
    print.traverse(iProject::getInstance().getProjectScene());
} 