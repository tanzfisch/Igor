#include "WidgetsExample.h"

#include <iRenderer.h>
#include <iApplication.h>
#include <iMouse.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iMaterialResourceFactory.h>
#include <iWidgetManager.h>
#include <iDialog.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetGroupBox.h>
#include <iWidgetGrid.h>
#include <iWidgetCheckBox.h>
#include <iWidgetNumberChooser.h>
#include <iWidgetTextEdit.h>
#include <iWidgetPicture.h>
#include <iWidgetScroll.h>
#include <iWidgetSelectBox.h>
#include <iDialogMessageBox.h>
#include <iWidgetDefaultTheme.h>
#include <iWidgetSpacer.h>
#include <iTextureResourceFactory.h>
#include <iStatistics.h>
#include <iWidgetGraph.h>
#include <iDialogColorChooser.h>
#include <iWidgetColor.h>
#include <iWidgetColorGradient.h>
#include <iDialogColorGradient.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

WidgetsExample::WidgetsExample()
{
    init();
}

WidgetsExample::~WidgetsExample()
{
    deinit();
}

void WidgetsExample::init()
{
    con(" -- Widget Example --" << endl);

    // register our render function to the view with the ortho projection
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &WidgetsExample::onRender));

    // init window with the orthogonal projected view
    _window.addView(&_viewOrtho);
    _window.setClientSize(1024, 768);
    // register to window close event so we can shut down the application properly
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &WidgetsExample::onWindowClosed));
    // register to window resize event so we can notify the widget manager of that change
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &WidgetsExample::onWindowResize));
    // opens the window
    _window.open();

    // set up font for FPS display
    _font = new iTextureFont("StandardFont.png");

    // prepare igor logo
    _igorLogo = iTextureResourceFactory::getInstance().loadFile("special/splash.png", iResourceCacheMode::Free, iTextureBuildMode::Normal);
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial("TextureAndBlending");
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    // initialize the GUI
    initGUI();

    // register to mouse move event
    iMouse::getInstance().registerMouseMoveDelegate(iMouseMoveDelegate(this, &WidgetsExample::onMouseMove));
}

