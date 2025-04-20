// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialogColorGradient.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/layouts/iWidgetGridLayout.h>
#include <igor/ui/widgets/iWidgetColorGradient.h>
#include <igor/ui/widgets/iWidgetColor.h>
#include <igor/ui/widgets/iWidgetGroupBox.h>
#include <igor/ui/widgets/iWidgetNumberChooser.h>
#include <igor/ui/user_controls/iUserControlColor.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	iDialogColorGradient::iDialogColorGradient(const iWidgetPtr parent)
		: iDialog(iWidgetType::iDialogColorGradient, parent)
	{
		initUI();
	}

	void iDialogColorGradient::open(iDialogCloseDelegate dialogCloseDelegate, bool modal)
	{
		iDialog::open(dialogCloseDelegate, modal);

		setEnabled();
		setVisible();
	}

	void iDialogColorGradient::setColorGradient(const iaKeyFrameGraphColor4f &gradient)
	{
		con_assert(_gradient.isEmpty(), "invalid parameter");

		_oldGradient = gradient;
		_gradient = gradient;
		update();
	}

	void iDialogColorGradient::setUseAlpha(bool useAlpha)
	{
		_useAlpha = useAlpha;
	}

	bool iDialogColorGradient::isUsingAlpha() const
	{
		return _useAlpha;
	}

	const iaKeyFrameGraphColor4f &iDialogColorGradient::getColorGradient() const
	{
		return _gradient;
	}

	const iaKeyFrameGraphColor4f &iDialogColorGradient::getResetColorGradient() const
	{
		return _oldGradient;
	}

	void iDialogColorGradient::update()
	{
		if (_gradient.isEmpty())
		{
			con_err("invalid data");
			return;
		}

		_gradientWidget->setGradient(_gradient);
		_gradientWidget->setUseAlpha(_useAlpha);

		_colorChooser->setColor(_gradient.getValues()[0].second);
		_colorChooser->setMode(_useAlpha ? iColorChooserMode::RGBA : iColorChooserMode::RGB);

		_position->setValue(_gradient.getValues()[0].first * 100.0f);
	}

	void iDialogColorGradient::initUI()
	{
		setTitle("Edit Color Gradient");
		setVerticalAlignment(iVerticalAlignment::Center);
		setHorizontalAlignment(iHorizontalAlignment::Center);

		setMinWidth(350);
		setMinHeight(100);

		iWidgetGridLayoutPtr grid = new iWidgetGridLayout(this);
		grid->appendRows(3);
		grid->setStretchRow(2);
		grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
		grid->setVerticalAlignment(iVerticalAlignment::Stretch);
		grid->setCellSpacing(4);
		grid->setBorder(4);

		iWidgetGroupBox *groupBoxGradient = new iWidgetGroupBox();
		groupBoxGradient->setText("Gradient");
		groupBoxGradient->setHorizontalAlignment(iHorizontalAlignment::Stretch);
		groupBoxGradient->setVerticalAlignment(iVerticalAlignment::Stretch);
		grid->addWidget(groupBoxGradient, 0, 0);

		iWidgetGridLayout *buttonGrid = new iWidgetGridLayout();
		buttonGrid->appendColumns(2);
		buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);
		grid->addWidget(buttonGrid, 0, 2);

		_gradientWidget = new iWidgetColorGradient();
		_gradientWidget->setHorizontalAlignment(iHorizontalAlignment::Stretch);
		_gradientWidget->setVerticalAlignment(iVerticalAlignment::Stretch);
		_gradientWidget->setInteractive();
		_gradientWidget->setMinHeight(60);
		_gradientWidget->registerOnSelectionChangedEvent(iSelectionChangedDelegate(this, &iDialogColorGradient::onSelectionChanged));
		_gradientWidget->registerOnColorCreatedEvent(iColorGradientColorCreatedDelegate(this, &iDialogColorGradient::onColorCreated));
		groupBoxGradient->addWidget(_gradientWidget);

		iWidgetGroupBox *groupBox = new iWidgetGroupBox();
		groupBox->setText("Selected Color");
		groupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
		groupBox->setVerticalAlignment(iVerticalAlignment::Stretch);
		grid->addWidget(groupBox, 0, 1);

		iWidgetGridLayout *controlGrid = new iWidgetGridLayout();
		controlGrid->appendRows(3);
		controlGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
		groupBox->addWidget(controlGrid);

		_colorChooser = new iUserControlColor();
		_colorChooser->setExpand();
		_colorChooser->setHeadlineVisible(false);
		_colorChooser->setHorizontalAlignment(iHorizontalAlignment::Stretch);
		_colorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &iDialogColorGradient::onColorChanged));
		controlGrid->addWidget(_colorChooser, 0, 0);

		iWidgetGridLayout *positionGrid = new iWidgetGridLayout();
		positionGrid->appendColumns(1);
		positionGrid->setStretchColumn(1);
		positionGrid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
		controlGrid->addWidget(positionGrid, 0, 1);

		iWidgetButton *delButton = new iWidgetButton();
		delButton->setText("Delete Color");
		delButton->setTooltip("Deletes selected color key.");
		delButton->getClickEvent().add(iClickDelegate(this, &iDialogColorGradient::onDelete));
		delButton->setHorizontalAlignment(iHorizontalAlignment::Right);
		controlGrid->addWidget(delButton, 0, 2);

		iWidgetLabel *labelPosition = new iWidgetLabel();
		labelPosition->setText("Position");
		labelPosition->setMinWidth(100);
		positionGrid->addWidget(labelPosition, 0, 0);

		_position = new iWidgetNumberChooser();
		_position->setMinMaxNumber(0.0f, 100.0f);
		_position->setStepping(0.1f, 0.1f);
		_position->setSteppingWheel(1.0f, 1.0f);
		_position->setAfterPoint(2);
		_position->setPostFix("%");
		_position->setHorizontalAlignment(iHorizontalAlignment::Stretch);
		_position->registerOnChangeEvent(iChangeDelegate(this, &iDialogColorGradient::onPositionChanged));
		positionGrid->addWidget(_position, 1, 0);

		iWidgetButton *okButton = new iWidgetButton();
		okButton->setText("OK");
		okButton->setTooltip("Closes the dialog and applies new color gradient.");
		okButton->getClickEvent().add(iClickDelegate(this, &iDialogColorGradient::onOK));
		buttonGrid->addWidget(okButton, 2, 0);

		iWidgetButton *cancelButton = new iWidgetButton();
		cancelButton->setText("Cancel");
		cancelButton->setTooltip("Closes the dialog without changes.");
		cancelButton->getClickEvent().add(iClickDelegate(this, &iDialogColorGradient::onCancel));
		buttonGrid->addWidget(cancelButton, 1, 0);

		iWidgetButton *resetButton = new iWidgetButton();
		resetButton->setText("Reset");
		resetButton->setTooltip("Resets dialog to previous color gradient.");
		resetButton->getClickEvent().add(iClickDelegate(this, &iDialogColorGradient::onReset));
		buttonGrid->addWidget(resetButton, 0, 0);
	}

	void iDialogColorGradient::onPositionChanged(const iWidgetPtr source)
	{
		iaKeyFrameGraphColor4f temp = _gradient;

		float32 at = _position->getValue() / 100.0f;
		iaColor4f color = _gradient.getValues()[_selectedColor].second;

		_gradient.removeIndex(_selectedColor);
		_gradient.setValue(at, color);

		int index = 0;
		auto values = _gradient.getValues();
		for (auto value : values)
		{
			if (value.first == at)
			{
				if (_selectedColor != index)
				{
					_selectedColor = index;

					updateSelection();
				}
				break;
			}

			index++;
		}

		_gradientWidget->setGradient(_gradient);
	}

	void iDialogColorGradient::onColorChanged(const iaColor4f &color)
	{
		float32 at = _gradient.getValues()[_selectedColor].first;
		_gradient.setValue(at, color);

		_gradientWidget->setGradient(_gradient);
	}

	void iDialogColorGradient::onSelectionChanged(const iWidgetPtr source)
	{
		iWidgetColorGradientPtr gradient = static_cast<iWidgetColorGradientPtr>(source);

		int32 index = gradient->getSelectedIndex();

		con_assert(index < _gradient.getValues().size(), "out of range");

		if (index < _gradient.getValues().size())
		{
			_selectedColor = index;
			updateSelection();
		}
	}

	void iDialogColorGradient::onColorCreated(float32 at, const iaColor4f &color)
	{
		_gradient.setValue(at, color);

		int index = 0;
		auto values = _gradient.getValues();
		for (auto value : values)
		{
			if (value.first == at)
			{
				if (_selectedColor != index)
				{
					_selectedColor = index;

					updateSelection();
				}
				break;
			}

			index++;
		}

		_gradientWidget->setGradient(_gradient);
	}

	void iDialogColorGradient::onOK(const iWidgetPtr source)
	{
		setReturnState(iDialogReturnState::Ok);
		close();
	}

	void iDialogColorGradient::updateSelection()
	{
		_colorChooser->setColor(_gradient.getValues()[_selectedColor].second);
		_position->setValue(_gradient.getValues()[_selectedColor].first * 100.0f);
	}

	void iDialogColorGradient::onDelete(const iWidgetPtr source)
	{
		if (_gradient.getValues().size() > 1)
		{
			_gradient.removeIndex(_selectedColor);

			_selectedColor = 0;
			updateSelection();
		}
	}

	void iDialogColorGradient::onCancel(const iWidgetPtr source)
	{
		setReturnState(iDialogReturnState::Cancel);
		close();
	}

	void iDialogColorGradient::onReset(const iWidgetPtr source)
	{
		_gradient = _oldGradient;
		_gradientWidget->setGradient(_gradient);

		_selectedColor = 0;
		updateSelection();
	}
} // namespace igor