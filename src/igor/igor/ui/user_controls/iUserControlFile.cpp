// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlFile.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/layouts/iWidgetGridLayout.h>
#include <igor/ui/widgets/iWidgetLineTextEdit.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetColor.h>
#include <igor/ui/widgets/iWidgetSlider.h>
#include <igor/ui/widgets/iWidgetNumberChooser.h>
#include <igor/resources/iResourceManager.h>

namespace igor
{

    iUserControlFile::iUserControlFile(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlFile, parent)
    {
        initGUI();
    }

    iUserControlFile::~iUserControlFile()
    {
        deinitGUI();
    }

    void iUserControlFile::setOptimizePath(bool optimizePath)
    {
        _optimizePath = optimizePath;
    }

    bool iUserControlFile::getOptimizePath() const
    {
        return _optimizePath;
    }

    void iUserControlFile::setPreselectedPath(const iaString &path)
    {
        _preselectedPath = path;
    }

    const iaString &iUserControlFile::getPreselectedPath() const
    {
        return _preselectedPath;
    }

    IGOR_DISABLE_WARNING(4100)
    void iUserControlFile::onTextChanged(const iWidgetPtr source)
    {
        _fileNameChanged(_fileNameTextEdit);
    }

    void iUserControlFile::onFileSelectButtonPressed(const iWidgetPtr source)
    {
        if (_fileDialog == nullptr)
        {
            _fileDialog = new iDialogFileSelect();
        }

        _fileDialog->open(iDialogCloseDelegate(this, &iUserControlFile::onFileLoadDialogClosed), iFileDialogPurpose::Load, _preselectedPath);
    }

    void iUserControlFile::onFileLoadDialogClosed(iDialogPtr dialog)
    {
        iDialogFileSelect *fileDialog = static_cast<iDialogFileSelect *>(dialog);

        if (fileDialog->getReturnState() == iDialogReturnState::Ok)
        {
            iaString filename = fileDialog->getFullPath();

            if (_optimizePath)
            {
                filename = iResourceManager::getInstance().getRelativePath(filename);
            }

            if (_fileNameTextEdit->getText() != filename)
            {
                setFileName(filename);
            }
        }
    }
    IGOR_ENABLE_WARNING(4100)

    void iUserControlFile::initGUI()
    {
        iWidgetGridLayoutPtr grid = new iWidgetGridLayout(this);
        grid->appendColumns(1);
        grid->setStretchColumn(0);
        grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        grid->setVerticalAlignment(iVerticalAlignment::Top);

        _fileNameTextEdit = new iWidgetLineTextEdit();
        _fileNameTextEdit->setMaxTextLength(256);
        _fileNameTextEdit->setMinWidth(180); // todo why does strech not work here?
        _fileNameTextEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
        _fileNameTextEdit->registerOnChangeEvent(iChangeDelegate(this, &iUserControlFile::onTextChanged));

        iWidgetButtonPtr fileSelectButton = new iWidgetButton();
        fileSelectButton->setText("...");
        fileSelectButton->setTooltip("Browse for file.");
        fileSelectButton->setHorizontalAlignment(iHorizontalAlignment::Left);
        fileSelectButton->registerOnClickEvent(iClickDelegate(this, &iUserControlFile::onFileSelectButtonPressed));

        grid->addWidget(_fileNameTextEdit, 0, 0);
        grid->addWidget(fileSelectButton, 1, 0);
    }

    void iUserControlFile::deinitGUI()
    {
        _fileNameTextEdit = nullptr;

        // destroy the file dialog
        if (_fileDialog != nullptr)
        {
            delete _fileDialog;
            _fileDialog = nullptr;
        }
    }

    void iUserControlFile::setFileName(const iaString &filename)
    {
        _fileNameTextEdit->setText(filename);
        _fileNameChanged(_fileNameTextEdit);
    }

    const iaString &iUserControlFile::getFileName() const
    {
        return _fileNameTextEdit->getText();
    }

    void iUserControlFile::registerOnChangedDelegate(iChangeDelegate changeDelegate)
    {
        _fileNameChanged.add(changeDelegate);
    }

    void iUserControlFile::unregisterOnChangedDelegate(iChangeDelegate changeDelegate)
    {
        _fileNameChanged.remove(changeDelegate);
    }

} // namespace igor