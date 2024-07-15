// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "MainDialog.h"

MainDialog::MainDialog(WorkspacePtr workspace)
: _workspace(workspace)
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
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onCreateProject), "Create Project", "Create a new project");
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onLoadProject), "Load Project", "Loading an existing project", "igor_icon_load");
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onSaveProject), "Save Project", "Saving the current project", "igor_icon_save");
    fileMenu->addSeparator();
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onLoadFile), "Load File", "Loading file", "igor_icon_load");
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onSaveFile), "Save File", "Saving file", "igor_icon_save");
    fileMenu->addSeparator();
    fileMenu->addAction("igor:exit");
    menuBar->addMenu(fileMenu);

    iWidgetMenuPtr editMenu = new iWidgetMenu("Edit");
    editMenu->addCallback(iClickDelegate(this, &MainDialog::onCut), "Cut", "Cut selection", "igor_icon_cut");
    editMenu->addCallback(iClickDelegate(this, &MainDialog::onCopy), "Copy", "Copy selection", "igor_icon_copy");
    editMenu->addCallback(iClickDelegate(this, &MainDialog::onPaste), "Paste", "Paste from clipboard", "igor_icon_paste");
    editMenu->addSeparator();
    editMenu->addCallback(iClickDelegate(this, &MainDialog::onDelete), "Delete", "Delete selection", "igor_icon_delete");
    menuBar->addMenu(editMenu);


    iWidgetMenuPtr projectMenu = new iWidgetMenu("Project");
    projectMenu->addCallback(iClickDelegate(this, &MainDialog::onCut), "Settings", "Open project settings", "igor_icon_gears");
    menuBar->addMenu(projectMenu);

    return menuBar;
}

CreateProjectEvent &MainDialog::getEventCreateProject()
{
    return _createProject;
}

LoadProjectEvent &MainDialog::getEventLoadProject()
{
    return _loadProject;
}

SaveProjectEvent &MainDialog::getEventSaveProject()
{
    return _saveProject;
}

LoadFileEvent &MainDialog::getEventLoadFile()
{
    return _loadFile;
}

SaveFileEvent &MainDialog::getEventSaveFile()
{
    return _saveFile;
}

CopyNodeEvent &MainDialog::getEventCopyNode()
{
    return _copyNode;
}

CutNodeEvent &MainDialog::getEventCutNode()
{
    return _cutNode;
}

PasteNodeEvent &MainDialog::getEventPasteNode()
{
    return _pasteNode;
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

void MainDialog::onLoadFile(const iWidgetPtr source)
{
    _loadFile();
}

void MainDialog::onSaveFile(const iWidgetPtr source)
{
    _saveFile();
}

void MainDialog::onDelete(const iWidgetPtr source)
{
    _workspace->deleteSelected();
}

void MainDialog::onCopy(const iWidgetPtr source)
{
    _workspace->copySelected();
}

void MainDialog::onPaste(const iWidgetPtr source)
{
    _workspace->pasteSelected();
}

void MainDialog::onCut(const iWidgetPtr source)
{
    _workspace->cutSelected();
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

void MainDialog::deinitGUI()
{
}