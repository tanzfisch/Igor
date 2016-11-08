// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iUserControlFileChooser.h>

#include <iWidgetManager.h>
#include <iWidgetGrid.h>
#include <iWidgetTextEdit.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetSpacer.h>
#include <iWidgetColor.h>
#include <iWidgetSlider.h>
#include <iWidgetNumberChooser.h>
#include <iResourceManager.h>

namespace Igor
{

    iUserControlFileChooser::iUserControlFileChooser()
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

    void iUserControlFileChooser::setPreselectedPath(const iaString& path)
    {
        _preselectedPath = path;
    }

    const iaString& iUserControlFileChooser::getPreselectedPath() const
    {
        return _preselectedPath;
    }

    void iUserControlFileChooser::onTextChanged(iWidget* source)
    {
        _fileNameChanged(_grid);
    }

    void iUserControlFileChooser::onFileSelectButtonPressed(iWidget* source)
    {
        _fileDialog->load(iDialogFileSelectCloseDelegate(this, &iUserControlFileChooser::onFileLoadDialogClosed), _preselectedPath);
    }

    /*! 
    \todo implement strech
    */
    void iUserControlFileChooser::initGUI()
    {
        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _allWidgets.push_back(_grid);
        _grid->appendCollumns(1);
        _grid->setStrechColumn(0);
        _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
        _grid->setVerticalAlignment(iVerticalAlignment::Top);

        _fileNameTextEdit = static_cast<iWidgetTextEdit*>(iWidgetManager::getInstance().createWidget(iWidgetType::TextEdit));
        _allWidgets.push_back(_fileNameTextEdit);
        _fileNameTextEdit->setMaxTextLength(256);
        _fileNameTextEdit->setWidth(180); // todo why does strech not work here?
        _fileNameTextEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
        _fileNameTextEdit->registerOnChangeEvent(iChangeDelegate(this, &iUserControlFileChooser::onTextChanged));

        _fileSelectButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
        _allWidgets.push_back(_fileSelectButton);
        _fileSelectButton->setText("...");
        _fileSelectButton->setHorizontalAlignment(iHorizontalAlignment::Left);
        _fileSelectButton->registerOnClickEvent(iClickDelegate(this, &iUserControlFileChooser::onFileSelectButtonPressed));

        _grid->addWidget(_fileNameTextEdit, 0, 0);
        _grid->addWidget(_fileSelectButton, 1, 0);

        _fileDialog = new iDialogFileSelect();
    }

    void iUserControlFileChooser::deinitGUI()
    {
        for (auto widget : _allWidgets)
        {
            iWidgetManager::getInstance().destroyWidget(widget);
        }
        _allWidgets.clear();

        _grid = nullptr;
        _fileNameTextEdit = nullptr;
        _fileSelectButton = nullptr;

        delete _fileDialog;
        _fileDialog = nullptr;
    }

    void iUserControlFileChooser::onFileLoadDialogClosed(iFileDialogReturnValue fileDialogReturnValue)
    {
        if (_fileDialog->getReturnState() == iFileDialogReturnValue::Ok)
        {
            iaString filename = _fileDialog->getFullPath();

            if (_optimizePath)
            {
                filename = iResourceManager::getInstance().getRelativePath(filename);
            }

            if (_fileNameTextEdit->getText() != filename)
            {
                _fileNameTextEdit->setText(filename);
                _fileNameChanged(_fileNameTextEdit);
            }
        }
    }

    void iUserControlFileChooser::setFileName(const iaString& filename)
    {
        _fileNameTextEdit->setText(filename);
        _fileNameChanged(_fileNameTextEdit);
    }

    const iaString& iUserControlFileChooser::getFileName() const
    {
        return _fileNameTextEdit->getText();
    }

    iWidget* iUserControlFileChooser::getWidget()
    {
        return _grid;
    }

    void iUserControlFileChooser::registerOnChangedDelegate(iChangeDelegate changeDelegate)
    {
        _fileNameChanged.append(changeDelegate);
    }

    void iUserControlFileChooser::unregisterOnChangedDelegate(iChangeDelegate changeDelegate)
    {
        _fileNameChanged.remove(changeDelegate);
    }

}