void WidgetsExample::initGUI()
{
    // create a theme and set it up. in this case the build in default theme
    _widgetDefaultTheme = new iWidgetDefaultTheme("StandardFont.png", "WidgetThemePattern.png");
    iWidgetManager::getInstance().setTheme(_widgetDefaultTheme);

    _dialog = static_cast<iDialog*>(iWidgetManager::getInstance().createDialog("Dialog"));
    _dialog->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _dialog->setVerticalAlignment(iVerticalAlignment::Center);
    _dialog->setHeight(200);
    _dialog->setActive();
    _dialog->setVisible();

    iWidgetGrid* grid1 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    // put all widgets in one list for easier later cleanup. this method might not always be suitable
    _allWidgets.push_back(grid1);
    grid1->appendRows(1);
    grid1->setHorizontalAlignment(iHorizontalAlignment::Strech);
    grid1->setVerticalAlignment(iVerticalAlignment::Strech);
    grid1->setBorder(10);
    grid1->setCellSpacing(5);
    grid1->setStrechRow(1);
    grid1->setStrechColumn(0);
    grid1->setSelectMode(iSelectionMode::NoSelection);

    iWidgetGroupBox* groupBox1 = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget("GroupBox"));
    _allWidgets.push_back(groupBox1);
    groupBox1->setText("Hello World. This is a group box!");
    groupBox1->setHorizontalAlignment(iHorizontalAlignment::Strech);
    groupBox1->setVerticalAlignment(iVerticalAlignment::Strech);

    // a scroll widget can contain a child that is bigger than it self. if so the scroll widget 
    // shows vertical and or horizontal sliders and allows to scroll the child.
    iWidgetScroll* widgetScoll1 = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget("Scroll"));
    _allWidgets.push_back(widgetScoll1);
    widgetScoll1->setHorizontalAlignment(iHorizontalAlignment::Strech);
    widgetScoll1->setVerticalAlignment(iVerticalAlignment::Strech);

    iWidgetScroll* widgetScoll2 = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget("Scroll"));
    _allWidgets.push_back(widgetScoll2);
    widgetScoll2->setWidth(100);
    widgetScoll2->setHorizontalAlignment(iHorizontalAlignment::Strech);
    widgetScoll2->setVerticalAlignment(iVerticalAlignment::Strech);

    iWidgetGrid* grid3 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(grid3);
    grid3->appendCollumns(3);
    grid3->appendRows(4);
    grid3->setCellSpacing(10);
    // this grid has to be top left aligned because we want to use it as child of the scroll widget
    grid3->setHorizontalAlignment(iHorizontalAlignment::Left);
    grid3->setVerticalAlignment(iVerticalAlignment::Top);
    grid3->setSelectMode(iSelectionMode::NoSelection);

    iWidgetGrid* grid4 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _allWidgets.push_back(grid4);
    grid4->appendCollumns(5);
    grid4->setStrechColumn(4);
    grid4->setHorizontalAlignment(iHorizontalAlignment::Strech);
    grid4->setVerticalAlignment(iVerticalAlignment::Top);
    grid4->setSelectMode(iSelectionMode::NoSelection);

    iWidgetLabel* label1 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(label1);
    label1->setText("This is a Label with just enough text.");

    iWidgetLabel* label2 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(label2);
    label2->setText("This is an other Label with just enough text.");

    iWidgetLabel* label3 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(label3);
    label3->setText("Top Right aligned Label");
    label3->setVerticalAlignment(iVerticalAlignment::Top);
    label3->setHorizontalAlignment(iHorizontalAlignment::Right);

    iWidgetLabel* label4 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(label4);
    label4->setText("This is a Label with a super long text so you can see the line break feature in action.");
    label4->setMaxTextWidth(200);

    iWidgetLabel* label5 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(label5);
    label5->setText("Bottom Left Aligned Label");
    label5->setVerticalAlignment(iVerticalAlignment::Bottom);
    label5->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetButton* button1 = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(button1);
    button1->setSize(70, 20);
    button1->setVerticalAlignment(iVerticalAlignment::Strech);
    button1->setHorizontalAlignment(iHorizontalAlignment::Center);
    button1->setText("Open Message Box");
	button1->setTooltip("Opens a message box");
    button1->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onOpenMessageBox));

    _color = static_cast<iWidgetColor*>(iWidgetManager::getInstance().createWidget("Color"));
    _allWidgets.push_back(_color);
    _color->setColor(iaColor4f(1, 1, 1, 0.5));
    _color->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onOpenColorChooser));

    _colorGradient = static_cast<iWidgetColorGradient*>(iWidgetManager::getInstance().createWidget("ColorGradient"));
    _allWidgets.push_back(_colorGradient);
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

    iWidgetSpacer* spacer = static_cast<iWidgetSpacer*>(iWidgetManager::getInstance().createWidget("Spacer"));
    _allWidgets.push_back(spacer);
    spacer->setSize(2, 30);

    _labelMousePos = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
    _allWidgets.push_back(_labelMousePos);

    iWidgetButton* exitButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _allWidgets.push_back(exitButton);
    exitButton->setText("");
	exitButton->setTooltip("Exists the application.");
    exitButton->setTexture("icons\\exit.png");
    exitButton->setVerticalTextAlignment(iVerticalAlignment::Bottom);
    exitButton->setVerticalAlignment(iVerticalAlignment::Center);
    exitButton->setHorizontalAlignment(iHorizontalAlignment::Center);
    exitButton->setSize(64, 64);
    exitButton->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onExitClick));

    iWidgetSelectBox* selectBox = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget("SelectBox"));
    _allWidgets.push_back(selectBox);
    selectBox->addSelectionEntry("bla");
    selectBox->addSelectionEntry("blub haha bla blub");
    selectBox->addSelectionEntry("haha");
    selectBox->setSelection(1);
    selectBox->setHorizontalAlignment(iHorizontalAlignment::Right);

    iWidgetNumberChooser* numberChooser = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
    _allWidgets.push_back(numberChooser);
    numberChooser->setPostFix("%");
    numberChooser->setValue(50);
    numberChooser->setHorizontalAlignment(iHorizontalAlignment::Right);

    iWidgetTextEdit* textEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
    _allWidgets.push_back(textEdit);
	textEdit->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    textEdit->setText("bla");
    textEdit->setMaxTextLength(20);

    iWidgetPicture* picture1 = static_cast<iWidgetPicture*>(iWidgetManager::getInstance().createWidget("Picture"));
    _allWidgets.push_back(picture1);
    picture1->setTexture("OpenGL-Logo.jpg");
    picture1->setMaxSize(200, 64);
    picture1->setVerticalAlignment(iVerticalAlignment::Top);
    picture1->setHorizontalAlignment(iHorizontalAlignment::Left);

    // if check boxes are supposed to be connected as radio buttons tell the widget manager by starting a radio button group
    iWidgetCheckBox::beginRadioButtonGroup();
    iWidgetCheckBox* radio1 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _allWidgets.push_back(radio1);
    radio1->setHorizontalAlignment(iHorizontalAlignment::Left);
    radio1->setText("radio1");

    iWidgetCheckBox* radio2 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _allWidgets.push_back(radio2);
    radio2->setText("radio2");
    radio2->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetCheckBox* radio3 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _allWidgets.push_back(radio3);
    radio3->setText("radio3");
    radio3->setHorizontalAlignment(iHorizontalAlignment::Left);
    // finish the radio button group
    iWidgetCheckBox::endRadioButtonGroup();

    iWidgetGraph* graph = static_cast<iWidgetGraph*>(iWidgetManager::getInstance().createWidget("Graph"));
    _allWidgets.push_back(graph);
    graph->setSize(300, 50);

    vector<iaVector2f> points;
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

    vector<iaVector2f> points2;
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

    vector<iaVector2f> points3;
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
    _dialog->addWidget(grid1);

    grid1->addWidget(groupBox1, 0, 0);
    groupBox1->addWidget(grid4);
    grid4->addWidget(exitButton, 0, 0);
    grid4->addWidget(spacer, 1, 0);
    grid4->addWidget(picture1, 2, 0);
    grid4->addWidget(_color, 3, 0);
    grid4->addWidget(_colorGradient, 4, 0);
    grid4->addWidget(graph, 5, 0);

    grid1->addWidget(widgetScoll1, 0, 1);
    widgetScoll1->addWidget(grid3);

    grid3->addWidget(label1, 0, 0);
    grid3->addWidget(_labelMousePos, 1, 0);
    grid3->addWidget(textEdit, 0, 1);
    grid3->addWidget(label2, 1, 1);
    grid3->addWidget(label3, 1, 2);
    grid3->addWidget(label4, 2, 2);
    grid3->addWidget(label5, 0, 2);
    grid3->addWidget(button1, 0, 3);
    grid3->addWidget(selectBox, 3, 2);
    grid3->addWidget(numberChooser, 3, 1);
    grid3->addWidget(radio1, 0, 4);
    grid3->addWidget(radio2, 1, 4);
    grid3->addWidget(radio3, 2, 4);
}

