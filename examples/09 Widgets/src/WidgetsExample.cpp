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
#include <iMessageBox.h>
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
    _viewOrtho.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
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
    _dialog1->setWidth(100);
    _dialog1->setHeight(300);
    _dialog1->setActive();
    _dialog1->setVisible();

	_groupBox1 = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::GroupBox));
	_allWidgets.push_back(_groupBox1);
	_groupBox1->setText("Hello World!");
	_groupBox1->setWidth(300);
	_groupBox1->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
	_groupBox1->setVerticalAlignment(iVerticalAlignment::Center);

	_groupBox2 = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::GroupBox));
	_allWidgets.push_back(_groupBox2);
	_groupBox2->setText("2nd Group Box");
	_groupBox2->setWidth(300);
	_groupBox2->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	_groupBox2->setVerticalAlignment(iVerticalAlignment::Top);

	_widgetScoll = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget(iWidgetType::Scroll));
	_allWidgets.push_back(_groupBox1);
	_widgetScoll->setWidth(400);
	_widgetScoll->setHeight(400);

	_grid1 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
	_allWidgets.push_back(_grid1);
	_grid1->appendCollumns(2);
	_grid1->appendRows(4);
	_grid1->setBorder(10);
	_grid1->setCellSpacing(5);
	_grid1->setHorrizontalAlignment(iHorrizontalAlignment::Center);
	_grid1->setStrechRow(2);
	_grid1->setVerticalAlignment(iVerticalAlignment::Strech);
	_grid1->setSelectMode(iSelectionMode::NoSelection);

	_label1 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
	_allWidgets.push_back(_label1);
	_label1->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	_label1->setVerticalAlignment(iVerticalAlignment::Top);
	_label1->setText("foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla ");
	_label1->setMaxTextWidth(200);

	_label2 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
	_allWidgets.push_back(_label2);
	_label2->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	_label2->setVerticalAlignment(iVerticalAlignment::Top);
	_label2->setText("foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla ");
	_label2->setMaxTextWidth(200);

	_label3 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
	_allWidgets.push_back(_label3);
	_label3->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	_label3->setVerticalAlignment(iVerticalAlignment::Top);
	_label3->setText("this label is supposed to apper in the lower right this label is supposed to apper in the lower right this label is supposed to apper in the lower right this label is supposed to apper in the lower right ");
	_label3->setMaxTextWidth(200);

	_grid2 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
	_allWidgets.push_back(_grid2);
	_grid2->appendCollumns(1);
	_grid2->appendRows(1);
	_grid2->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	_grid2->setVerticalAlignment(iVerticalAlignment::Top);
	_grid2->setSelectMode(iSelectionMode::NoSelection);

	_labelNum1 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
	_allWidgets.push_back(_labelNum1);
	_labelNum1->setText("Label");

	_labelNum2 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
	_allWidgets.push_back(_labelNum2);
	_labelNum2->setText("foo bar");

	_labelNum3 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
	_allWidgets.push_back(_labelNum3);
	_labelNum3->setText("yet another Label");

	_labelNum4 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
	_allWidgets.push_back(_labelNum4);
	_labelNum4->setText("Hello Label");

	_button1 = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allWidgets.push_back(_button1);
	_button1->setWidth(150);
	_button1->setHeight(30);
	_button1->setText("Open Message Box");
	_button1->setVerticalAlignment(iVerticalAlignment::Strech);
	_button1->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
	_button1->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onMessageBox));

	_dialog1->addWidget(_grid1);
	
	_grid1->addWidget(_label1, 0, 0);
	_grid1->addWidget(_labelNum1, 2, 1);
	_grid1->addWidget(_button1, 1, 2);
	_grid1->addWidget(_labelNum2, 0, 3);
	
	
/*	2






	_label2 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
	_label2->setText("foobar2");
	_label2->setMaxTextWidth(50);
	_grid1->addWidget(_label2, 0, 1);

	_button1 = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_button1->setText("MessageBox");
    _button1->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onMessageBox));
	_grid1->addWidget(_button1, 1, 0);
	
	_button2 = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_button2->setText("");
    _button2->setTexture("icons\\exit.png");
    _button2->setVerticalTextAlignment(iVerticalAlignment::Bottom);
    _button2->setWidth(64);
    _button2->setHeight(64);
    _button2->registerOnClickEvent(iClickDelegate(this, &WidgetsExample::onExitClick));
	_grid1->addWidget(_button2, 1, 1);

	_numberChooser = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget(iWidgetType::NumberChooser));
	_numberChooser->setPostFix("%");
    _numberChooser->setValue(50);
	_grid1->addWidget(_numberChooser, 2, 1);

	_textEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
	_textEdit->setText("bla");
	_textEdit->setMaxTextLength(20);
	_grid1->addWidget(_textEdit, 0, 2);

	_grid2 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _grid2->setSelectMode(iSelectionMode::NoSelection);
    _grid2->appendRows(3);
	iWidgetCheckBox::beginRadioButtonGroup();
	_radio1 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
	_radio1->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	_radio1->setText("radio1");
	_grid2->addWidget(_radio1, 0, 0);

	_radio2 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
	_radio2->setText("radio2");
	_radio2->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	_grid2->addWidget(_radio2, 0, 1);

	_radio3 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
	_radio3->setText("radio3");
	_radio3->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	_grid2->addWidget(_radio3, 0, 2);
	iWidgetCheckBox::endRadioButtonGroup();

	_checkBox1 = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
	_checkBox1->setText("checkBox1");
	_checkBox1->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	_grid2->addWidget(_checkBox1, 0, 3);

	_grid1->addWidget(_grid2, 2, 0);

	//_groupBox1->addWidget(_grid1);
	_groupBox1->addWidget(_widgetScoll);

    _picture = static_cast<iWidgetPicture*>(iWidgetManager::getInstance().createWidget(iWidgetType::Picture));
    _picture->setTexture("OpenGL-Logo.jpg");
    _grid1->addWidget(_picture, 1, 2);*/

    _messageBox = new iMessageBox();
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

