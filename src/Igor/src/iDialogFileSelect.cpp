// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogFileSelect.h>

#include <iDialog.h>
#include <iWidgetManager.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetGrid.h>
#include <iWidgetSpacer.h>
#include <iWidgetTextEdit.h>
#include <iWidgetScroll.h>
#include <iWidgetPicture.h>

#include <iaConsole.h>
#include <iaFile.h>
#include <iaDirectory.h>
using namespace IgorAux;

namespace Igor
{

	iDialogFileSelect::iDialogFileSelect()
	{
		initGUI();
	}

	iDialogFileSelect::~iDialogFileSelect()
	{
		deinitGUI();
	}

	iDialog* iDialogFileSelect::createInstance()
	{
		return new iDialogFileSelect();
	}

	void iDialogFileSelect::initGUI()
	{
		_grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
		_grid->setBorder(4);
		_grid->setCellSpacing(4);
		_grid->appendRows(4);
		addWidget(_grid);

		_headerLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
		_headerLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
		_grid->addWidget(_headerLabel, 0, 0);

		_pathEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
		_pathEdit->setWidth(600);
		_pathEdit->setWriteProtected(false);
		_pathEdit->setChangeEventOnEnterAndLosFocus();
		_pathEdit->setMaxTextLength(1024);
		_pathEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
		_pathEdit->setVerticalAlignment(iVerticalAlignment::Top);
		_pathEdit->registerOnChangeEvent(iChangeDelegate(this, &iDialogFileSelect::onPathEditChange));
		_grid->addWidget(_pathEdit, 0, 1);

		_scroll = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget("Scroll"));
		_scroll->setWidth(600);
		_scroll->setHeight(300);
		_grid->addWidget(_scroll, 0, 2);

		_fileGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
		_fileGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
		_fileGrid->setVerticalAlignment(iVerticalAlignment::Top);
		_fileGrid->setSelectMode(iSelectionMode::Field);
		_fileGrid->registerOnDoubleClickEvent(iDoubleClickDelegate(this, &iDialogFileSelect::onDoubleClick));
		_scroll->addWidget(_fileGrid);

		_filenameGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
		_filenameGrid->setHorizontalAlignment(iHorizontalAlignment::Right);
		_filenameGrid->setVerticalAlignment(iVerticalAlignment::Bottom);
		_filenameGrid->appendCollumns(1);
		_filenameGrid->setCellSpacing(4);
		_grid->addWidget(_filenameGrid, 0, 3);

		_filenameLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
		_filenameLabel->setHorizontalAlignment(iHorizontalAlignment::Right);
		_filenameLabel->setText("File name:");
		_filenameGrid->addWidget(_filenameLabel, 0, 0);

		_filenameEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
		_filenameEdit->setWidth(300);
		_filenameEdit->setWriteProtected(false);
		_filenameEdit->setChangeEventOnEnterAndLosFocus();
		_filenameEdit->setMaxTextLength(256);
		_filenameEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
		_filenameEdit->setVerticalAlignment(iVerticalAlignment::Top);
		_filenameEdit->registerOnChangeEvent(iChangeDelegate(this, &iDialogFileSelect::onFilenameEditChange));
		_filenameGrid->addWidget(_filenameEdit, 1, 0);

