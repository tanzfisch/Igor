// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlMesh.h"

#include "../../MicaDefines.h"

#include <igor/igor.h>
using namespace igor;

UserControlMesh::UserControlMesh(iNodeID nodeID, const iWidgetPtr parent)
    : UserControlNode(nodeID, parent)
{
}

void UserControlMesh::updateNode()
{
    if (_ignoreNodeUpdate)
    {
        return;
    }

    iNodeMesh *node = static_cast<iNodeMesh *>(iNodeManager::getInstance().getNode(getNodeID()));
    if (node == nullptr)
    {
        return;
    }

    iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>(_materialChooser->getID());
    node->setMaterial(material);
}

void UserControlMesh::update()
{
    UserControlNode::update();

    _ignoreNodeUpdate = true;

    iNodeMesh *node = static_cast<iNodeMesh *>(iNodeManager::getInstance().getNode(getNodeID()));
    if (node->getMaterial() != nullptr)
    {
        _materialChooser->setID(node->getMaterial()->getID());
    }

    iMeshPtr mesh = node->getMesh();
    _textVertices->setText(iaString::toString(mesh->getVertexCount()));
    _textTriangles->setText(iaString::toString(mesh->getTrianglesCount()));
    _textIndices->setText(iaString::toString(mesh->getIndexCount()));

    _ignoreNodeUpdate = false;
}

void UserControlMesh::init()
{
    UserControlNode::init();

    iWidgetBoxLayoutPtr layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, getLayout());
    layout->setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetGroupBoxPtr materialGroup = new iWidgetGroupBox(layout);
    materialGroup->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    materialGroup->setVerticalAlignment(iVerticalAlignment::Top);
    materialGroup->setHeaderOnly();
    materialGroup->setText("Material");

    iWidgetBoxLayoutPtr materialLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, materialGroup);

    iWidgetLabel *labelMaterial = new iWidgetLabel(materialLayout);
    labelMaterial->setText("Material");
    labelMaterial->setHorizontalAlignment(iHorizontalAlignment::Left);
    _materialChooser = new iUserControlMaterial(materialLayout);
    _materialChooser->registerOnChangeEvent(iChangeDelegate(this, &UserControlMesh::onDoUpdateNode));

    iWidgetGroupBoxPtr detailsGroup = new iWidgetGroupBox(layout);
    detailsGroup->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    detailsGroup->setVerticalAlignment(iVerticalAlignment::Top);
    detailsGroup->setHeaderOnly();
    detailsGroup->setText("Topology");

    iWidgetBoxLayoutPtr detailsLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, detailsGroup);
    detailsLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr layoutVertices = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, detailsLayout);
    layoutVertices->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    layoutVertices->setStretchIndex(1);

    iWidgetLabel *labelVertexes = new iWidgetLabel(layoutVertices);
    labelVertexes->setText("Vertices");
    labelVertexes->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelVertexes->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textVertices = new iWidgetLineTextEdit(layoutVertices);
    _textVertices->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textVertices->setHorizontalTextAlignment(iHorizontalAlignment::Right);
    _textVertices->setEnabled(false);
    _textVertices->setText("");

    iWidgetBoxLayoutPtr layoutTriangles = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, detailsLayout);
    layoutTriangles->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    layoutTriangles->setStretchIndex(1);

    iWidgetLabel *labelTriangles = new iWidgetLabel(layoutTriangles);
    labelTriangles->setText("Triangles");
    labelTriangles->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelTriangles->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textTriangles = new iWidgetLineTextEdit(layoutTriangles);
    _textTriangles->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textTriangles->setHorizontalTextAlignment(iHorizontalAlignment::Right);
    _textTriangles->setEnabled(false);
    _textTriangles->setText("");

    iWidgetBoxLayoutPtr layoutIndices = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, detailsLayout);
    layoutIndices->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    layoutIndices->setStretchIndex(1);

    iWidgetLabel *labelIndices = new iWidgetLabel(layoutIndices);
    labelIndices->setText("Indexes");
    labelIndices->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelIndices->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textIndices = new iWidgetLineTextEdit(layoutIndices);
    _textIndices->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textIndices->setHorizontalTextAlignment(iHorizontalAlignment::Right);
    _textIndices->setEnabled(false);
    _textIndices->setText("");
}

void UserControlMesh::onDoUpdateNode(const iWidgetPtr source)
{
    updateNode();
}