void WidgetsExample::deinit()
{
    _igorLogo = nullptr;

    iMouse::getInstance().unregisterMouseMoveDelegate(iMouseMoveDelegate(this, &WidgetsExample::onMouseMove));

    deinitGUI();

    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTextureAndBlending);

    if (_font)
    {
        delete _font;
        _font = nullptr;
    }

    _viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &WidgetsExample::onRender));

    _window.close();
    _window.removeView(&_viewOrtho);
}

void WidgetsExample::deinitGUI()
{
    if (_widgetDefaultTheme)
    {
        delete _widgetDefaultTheme;
    }

    for (auto widget : _allWidgets)
    {
        iWidgetManager::getInstance().destroyWidget(widget);
    }

    if (_dialog != nullptr)
    {
        iWidgetManager::getInstance().destroyDialog(_dialog);
        _dialog = nullptr;
    }

    if (_messageBox != nullptr)
    {
        iWidgetManager::getInstance().destroyDialog(_messageBox);
        _messageBox = nullptr;
    }

    if (_colorChooserDialog != nullptr)
    {
        iWidgetManager::getInstance().destroyDialog(_colorChooserDialog);
        _colorChooserDialog = nullptr;
    }

    _color = nullptr;
}

void WidgetsExample::onWindowResize(int32 clientWidth, int32 clientHeight)
{
    // update the widget managers desktop dimensions
    iWidgetManager::getInstance().setDesktopDimensions(_window.getClientWidth(), _window.getClientHeight());

    // update the the view port projection matrix so the widget manager desktop will fit on screen
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(_window.getClientWidth()), static_cast<float32>(_window.getClientHeight()), 0.0f);
}

