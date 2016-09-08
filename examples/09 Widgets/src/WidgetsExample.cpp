#include "WidgetsExample.h"

#include <iaConsole.h>
using namespace IgorAux;

#include <iMaterial.h>
#include <iMaterialGroup.h>
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

#include <sstream>
using namespace std;

WidgetsExample::WidgetsExample()
{
	init();
}

WidgetsExample::~WidgetsExample()
{
	deinit();
}

void WidgetsExample::onWindowResize(int32 clientWidth, int32 clientHeight)
{
    iWidgetManager::getInstance().setDesktopDimensions(_window.getClientWidth(), _window.getClientHeight());
    _viewOrtho.setOrthogonal(0.0f, _window.getClientWidth(), _window.getClientHeight(), 0.0f);
}

void WidgetsExample::init()
{
    con(" -- Widget Example --" << endl);

    _viewOrtho.setClearColor(true);
    _viewOrtho.setClearDepth(true);
    _viewOrtho.registerRenderDelegate(RenderDelegate(this, &WidgetsExample::render));

    _window.addView(&_viewOrtho);
    _window.setSize(1024, 768);
    _window.registerWindowCloseDelegate(WindowCloseDelegate(this, &WidgetsExample::onWindowClosed));
    _window.registerWindowResizeDelegate(WindowResizeDelegate(this, &WidgetsExample::onWindowResize));
    _window.open();
    // set up font
    _font = new iTextureFont("StandardFont.png");

    _fontMaterial = iMaterialResourceFactory::getInstance().createMaterial("TextureAndBlending");
    iMaterialResourceFactory::getInstance().getMaterial(_fontMaterial)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_fontMaterial)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_fontMaterial)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _modelViewOrtho.translate(iaVector3f(0, 0, -30));

    initGUI();

	iMouse::getInstance().registerMouseMoveDelegate(iMouseMoveDelegate(this, &WidgetsExample::onMouseMove));
}