		_buttonGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
		_buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);
		_buttonGrid->setVerticalAlignment(iVerticalAlignment::Bottom);
		_buttonGrid->appendCollumns(1);
		_buttonGrid->setCellSpacing(4);
		_grid->addWidget(_buttonGrid, 0, 4);

		_okButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
		_okButton->registerOnClickEvent(iClickDelegate(this, &iDialogFileSelect::onOK));
		_buttonGrid->addWidget(_okButton, 0, 0);

		_cancelButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
		_cancelButton->setText("Cancel");
		_cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogFileSelect::onCancel));
		_buttonGrid->addWidget(_cancelButton, 1, 0);
	}

	void iDialogFileSelect::deinitGUI()
	{
		removeWidget(_grid);

		if (_filenameEdit != nullptr)
		{
			_filenameEdit->unregisterOnChangeEvent(iChangeDelegate(this, &iDialogFileSelect::onFilenameEditChange));
			iWidgetManager::getInstance().destroyWidget(_filenameEdit);
		}

		if (_filenameLabel != nullptr)
		{
			iWidgetManager::getInstance().destroyWidget(_filenameLabel);
		}

		if (_filenameGrid != nullptr)
		{
			iWidgetManager::getInstance().destroyWidget(_filenameGrid);
		}

		if (_fileGrid != nullptr)
		{
			_fileGrid->unregisterOnDoubleClickEvent(iDoubleClickDelegate(this, &iDialogFileSelect::onDoubleClick));
			clearFileGrid();
			iWidgetManager::getInstance().destroyWidget(_fileGrid);
		}

		if (_pathEdit != nullptr)
		{
			_pathEdit->unregisterOnChangeEvent(iChangeDelegate(this, &iDialogFileSelect::onPathEditChange));
			iWidgetManager::getInstance().destroyWidget(_pathEdit);
		}

		if (_okButton != nullptr)
		{
			_okButton->unregisterOnClickEvent(iClickDelegate(this, &iDialogFileSelect::onOK));
			iWidgetManager::getInstance().destroyWidget(_okButton);
		}

		if (_cancelButton != nullptr)
		{
			_cancelButton->unregisterOnClickEvent(iClickDelegate(this, &iDialogFileSelect::onCancel));
			iWidgetManager::getInstance().destroyWidget(_cancelButton);
		}

		if (_grid != nullptr)
		{
			iWidgetManager::getInstance().destroyWidget(_grid);
		}

		if (_headerLabel != nullptr)
		{
			iWidgetManager::getInstance().destroyWidget(_headerLabel);
		}

		if (_scroll != nullptr)
		{
			iWidgetManager::getInstance().destroyWidget(_scroll);
		}

		if (_buttonGrid != nullptr)
		{
			iWidgetManager::getInstance().destroyWidget(_buttonGrid);
		}
	}

	void iDialogFileSelect::initDialog(const iaString& path)
	{
		_directory = path != "" ? path : iaDirectory::getApplicationDirectory();
		updateFileDir();

		iWidgetManager::setModal(this);
		setActive();
		setVisible();

		// update content
		updateFileGrid();
		_pathEdit->setText(getFullPath());
	}

	void iDialogFileSelect::load(iDialogFileSelectCloseDelegate closeDelegate, const iaString& path)
	{
		_load = true;
		_fileDialogCloseEvent.append(closeDelegate);
		_headerLabel->setText("Load File");
		_okButton->setText("Load");
		
		initDialog(path);
	}

	void iDialogFileSelect::save(iDialogFileSelectCloseDelegate closeDelegate, const iaString& path)
	{
		_load = false;
		_fileDialogCloseEvent.append(closeDelegate);
		_headerLabel->setText("Save File");
		_okButton->setText("Save");

		initDialog(path);
	}

	const iaString& iDialogFileSelect::getDirectory() const
	{
		return _directory;
	}

	const iaString& iDialogFileSelect::getFilename() const
	{
		return _filename;
	}

	iaString iDialogFileSelect::getFullPath()
	{
		iaString temp;

		if (_filename.isEmpty())
		{
			temp = iaDirectory::fixPath(_directory, false);
		}
		else
		{
			temp = iaDirectory::fixPath(_directory + iaDirectory::getPathSeperator() + _filename, true);
		}

		return temp;
	}

	void iDialogFileSelect::updateFileDir()
	{
		if (iaFile::exist(_filename))
		{
			iaFile file(_filename);
			_directory = file.getPath();
			_filename = file.getFileName();
		}
		else if (iaFile::exist(_directory))
		{
			iaFile file(_directory);
			_directory = file.getPath();
			_filename = file.getFileName();
		}
	}

	void iDialogFileSelect::updateFileGrid()
	{
		clearFileGrid();

		_fileGrid->appendRows(9);

		int32 index = 0;
		iaDirectory directory(_directory);
		auto directories = directory.getDirectorys();
		auto files = directory.getFiles();

		if (!directory.isRoot())
		{
			addToFileGrid(0, 0, directory.getFullParentDirectoryName(), "..", true);
			index++;
		}

		for (auto iter : directories)
		{
			addToFileGrid(index / 10, index % 10, iter.getFullDirectoryName(), iter.getDirectoryName(), true);
			index++;
		}

		for (auto iter : files)
		{
			addToFileGrid(index / 10, index % 10, iter.getFullFileName(), iter.getFileName(), false);
			index++;
		}

		_scroll->setHorizontalScroll(0);
		_scroll->setVerticalScroll(0);
	}

	void iDialogFileSelect::onFilenameEditChange(iWidget* source)
	{
		_filename = _filenameEdit->getText();

		updateFileDir();

		if (_load)
		{
			if (!_filename.isEmpty())
			{
				_fileDialogReturnValue = iFileDialogReturnValue::Ok;
				close();
				return;
			}
		}
	}

	void iDialogFileSelect::onPathEditChange(iWidget* source)
	{
		_directory = _pathEdit->getText();
		updateFileDir();

		if (_load)
		{
			if (!_filename.isEmpty())
			{
				_fileDialogReturnValue = iFileDialogReturnValue::Ok;
				close();
				return;
			}
		}

		updateFileGrid();
	}

	void iDialogFileSelect::addToFileGrid(int32 col, int32 row, const iaString& path, iaString displayName, bool isFolder)
	{
		if (col >= static_cast<int32>(_fileGrid->getColumnCount()))
		{
			_fileGrid->appendCollumns(1);
		}

		iaString* userData = new iaString(path);
		iWidgetGrid* entry = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
		entry->setHorizontalAlignment(iHorizontalAlignment::Left);
		entry->appendCollumns(1);

		iWidgetPicture* icon = static_cast<iWidgetPicture*>(iWidgetManager::getInstance().createWidget("Picture"));
		icon->setMaxSize(24, 24);
		if (isFolder)
		{
			icon->setTexture("icons/folder.png");
		}
		else
		{
			icon->setTexture("icons/file.png");
		}
		entry->addWidget(icon, 0, 0);

		iWidgetLabel* label = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
		label->setHorizontalAlignment(iHorizontalAlignment::Left);
		label->setText(displayName);
		entry->addWidget(label, 1, 0);

		_fileGrid->addWidget(entry, col, row, userData);

		_fileGridWidgets.push_back(entry);
		_fileGridWidgets.push_back(icon);
		_fileGridWidgets.push_back(label);
	}

	void iDialogFileSelect::clearFileGrid()
	{
		for (auto iter : _fileGridWidgets)
		{
			iWidgetManager::getInstance().destroyWidget(iter);
		}

		_fileGridWidgets.clear();

		for (uint32 row = 0; row < _fileGrid->getRowCount(); ++row)
		{
			for (uint32 col = 0; col < _fileGrid->getColumnCount(); ++col)
			{
				void* data = _fileGrid->getUserData(col, row);
				if (data != nullptr)
				{
					delete data;
				}
			}
		}

		_fileGrid->clear();
	}

	void iDialogFileSelect::onDoubleClick(iWidget* source)
	{
		iaString* userData = static_cast<iaString*>(_fileGrid->getSelectedUserData());
		if (userData != nullptr)
		{
			_directory = *userData;
			updateFileDir();
		}

		if (_load)
		{
			if (!_filename.isEmpty())
			{
				_fileDialogReturnValue = iFileDialogReturnValue::Ok;
				close();
				return;
			}
		}

		updateFileGrid();
		_pathEdit->setText(getFullPath());
	}

	iFileDialogReturnValue iDialogFileSelect::getReturnState()
	{
		return _fileDialogReturnValue;
	}

	void iDialogFileSelect::onOK(iWidget* source)
	{
		_fileDialogReturnValue = iFileDialogReturnValue::Ok;
		close();
	}

	void iDialogFileSelect::onCancel(iWidget* source)
	{
		_fileDialogReturnValue = iFileDialogReturnValue::Cancel;
		close();
	}

	void iDialogFileSelect::close()
	{
		setActive(false);
		setVisible(false);
		iWidgetManager::resetModal();

		iaFile file(getFullPath());
		if (!file.exist())
		{
			_fileDialogReturnValue = iFileDialogReturnValue::Error;
		}

		_fileDialogCloseEvent(_fileDialogReturnValue);
		_fileDialogCloseEvent.clear();
	}
}