void WidgetsExample::onMouseMove(const iaVector2i& pos)
{
    // updates a label with current mouse position
    if (_labelMousePos != nullptr)
    {
        iaString text;
        text += iaString::itoa(pos._x);
        text += ":";
        text += iaString::itoa(pos._y);

        _labelMousePos->setText(text);
    }
}

void WidgetsExample::onOpenColorChooser(iWidget* source)
{
    if (_colorChooserDialog == nullptr)
    {
        _colorChooserDialog = static_cast<iDialogColorChooser*>(iWidgetManager::getInstance().createDialog("DialogColorChooser"));
    }

    _colorChooserDialog->show(iColorChooserCloseDelegate(this, &WidgetsExample::onCloseColorChooser), _color->getColor(), true);
}

void WidgetsExample::onOpenColorGradientEditor(iWidget* source)
{
    if (_colorGradientDialog == nullptr)
    {
        _colorGradientDialog = static_cast<iDialogColorGradient*>(iWidgetManager::getInstance().createDialog("DialogColorGradient"));
    }

    _colorGradientDialog->show(iColorGradientCloseDelegate(this, &WidgetsExample::onCloseColorGradient), _colorGradient->getGradient(), false);
}


void WidgetsExample::onCloseColorGradient(bool ok, const iaGradientColor4f& gradient)
{
    if (ok)
    {
        _colorGradient->setGradient(gradient);
    }
}


void WidgetsExample::onCloseColorChooser(bool ok, const iaColor4f& color)
{
    if (ok)
    {
        _color->setColor(color);
    }
}

void WidgetsExample::onOpenMessageBox(iWidget* source)
{
    // create message box instance on demant
    if (_messageBox == nullptr)
    {
        _messageBox = static_cast<iDialogMessageBox*>(iWidgetManager::getInstance().createDialog("DialogMessageBox"));
    }

    // open a message box with some text
    _messageBox->show("Please click Yes No or Cancel. Nothing will happen in an case.", iMessageBoxButtons::YesNoCancel);
}

void WidgetsExample::onExitClick(iWidget* source)
{
    // shut down application
    iApplication::getInstance().stop();
}

void WidgetsExample::onWindowClosed()
{
    // shut down application
    // closing the window alone will not shut down the application 
    // because it's a console application and the windows come on top
    iApplication::getInstance().stop();
}

void WidgetsExample::onRender()
{
    // initialize view matrix with identity matrix
    iaMatrixd identity;
    iRenderer::getInstance().setViewMatrix(identity);

    // move scene between near and far plane so be ca actually see what we render
    // any value between near and far plane would do the trick
    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    // tell the widget manager to draw the widgets
    iWidgetManager::getInstance().draw();

    // draw Igor Logo
    drawLogo();

    // draw some render statistics
    _statisticsVisualizer.drawStatistics(&_window, _font, iaColor4f(0, 1, 0, 1));
}

void WidgetsExample::drawLogo()
{
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    float32 width = static_cast<float32>(_igorLogo->getWidth());
    float32 height = static_cast<float32>(_igorLogo->getHeight());
    float32 x = static_cast<float32>(_window.getClientWidth()) - width;
    float32 y = static_cast<float32>(_window.getClientHeight()) - height;

    iRenderer::getInstance().drawTexture(x, y, width, height, _igorLogo);
}

void WidgetsExample::run()
{
    // call application main loop. will not stop until application was shut down
    iApplication::getInstance().run();
}
