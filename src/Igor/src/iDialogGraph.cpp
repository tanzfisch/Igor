// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogGraph.h>

#include <iDialog.h>
#include <iWidgetManager.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetGrid.h>
#include <iWidgetSpacer.h>
#include <iWidgetCheckBox.h>
#include <iWidgetManager.h>
#include <iWidgetGraph.h>
#include <iWidgetNumberChooser.h>
#include <iWidgetGroupBox.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iDialogGraph::~iDialogGraph()
    {
        deinitGUI();
    }

    iDialog* iDialogGraph::createInstance()
    {
        return new iDialogGraph();
    }

    void iDialogGraph::configureXAxis(float32 xMin, float32 xMax, float32 xStepping)
    {
        _xMin = xMin;
        _xMax = xMax;
        _xStepping = xStepping;
    }

    void iDialogGraph::configureYAxis(float32 yMin, float32 yMax, float32 yStepping)
    {
        _yMin = yMin;
        _yMax = yMax;
        _yStepping = yStepping;
    }

    void iDialogGraph::show(iDialogGraphCloseDelegate closeDelegate, const vector<vector<iaVector2f>>& graphs)
    {
        con_assert(graphs.size() > 0 && graphs[0].size() > 0 && graphs[0].size() < 4, "invalid data");

        if (graphs.size() > 0 && 
            graphs[0].size() > 0 && 
            graphs[0].size() < 4)
        {
            _oldGraphs = graphs;
            _graphs = graphs;
            _closeEvent.append(closeDelegate);

            deinitGUI();
            initGUI();
        }
        else
        {
            con_err("invalid data");
        }
    }

    void iDialogGraph::deinitGUI()
    {
        if (_grid != nullptr &&
            _grid->hasParent())
        {
            removeWidget(_grid);
        }

        if (_graph != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_graph);
            _graph = nullptr;
        }

        for (auto widget : _allWidgets)
        {
            iWidgetManager::getInstance().destroyWidget(widget);
        }
        _allWidgets.clear();
    }

    void iDialogGraph::initGUI()
    {
        iWidgetManager::setModal(this);
        setActive();
        setVisible();
        setWidth(450);
        setHeight(20);

        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _allWidgets.push_back(_grid);
        _grid->appendRows(4);
        _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
        _grid->setVerticalAlignment(iVerticalAlignment::Strech);
        _grid->setCellSpacing(4);
        _grid->setBorder(4);

        _titleLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
        _allWidgets.push_back(_titleLabel);
        _titleLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
        _titleLabel->setText(_title);

        iWidgetGroupBox* groupBoxGradient = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget("GroupBox"));
        _allWidgets.push_back(groupBoxGradient);
        groupBoxGradient->setText("Gradient");
        groupBoxGradient->setHorizontalAlignment(iHorizontalAlignment::Strech);
        groupBoxGradient->setVerticalAlignment(iVerticalAlignment::Strech);

        _graph = static_cast<iWidgetGraph*>(iWidgetManager::getInstance().createWidget("Graph"));
        _allWidgets.push_back(_graph);
        _graph->setHorizontalAlignment(iHorizontalAlignment::Strech);
        _graph->setHeight(120);
        _graph->setInteractive();
        _graph->setExtrapolateData();
        _graph->setViewGrid();
        _graph->setViewLabels();
        _graph->registerOnSelectionChangedEvent(iSelectionChangedDelegate(this, &iDialogGraph::onSelectionChanged));
        _graph->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onGraphChanged));

        iWidgetGroupBox* groupBoxSelection = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget("GroupBox"));
        _allWidgets.push_back(groupBoxSelection);
        groupBoxSelection->setText("Selected Value");
        groupBoxSelection->setHorizontalAlignment(iHorizontalAlignment::Strech);
        groupBoxSelection->setVerticalAlignment(iVerticalAlignment::Strech);

        iWidgetGrid* axisGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _allWidgets.push_back(axisGrid);
        axisGrid->appendRows(_graphs.size() + 2);
        axisGrid->appendCollumns(1);
        axisGrid->setHorizontalAlignment(iHorizontalAlignment::Strech);
        axisGrid->setVerticalAlignment(iVerticalAlignment::Strech);
        axisGrid->setStrechColumn(1);
        axisGrid->setCellSpacing(4);
        axisGrid->setBorder(4);

        iWidgetLabel* labelX = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
        _allWidgets.push_back(labelX);
        labelX->setHorizontalAlignment(iHorizontalAlignment::Left);
        labelX->setText(_axisNames[0]);
        labelX->setWidth(100);

        _axisNumberChooser[0] = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
        _allWidgets.push_back(_axisNumberChooser[0]);
        _axisNumberChooser[0]->setMinMaxNumber(_xMin, _xMax);
        _axisNumberChooser[0]->setStepping(_xStepping, _xStepping);
        _axisNumberChooser[0]->setSteppingWheel(_xStepping * 10, _xStepping * 10);
        _axisNumberChooser[0]->setHorizontalAlignment(iHorizontalAlignment::Strech);
        _axisNumberChooser[0]->setValue(_graphs[0][0]._x);
        _axisNumberChooser[0]->setAfterPoint(2);
        _axisNumberChooser[0]->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onValueChanged));

        axisGrid->addWidget(labelX, 0, 0);
        axisGrid->addWidget(_axisNumberChooser[0], 1, 0);

        iWidgetLabel* labelY1 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
        _allWidgets.push_back(labelY1);
        labelY1->setHorizontalAlignment(iHorizontalAlignment::Left);
        labelY1->setText(_axisNames[1]);
        labelY1->setWidth(100);

        _axisNumberChooser[1] = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
        _allWidgets.push_back(_axisNumberChooser[1]);
        _axisNumberChooser[1]->setMinMaxNumber(_xMin, _xMax);
        _axisNumberChooser[1]->setStepping(_xStepping, _xStepping);
        _axisNumberChooser[1]->setSteppingWheel(_xStepping * 10, _xStepping * 10);
        _axisNumberChooser[1]->setHorizontalAlignment(iHorizontalAlignment::Strech);
        _axisNumberChooser[1]->setValue(_graphs[0][0]._y);
        _axisNumberChooser[1]->setAfterPoint(2);
        _axisNumberChooser[1]->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onValueChanged));

        axisGrid->addWidget(labelY1, 0, 1);
        axisGrid->addWidget(_axisNumberChooser[1], 1, 1);

        if (_graphs.size() > 1)
        {
            iWidgetLabel* labelY2 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
            _allWidgets.push_back(labelY2);
            labelY2->setHorizontalAlignment(iHorizontalAlignment::Left);
            labelY2->setText(_axisNames[2]);
            labelY2->setWidth(100);

            _axisNumberChooser[2] = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
            _allWidgets.push_back(_axisNumberChooser[2]);
            _axisNumberChooser[2]->setMinMaxNumber(_xMin, _xMax);
            _axisNumberChooser[2]->setStepping(_xStepping, _xStepping);
            _axisNumberChooser[2]->setSteppingWheel(_xStepping * 10, _xStepping * 10);
            _axisNumberChooser[2]->setHorizontalAlignment(iHorizontalAlignment::Strech);
            _axisNumberChooser[2]->setValue(_graphs[1][0]._y);
            _axisNumberChooser[2]->setAfterPoint(2);
            _axisNumberChooser[2]->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onValueChanged));
            
            axisGrid->addWidget(labelY2, 0, 2);
            axisGrid->addWidget(_axisNumberChooser[2], 1, 2);
        }

        if (_graphs.size() > 2)
        {
            iWidgetLabel* labelY3 = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
            _allWidgets.push_back(labelY3);
            labelY3->setHorizontalAlignment(iHorizontalAlignment::Left);
            labelY3->setText(_axisNames[3]);
            labelY3->setWidth(100);

            _axisNumberChooser[3] = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
            _allWidgets.push_back(_axisNumberChooser[3]);
            _axisNumberChooser[3]->setMinMaxNumber(_xMin, _xMax);
            _axisNumberChooser[3]->setStepping(_xStepping, _xStepping);
            _axisNumberChooser[3]->setSteppingWheel(_xStepping * 10, _xStepping * 10);
            _axisNumberChooser[3]->setHorizontalAlignment(iHorizontalAlignment::Strech);
            _axisNumberChooser[3]->setValue(_graphs[2][0]._y);
            _axisNumberChooser[3]->setAfterPoint(2);
            _axisNumberChooser[3]->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onValueChanged));

            axisGrid->addWidget(labelY3, 0, 3);
            axisGrid->addWidget(_axisNumberChooser[3], 1, 3);
        }

        iWidgetButton* delButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
        _allWidgets.push_back(delButton);
        delButton->setText("Delete Value");
        delButton->setHorizontalAlignment(iHorizontalAlignment::Right);
        delButton->registerOnClickEvent(iClickDelegate(this, &iDialogGraph::onDelete));
        axisGrid->addWidget(delButton, 1, 4);

        iWidgetGrid* buttonGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _allWidgets.push_back(buttonGrid);
        buttonGrid->appendCollumns(2);
        buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);

        iWidgetButton* okButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
        _allWidgets.push_back(okButton);
        okButton->setText("OK");
        okButton->registerOnClickEvent(iClickDelegate(this, &iDialogGraph::onOK));

        iWidgetButton* cancelButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
        _allWidgets.push_back(cancelButton);
        cancelButton->setText("Cancel");
        cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogGraph::onCancel));

        iWidgetButton* resetButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
        _allWidgets.push_back(resetButton);
        resetButton->setText("Reset");
        resetButton->registerOnClickEvent(iClickDelegate(this, &iDialogGraph::onReset));

        addWidget(_grid);

        _grid->addWidget(_titleLabel, 0, 0);
        _grid->addWidget(groupBoxGradient, 0, 1);
        _grid->addWidget(groupBoxSelection, 0, 2);
        _grid->addWidget(buttonGrid, 0, 3);

        groupBoxGradient->addWidget(_graph);
        groupBoxSelection->addWidget(axisGrid);

        buttonGrid->addWidget(resetButton, 0, 0);
        buttonGrid->addWidget(cancelButton, 1, 0);
        buttonGrid->addWidget(okButton, 2, 0);

        updateGraph();
    }

    void iDialogGraph::onGraphChanged(iWidget* source)
    {
        for (int i = 0; i < _graph->getGraphCount(); ++i)
        {
            _graphs[i] = _graph->getPoints(i);
        }

        updateGraph();
        updateSelection();
    }

    void iDialogGraph::onSelectionChanged(int32 index)
    {
        con_assert(index >= 0 && index < _graphs[0].size(), "out of range");

        if (index >= 0 && index < _graphs[0].size())
        {
            _selectedValueIndex = index;
            updateSelection();
        }
    }

    void iDialogGraph::onValueChanged(iWidget* source)
    {
        if (_axisNumberChooser[0] == source)
        {
            for (int i = 0; i < _graphs.size(); ++i)
            {
                _graphs[i][_selectedValueIndex]._x = _axisNumberChooser[0]->getValue();
            }
        }
        else if (_axisNumberChooser[1] == source)
        {
            _graphs[0][_selectedValueIndex]._y = _axisNumberChooser[1]->getValue();
        }
        else if (_axisNumberChooser[2] == source)
        {
            _graphs[1][_selectedValueIndex]._y = _axisNumberChooser[2]->getValue();
        }
        else if (_axisNumberChooser[3] == source)
        {
            _graphs[2][_selectedValueIndex]._y = _axisNumberChooser[3]->getValue();
        }

        updateGraph();
    }

    void iDialogGraph::setAxisName(uint32 index, const iaString& name)
    {
        _axisNames[index] = name;
    }
    
    void iDialogGraph::setTitle(const iaString& title)
    {
        _title = title;
        if (_titleLabel != nullptr)
        {
            _titleLabel->setText(_title);
        }
    }

    iaString iDialogGraph::getTitle() const
    {
        return _title;
    }

    void iDialogGraph::updateGraph()
    {
        _graph->clearPoints();
        _graph->setLineColor(0, iaColor4f(1.0f, 0.0f, 0.0f, 1.0f));

        if (_graphs.size() > 1)
        {
            _graph->setLineColor(1, iaColor4f(0.0f, 1.0f, 0.0f, 1.0f));
        }

        if (_graphs.size() > 2)
        {
            _graph->setLineColor(1, iaColor4f(0.0f, 0.0f, 1.0f, 1.0f));
        }

        int32 i = 0;
        for (auto graph : _graphs)
        {
            _graph->setPoints(i++, graph);
        }
    }

    void iDialogGraph::updateSelection()
    {
        _axisNumberChooser[0]->setValue(_graphs[0][_selectedValueIndex]._x);
        _axisNumberChooser[1]->setValue(_graphs[0][_selectedValueIndex]._y);

        if (_graphs.size() > 1)
        {
            _axisNumberChooser[2]->setValue(_graphs[1][_selectedValueIndex]._y);
        }

        if (_graphs.size() > 2)
        {
            _axisNumberChooser[3]->setValue(_graphs[2][_selectedValueIndex]._y);
        }
    }

    void iDialogGraph::onOK(iWidget* source)
    {
        close();
        _closeEvent(true, _graphs);
        _closeEvent.clear();
    }

    void iDialogGraph::onDelete(iWidget* source)
    {
        if (_graphs[0].size() > 1)
        {
            for (int i = 0; i < _graphs.size(); ++i)
            {
                _graphs[i].erase(_graphs[i].begin() + _selectedValueIndex);
            }

            _selectedValueIndex = 0;
            updateGraph();
            updateSelection();
        }
    }

    void iDialogGraph::onCancel(iWidget* source)
    {
        close();
        _closeEvent(false, _graphs);
        _closeEvent.clear();
    }

    void iDialogGraph::onReset(iWidget* source)
    {
        _graphs = _oldGraphs;
        _selectedValueIndex = 0;
        updateGraph();
        updateSelection();
    }

    void iDialogGraph::close()
    {
        setActive(false);
        setVisible(false);
        iWidgetManager::resetModal();
    }
}