// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
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

    iWidget* iUserControlFileChooser::createInstance()
    {
        return new iUserControlFileChooser();
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

	__IGOR_DISABLE_WARNING__(4100)
    void iUserControlFileChooser::onTextChanged(iWidget* source)
    {
        _fileNameChanged(_grid);
    }

    void iUserControlFileChooser::onFileSelectButtonPressed(iWidget* source)
    {
        _fileDialog->load(iDialogFileSelectCloseDelegate(this, &iUserControlFileChooser::onFileLoadDialogClosed), _preselectedPath);
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
				setFileName(filename);
			}
		}
	}
	__IGOR_ENABLE_WARNING__(4100)

    void iUserControlFileChooser::initGUI()
    {
        _grid = new iWidgetGrid();
        _allWidgets.push_back(_grid);
        _grid->appendCollumns(1);
        _grid->setStrechColumn(0);
        _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
        _grid->setVerticalAlignment(iVerticalAlignment::Top);

        _fileNameTextEdit = new iWidgetTextEdit();
        _allWidgets.push_back(_fileNameTextEdit);
        _fileNameTextEdit->setMaxTextLength(256);
        _fileNameTextEdit->setWidth(180); // todo why does strech not work here?
        _fileNameTextEdit->setHorizontalAlignment(iHorizontalAlignment::Left);
        _fileNameTextEdit->registerOnChangeEvent(iChangeDelegate(this, &iUserControlFileChooser::onTextChanged));

        _fileSelectButton = new iWidgetButton();
        _allWidgets.push_back(_fileSelectButton);
        _fileSelectButton->setText("...");
		_fileSelectButton->setTooltip("Browse for file.");
        _fileSelectButton->setHorizontalAlignment(iHorizontalAlignment::Left);
        _fileSelectButton->registerOnClickEvent(iClickDelegate(this, &iUserControlFileChooser::onFileSelectButtonPressed));

        addWidget(_grid);

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

        if (_fileDialog != nullptr)
        {
            iWidgetManager::getInstance().destroyDialog(_fileDialog);
            _fileDialog = nullptr;
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

    void iUserControlFileChooser::registerOnChangedDelegate(iChangeDelegate changeDelegate)
    {
        _fileNameChanged.append(changeDelegate);
    }

    void iUserControlFileChooser::unregisterOnChangedDelegate(iChangeDelegate changeDelegate)
    {
        _fileNameChanged.remove(changeDelegate);
    }

}