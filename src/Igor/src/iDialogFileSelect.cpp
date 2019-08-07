// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
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
		if (_grid != nullptr)
		{
			return;
		}

		_grid = new iWidgetGrid();
		_grid->setBorder(4);
		_grid->setCellSpacing(4);
		_grid->appendRows(4);
		addWidget(_grid);

		_headerLabel = new iWidgetLabel();
		_headerLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
		_grid->addWidget(_headerLabel, 0, 0);

		_pathEdit = new iWidgetTextEdit();
		_pathEdit->setWidth(600);
		_pathEdit->setWriteProtected(false);
		_pathEdit->setChangeEventOnEnterAndLostFocus();
		_pathEdit->setMaxTextLength(1024);
		_pathEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
		_pathEdit->setVerticalAlignment(iVerticalAlignment::Top);
		_pathEdit->registerOnChangeEvent(iChangeDelegate(this, &iDialogFileSelect::onPathEditChange));
		_grid->addWidget(_pathEdit, 0, 1);

		_scroll = new iWidgetScroll();
		_scroll->setWidth(600);
		_scroll->setHeight(300);
		_grid->addWidget(_scroll, 0, 2);

		_fileGrid = new iWidgetGrid();
		_fileGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
		_fileGrid->setVerticalAlignment(iVerticalAlignment::Top);
		_fileGrid->setSelectMode(iSelectionMode::Field);
		_fileGrid->registerOnDoubleClickEvent(iDoubleClickDelegate(this, &iDialogFileSelect::onDoubleClick));
		_scroll->addWidget(_fileGrid);

		_filenameGrid = new iWidgetGrid();
		_filenameGrid->setHorizontalAlignment(iHorizontalAlignment::Right);
		_filenameGrid->setVerticalAlignment(iVerticalAlignment::Bottom);
		_filenameGrid->appendCollumns(1);
		_filenameGrid->setCellSpacing(4);
		_grid->addWidget(_filenameGrid, 0, 3);

		_filenameLabel = new iWidgetLabel();
		_filenameLabel->setHorizontalAlignment(iHorizontalAlignment::Right);
		_filenameLabel->setText("File name:");
		_filenameGrid->addWidget(_filenameLabel, 0, 0);

		_filenameEdit = new iWidgetTextEdit();
		_filenameEdit->setWidth(300);
		_filenameEdit->setWriteProtected(false);
		_filenameEdit->setChangeEventOnEnterAndLostFocus();
		_filenameEdit->setMaxTextLength(256);
		_filenameEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
		_filenameEdit->setVerticalAlignment(iVerticalAlignment::Top);
		_filenameEdit->registerOnChangeEvent(iChangeDelegate(this, &iDialogFileSelect::onFilenameEditChange));
		_filenameGrid->addWidget(_filenameEdit, 1, 0);

		_buttonGrid = new iWidgetGrid();
		_buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);
		_buttonGrid->setVerticalAlignment(iVerticalAlignment::Bottom);
		_buttonGrid->appendCollumns(1);
		_buttonGrid->setCellSpacing(4);
		_grid->addWidget(_buttonGrid, 0, 4);

		_okButton = new iWidgetButton();
		_okButton->registerOnClickEvent(iClickDelegate(this, &iDialogFileSelect::onOK));
		_buttonGrid->addWidget(_okButton, 0, 0);

		_cancelButton = new iWidgetButton();
		_cancelButton->setText("Cancel");
		_cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogFileSelect::onCancel));
		_buttonGrid->addWidget(_cancelButton, 1, 0);
	}

	void iDialogFileSelect::deinitGUI()
	{
		if (_grid == nullptr)
		{
			return;
		}

		clearFileGrid();

		removeWidget(_grid);
		iWidgetManager::getInstance().destroyWidget(_grid);

		_okButton = nullptr;
		_cancelButton = nullptr;
		_scroll = nullptr;
		_grid = nullptr;
		_fileGrid = nullptr;
		_headerLabel = nullptr;
		_filenameLabel = nullptr;
		_pathEdit = nullptr;
		_filenameEdit = nullptr;
		_buttonGrid = nullptr;
		_filenameGrid = nullptr;
	}

	void iDialogFileSelect::configure(const iaString& path)
	{
		initGUI();

		if (_load)
		{
			_headerLabel->setText("Load File");
			_okButton->setText("Load");
		}
		else
		{
			_headerLabel->setText("Save File");
			_okButton->setText("Save");
		}

		if (iaFile::exist(path))
		{
			iaFile file(path);
			_filename = file.getFileName();
			_directory = file.getPath();
		}
		else if (iaDirectory::isDirectory(path))
		{
			_filename = "";
			_directory = iaDirectory::fixPath(path, false);
		}
		else
		{
			_filename = "";
			_directory = iaDirectory::getApplicationDirectory();
		}

		updateFileDir();

		iWidgetManager::setModal(this);
		setActive();
		setVisible();

		updateFileGrid();
	}

	void iDialogFileSelect::load(iDialogFileSelectCloseDelegate closeDelegate, const iaString& path)
	{
		_load = true;
		_fileDialogCloseEvent.append(closeDelegate);
	
		configure(path);
	}

	void iDialogFileSelect::save(iDialogFileSelectCloseDelegate closeDelegate, const iaString& path)
	{
		_load = false;
		_fileDialogCloseEvent.append(closeDelegate);

		configure(path);
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

		_pathEdit->setText(_directory);
		_filenameEdit->setText(_filename);
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
		iWidgetGrid* entry = new iWidgetGrid();
		entry->setHorizontalAlignment(iHorizontalAlignment::Left);
		entry->appendCollumns(1);

		iWidgetPicture* icon = new iWidgetPicture();
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

		iWidgetLabel* label = new iWidgetLabel();
		label->setHorizontalAlignment(iHorizontalAlignment::Left);
		label->setText(displayName);
		entry->addWidget(label, 1, 0);

		_fileGrid->addWidget(entry, col, row, userData);

		_fileGridWidgets.push_back(entry);
	}

	void iDialogFileSelect::clearFileGrid()
	{
		if (_fileGrid != nullptr)
		{
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

		for (auto iter : _fileGridWidgets)
		{
			iWidgetManager::getInstance().destroyWidget(iter);
		}

		_fileGridWidgets.clear();
	}

	void iDialogFileSelect::onDoubleClick(iWidget* source)
	{
		iaString* userData = static_cast<iaString*>(_fileGrid->getSelectedUserData());
		if (userData != nullptr)
		{
			_directory = *userData;
			updateFileDir();
			updateFileGrid();
			_fileGrid->unSelect();
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

		if (_load)
		{
			iaFile file(getFullPath());
			if (!file.exist())
			{
				_fileDialogReturnValue = iFileDialogReturnValue::Error;
			}
		}

		_fileDialogCloseEvent(_fileDialogReturnValue);
		_fileDialogCloseEvent.clear();

		deinitGUI();
	}
}