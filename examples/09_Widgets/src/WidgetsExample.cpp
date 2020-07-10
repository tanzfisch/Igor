// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
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
        setPicturePath("icons/camera.png");
    }

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override
    {
        con_endl("action one");
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
};

// action with only an icon
class Action3 : public iAction
{

public:
    Action3()
        : iAction("example:three")
    {
        setPicturePath("icons/delete.png");
    }

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override
    {
        con_endl("action three");
    }
};

// set an increase z index of 1 to make sure the ui is rendered above the background
WidgetsExample::WidgetsExample(iWindow *window)
    : iLayerWidgets(new iWidgetDefaultTheme("StandardFont.png", "WidgetThemePattern.png"), window, "Widgets", 10)
{
    // register the actions to make them globaly available
    iActionManager::getInstance().registerAction(new Action1());
    iActionManager::getInstance().registerAction(new Action2());
    iActionManager::getInstance().registerAction(new Action3());
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

void WidgetsExample::onInit()
{
    // call base class
    iLayerWidgets::onInit();

    _dialog = new iDialog();
    _dialog->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _dialog->setVerticalAlignment(iVerticalAlignment::Center);
    _dialog->setHeight(200);
    // it does not matter if we open it now or after adding all the child widgets
    _dialog->open(iDialogCloseDelegate(this, &WidgetsExample::onCloseDialog));

    iWidgetGrid *grid1 = new iWidgetGrid(_dialog);
    // put all widgets in one list for easier later cleanup. this method might not always be suitable
    grid1->appendRows(2);
    grid1->setHorizontalAlignment(iHorizontalAlignment::Strech);
    grid1->setVerticalAlignment(iVerticalAlignment::Strech);
    grid1->setBorder(10);
    grid1->setCellSpacing(5);
    grid1->setStrechRow(2);
    grid1->setStrechColumn(0);
    grid1->setSelectMode(iSelectionMode::NoSelection);

    // create a menu
    iWidgetMenuBarPtr menuBar = new iWidgetMenuBar();
    grid1->addWidget(menuBar, 0, 0);

    // get some actions to add to the menu
    iActionPtr action1 = iActionManager::getInstance().getAction("example:one");
    iActionPtr action2 = iActionManager::getInstance().getAction("example:two");
    iActionPtr action3 = iActionManager::getInstance().getAction("example:three");

    // build up menu and submenus
    iWidgetMenuPtr menu1 = new iWidgetMenu();
    menu1->setTitle("Bar");
    menu1->addAction(action1);
    menu1->addAction(action2);
    menuBar->addMenu(menu1);

    iWidgetMenuPtr menu2 = new iWidgetMenu();
    menu2->setTitle("Foo");
    menu2->addAction(action2);
    menu2->addAction(action1);
    menu2->addAction(action3);

    iWidgetMenuPtr menu2b = new iWidgetMenu();
    menu2b->setTitle("Sub Menu");
    menu2b->addAction(action1);
    menu2b->addAction(action2);
    menu2b->addAction(action1);
    menu2->addMenu(menu2b);

    iWidgetMenuPtr menu2c = new iWidgetMenu();
    menu2c->setTitle("An Other Sub Menu");
    menu2c->addAction(action2);
    menu2c->addAction(action3);
    menu2c->addAction(action3);
    menu2c->addAction(action3);
    menu2->addMenu(menu2c);

    menu2->addAction(action1);
    menuBar->addMenu(menu2);

    // adding a group box
    iWidgetGroupBox *groupBox1 = new iWidgetGroupBox();
    groupBox1->setText("Hello World. This is a group box!");
    groupBox1->setHorizontalAlignment(iHorizontalAlignment::Strech);
    groupBox1->setVerticalAlignment(iVerticalAlignment::Strech);

    // a scroll widget can contain a child that is bigger than it self. if so the scroll widget
    // shows vertical and or horizontal sliders and allows to scroll the child.
    iWidgetScroll *widgetScoll = new iWidgetScroll();
    widgetScoll->setHorizontalAlignment(iHorizontalAlignment::Strech);
    widgetScoll->setVerticalAlignment(iVerticalAlignment::Strech);

    iWidgetGrid *grid3 = new iWidgetGrid();
    grid3->appendCollumns(3);
    grid3->appendRows(4);
    grid3->setCellSpacing(10);
    // this grid has to be top left aligned because we want to use it as child of the scroll widget
    grid3->setHorizontalAlignment(iHorizontalAlignment::Left);
    grid3->setVerticalAlignment(iVerticalAlignment::Top);
    grid3->setSelectMode(iSelectionMode::NoSelection);

    iWidgetGrid *grid4 = new iWidgetGrid();
    grid4->appendCollumns(5);
    grid4->setStrechColumn(4);
    grid4->setHorizontalAlignment(iHorizontalAlignment::Strech);
    grid4->setVerticalAlignment(iVerticalAlignment::Top);
    grid4->setSelectMode(iSelectionMode::NoSelection);

    iWidgetLabel *label1 = new iWidgetLabel();
    label1->setText("This is a Label with just enough text.");

    iWidgetLabel *label2 = new iWidgetLabel();
    label2->setText("This is an other Label with just enough text.");

    iWidgetLabel *label3 = new iWidgetLabel();
    label3->setText("Top Right aligned Label");
    label3->setVerticalAlignment(iVerticalAlignment::Top);
    label3->setHorizontalAlignment(iHorizontalAlignment::Right);

    iWidgetLabel *label4 = new iWidgetLabel();
    label4->setText("This is a Label with a super long text so you can see the line break feature in action.");
    label4->setMaxTextWidth(200);

    iWidgetLabel *label5 = new iWidgetLabel();
    label5->setText("Bottom Left Aligned Label");
    label5->setVerticalAlignment(iVerticalAlignment::Bottom);
    label5->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetButton *button1 = new iWidgetButton();
    button1->setSize(70, 20);
    button1->setVerticalAlignment(iVerticalAlignment::Strech);
    button1->setHorizontalAlignment(iHorizontalAlignment::Center);
    button1->setText("Open Message Box");
    button1->setTooltip("Opens a message box");
    button1->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onOpenMessageBox));

    _color = new iWidgetColor();
    _color->setColor(iaColor4f(1, 1, 1, 0.5));
    _color->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onOpenColorChooser));

    _colorGradient = new iWidgetColorGradient();
    iaGradientColor4f rainbow;
    rainbow.setValue(0.0f, iaColor4f(1.0f, 0.0f, 1.0f, 0.0f));
    rainbow.setValue(0.2f, iaColor4f(0.0f, 0.0f, 1.0f, 0.2f));
    rainbow.setValue(0.4f, iaColor4f(0.0f, 1.0f, 1.0f, 0.4f));
    rainbow.setValue(0.6f, iaColor4f(0.0f, 1.0f, 0.0f, 0.6f));
    rainbow.setValue(0.8f, iaColor4f(1.0f, 1.0f, 0.0f, 0.8f));
    rainbow.setValue(1.0f, iaColor4f(1.0f, 0.0f, 0.0f, 1.0f));
    _colorGradient->setGradient(rainbow);
    _colorGradient->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _colorGradient->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onOpenColorGradientEditor));

    _labelMousePos = new iWidgetLabel();

    iWidgetButton *exitButton = new iWidgetButton();
    exitButton->setText("");
    exitButton->setTooltip("Exists the application.");
    exitButton->setTexture("icons/exit.png");
    exitButton->setVerticalTextAlignment(iVerticalAlignment::Bottom);
    exitButton->setVerticalAlignment(iVerticalAlignment::Center);
    exitButton->setHorizontalAlignment(iHorizontalAlignment::Center);
    exitButton->setSize(64, 64);
    exitButton->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onExitClick));

    iWidgetSelectBox *selectBox = new iWidgetSelectBox();
    selectBox->addSelectionEntry("bla");
    selectBox->addSelectionEntry("blub haha bla blub");
    selectBox->addSelectionEntry("haha");
    selectBox->setSelection(1);
    selectBox->setHorizontalAlignment(iHorizontalAlignment::Right);

    iWidgetNumberChooser *numberChooser = new iWidgetNumberChooser();
    numberChooser->setPostFix("%");
    numberChooser->setValue(50);
    numberChooser->setHorizontalAlignment(iHorizontalAlignment::Right);

    iWidgetTextEdit *textEditLeft = new iWidgetTextEdit();
    textEditLeft->setWidth(150);
    textEditLeft->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    textEditLeft->setText("left aligned");
    textEditLeft->setMaxTextLength(300);

    iWidgetTextEdit *textEditCenter = new iWidgetTextEdit();
    textEditCenter->setWidth(150);
    textEditCenter->setHorizontalTextAlignment(iHorizontalAlignment::Center);
    textEditCenter->setText("center aligned");
    textEditCenter->setMaxTextLength(300);

    iWidgetTextEdit *textEditRight = new iWidgetTextEdit();
    textEditRight->setWidth(150);
    textEditRight->setHorizontalTextAlignment(iHorizontalAlignment::Right);
    textEditRight->setText("right aligned");
    textEditRight->setMaxTextLength(300);

    iWidgetPicture *picture1 = new iWidgetPicture();
    picture1->setTexture("OpenGL-Logo.jpg");
    picture1->setMaxSize(200, 64);
    picture1->setVerticalAlignment(iVerticalAlignment::Top);
    picture1->setHorizontalAlignment(iHorizontalAlignment::Left);

    // if check boxes are supposed to be connected as radio buttons tell the widget manager by starting a radio button group
    iWidgetCheckBox::beginRadioButtonGroup();
    iWidgetCheckBox *radio1 = new iWidgetCheckBox();
    radio1->setHorizontalAlignment(iHorizontalAlignment::Left);
    radio1->setText("radio1");

    iWidgetCheckBox *radio2 = new iWidgetCheckBox();
    radio2->setText("radio2");
    radio2->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetCheckBox *radio3 = new iWidgetCheckBox();
    radio3->setText("radio3");
    radio3->setHorizontalAlignment(iHorizontalAlignment::Left);
    // finish the radio button group
    iWidgetCheckBox::endRadioButtonGroup();

    iWidgetGraph *graph = new iWidgetGraph();
    graph->setSize(300, 50);

    std::vector<iaVector2f> points;
    points.push_back(iaVector2f(0.0f, 0.2f));
    points.push_back(iaVector2f(0.2f, 0.4f));
    points.push_back(iaVector2f(0.3f, 0.5f));
    points.push_back(iaVector2f(0.5f, 0.1f));
    points.push_back(iaVector2f(0.6f, 0.2f));
    points.push_back(iaVector2f(0.8f, 0.4f));
    points.push_back(iaVector2f(1.0f, 0.2f));
    graph->setPoints(0, points);
    graph->setLineColor(0, iaColor4f(1.0f, 0.0f, 0.0f, 1.0f));
    graph->setPointColor(0, iaColor4f(1.0f, 0.5f, 0.5f, 1.0f));
    graph->setPointSize(0, 4.0f);
    graph->setLineWidth(0, 2.0f);

    std::vector<iaVector2f> points2;
    points2.push_back(iaVector2f(0.2f, 0.5f));
    points2.push_back(iaVector2f(0.3f, 0.3f));
    points2.push_back(iaVector2f(0.4f, 0.2f));
    points2.push_back(iaVector2f(0.5f, 0.4f));
    points2.push_back(iaVector2f(0.6f, 0.7f));
    points2.push_back(iaVector2f(0.7f, 0.1f));
    points2.push_back(iaVector2f(0.8f, 0.9f));
    graph->setPoints(1, points2);
    graph->setLineColor(1, iaColor4f(0.0f, 1.0f, 0.0f, 1.0f));
    graph->setPointColor(1, iaColor4f(0.5f, 1.0f, 0.5f, 1.0f));
    graph->setPointSize(1, 4.0f);
    graph->setLineWidth(1, 2.0f);

    std::vector<iaVector2f> points3;
    points3.push_back(iaVector2f(0.0f, 0.9f));
    points3.push_back(iaVector2f(0.1f, 0.5f));
    points3.push_back(iaVector2f(0.2f, 0.6f));
    points3.push_back(iaVector2f(0.3f, 0.7f));
    points3.push_back(iaVector2f(0.8f, 0.5f));
    points3.push_back(iaVector2f(0.9f, 0.6f));
    points3.push_back(iaVector2f(1.0f, 0.7f));
    graph->setPoints(2, points3);
    graph->setLineColor(2, iaColor4f(0.0f, 0.0f, 1.0f, 1.0f));
    graph->setPointColor(2, iaColor4f(0.5f, 0.5f, 1.0f, 1.0f));
    graph->setPointSize(2, 4.0f);
    graph->setLineWidth(2, 2.0f);

    graph->setExtrapolateData();
    graph->setBoundings(iRectanglef(0.0f, 0.0f, 1.0f, 1.0f));
    graph->setUseBoundings();
    graph->setViewGrid();

    // assemble all the widgets with their parents
    grid1->addWidget(groupBox1, 0, 1);
    groupBox1->addWidget(grid4);
    grid4->addWidget(exitButton, 0, 0);
    grid4->addWidget(new iWidgetSpacer(30, 2), 1, 0);
    grid4->addWidget(picture1, 2, 0);
    grid4->addWidget(_color, 3, 0);
    grid4->addWidget(_colorGradient, 4, 0);
    grid4->addWidget(graph, 5, 0);

    grid1->addWidget(widgetScoll, 0, 2);
    widgetScoll->addWidget(grid3);

    grid3->addWidget(label1, 0, 0);
    grid3->addWidget(_labelMousePos, 1, 0);
    grid3->addWidget(textEditLeft, 0, 1);
    grid3->addWidget(textEditCenter, 0, 2);
    grid3->addWidget(textEditRight, 0, 3);
    grid3->addWidget(label2, 1, 1);
    grid3->addWidget(label3, 1, 2);
    grid3->addWidget(label4, 2, 2);
    grid3->addWidget(label5, 2, 3);
    grid3->addWidget(button1, 1, 3);
    grid3->addWidget(selectBox, 3, 2);
    grid3->addWidget(numberChooser, 3, 1);
    grid3->addWidget(radio1, 0, 4);
    grid3->addWidget(radio2, 1, 4);
    grid3->addWidget(radio3, 2, 4);

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

