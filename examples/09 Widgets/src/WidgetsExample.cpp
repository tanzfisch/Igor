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
    con(" -- OpenGL 3D Test --" << endl);

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
    _dialog1->setHorrizontalAlignment(iHorrizontalAlignment::Center);
    _dialog1->setVerticalAlignment(iVerticalAlignment::Center);
    _dialog1->setWidth(100);
    _dialog1->setHeight(100);
    _dialog1->setActive();
    _dialog1->setVisible();

	_grid1 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _grid1->appendCollumns(2);
    _grid1->appendRows(2);
    _grid1->setSelectMode(iSelectionMode::NoSelection);

	_widgetScoll = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget(iWidgetType::Scroll));
	_widgetScoll->setWidth(200);
	_widgetScoll->setHeight(400);
    _grid1->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _grid1->setVerticalAlignment(iVerticalAlignment::Top);
	_widgetScoll->addWidget(_grid1);

	_label1 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
	_label1->setText("foobar blub bla foobar blub bla foobar blub bla foobar blub bla foobar blub bla ");
	_label1->setMaxTextWidth(150);
	_grid1->addWidget(_label1, 0, 0);

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

	_groupBox1 = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::GroupBox));
	_groupBox1->setText("HALLO WELT");
	_dialog1->addWidget(_groupBox1);
	//_groupBox1->addWidget(_grid1);
	_groupBox1->addWidget(_widgetScoll);

    _picture = static_cast<iWidgetPicture*>(iWidgetManager::getInstance().createWidget(iWidgetType::Picture));
    _picture->setTexture("OpenGL-Logo.jpg");
    _grid1->addWidget(_picture, 1, 2);

    _messageBox = new iMessageBox();
}

void WidgetsExample::deinitGUI()
{
	if (_widgetDefaultTheme)
	{
		delete _widgetDefaultTheme;
	}

	iWidgetManager::getInstance().destroyWidget(_dialog1);
	iWidgetManager::getInstance().destroyWidget(_label1);
	iWidgetManager::getInstance().destroyWidget(_label2);
	iWidgetManager::getInstance().destroyWidget(_button1);
	iWidgetManager::getInstance().destroyWidget(_button2);
	iWidgetManager::getInstance().destroyWidget(_groupBox1);

	iWidgetManager::getInstance().destroyWidget(_grid1);
	iWidgetManager::getInstance().destroyWidget(_grid2);
	iWidgetManager::getInstance().destroyWidget(_radio1);
	iWidgetManager::getInstance().destroyWidget(_radio2);
	iWidgetManager::getInstance().destroyWidget(_radio3);
	iWidgetManager::getInstance().destroyWidget(_checkBox1);
	iWidgetManager::getInstance().destroyWidget(_numberChooser);
	iWidgetManager::getInstance().destroyWidget(_textEdit);
    iWidgetManager::getInstance().destroyWidget(_picture);
	iWidgetManager::getInstance().destroyWidget(_widgetScoll);

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
    _messageBox->show("bla bla arfg aeg aeg arth aethg", iMessageBoxButtons::YesNoCancel);
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
