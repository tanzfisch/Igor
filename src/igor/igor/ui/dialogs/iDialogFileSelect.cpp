// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialogFileSelect.h>

#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/layouts/iWidgetGridLayout.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetLineTextEdit.h>
#include <igor/ui/widgets/iWidgetScroll.h>
#include <igor/ui/widgets/iWidgetPicture.h>
#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaConsole.h>
#include <iaux/system/iaFile.h>
#include <iaux/system/iaDirectory.h>
using namespace iaux;

namespace igor
{

    iDialogFileSelect::iDialogFileSelect(const iWidgetPtr parent)
        : iDialog(iWidgetType::iDialogFileSelect, parent)
    {
        setAcceptESCToClose(true);
    }

    void iDialogFileSelect::open(iDialogCloseDelegate dialogCloseDelegate, iFileDialogPurpose purpose, const iaString &path)
    {
        iDialog::open(dialogCloseDelegate, true);

        _purpose = purpose;
        initGUI();

        if (iaFile::exists(path))
        {
            iaFile file(path);
            _filename = file.getFileName();
            _directory = file.getPath();
        }
        else if (iaDirectory::isDirectory(path))
        {
            _filename = "";
            _directory = path;
        }
        else
        {
            if (!path.isEmpty())
            {
                _filename = "";
                _directory = iaDirectory::fixPath(path);

                if (!iaDirectory::isDirectory(_directory))
                {
                    _directory = iaDirectory::getCurrentDirectory();
                }
            }
            else
            {
                _filename = "";
                _directory = iaDirectory::getCurrentDirectory();
            }
        }

        updateFileDir();

        setEnabled();
        setVisible();

        updateFileGrid();
    }

    iFileDialogPurpose iDialogFileSelect::getPurpose() const
    {
        return _purpose;
    }

    void iDialogFileSelect::initGUI()
    {
        clearChildren();

        setMinWidth(700);
        setMinHeight(500);

        setHorizontalAlignment(iHorizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);

        iWidgetGridLayoutPtr grid = new iWidgetGridLayout(this);
        grid->setVerticalAlignment(iVerticalAlignment::Stretch);
        grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        grid->setBorder(4);
        grid->setCellSpacing(4);
        grid->appendRows(3);
        grid->setStretchRow(1);

        _pathEdit = new iWidgetLineTextEdit();
        _pathEdit->setWriteProtected(false);
        _pathEdit->setChangeEventOnEnterAndLostFocus();
        _pathEdit->setMaxTextLength(1024);
        _pathEdit->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        _pathEdit->setVerticalAlignment(iVerticalAlignment::Top);
        _pathEdit->registerOnChangeEvent(iChangeDelegate(this, &iDialogFileSelect::onPathEditChange));
        grid->addWidget(_pathEdit, 0, 0);

        _scroll = new iWidgetScroll();
        grid->addWidget(_scroll, 0, 1);

        _fileGrid = new iWidgetGridLayout(_scroll);
        _fileGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
        _fileGrid->setVerticalAlignment(iVerticalAlignment::Top);
        _fileGrid->setSelectMode(iSelectionMode::Cell);
        _fileGrid->registerOnDoubleClickEvent(iDoubleClickDelegate(this, &iDialogFileSelect::onDoubleClick));

        if (_purpose != iFileDialogPurpose::SelectFolder)
        {
            iWidgetGridLayoutPtr filenameGrid = new iWidgetGridLayout();
            filenameGrid->setHorizontalAlignment(iHorizontalAlignment::Right);
            filenameGrid->setVerticalAlignment(iVerticalAlignment::Bottom);
            filenameGrid->appendColumns(1);
            filenameGrid->setCellSpacing(4);
            grid->addWidget(filenameGrid, 0, 2);

            iWidgetLabelPtr filenameLabel = new iWidgetLabel();
            filenameLabel->setHorizontalAlignment(iHorizontalAlignment::Right);
            filenameLabel->setText("File name:");
            filenameGrid->addWidget(filenameLabel, 0, 0);

            _filenameEdit = new iWidgetLineTextEdit();
            _filenameEdit->setMinWidth(300);
            _filenameEdit->setWriteProtected(false);
            _filenameEdit->setChangeEventOnEnterAndLostFocus();
            _filenameEdit->setMaxTextLength(256);
            _filenameEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
            _filenameEdit->setVerticalAlignment(iVerticalAlignment::Top);
            _filenameEdit->registerOnChangeEvent(iChangeDelegate(this, &iDialogFileSelect::onFilenameEditChange));
            filenameGrid->addWidget(_filenameEdit, 1, 0);
        }

        iWidgetGridLayoutPtr buttonGrid = new iWidgetGridLayout();
        buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);
        buttonGrid->setVerticalAlignment(iVerticalAlignment::Bottom);
        buttonGrid->appendColumns(1);
        buttonGrid->setCellSpacing(4);
        grid->addWidget(buttonGrid, 0, 3);

        iWidgetButtonPtr okButton = new iWidgetButton();
        okButton->registerOnClickEvent(iClickDelegate(this, &iDialogFileSelect::onOK));
        buttonGrid->addWidget(okButton, 0, 0);