void WidgetsExample::onEvent(iEvent &event)
{
    iLayerWidgets::onEvent(event);

    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(WidgetsExample::onMouseMoveEvent));
}

bool WidgetsExample::onMouseMoveEvent(iEventMouseMove &event)
{
    // updates a label with current mouse position
    if (_labelMousePos != nullptr)
    {
        iaString text;
        text += iaString::toString(event.getPosition()._x);
        text += ":";
        text += iaString::toString(event.getPosition()._y);

        _labelMousePos->setText(text);
    }

    return false;
}

void WidgetsExample::onOpenColorChooser(const iWidgetPtr source)
{
    if (_colorChooserDialog == nullptr)
    {
        _colorChooserDialog = new iDialogColorChooser();
    }
    _colorChooserDialog->open(iDialogCloseDelegate(this, &WidgetsExample::onCloseColorChooser), _color->getColor(), true);
}

void WidgetsExample::onOpenColorGradientEditor(const iWidgetPtr source)
{
    if (_colorGradientDialog == nullptr)
    {
        _colorGradientDialog = new iDialogColorGradient();
    }

    _colorGradientDialog->setColorGradient(_colorGradient->getGradient());
    _colorGradientDialog->setUseAlpha(false);
    _colorGradientDialog->open(iDialogCloseDelegate(this, &WidgetsExample::onCloseColorGradient));
}

