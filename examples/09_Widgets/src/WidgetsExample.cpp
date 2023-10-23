// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "WidgetsExample.h"

// define some actions
class Action1 : public iAction
{

public:
    Action1()
        : iAction("exampleAction:one")
    {
        setDescription("Action One");
        setIcon("igor_icon_camera");
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
        : iAction("exampleAction:two")
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
        : iAction("exampleAction:three")
    {
        setIcon("igor_icon_delete");
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
        : iAction("exampleAction:four")
    {
        setIcon("igor_icon_delete");
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
WidgetsExample::WidgetsExample(iWindowPtr window, iLayerProfilerPtr layerProfiler, Background3D *backGround)
    : iLayerWidgets(iWidgetThemePtr(new iWidgetDefaultTheme(iResourceManager::getInstance().loadResource<iTexture>("igor_font_default"),
                                                            iResourceManager::getInstance().loadResource<iTexture>("example_texture_widget_theme_pattern"))),
                    window, "Widgets", 10),
      _layerProfiler(layerProfiler), _backGround(backGround)
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
    iActionPtr action1 = iActionManager::getInstance().getAction("exampleAction:one");
    iActionPtr action2 = iActionManager::getInstance().getAction("exampleAction:two");
    iActionPtr action3 = iActionManager::getInstance().getAction("exampleAction:three");
    iActionPtr action4 = iActionManager::getInstance().getAction("exampleAction:four");

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
    _dialog->setPos(iaVector2f(40, getWindow()->getClientHeight() - 420));
    _dialog->setResizeable(false);

    iWidgetBoxLayoutPtr mainLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, _dialog);
    mainLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    mainLayout->setVerticalAlignment(iVerticalAlignment::Stretch);

    // create a menu and add to layout
    iWidgetMenuBarPtr menuBar = createMenu();
    mainLayout->addWidget(menuBar);

    // add some buttons
    iWidgetBoxLayoutPtr buttonLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal);
    mainLayout->addWidget(buttonLayout);

    iWidgetButton *exitButton = new iWidgetButton();
    exitButton->setAction(iActionManager::getInstance().getAction("igor:exit"));
    exitButton->setText(""); // clear text
    exitButton->setSize(24, 24);
    buttonLayout->addWidget(exitButton);

    iWidgetButton *button2 = new iWidgetButton();
    button2->setAction(iActionManager::getInstance().getAction("exampleAction:one"));
    button2->setText(""); // clear text
    button2->setSize(24, 24);
    buttonLayout->addWidget(button2);

    iWidgetButton *button3 = new iWidgetButton();
    button3->setAction(iActionManager::getInstance().getAction("exampleAction:two"));
    button3->setSize(24, 24);
    buttonLayout->addWidget(button3);

    // adding a group box
    iWidgetGroupBox *groupBox1 = new iWidgetGroupBox();
    groupBox1->setText("Group Box");
    groupBox1->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    mainLayout->addWidget(groupBox1);

    iWidgetBoxLayoutPtr vBoxLayout2 = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical);
    groupBox1->addWidget(vBoxLayout2);

    iWidgetBoxLayoutPtr profileLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal);
    vBoxLayout2->addWidget(profileLayout);

    iWidgetLabel *labelProfiler = new iWidgetLabel();
    labelProfiler->setText("Profiler:");
    labelProfiler->setHorizontalAlignment(iHorizontalAlignment::Right);
    labelProfiler->setVerticalAlignment(iVerticalAlignment::Top);
    labelProfiler->setWidth(80);
    profileLayout->addWidget(labelProfiler);

    iWidgetSelectBoxPtr selectBox = new iWidgetSelectBox();
    selectBox->addItem("None");
    selectBox->addItem("FPSOnly");
    selectBox->addItem("FPSAndMetrics");
    selectBox->addItem("FPSMetricsAndTasks");
    selectBox->addItem("Sections");
    selectBox->setSelection((int)_layerProfiler->getVerbosity());
    selectBox->setHorizontalAlignment(iHorizontalAlignment::Right);
    selectBox->registerOnChangeEvent(iChangeDelegate(this, &WidgetsExample::onProfilerVerbosityChange));
    profileLayout->addWidget(selectBox);

    iWidgetBoxLayoutPtr optionsLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal);
    vBoxLayout2->addWidget(optionsLayout);

    iWidgetLabel *labelOptions = new iWidgetLabel();
    labelOptions->setText("Options:");
    labelOptions->setHorizontalAlignment(iHorizontalAlignment::Right);
    labelOptions->setVerticalAlignment(iVerticalAlignment::Top);
    labelOptions->setWidth(80);
    optionsLayout->addWidget(labelOptions);    

    iWidgetBoxLayoutPtr radioLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical);
    optionsLayout->addWidget(radioLayout);

    iWidgetCheckBox *radioWireframe = new iWidgetCheckBox();
    radioWireframe->setText("Wireframe");
    radioWireframe->setHorizontalAlignment(iHorizontalAlignment::Left);
    radioWireframe->registerOnChangeEvent(iChangeDelegate(this, &WidgetsExample::onWireframeChange));
    radioLayout->addWidget(radioWireframe);

    iWidgetCheckBox *radioOctree = new iWidgetCheckBox();
    radioOctree->setText("Octree");
    radioOctree->setHorizontalAlignment(iHorizontalAlignment::Left);
    radioOctree->registerOnChangeEvent(iChangeDelegate(this, &WidgetsExample::onOctreeChange));
    radioLayout->addWidget(radioOctree);

    iWidgetCheckBox *radioBoundings = new iWidgetCheckBox();
    radioBoundings->setText("Boundings");
    radioBoundings->setHorizontalAlignment(iHorizontalAlignment::Left);
    radioBoundings->registerOnChangeEvent(iChangeDelegate(this, &WidgetsExample::onBoundsChange));
    radioLayout->addWidget(radioBoundings);

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
