#include "WidgetsExample.h"

#include <iaConsole.h>
using namespace IgorAux;

#include <iRenderer.h>
#include <iApplication.h>
#include <iMouse.h>
#include <iTimer.h>
#include <iTextureFont.h>
#include <iMaterialResourceFactory.h>

#include <iWidgetManager.h>
#include <iWidgetDialog.h>
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
using namespace Igor;

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
    _window.setSize(1024, 768);
    // register to window close event so we can shut down the application properly
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &WidgetsExample::onWindowClosed));
    // register to window resize event so we can notify the widget manager of that change
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &WidgetsExample::onWindowResize));
    // opens the window
    _window.open();

    // set up font for FPS display
    _font = new iTextureFont("StandardFont.png");

    // create a material for the font
    _fontMaterial = iMaterialResourceFactory::getInstance().createMaterial("TextureAndBlending");
    iMaterialResourceFactory::getInstance().getMaterial(_fontMaterial)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_fontMaterial)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_fontMaterial)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

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

    iWidgetDialog* dialog = static_cast<iWidgetDialog*>(iWidgetManager::getInstance().createWidget(iWidgetType::Dialog));
    // put all widgets in one list for easier later cleanup. this method might not always be suitable
    _allWidgets.push_back(dialog);
    dialog->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    dialog->setVerticalAlignment(iVerticalAlignment::Center);
    dialog->setHeight(200);
    dialog->setActive();
    dialog->setVisible();

    iWidgetGrid* grid1 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(grid1);
    grid1->appendRows(1);
    grid1->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    grid1->setVerticalAlignment(iVerticalAlignment::Strech);
    grid1->setBorder(10);
    grid1->setCellSpacing(5);
    grid1->setStrechRow(0);
    grid1->setStrechColumn(0);
    grid1->setSelectMode(iSelectionMode::NoSelection);

    iWidgetGroupBox* groupBox = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::GroupBox));
    _allWidgets.push_back(groupBox);
    groupBox->setText("Hello World. This is a group box!");
    groupBox->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    groupBox->setVerticalAlignment(iVerticalAlignment::Strech);
    groupBox->setHeight(30);

    // a scroll widget can contain a child that is bigger than it self. if so the scroll widget 
    // shows vertical and or horizontal sliders and allows to scroll the child.
    iWidgetScroll* widgetScoll = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget(iWidgetType::Scroll));
    _allWidgets.push_back(widgetScoll);
    widgetScoll->setHeight(100);
    widgetScoll->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    widgetScoll->setVerticalAlignment(iVerticalAlignment::Strech);

    iWidgetGrid* grid3 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(grid3);
    grid3->appendCollumns(3);
    grid3->appendRows(4);
    grid3->setCellSpacing(10);
    // this grid has to be top left aligned because we want to use it as child of the scroll widget
    grid3->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    grid3->setVerticalAlignment(iVerticalAlignment::Top);
    grid3->setSelectMode(iSelectionMode::NoSelection);

    iWidgetGrid* grid4 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(grid4);
    grid4->appendCollumns(3);
    grid4->setStrechColumn(1);
    grid4->setStrechRow(0);
    grid4->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    grid4->setVerticalAlignment(iVerticalAlignment::Strech);
    grid4->setSelectMode(iSelectionMode::NoSelection);

    iWidgetLabel* label1 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(label1);
    label1->setText("This is a Label with just enough text.");

    iWidgetLabel* label2 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(label2);
    label2->setText("This is an other Label with just enough text.");

    iWidgetLabel* label3 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(label3);
    label3->setText("Top Right aligned Label");
    label3->setVerticalAlignment(iVerticalAlignment::Top);
    label3->setHorrizontalAlignment(iHorrizontalAlignment::Right);

    iWidgetLabel* label4 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(label4);
    label4->setText("This is a Label with a super long text so you can see the line break feature in action.");
    label4->setMaxTextWidth(200);

    iWidgetLabel* label5 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(label5);
    label5->setText("Bottom Left Aligned Label");
    label5->setVerticalAlignment(iVerticalAlignment::Bottom);
    label5->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    iWidgetButton* button1 = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allWidgets.push_back(button1);
    button1->setWidth(70);
    button1->setHeight(20);
    button1->setVerticalAlignment(iVerticalAlignment::Strech);
    button1->setHorrizontalAlignment(iHorrizontalAlignment::Center);
    button1->setText("Open Message Box");
    button1->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onMessageBox));

    _labelMousePos = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelMousePos);

    iWidgetButton* button2 = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allWidgets.push_back(button2);
    button2->setText("");
    button2->setTexture("icons\\exit.png");
    button2->setVerticalTextAlignment(iVerticalAlignment::Bottom);
    button2->setVerticalAlignment(iVerticalAlignment::Center);
    button2->setHorrizontalAlignment(iHorrizontalAlignment::Center);
    button2->setWidth(64);
    button2->setHeight(64);
    button2->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onExitClick));

    iWidgetSelectBox* selectBox = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::SelectBox));
    _allWidgets.push_back(selectBox);
    selectBox->addSelectionEntry("bla");
    selectBox->addSelectionEntry("blub haha bla blub");
    selectBox->addSelectionEntry("haha");
    selectBox->setSelection(1);
    selectBox->setHorrizontalAlignment(iHorrizontalAlignment::Right);

    iWidgetNumberChooser* numberChooser = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget(iWidgetType::NumberChooser));
    _allWidgets.push_back(numberChooser);
    numberChooser->setPostFix("%");
    numberChooser->setValue(50);
    numberChooser->setHorrizontalAlignment(iHorrizontalAlignment::Right);

    iWidgetTextEdit* textEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _allWidgets.push_back(textEdit);
    textEdit->setText("bla");
    textEdit->setMaxTextLength(20);

    iWidgetPicture* picture = static_cast<iWidgetPicture*>(iWidgetManager::getInstance().createWidget(iWidgetType::Picture));
    _allWidgets.push_back(picture);
    picture->setTexture("OpenGL-Logo.jpg");
    picture->setMaxSize(200, 64);
    picture->setVerticalAlignment(iVerticalAlignment::Top);
    picture->setHorrizontalAlignment(iHorrizontalAlignment::Right);

    // if check boxes are supposed to be connected as radio buttons tell the widget manager by starting a radio button group
    iWidgetCheckBox::beginRadioButtonGroup();
    iWidgetCheckBox* radio1 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
    _allWidgets.push_back(radio1);
    radio1->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    radio1->setText("radio1");

    iWidgetCheckBox* radio2 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
    _allWidgets.push_back(radio2);
    radio2->setText("radio2");
    radio2->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    iWidgetCheckBox* radio3 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
    _allWidgets.push_back(radio3);
    radio3->setText("radio3");
    radio3->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    // finish the radio button group
    iWidgetCheckBox::endRadioButtonGroup();

    // assemble all the widgets with their parents
    dialog->addWidget(grid1);

    grid1->addWidget(groupBox, 0, 0);

    groupBox->addWidget(grid4);

    grid4->addWidget(button2, 0, 0);
    grid4->addWidget(picture, 3, 0);

    grid1->addWidget(widgetScoll, 0, 1);

    widgetScoll->addWidget(grid3);

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
    iMouse::getInstance().unregisterMouseMoveDelegate(iMouseMoveDelegate(this, &WidgetsExample::onMouseMove));

    deinitGUI();

    iMaterialResourceFactory::getInstance().destroyMaterial(_fontMaterial);

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

    if (_messageBox != nullptr)
    {
        delete _messageBox;
        _messageBox = nullptr;
    }
}