void WidgetsExample::onCloseColorGradient(iDialogPtr dialog)
{
    if (dialog != _colorGradientDialog)
    {
        return;
    }

    if (_colorGradientDialog->getReturnState() == iDialogReturnState::Ok)
    {
        _colorGradient->setGradient(_colorGradientDialog->getColorGradient());
    }

    delete _colorGradientDialog;
    _colorGradientDialog = nullptr;
}

void WidgetsExample::onCloseColorChooser(iDialogPtr dialog)
{
    if (dialog != _colorChooserDialog)
    {
        return;
    }

    if (_colorChooserDialog->getReturnState() == iDialogReturnState::Ok)
    {
        _color->setColor(_colorChooserDialog->getColor());
    }

    delete _colorChooserDialog;
    _colorChooserDialog = nullptr;
}

void WidgetsExample::onOpenMessageBox(const iWidgetPtr source)
{
    // open a message box with some text
    if (_messageBox == nullptr)
    {
        _messageBox = new iDialogMessageBox();
    }

    _messageBox->open(iDialogCloseDelegate(this, &WidgetsExample::onCloseMessageBox), "Please click Yes No or Cancel and see the output in the console.", iMessageBoxButtons::YesNoCancel);
}

void WidgetsExample::onCloseMessageBox(iDialogPtr dialog)
{
    iaString returnString;
    switch (static_cast<iDialogMessageBox *>(dialog)->getReturnState())
    {
    case iDialogReturnState::No:
        returnString = "No";
        break;

    case iDialogReturnState::Yes:
        returnString = "Yes/Ok";
        break;

    case iDialogReturnState::Cancel:
        returnString = "Cancel";
        break;
    }

    con_endl("Message box return value is " << returnString);

    delete _messageBox;
    _messageBox = nullptr;
}

void WidgetsExample::onExitClick(const iWidgetPtr source)
{
    // close the dialog
    _dialog->close();

    // shut down application
    iApplication::getInstance().stop();
}