void WidgetsExample::initGUI()
{
	_widgetDefaultTheme = new iWidgetDefaultTheme("StandardFont.png", "WidgetThemePattern.png");
	iWidgetManager::getInstance().setTheme(_widgetDefaultTheme);
	iWidgetManager::getInstance().setDesktopDimensions(_window.getClientWidth(), _window.getClientHeight());

	_dialog1 = static_cast<iWidgetDialog*>(iWidgetManager::getInstance().createWidget(iWidgetType::Dialog));
	_allWidgets.push_back(_dialog1);
    _dialog1->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _dialog1->setVerticalAlignment(iVerticalAlignment::Center);
    _dialog1->setHeight(200);
    _dialog1->setActive();
    _dialog1->setVisible();

    _grid1 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_grid1);
    _grid1->appendRows(2);
    _grid1->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _grid1->setVerticalAlignment(iVerticalAlignment::Strech);
    _grid1->setBorder(10);
    _grid1->setCellSpacing(5);
    _grid1->setSelectMode(iSelectionMode::NoSelection);

    _groupBox1 = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::GroupBox));
    _allWidgets.push_back(_groupBox1);
    _groupBox1->setText("Hello World. This is a group box!");
    _groupBox1->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _groupBox1->setVerticalAlignment(iVerticalAlignment::Top);
    _groupBox1->setHeight(100);

    _grid2 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_grid2);
    _grid2->appendCollumns(1);
    _grid2->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _grid2->setVerticalAlignment(iVerticalAlignment::Strech);
    _grid2->setSelectMode(iSelectionMode::NoSelection);

    _widgetScoll = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget(iWidgetType::Scroll));
    _allWidgets.push_back(_widgetScoll);
    _widgetScoll->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _widgetScoll->setVerticalAlignment(iVerticalAlignment::Top);
    _widgetScoll->setHeight(100);

    _grid3 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_grid2);
    _grid3->appendCollumns(3);
    _grid3->appendRows(4);
    _grid3->setCellSpacing(10);
    _grid3->setHeight(100);
    _grid3->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _grid3->setVerticalAlignment(iVerticalAlignment::Top);
    _grid3->setSelectMode(iSelectionMode::NoSelection);

    _grid4 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_grid4);
    _grid4->appendCollumns(3);
    _grid4->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _grid4->setVerticalAlignment(iVerticalAlignment::Strech);
    _grid4->setSelectMode(iSelectionMode::NoSelection);

    _label1 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_label1);
    _label1->setText("This is a Label with just enough text.");

    _label2 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_label2);
    _label2->setText("This is an other Label with just enough text.");

    _label3 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_label3);
    _label3->setText("Top Right aligned Label");
    _label3->setVerticalAlignment(iVerticalAlignment::Top);
    _label3->setHorrizontalAlignment(iHorrizontalAlignment::Right);

    _label4 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_label4);
    _label4->setText("This is a Label with a super long text so you can see the line break feature in action.");
    _label4->setMaxTextWidth(200);

    _label5 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_label5);
    _label5->setText("Bottom Left Aligned Label");
    _label5->setVerticalAlignment(iVerticalAlignment::Bottom);
    _label5->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _button1 = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allWidgets.push_back(_button1);
    _button1->setWidth(70);
    _button1->setHeight(20);
    _button1->setText("Open Message Box");
    _button1->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onMessageBox));

    _labelMousePos = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
    _allWidgets.push_back(_labelMousePos);

    _button2 = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _button2->setText("");
    _button2->setTexture("icons\\exit.png");
    _button2->setVerticalTextAlignment(iVerticalAlignment::Bottom);
    _button2->setVerticalAlignment(iVerticalAlignment::Center);
    _button2->setHorrizontalAlignment(iHorrizontalAlignment::Center);
    _button2->setWidth(64);
    _button2->setHeight(64);
    _button2->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onExitClick));

    _selectBox = static_cast<iWidgetSelectBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::SelectBox));
    _allWidgets.push_back(_selectBox);
    _selectBox->addSelectionEntry("bla");
    _selectBox->addSelectionEntry("blub haha bla blub");
    _selectBox->addSelectionEntry("haha");
    _selectBox->setSelection(1);
    _selectBox->setHorrizontalAlignment(iHorrizontalAlignment::Right);

    _numberChooser = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget(iWidgetType::NumberChooser));
    _allWidgets.push_back(_numberChooser);
    _numberChooser->setPostFix("%");
    _numberChooser->setValue(50);
    _numberChooser->setHorrizontalAlignment(iHorrizontalAlignment::Right);

    _textEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
    _textEdit->setText("bla");
    _textEdit->setMaxTextLength(20);

    _picture = static_cast<iWidgetPicture*>(iWidgetManager::getInstance().createWidget(iWidgetType::Picture));
    _picture->setTexture("OpenGL-Logo.jpg");

    iWidgetCheckBox::beginRadioButtonGroup();
    _radio1 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
    _radio1->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _radio1->setText("radio1");

    _radio2 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
    _radio2->setText("radio2");
    _radio2->setHorrizontalAlignment(iHorrizontalAlignment::Left);

    _radio3 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
    _radio3->setText("radio3");
    _radio3->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    iWidgetCheckBox::endRadioButtonGroup();

    _messageBox = new iDialogMessageBox();

    _dialog1->addWidget(_grid1);

    _grid1->addWidget(_groupBox1, 0, 0);
    _grid1->addWidget(_grid2, 0, 1);

    _grid2->addWidget(_widgetScoll, 0, 0);

    _widgetScoll->addWidget(_grid3);

    _grid3->addWidget(_label1, 0, 0);
    _grid3->addWidget(_labelMousePos, 1, 0);
    _grid3->addWidget(_textEdit, 0, 1);
    _grid3->addWidget(_label2, 1, 1);
    _grid3->addWidget(_label3, 1, 2);
    _grid3->addWidget(_label4, 2, 2);
    _grid3->addWidget(_label5, 0, 2);
    _grid3->addWidget(_button1, 0, 3);
    _grid3->addWidget(_selectBox, 3, 2);
    _grid3->addWidget(_numberChooser, 3, 1);
    _grid3->addWidget(_radio1, 0, 4);
    _grid3->addWidget(_radio2, 1, 4);
    _grid3->addWidget(_radio3, 2, 4);

    _groupBox1->addWidget(_grid4);

    _grid4->addWidget(_button2, 0, 0);
    _grid4->addWidget(_picture, 3, 0);
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
    }
}

void WidgetsExample::deinit()
{
	iMouse::getInstance().unregisterMouseMoveDelegate(iMouseMoveDelegate(this, &WidgetsExample::onMouseMove));

    deinitGUI();

    iMaterialResourceFactory::getInstance().destroyMaterial(_fontMaterial);

	_viewOrtho.unregisterRenderDelegate(RenderDelegate(this, &WidgetsExample::render));

	_window.close();
	_window.removeView(&_viewOrtho);

	if (_font)
	{
		delete _font;
	}
}

void WidgetsExample::onMouseMove(int32 x, int32 y)
{
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
	if (_messageBox != nullptr)
	{
		_messageBox->show("Please click Yes No or Cancel. Nothing will happen in an case.", iMessageBoxButtons::YesNoCancel);
	}
}

void WidgetsExample::onExitClick(iWidget* source)
{
    iApplication::getInstance().stop();
}

void WidgetsExample::onWindowClosed()
{
	con_endl("window was closed");
	iApplication::getInstance().stop();
}

void WidgetsExample::render()
{
    iaMatrixf identity;
    iRenderer::getInstance().setViewMatrix(identity);
    iRenderer::getInstance().setModelMatrix(_modelViewOrtho);

    iWidgetManager::getInstance().draw();

    drawFPS();
}

void WidgetsExample::drawFPS()
{
    iMaterialResourceFactory::getInstance().setMaterial(_fontMaterial);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    iRenderer::getInstance().setFont(_font);
    iRenderer::getInstance().setFontSize(15.0f);

    stringstream stream;
    stream << "fps: " << iTimer::getInstance().getFPS();
    iRenderer::getInstance().drawString(0, 0, stream.str().c_str());
}

void WidgetsExample::run()
{  
	iApplication::getInstance().run();
}

