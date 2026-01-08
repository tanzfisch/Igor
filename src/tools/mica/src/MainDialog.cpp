// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include "MainDialog.h"

MainDialog::MainDialog()
{
    onInitUI();

    iProject::getInstance().getProjectLoadedEvent().add(iProjectLoadedDelegate(this, &MainDialog::onProjectLoaded));
    iProject::getInstance().getProjectUnloadedEvent().add(iProjectUnloadedDelegate(this, &MainDialog::onProjectUnloaded));
}

void MainDialog::onInitUI()
{
    setTitle("Mica");
    setZValue(10000);
    setDockable(false);
    setMoveable(false);
    setHeaderEnabled(false);
    setHorizontalAlignment(iHorizontalAlignment::Stretch);
    setVerticalAlignment(iVerticalAlignment::Stretch);

    _playStopButton = new iWidgetButton();
    _playStopButton->setIcon("igor_icon_play");
    _playStopButton->setCheckable(true);
    _playStopButton->setChecked(false);
    _playStopButton->getClickEvent().add(iClickDelegate(this, &MainDialog::onPlayStop));
    _playStopButton->setEnabled(false);

    iWidgetBoxLayoutPtr playButtonBox = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal);
    playButtonBox->setHorizontalAlignment(iHorizontalAlignment::Right);
    playButtonBox->setVerticalAlignment(iVerticalAlignment::Stretch);
    playButtonBox->addWidget(_playStopButton);

    iWidgetGridLayoutPtr mainMenuLayout = new iWidgetGridLayout();
    mainMenuLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    mainMenuLayout->setVerticalAlignment(iVerticalAlignment::Top);
    mainMenuLayout->appendColumns(1);
    mainMenuLayout->addWidget(createMenu(), 0, 0);
    mainMenuLayout->addWidget(playButtonBox, 1, 0);

    iWidgetBoxLayoutPtr vbox = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    vbox->setStretchIndex(1);
    vbox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    vbox->setVerticalAlignment(iVerticalAlignment::Stretch);
    vbox->addWidget(mainMenuLayout);
    vbox->addWidget(new iWidgetDockingLayout());
}

void MainDialog::onPlayStop(iWidgetPtr source)
{
    _playStopButton->setChecked(!_playStopButton->isChecked());
    bool playing = _playStopButton->isChecked();
    _playStopButton->setIcon(playing ? "igor_icon_stop" : "igor_icon_play");

    if (playing)
    {
        iProject::getInstance().save();
    }
    iProject::getInstance().setMode(playing ? iProject::iMode::Runtime : iProject::iMode::Edit);
}

void MainDialog::onRecentProjectOpen(iWidgetMenuPtr menu)
{
    menu->clear();
    if (iConfig::getInstance().hasValue("mica.recentProjects"))
    {
        const std::vector<iaString> recent = iConfig::getInstance().getValueAsArray("mica.recentProjects");
        std::vector<iaString> cleanup;

        for (const auto &project : recent)
        {
            if (!iaPath::exists(project))
            {
                continue;
            }

            cleanup.push_back(project);

            iActionContextPtr actionContext = std::make_shared<iFilesystemActionContext>(project);
            menu->addAction("igor:load_project", actionContext);
        }

        iConfig::getInstance().setValue("mica.recentProjects", cleanup);
    }
}

iWidgetMenuBarPtr MainDialog::createMenu()
{
    iWidgetMenuBarPtr menuBar = new iWidgetMenuBar();
    menuBar->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetMenuPtr fileMenu = new iWidgetMenu("File");
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onCreateProject), "New Project", "Create a new project");
    fileMenu->addSeparator();
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onLoadProject), "Open Project", "Loading an existing project", "igor_icon_load");
    iWidgetMenuPtr recentMenu = new iWidgetMenu("Open Recent");
    recentMenu->getPreMenuOpenEvent().add(iPreMenuOpenDelegate(this, &MainDialog::onRecentProjectOpen));
    fileMenu->addMenu(recentMenu);
    fileMenu->addSeparator();
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onSaveProject), "Save", "Saving the current project", "igor_icon_save");
    fileMenu->addSeparator();
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onCloseProject), "Close Project", "Closing the current project");
    fileMenu->addSeparator();
    fileMenu->addCallback(iClickDelegate(this, &MainDialog::onOpenSettings), "Settings", "Open Settings");

    fileMenu->addSeparator();
    fileMenu->addAction("igor:exit");
    menuBar->addMenu(fileMenu);

    iWidgetMenuPtr editMenu = new iWidgetMenu("Edit");
    // TODO copy paste etc
    menuBar->addMenu(editMenu);

    iWidgetMenuPtr viewMenu = new iWidgetMenu("View");
    // TODO move viewport buttons in here
    menuBar->addMenu(viewMenu);

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

CloseProjectEvent &MainDialog::getOpenSettingsEvent()
{
    return _openSettings;
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

void MainDialog::onOpenSettings(const iWidgetPtr source)
{
    _openSettings();
}

void MainDialog::onPrintProjectTree(const iWidgetPtr source)
{
    iEntityPrintTraverser print(true);
    print.traverse(iProject::getInstance().getRootScene());
    con_endl("\n"
             << print.getOutput());
}

void MainDialog::onProjectLoaded(const iaString &projectfile)
{
    _playStopButton->setEnabled(true);
}

void MainDialog::onProjectUnloaded()
{
    _playStopButton->setEnabled(false);
}