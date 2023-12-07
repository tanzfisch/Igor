// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlFileChooser.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetGridLayout.h>
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

    iUserControlFileChooser::iUserControlFileChooser(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlFileChooser, parent)
    {
        initGUI();
    }

    iUserControlFileChooser::~iUserControlFileChooser()
    {
        deinitGUI();
    }

    void iUserControlFileChooser::setOptimizePath(bool optimizePath)
    {
        _optimizePath = optimizePath;
    }

    bool iUserControlFileChooser::getOptimizePath() const
    {
        return _optimizePath;
    }

    void iUserControlFileChooser::setPreselectedPath(const iaString &path)
    {
        _preselectedPath = path;
    }

    const iaString &iUserControlFileChooser::getPreselectedPath() const
    {
        return _preselectedPath;
    }

    IGOR_DISABLE_WARNING(4100)
    void iUserControlFileChooser::onTextChanged(const iWidgetPtr source)
    {
        _fileNameChanged(_fileNameTextEdit);
    }

    void iUserControlFileChooser::onFileSelectButtonPressed(const iWidgetPtr source)
    {
        if (_fileDialog == nullptr)
        {
            _fileDialog = new iDialogFileSelect();
        }

        _fileDialog->open(iDialogCloseDelegate(this, &iUserControlFileChooser::onFileLoadDialogClosed), iFileDialogPurpose::Load, _preselectedPath);
    }

    void iUserControlFileChooser::onFileLoadDialogClosed(iDialogPtr dialog)
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

    void iUserControlFileChooser::initGUI()
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
        _fileNameTextEdit->registerOnChangeEvent(iChangeDelegate(this, &iUserControlFileChooser::onTextChanged));

        iWidgetButtonPtr fileSelectButton = new iWidgetButton();
        fileSelectButton->setText("...");
        fileSelectButton->setTooltip("Browse for file.");
        fileSelectButton->setHorizontalAlignment(iHorizontalAlignment::Left);
        fileSelectButton->registerOnClickEvent(iClickDelegate(this, &iUserControlFileChooser::onFileSelectButtonPressed));

        grid->addWidget(_fileNameTextEdit, 0, 0);
        grid->addWidget(fileSelectButton, 1, 0);
    }

    void iUserControlFileChooser::deinitGUI()
    {
        _fileNameTextEdit = nullptr;

        // destroy the file dialog
        if (_fileDialog != nullptr)
        {
            delete _fileDialog;
            _fileDialog = nullptr;
        }
    }

    void iUserControlFileChooser::setFileName(const iaString &filename)
    {
        _fileNameTextEdit->setText(filename);
        _fileNameChanged(_fileNameTextEdit);
    }

    const iaString &iUserControlFileChooser::getFileName() const
    {
        return _fileNameTextEdit->getText();
    }

    void iUserControlFileChooser::registerOnChangedDelegate(iChangeDelegate changeDelegate)
    {
        _fileNameChanged.add(changeDelegate);
    }

    void iUserControlFileChooser::unregisterOnChangedDelegate(iChangeDelegate changeDelegate)
    {
        _fileNameChanged.remove(changeDelegate);
    }

} // namespace igor