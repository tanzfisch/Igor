// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogColorGradient.h>

#include <iDialog.h>
#include <iWidgetManager.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetGrid.h>
#include <iWidgetSpacer.h>
#include <iWidgetCheckBox.h>
#include <iWidgetManager.h>
#include <iUserControlColorChooser.h>
#include <iWidgetColorGradient.h>
#include <iWidgetColor.h>
#include <iWidgetGroupBox.h>
#include <iWidgetNumberChooser.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	void iDialogColorGradient::show(iColorGradientCloseDelegate closeDelegate, const iaGradientColor4f& gradient, bool useAlpha)
	{
		_oldGradient = gradient;
		_closeEvent.append(closeDelegate);

		initGUI(gradient, useAlpha);
	}

	void iDialogColorGradient::deinitGUI()
	{
		if (_grid != nullptr)
		{
			removeWidget(_grid);
			iWidgetManager::getInstance().destroyWidget(_grid);
			_grid = nullptr;
		}
	}

	void iDialogColorGradient::initGUI(const iaGradientColor4f& gradient, bool useAlpha)
	{
		con_assert(gradient.getValues().size() > 0, "invalid data");

		if (gradient.getValues().size() > 0)
		{
			_gradient = gradient;

			iWidgetManager::setModal(this);
			setActive();
			setVisible();
			setWidth(350);
			setHeight(100);

			_grid = new iWidgetGrid();
			_grid->appendRows(4);
			_grid->setStrechRow(2);
			_grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
			_grid->setVerticalAlignment(iVerticalAlignment::Strech);
			_grid->setCellSpacing(4);
			_grid->setBorder(4);

			iWidgetLabel* headerLabel = new iWidgetLabel();
			headerLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
			headerLabel->setText("Edit Gradient");

			iWidgetGroupBox* groupBoxGradient = new iWidgetGroupBox();
			groupBoxGradient->setText("Gradient");
			groupBoxGradient->setHorizontalAlignment(iHorizontalAlignment::Strech);
			groupBoxGradient->setVerticalAlignment(iVerticalAlignment::Strech);

			_gradientWidget = new iWidgetColorGradient();
			_gradientWidget->setGradient(_gradient);
			_gradientWidget->setHorizontalAlignment(iHorizontalAlignment::Strech);
			_gradientWidget->setVerticalAlignment(iVerticalAlignment::Strech);
			_gradientWidget->setUseAlpha(useAlpha);
			_gradientWidget->setInteractive();
			_gradientWidget->setHeight(60);
			_gradientWidget->registerOnSelectionChangedEvent(iSelectionChangedDelegate(this, &iDialogColorGradient::onSelectionChanged));
			_gradientWidget->registerOnColorCreatedEvent(iColorGradientColorCreatedDelegate(this, &iDialogColorGradient::onColorCreated));

			iWidgetGroupBox* groupBox = new iWidgetGroupBox();
			groupBox->setText("Selected Color");
			groupBox->setHorizontalAlignment(iHorizontalAlignment::Strech);
			groupBox->setVerticalAlignment(iVerticalAlignment::Strech);

			iWidgetGrid* controlGrid = new iWidgetGrid();
			controlGrid->appendRows(3);
			controlGrid->setHorizontalAlignment(iHorizontalAlignment::Left);

			iWidgetGrid* positionGrid = new iWidgetGrid();
			positionGrid->appendCollumns(1);
			positionGrid->setStrechColumn(1);
			positionGrid->setHorizontalAlignment(iHorizontalAlignment::Strech);

			iWidgetLabel* labelPosition = new iWidgetLabel();
			labelPosition->setText("Position");
			labelPosition->setWidth(100);

			_position = new iWidgetNumberChooser();
			_position->setMinMaxNumber(0.0f, 100.0f);
			_position->setStepping(0.1f, 0.1f);
			_position->setSteppingWheel(1.0f, 1.0f);
			_position->setAfterPoint(2);
			_position->setPostFix("%");
			_position->setValue(_gradient.getValues()[0].first * 100.0f);
			_position->setHorizontalAlignment(iHorizontalAlignment::Strech);
			_position->registerOnChangeEvent(iChangeDelegate(this, &iDialogColorGradient::onPositionChanged));

			iWidgetButton* delButton = new iWidgetButton();
			delButton->setText("Delete Color");
			delButton->setTooltip("Deletes selected color key.");
			delButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorGradient::onDelete));
			delButton->setHorizontalAlignment(iHorizontalAlignment::Right);

			_colorChooser = new iUserControlColorChooser();
			_colorChooser->setMode(useAlpha ? iColorChooserMode::RGBA : iColorChooserMode::RGB);
			_colorChooser->setExpand();
			_colorChooser->setHeadlineVisible(false);
			_colorChooser->setColor(_gradient.getValues()[0].second);
			_colorChooser->setHorizontalAlignment(iHorizontalAlignment::Strech);
			_colorChooser->registerOnColorChangedEvent(iColorChangedDelegate(this, &iDialogColorGradient::onColorChanged));

			iWidgetGrid* buttonGrid = new iWidgetGrid();
			buttonGrid->appendCollumns(2);
			buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);

			iWidgetButton* okButton = new iWidgetButton();
			okButton->setText("OK");
			okButton->setTooltip("Closes the dialog and applies new color gradient.");
			okButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorGradient::onOK));

			iWidgetButton* cancelButton = new iWidgetButton();
			cancelButton->setText("Cancel");
			cancelButton->setTooltip("Closes the dialog without changes.");
			cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorGradient::onCancel));

			iWidgetButton* resetButton = new iWidgetButton();
			resetButton->setText("Reset");
			resetButton->setTooltip("Resets dialog to previous color gradient.");
			resetButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorGradient::onReset));

			addWidget(_grid);

			_grid->addWidget(headerLabel, 0, 0);
			_grid->addWidget(groupBoxGradient, 0, 1);
			_grid->addWidget(groupBox, 0, 2);
			_grid->addWidget(buttonGrid, 0, 3);

			groupBoxGradient->addWidget(_gradientWidget);

			groupBox->addWidget(controlGrid);

			controlGrid->addWidget(_colorChooser, 0, 0);
			controlGrid->addWidget(positionGrid, 0, 1);
			controlGrid->addWidget(delButton, 0, 2);

			positionGrid->addWidget(labelPosition, 0, 0);
			positionGrid->addWidget(_position, 1, 0);

			buttonGrid->addWidget(resetButton, 0, 0);
			buttonGrid->addWidget(cancelButton, 1, 0);
			buttonGrid->addWidget(okButton, 2, 0);
		}
	}

	void iDialogColorGradient::onPositionChanged(iWidget* source)
	{
		iaGradientColor4f temp = _gradient;

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

	void iDialogColorGradient::onColorChanged(const iaColor4f& color)
	{
		float32 at = _gradient.getValues()[_selectedColor].first;
		_gradient.setValue(at, color);

		_gradientWidget->setGradient(_gradient);
	}

	void iDialogColorGradient::onSelectionChanged(int index)
	{
		con_assert(index < _gradient.getValues().size(), "out of range");

		if (index < _gradient.getValues().size())
		{
			_selectedColor = index;
			updateSelection();
		}
	}

	void iDialogColorGradient::onColorCreated(float32 at, const iaColor4f& color)
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

	void iDialogColorGradient::onOK(iWidget* source)
	{
		close();

		_closeEvent(true, _gradient);
		_closeEvent.clear();
	}

	void iDialogColorGradient::updateSelection()
	{
		_colorChooser->setColor(_gradient.getValues()[_selectedColor].second);
		_position->setValue(_gradient.getValues()[_selectedColor].first * 100.0f);
	}

	void iDialogColorGradient::onDelete(iWidget* source)
	{
		if (_gradient.getValues().size() > 1)
		{
			_gradient.removeIndex(_selectedColor);

			_selectedColor = 0;
			updateSelection();
		}
	}

	void iDialogColorGradient::onCancel(iWidget* source)
	{
		close();

		_closeEvent(true, _gradient);
		_closeEvent.clear();
	}

	void iDialogColorGradient::onReset(iWidget* source)
	{
		_gradient = _oldGradient;
		_gradientWidget->setGradient(_gradient);

		_selectedColor = 0;
		updateSelection();
	}

	void iDialogColorGradient::close()
	{
		setActive(false);
		setVisible(false);
		iWidgetManager::resetModal();

		deinitGUI();
	}
}