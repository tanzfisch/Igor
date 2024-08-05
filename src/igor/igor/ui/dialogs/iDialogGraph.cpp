// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialogGraph.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/layouts/iWidgetGridLayout.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetCheckBox.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetGraph.h>
#include <igor/ui/widgets/iWidgetNumberChooser.h>
#include <igor/ui/widgets/iWidgetGroupBox.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iDialogGraph::iDialogGraph(const iWidgetPtr parent)
        : iDialog(iWidgetType::iDialogGraph, parent)
    {
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

    void iDialogGraph::open(iDialogCloseDelegate dialogCloseDelegate, const std::vector<std::vector<iaVector2f>> &graphs)
    {
        iDialog::open(dialogCloseDelegate, true);

        con_assert(!graphs.empty() && !graphs[0].empty(), "invalid data");

        if (graphs.empty() ||
            graphs[0].empty())
        {
            con_err("invalid graph data");
            return;
        }

        _selectedValueIndex = 0;

        _oldGraphs = graphs;
        _graphs = graphs;

        initGUI();
    }

    void iDialogGraph::initGUI()
    {
        setHorizontalAlignment(iHorizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);

        setEnabled();
        setVisible();
        setMinWidth(450);
        setMinHeight(20);

        iWidgetGridLayoutPtr grid = new iWidgetGridLayout(this);
        grid->appendRows(3);
        grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        grid->setVerticalAlignment(iVerticalAlignment::Stretch);
        grid->setCellSpacing(4);
        grid->setBorder(4);

        iWidgetGroupBox *groupBoxGradient = new iWidgetGroupBox();
        groupBoxGradient->setText("Gradient");
        groupBoxGradient->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        groupBoxGradient->setVerticalAlignment(iVerticalAlignment::Stretch);

        _graph = new iWidgetGraph();
        _graph->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        _graph->setMinHeight(120);
        _graph->setInteractive();
        _graph->setExtrapolateData();
        _graph->setViewGrid();
        _graph->setViewLabels();
        _graph->registerOnSelectionChangedEvent(iSelectionChangedDelegate(this, &iDialogGraph::onSelectionChanged));
        _graph->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onGraphChanged));

        iWidgetGroupBox *groupBoxSelection = new iWidgetGroupBox();
        groupBoxSelection->setText("Selected Value");
        groupBoxSelection->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        groupBoxSelection->setVerticalAlignment(iVerticalAlignment::Stretch);

        iWidgetGridLayout *axisGrid = new iWidgetGridLayout();
        axisGrid->appendRows(static_cast<uint32>(_graphs.size() + 2));
        axisGrid->appendColumns(1);
        axisGrid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        axisGrid->setVerticalAlignment(iVerticalAlignment::Stretch);
        axisGrid->setStretchColumn(1);
        axisGrid->setCellSpacing(4);
        axisGrid->setBorder(4);

        iWidgetLabel *labelX = new iWidgetLabel();
        labelX->setHorizontalAlignment(iHorizontalAlignment::Left);
        labelX->setText(_axisNames[0]);
        labelX->setMinWidth(100);

        _axisNumberChooser[0] = new iWidgetNumberChooser();
        _axisNumberChooser[0]->setMinMaxNumber(_xMin, _xMax);
        _axisNumberChooser[0]->setStepping(_xStepping, _xStepping);
        _axisNumberChooser[0]->setSteppingWheel(_xStepping * 10, _xStepping * 10);
        _axisNumberChooser[0]->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        _axisNumberChooser[0]->setValue(_graphs[0][0]._x);
        _axisNumberChooser[0]->setAfterPoint(_afterPoint);
        _axisNumberChooser[0]->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onValueChanged));

        axisGrid->addWidget(labelX, 0, 0);
        axisGrid->addWidget(_axisNumberChooser[0], 1, 0);

        iWidgetLabel *labelY1 = new iWidgetLabel();
        labelY1->setHorizontalAlignment(iHorizontalAlignment::Left);
        labelY1->setText(_axisNames[1]);
        labelY1->setMinWidth(100);

        _axisNumberChooser[1] = new iWidgetNumberChooser();
        _axisNumberChooser[1]->setMinMaxNumber(_yMin, _yMax);
        _axisNumberChooser[1]->setStepping(_yStepping, _yStepping);
        _axisNumberChooser[1]->setSteppingWheel(_yStepping * 10, _yStepping * 10);
        _axisNumberChooser[1]->setHorizontalAlignment(iHorizontalAlignment::Stretch);
        _axisNumberChooser[1]->setValue(_graphs[0][0]._y);
        _axisNumberChooser[1]->setAfterPoint(_afterPoint);
        _axisNumberChooser[1]->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onValueChanged));

        axisGrid->addWidget(labelY1, 0, 1);
        axisGrid->addWidget(_axisNumberChooser[1], 1, 1);

        if (_graphs.size() > 1)
        {
            iWidgetLabel *labelY2 = new iWidgetLabel();
            labelY2->setHorizontalAlignment(iHorizontalAlignment::Left);
            labelY2->setText(_axisNames[2]);
            labelY2->setMinWidth(100);

            _axisNumberChooser[2] = new iWidgetNumberChooser();
            _axisNumberChooser[2]->setMinMaxNumber(_yMin, _yMax);
            _axisNumberChooser[2]->setStepping(_yStepping, _yStepping);
            _axisNumberChooser[2]->setSteppingWheel(_yStepping * 10, _yStepping * 10);
            _axisNumberChooser[2]->setHorizontalAlignment(iHorizontalAlignment::Stretch);
            _axisNumberChooser[2]->setValue(_graphs[1][0]._y);
            _axisNumberChooser[2]->setAfterPoint(_afterPoint);
            _axisNumberChooser[2]->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onValueChanged));

            axisGrid->addWidget(labelY2, 0, 2);
            axisGrid->addWidget(_axisNumberChooser[2], 1, 2);
        }

        if (_graphs.size() > 2)
        {
            iWidgetLabel *labelY3 = new iWidgetLabel();
            labelY3->setHorizontalAlignment(iHorizontalAlignment::Left);
            labelY3->setText(_axisNames[3]);
            labelY3->setMinWidth(100);

            _axisNumberChooser[3] = new iWidgetNumberChooser();
            _axisNumberChooser[3]->setMinMaxNumber(_yMin, _yMax);
            _axisNumberChooser[3]->setStepping(_yStepping, _yStepping);
            _axisNumberChooser[3]->setSteppingWheel(_yStepping * 10, _yStepping * 10);
            _axisNumberChooser[3]->setHorizontalAlignment(iHorizontalAlignment::Stretch);
            _axisNumberChooser[3]->setValue(_graphs[2][0]._y);
            _axisNumberChooser[3]->setAfterPoint(_afterPoint);
            _axisNumberChooser[3]->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onValueChanged));

            axisGrid->addWidget(labelY3, 0, 3);
            axisGrid->addWidget(_axisNumberChooser[3], 1, 3);
        }

        iWidgetButton *delButton = new iWidgetButton();
        delButton->setText("Delete Value");
        delButton->setHorizontalAlignment(iHorizontalAlignment::Right);
        delButton->registerOnClickEvent(iClickDelegate(this, &iDialogGraph::onDelete));
        axisGrid->addWidget(delButton, 1, static_cast<uint32>(_graphs.size() + 1));

        iWidgetGridLayout *buttonGrid = new iWidgetGridLayout();
        buttonGrid->appendColumns(2);
        buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);

        iWidgetButton *okButton = new iWidgetButton();
        okButton->setText("OK");
        okButton->registerOnClickEvent(iClickDelegate(this, &iDialogGraph::onOK));

        iWidgetButton *cancelButton = new iWidgetButton();
        cancelButton->setText("Cancel");
        cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogGraph::onCancel));

        iWidgetButton *resetButton = new iWidgetButton();
        resetButton->setText("Reset");
        resetButton->registerOnClickEvent(iClickDelegate(this, &iDialogGraph::onReset));

        grid->addWidget(groupBoxGradient, 0, 0);
        grid->addWidget(groupBoxSelection, 0, 1);
        grid->addWidget(buttonGrid, 0, 2);

        groupBoxGradient->addWidget(_graph);
        groupBoxSelection->addWidget(axisGrid);

        buttonGrid->addWidget(resetButton, 0, 0);
        buttonGrid->addWidget(cancelButton, 1, 0);
        buttonGrid->addWidget(okButton, 2, 0);

        updateGraph();
    }

    void iDialogGraph::setAfterPoint(int32 afterPoint)
    {
        _afterPoint = afterPoint;
    }

    int32 iDialogGraph::getAfterPoint() const
    {
        return _afterPoint;
    }

    void iDialogGraph::onGraphChanged(const iWidgetPtr source)
    {
        for (int i = 0; i < _graph->getGraphCount(); ++i)
        {
            _graphs[i] = _graph->getPoints(i);
        }

        updateGraph();
        updateSelection();
    }

    void iDialogGraph::onSelectionChanged(const iWidgetPtr source)
    {   
        iWidgetGraphPtr graph = static_cast<iWidgetGraphPtr>(source);

        int32 index = graph->getSelectedIndex();

        con_assert(index >= 0 && index < _graphs[0].size(), "out of range");

        if (index >= 0 && index < _graphs[0].size())
        {
            _selectedValueIndex = index;
            updateSelection();
        }
    }

    void iDialogGraph::onValueChanged(const iWidgetPtr source)
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

    void iDialogGraph::setAxisName(uint32 index, const iaString &name)
    {
        _axisNames[index] = name;
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

    void iDialogGraph::onOK(const iWidgetPtr source)
    {
        setReturnState(iDialogReturnState::Ok);
        close();
    }

    void iDialogGraph::onDelete(const iWidgetPtr source)
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

    void iDialogGraph::onCancel(const iWidgetPtr source)
    {
        setReturnState(iDialogReturnState::Cancel);
        close();
    }

    void iDialogGraph::onReset(const iWidgetPtr source)
    {
        _graphs = _oldGraphs;
        _selectedValueIndex = 0;
        updateGraph();
        updateSelection();
    }

    const std::vector<std::vector<iaVector2f>> &iDialogGraph::getGraphs() const
    {
        return _graphs;
    }

} // namespace igor