        iWidgetButtonPtr cancelButton = new iWidgetButton();
        cancelButton->setText("Cancel");
        cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogFileSelect::onCancel));
        buttonGrid->addWidget(cancelButton, 1, 0);

        switch (_purpose)
        {
        case iFileDialogPurpose::Load:
            setTitle("Load File");
            okButton->setText("Load");
            break;

        case iFileDialogPurpose::Save:
            setTitle("Save File");
            okButton->setText("Save");
            break;

        case iFileDialogPurpose::SelectFolder:
            setTitle("Select Folder");
            okButton->setText("Select");
            break;
        }
    }

    const iaString &iDialogFileSelect::getDirectory() const
    {
        return _directory;
    }

    const iaString &iDialogFileSelect::getFilename() const
    {
        return _filename;
    }

    iaString iDialogFileSelect::getFullPath()
    {
        iaString temp;

        if (_filename.isEmpty())
        {
            temp = iaDirectory::fixPath(_directory);
        }
        else
        {
            temp = iaDirectory::fixPath(_directory + IGOR_PATHSEPARATOR + _filename);
        }

        return temp;
    }

    void iDialogFileSelect::updateFileDir()
    {
        if (iaFile::exists(_filename))
        {
            iaFile file(_filename);
            _directory = file.getPath();
            _filename = file.getFileName();
        }
        else if (iaFile::exists(_directory))
        {
            iaFile file(_directory);
            _directory = file.getPath();
            _filename = file.getFileName();
        }

        _pathEdit->setText(_directory);

        if (_filenameEdit != nullptr)
        {
            _filenameEdit->setText(_filename);
        }
    }

    void iDialogFileSelect::updateFileGrid()
    {
        clearFileGrid();

        _fileGrid->appendRows(9);

        int32 index = 0;
        iaDirectory directory(_directory);
        auto directories = directory.getDirectories();
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

    void iDialogFileSelect::onFilenameEditChange(const iWidgetPtr source)
    {
        if (_filenameEdit == nullptr)
        {
            return;
        }

        _filename = _filenameEdit->getText();

        updateFileDir();

        if (_purpose == iFileDialogPurpose::Load &&
            !_filename.isEmpty())
        {
            setReturnState(iDialogReturnState::Ok);
            close();
        }
    }

    void iDialogFileSelect::onPathEditChange(const iWidgetPtr source)
    {
        _directory = _pathEdit->getText();
        updateFileDir();

        if (_purpose == iFileDialogPurpose::Load &&
            !_filename.isEmpty())
        {
            setReturnState(iDialogReturnState::Ok);
            close();
            return;
        }

        updateFileGrid();
    }

    void iDialogFileSelect::addToFileGrid(int32 col, int32 row, const iaString &path, iaString displayName, bool isFolder)
    {
        if (col >= static_cast<int32>(_fileGrid->getColumnCount()))
        {
            _fileGrid->appendColumns(1);
        }

        iWidgetGridLayout *entry = new iWidgetGridLayout();
        entry->setHorizontalAlignment(iHorizontalAlignment::Left);
        entry->appendColumns(1);

        iWidgetPicture *icon = new iWidgetPicture();
        icon->setMaxSize(24, 24);
        if (isFolder)
        {
            icon->setTexture(iResourceManager::getInstance().loadResource<iTexture>("igor_icon_folder"));
        }
        else
        {
            icon->setTexture(iResourceManager::getInstance().loadResource<iTexture>("igor_icon_file"));
        }
        entry->addWidget(icon, 0, 0);

        iWidgetLabel *label = new iWidgetLabel();
        label->setHorizontalAlignment(iHorizontalAlignment::Left);
        label->setText(displayName);
        entry->addWidget(label, 1, 0);

        _fileGrid->addWidget(entry, col, row, iaString(path));
    }

    void iDialogFileSelect::clearFileGrid()
    {
        if (_fileGrid != nullptr)
        {
            _fileGrid->clear();
        }
    }

    void iDialogFileSelect::onDoubleClick(const iWidgetPtr source)
    {
        if (_fileGrid == nullptr)
        {
            return;
        }

        std::any userData = _fileGrid->getSelectedUserData();
        if (userData.has_value())
        {
            _directory = std::any_cast<iaString>(userData);
            updateFileDir();
            updateFileGrid();
            _fileGrid->unselect();
        }

        if (_purpose == iFileDialogPurpose::Load &&
            !_filename.isEmpty())
        {
            setReturnState(iDialogReturnState::Ok);
            close();
        }
    }

    void iDialogFileSelect::onOK(const iWidgetPtr source)
    {
        setReturnState(iDialogReturnState::Ok);
        close();
    }

    void iDialogFileSelect::onCancel(const iWidgetPtr source)
    {
        setReturnState(iDialogReturnState::Cancel);
        close();
    }

    void iDialogFileSelect::close()
    {
        if (_purpose == iFileDialogPurpose::Load)
        {
            iaFile file(getFullPath());
            if (!file.exists())
            {
                setReturnState(iDialogReturnState::Error);
            }
        }

        iDialog::close();
    }
} // namespace igor