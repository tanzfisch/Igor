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

    void iDialogGraph::setConfigurationXAxis(float32 xMin, float32 xMax, float32 xStepping)
    {
        _xMin = xMin;
        _xMax = xMax;
        _xStepping = xStepping;
    }

    void iDialogGraph::setConfigurationYAxis(float32 yMin, float32 yMax, float32 yStepping)
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
        setWidth(300);
        setHeight(20);

        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _allWidgets.push_back(_grid);
        _grid->appendRows(3);
        _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
        _grid->setVerticalAlignment(iVerticalAlignment::Strech);
        _grid->setCellSpacing(4);
        _grid->setBorder(4);

        iWidgetLabel* headerLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
        _allWidgets.push_back(headerLabel);
        headerLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
        headerLabel->setText("Graph Editor");

        _graph = static_cast<iWidgetGraph*>(iWidgetManager::getInstance().createWidget("Graph"));
        _allWidgets.push_back(_graph);
        _graph->setHorizontalAlignment(iHorizontalAlignment::Strech);
        _graph->setViewFrame();
        _graph->setExtrapolateData();
        _graph->setViewGrid();

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

        _grid->addWidget(headerLabel, 0, 0);
        _grid->addWidget(_graph, 0, 1);        
        _grid->addWidget(buttonGrid, 0, 3);

        buttonGrid->addWidget(resetButton, 0, 0);
        buttonGrid->addWidget(cancelButton, 1, 0);
        buttonGrid->addWidget(okButton, 2, 0);

        updateGraph();
        updateEntries();
    }

    void iDialogGraph::updateGraph()
    {
        _graph->clear();
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

    void iDialogGraph::updateEntries()
    {
        // clear grid 
        for (auto widget : _dataGridEntryWidgets)
        {
            iWidgetManager::getInstance().destroyWidget(widget);
        }
        _dataGridEntryWidgets.clear();

        _widgetLocationMatrix.clear();

        // build up grid again
        _dataGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _dataGridEntryWidgets.push_back(_dataGrid);
        _dataGrid->setHorizontalAlignment(iHorizontalAlignment::Strech);
        _dataGrid->setVerticalAlignment(iVerticalAlignment::Strech);
        _dataGrid->setCellSpacing(4);
        _dataGrid->setBorder(4);
        _dataGrid->appendRows(_graphs[0].size());

        for (int row = 0; row < _graphs[0].size(); ++row)
        {
            iWidgetGrid* entry = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
            entry->setSelectMode(iSelectionMode::NoSelection);
            entry->setBorder(0);
            entry->setCellSpacing(2);
            entry->appendCollumns(3);
            entry->setHorizontalAlignment(iHorizontalAlignment::Left);
            entry->setWidth(330);

            iWidgetNumberChooser* x = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
            _dataGridEntryWidgets.push_back(x);
            x->setMinMaxNumber(_xMin, _xMax);
            x->setStepping(_xStepping, _xStepping);
            x->setSteppingWheel(_xStepping * 10, _xStepping * 10);
            x->setValue(_graphs[0][row]._x);
            x->setAfterPoint(2);
            x->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onGraphUpdate));
            _widgetLocationMatrix[x->getID()] = iaVector2i(0, row);
            entry->addWidget(x, 0, 0);

            iWidgetNumberChooser* y1 = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
            _dataGridEntryWidgets.push_back(y1);
            y1->setMinMaxNumber(_yMin, _yMax);
            y1->setStepping(_yStepping, _yStepping);
            y1->setSteppingWheel(_yStepping * 10, _yStepping * 10);
            y1->setValue(_graphs[0][row]._y);
            y1->setAfterPoint(2);
            y1->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onGraphUpdate));
            _widgetLocationMatrix[y1->getID()] = iaVector2i(1, row);
            entry->addWidget(y1, 1, 0);

            if (_graphs.size() > 1)
            {
                iWidgetNumberChooser* y2 = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
                _dataGridEntryWidgets.push_back(y2);
                y2->setMinMaxNumber(_yMin, _yMax);
                y2->setStepping(_yStepping, _yStepping);
                y2->setSteppingWheel(_yStepping * 10, _yStepping * 10);
                y2->setValue(_graphs[1][row]._y);
                y2->setAfterPoint(2);
                y2->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onGraphUpdate));
                _widgetLocationMatrix[y2->getID()] = iaVector2i(2, row);
                entry->addWidget(y2, 2, 0);
            }

            if (_graphs.size() > 2)
            {
                iWidgetNumberChooser* y3 = static_cast<iWidgetNumberChooser*>(iWidgetManager::getInstance().createWidget("NumberChooser"));
                _dataGridEntryWidgets.push_back(y3);
                y3->setMinMaxNumber(_yMin, _yMax);
                y3->setStepping(_yStepping, _yStepping);
                y3->setSteppingWheel(_yStepping * 10, _yStepping * 10);
                y3->setValue(_graphs[2][row]._y);
                y3->setAfterPoint(2);
                y3->registerOnChangeEvent(iChangeDelegate(this, &iDialogGraph::onGraphUpdate));
                _widgetLocationMatrix[y3->getID()] = iaVector2i(3, row);
                entry->addWidget(y3, 2, 0);
            }

            _dataGrid->addWidget(entry, 0, row);
        }

        _grid->addWidget(_dataGrid, 0, 2);
    }

    void iDialogGraph::retriveGraphs(vector<vector<iaVector2f>>& graphs)
    {
        for (int i = 0; i < _graph->getGraphCount(); ++i)
        {
            vector<iaVector2f> temp = _graph->getPoints(i);
            graphs.push_back(temp);
        }
    }

    void iDialogGraph::onGraphUpdate(iWidget* source)
    {
        auto widgetMeta = _widgetLocationMatrix.find(source->getID());
        if (widgetMeta != _widgetLocationMatrix.end())
        {
            iWidgetNumberChooser* numberChooser = static_cast<iWidgetNumberChooser*>(source);
            float32 newValue = numberChooser->getValue();

            if ((*widgetMeta).second._x == 0)
            {
                int32 index = (*widgetMeta).second._y;

                for (int i = 0; i < _graphs.size(); ++i)
                {
                    _graphs[i][index]._x = newValue;
                }
            }
            else
            {
                _graphs[(*widgetMeta).second._x - 1][(*widgetMeta).second._y]._y = newValue;
            }

            updateGraph();
        }
        else
        {
            con_err("internal error");
        }
    }

    void iDialogGraph::onFullUpdate(iWidget* source)
    {
    }

    void iDialogGraph::onOK(iWidget* source)
    {
        close();
        _closeEvent(true, _graphs);
        _closeEvent.clear();
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
        updateEntries();
    }

    void iDialogGraph::close()
    {
        setActive(false);
        setVisible(false);
        iWidgetManager::resetModal();
    }
}