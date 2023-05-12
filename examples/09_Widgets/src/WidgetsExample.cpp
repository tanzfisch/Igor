// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "WidgetsExample.h"

// define some actions
class Action1 : public iAction
{

public:
    Action1()
        : iAction("example:one")
    {
        setDescription("Action One");
        setPicturePath("igor/icons/camera.png");
    }

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override
    {
        con_endl("action one");
    }

    /*! \returns true if this action will execute with given context

    \param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override
    {
        return true;
    }
};

// action with only description
class Action2 : public iAction
{

public:
    Action2()
        : iAction("example:two")
    {
        setDescription("Action Two");
    }

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override
    {
        con_endl("action two");
    }

    /*! \returns true if this action will execute with given context

    \param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override
    {
        return true;
    }
};

// action with only an icon
class Action3 : public iAction
{

public:
    Action3()
        : iAction("example:three")
    {
        setPicturePath("igor/icons/delete.png");
    }

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override
    {
        con_endl("action three");
    }

    /*! \returns true if this action will execute with given context

    \param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override
    {
        return true;
    }
};

// action that is incompatible and will not appear in any menu
class Action4 : public iAction
{

public:
    Action4()
        : iAction("example:four")
    {
        setPicturePath("igor/icons/delete.png");
    }

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override
    {
        con_endl("action four");
    }

    /*! \returns true if this action will execute with given context

    \param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override
    {
        return false;
    }
};

// set an increase z index of 1 to make sure the ui is rendered above the background
WidgetsExample::WidgetsExample(iWindowPtr window, iLayerProfilerPtr layerProfiler, Background3D* backGround)
    : iLayerWidgets(iWidgetThemePtr(new iWidgetDefaultTheme("igor/textures/StandardFont.png", "WidgetThemePattern.png")), window, "Widgets", 10)
    , _layerProfiler(layerProfiler)
    , _backGround(backGround)
{
    // register the actions to make them globally available
    iActionManager::getInstance().registerAction(new Action1());
    iActionManager::getInstance().registerAction(new Action2());
    iActionManager::getInstance().registerAction(new Action3());
    iActionManager::getInstance().registerAction(new Action4());
}

void WidgetsExample::onCloseDialog(iDialogPtr dialog)
{
    if (_dialog != dialog)
    {
        return;
    }

    delete _dialog;
    _dialog = nullptr;
}

iWidgetMenuBarPtr WidgetsExample::createMenu()
{
    iWidgetMenuBarPtr menuBar = new iWidgetMenuBar();

    // get some actions to add to the menu
    iActionPtr action1 = iActionManager::getInstance().getAction("example:one");
    iActionPtr action2 = iActionManager::getInstance().getAction("example:two");
    iActionPtr action3 = iActionManager::getInstance().getAction("example:three");
    iActionPtr action4 = iActionManager::getInstance().getAction("example:four");

    // build up menu and submenus
    iWidgetMenuPtr menu1 = new iWidgetMenu();
    menu1->setTitle("Menu");
    menu1->addAction(action1);
    menu1->addAction(action2);
    menu1->addAction(action4);
    menuBar->addMenu(menu1);

    iWidgetMenuPtr menu2 = new iWidgetMenu();
    menu2->setTitle("Settings");
    menu2->addAction(action2);
    menu2->addAction(action1);
    menu2->addAction(action4);
    menu2->addAction(action3);

    iWidgetMenuPtr menu2b = new iWidgetMenu();
    menu2b->setTitle("Sub Menu");
    menu2b->addAction(action1);
    menu2b->addAction(action2);
    menu2b->addAction(action1);
    menu2->addMenu(menu2b);

    iWidgetMenuPtr menu2ba = new iWidgetMenu();
    menu2ba->setTitle("Sub Sub Menu");
    menu2ba->addAction(action1);
    menu2ba->addAction(action2);
    menu2ba->addAction(action1);
    menu2b->addMenu(menu2ba);

    iWidgetMenuPtr menu2c = new iWidgetMenu();
    menu2c->setTitle("An Other Sub Menu");
    menu2c->addAction(action2);
    menu2c->addAction(action3);
    menu2c->addAction(action3);
    menu2c->addAction(action4);
    menu2c->addAction(action3);
    menu2->addMenu(menu2c);

    menu2->addAction(action1);
    menuBar->addMenu(menu2);

    return menuBar;
}

void WidgetsExample::onInit()
{
    // call base class
    iLayerWidgets::onInit();

    _dialog = new iDialog();
    _dialog->setPos(40, getWindow()->getClientHeight() - 420);

    iWidgetGrid *gridMain = new iWidgetGrid(_dialog);
    gridMain->appendRows(3);
    gridMain->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    gridMain->setVerticalAlignment(iVerticalAlignment::Stretch);
    gridMain->setBorder(10);
    gridMain->setStretchRow(2);

    // create a menu and add to grid
    iWidgetMenuBarPtr menuBar = createMenu();
    gridMain->addWidget(menuBar, 0, 0);

    // adding a group box
    iWidgetGroupBox *groupBox1 = new iWidgetGroupBox();
    groupBox1->setText("Hello World. This is a group box!");
    groupBox1->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    gridMain->addWidget(groupBox1, 0, 1);

    // add some buttons
    iWidgetGrid *gridButtons = new iWidgetGrid();
    gridButtons->appendColumns(3);
    gridButtons->setStretchColumn(3);
    gridButtons->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    gridMain->addWidget(gridButtons, 0, 1);

    iWidgetButton *exitButton = new iWidgetButton();
    exitButton->setText("");
    exitButton->setTooltip("Exists the application.");
    exitButton->setTexture("igor/icons/exit.png");
    exitButton->setVerticalTextAlignment(iVerticalAlignment::Bottom);
    exitButton->setVerticalAlignment(iVerticalAlignment::Center);
    exitButton->setHorizontalAlignment(iHorizontalAlignment::Center);
    exitButton->setSize(24, 24);
    exitButton->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onExitClick));
    gridButtons->addWidget(exitButton, 0, 0);

    // controls on the left
    iWidgetGrid *gridControls = new iWidgetGrid();
    gridControls->appendColumns(1);
    gridControls->appendRows(3);
    gridControls->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    gridControls->setVerticalAlignment(iVerticalAlignment::Top);
    gridMain->addWidget(gridControls, 0, 2);

    iWidgetLabel *labelProfiler = new iWidgetLabel();
    labelProfiler->setText("Profiler:");
    labelProfiler->setHorizontalAlignment(iHorizontalAlignment::Right);
    gridControls->addWidget(labelProfiler, 0, 0);

    iWidgetSelectBoxPtr selectBox = new iWidgetSelectBox();
    selectBox->addSelectionEntry("None");
    selectBox->addSelectionEntry("FPSOnly");
    selectBox->addSelectionEntry("FPSAndMetrics");
    selectBox->addSelectionEntry("FPSMetricsAndTasks");
    selectBox->addSelectionEntry("Sections");
    selectBox->setSelection((int)_layerProfiler->getVerbosity());
    selectBox->setHorizontalAlignment(iHorizontalAlignment::Right);
    selectBox->registerOnChangeEvent(iChangeDelegate(this, &WidgetsExample::onProfilerVerbosityChange));
    gridControls->addWidget(selectBox, 1, 0);

    iWidgetCheckBox *radioWireframe = new iWidgetCheckBox();
    radioWireframe->setText("Wireframe");
    radioWireframe->setHorizontalAlignment(iHorizontalAlignment::Left);    
    radioWireframe->registerOnChangeEvent(iChangeDelegate(this, &WidgetsExample::onWireframeChange));
    gridControls->addWidget(radioWireframe, 1, 1);

    iWidgetCheckBox *radioOctree = new iWidgetCheckBox();
    radioOctree->setText("Octree");
    radioOctree->setHorizontalAlignment(iHorizontalAlignment::Left);    
    radioOctree->registerOnChangeEvent(iChangeDelegate(this, &WidgetsExample::onOctreeChange));
    gridControls->addWidget(radioOctree, 1, 2);

    iWidgetCheckBox *radioBoundings = new iWidgetCheckBox();
    radioBoundings->setText("Boundings");
    radioBoundings->setHorizontalAlignment(iHorizontalAlignment::Left);    
    radioBoundings->registerOnChangeEvent(iChangeDelegate(this, &WidgetsExample::onBoundsChange));
    gridControls->addWidget(radioBoundings, 1, 3);

    _dialog->open(iDialogCloseDelegate(this, &WidgetsExample::onCloseDialog));

    // update desktop size
    iWidgetManager::getInstance().setDesktopDimensions(getWindow()->getClientWidth(), getWindow()->getClientHeight());
}

void WidgetsExample::onDeinit()
{
    // if dialog is still open close it now
    if (_dialog != nullptr &&
        _dialog->isOpen())
    {
        _dialog->close();
    }

    iLayerWidgets::onDeinit();
}

void WidgetsExample::onWireframeChange(const iWidgetPtr source)
{
    iWidgetCheckBoxPtr checkBox = static_cast<iWidgetCheckBoxPtr>(source);
    _backGround->getView().setWireframeVisible(checkBox->isChecked());
}

void WidgetsExample::onOctreeChange(const iWidgetPtr source)
{
    iWidgetCheckBoxPtr checkBox = static_cast<iWidgetCheckBoxPtr>(source);
    _backGround->getView().setOctreeVisible(checkBox->isChecked());
}

void WidgetsExample::onBoundsChange(const iWidgetPtr source)
{
    iWidgetCheckBoxPtr checkBox = static_cast<iWidgetCheckBoxPtr>(source);
    _backGround->getView().setBoundingBoxVisible(checkBox->isChecked());
}

void WidgetsExample::onProfilerVerbosityChange(const iWidgetPtr source)
{
    iWidgetSelectBoxPtr selectBox = static_cast<iWidgetSelectBoxPtr>(source);
    _layerProfiler->setVerbosity((iProfilerVerbosity)selectBox->getSelectedIndex());
}

void WidgetsExample::onExitClick(const iWidgetPtr source)
{
    // close the dialog
    _dialog->close();

    // shut down application
    iApplication::getInstance().exit();
}