void WidgetsExample::onWindowResize(int32 clientWidth, int32 clientHeight)
{
    // update the widget managers desktop dimensions
    iWidgetManager::getInstance().setDesktopDimensions(_window.getClientWidth(), _window.getClientHeight());

    // update the the view port projection matrix so the widget manager desktop will fit on screen
    _viewOrtho.setOrthogonal(0.0f, _window.getClientWidth(), _window.getClientHeight(), 0.0f);
}

void WidgetsExample::onMouseMove(int32 x, int32 y)
{
    // updates a label with current mouse position
	if (_labelMousePos != nullptr)
	{
		iaString text;
		text += iaString::itoa(x);
		text += ":";
		text += iaString::itoa(y);

		_labelMousePos->setText(text);
	}
}

void WidgetsExample::onMessageBox(iWidget* source)
{
    // create message box instance on demant
    if (_messageBox == nullptr)
    {
        _messageBox = new iDialogMessageBox();
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
    iaMatrixf identity;
    iRenderer::getInstance().setViewMatrix(identity);

    // move scene between near and far plane so be ca actually see what we render
    // any value between near and far plane would do the trick
    iaMatrixf modelViewOrtho;
    modelViewOrtho.translate(iaVector3f(0, 0, -30)); 
    iRenderer::getInstance().setModelMatrix(modelViewOrtho);

    // tell the widget manager to draw the widgets
    iWidgetManager::getInstance().draw();

    // draw framerate
    drawFPS();
}

void WidgetsExample::drawFPS()
{
    // set the texture font material to render text
    iMaterialResourceFactory::getInstance().setMaterial(_fontMaterial);

    // set the color to render
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    // set the font to use
    iRenderer::getInstance().setFont(_font);

    // set the font size
    iRenderer::getInstance().setFontSize(15.0f);

    // assemble some text with framerate
    iaString fps = "fps ";
    fps += iaString::ftoa(iTimer::getInstance().getFPS(), 2);

    // render framerate
    iRenderer::getInstance().drawString(0, 0, fps);
}

void WidgetsExample::run()
{  
    // call application main loop. will not stop until application was shut down
	iApplication::getInstance().run();
}
