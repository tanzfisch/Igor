// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogFileSelect.h>

#include <iWidgetDialog.h>
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

    iaString iDialogFileSelect::_lastLoadPath;
    iaString iDialogFileSelect::_lastSavePath;

    iDialogFileSelect::iDialogFileSelect()
    {
        initGUI();
    }

    iDialogFileSelect::~iDialogFileSelect()
    {
        deinitGUI();
    }

    iWidgetDialog* iDialogFileSelect::createInstance()
    {
        return new iDialogFileSelect();
    }

    void iDialogFileSelect::initGUI()
    {
        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _grid->setBorder(4);
        _grid->setCellSpacing(4);
        _grid->appendRows(3);
        addWidget(_grid);

        _headerLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
        _headerLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
        _grid->addWidget(_headerLabel, 0, 0);

        _pathEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget("TextEdit"));
        _pathEdit->setText(getFullPath());
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

        _buttonGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);
        _buttonGrid->setVerticalAlignment(iVerticalAlignment::Bottom);
        _buttonGrid->appendCollumns(1);
        _buttonGrid->setCellSpacing(4);
        _grid->addWidget(_buttonGrid, 0, 3);

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

        if (_fileGrid)
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

    void iDialogFileSelect::load(iDialogFileSelectCloseDelegate closeDelegate, iaString path)
    {
        _load = true;

        if (path != "")
        {
            _directory = path;
        }
        else
        {
            _directory = iDialogFileSelect::_lastLoadPath;
        }

        initFileDir();

        _fileDialogCloseEvent.append(closeDelegate);
        _headerLabel->setText("Load File");
        _okButton->setText("Load");

        iWidgetManager::setModal(this);
        setActive();
        setVisible();

        updateFileGrid();
    }

    void iDialogFileSelect::save(iDialogFileSelectCloseDelegate closeDelegate, iaString path)
    {
        _load = false;

        if (path != "")
        {
            _directory = path;
        }
        else
        {
            _directory = iDialogFileSelect::_lastSavePath;
        }

        initFileDir();
        
        _fileDialogCloseEvent.append(closeDelegate);
        _headerLabel->setText("Save File");
        _okButton->setText("Save");

        iWidgetManager::setModal(this);
        setActive();
        setVisible();

        updateFileGrid();
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

    void iDialogFileSelect::initFileDir()
    {
        _filename = "";
        iaFile checkFile(_directory);
        if (checkFile.exist())
        {
            if (!iaDirectory::isDirectory(_directory))
            {
                _directory = checkFile.getPath();
                _filename = checkFile.getFileName();
            }
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

    void iDialogFileSelect::onPathEditChange(iWidget* source)
    {
        _directory = _pathEdit->getText();
        initFileDir();

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

    void iDialogFileSelect::addToFileGrid(int32 col, int32 row, iaString path, iaString displayName, bool isFolder)
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
            initFileDir();
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

        if (_load)
        {
            iDialogFileSelect::_lastLoadPath = _directory;
        }
        else
        {
            iDialogFileSelect::_lastSavePath = _directory;
        }

        _fileDialogCloseEvent(_fileDialogReturnValue);
        _fileDialogCloseEvent.clear();
    }
}