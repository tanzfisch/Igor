// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlNode.h"

#include "../../MicaDefines.h"

UserControlNode::UserControlNode(iNodeID nodeID, const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent), _nodeID(nodeID)
{
    con_assert(iNodeManager::getInstance().getNode(_nodeID) != nullptr, "invalid node id");
}

void UserControlNode::init()
{
    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    _layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    _layout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _layout->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetBoxLayoutPtr nameLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    nameLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    nameLayout->setStretchIndex(1);
    iWidgetLabelPtr labelName = new iWidgetLabel(nameLayout);
    labelName->setText("Name");
    labelName->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelName->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textName = new iWidgetLineTextEdit(nameLayout);
    _textName->setMaxTextLength(256);
    _textName->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textName->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textName->setText("");
    _textName->registerOnChangeEvent(iChangeDelegate(this, &UserControlNode::onNameChanged));

    iWidgetBoxLayoutPtr activeLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    iWidgetLabelPtr labelActive = new iWidgetLabel(activeLayout);
    labelActive->setText("Active");
    labelActive->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelActive->setHorizontalAlignment(iHorizontalAlignment::Left);

    _checkBoxActive = new iWidgetCheckBox(activeLayout);
    _checkBoxActive->setText("");
    _checkBoxActive->setEnabled(false);
    _checkBoxActive->setHorizontalAlignment(iHorizontalAlignment::Left);
}

iWidgetBoxLayoutPtr UserControlNode::getLayout()
{
    return _layout;
}

void UserControlNode::update()
{
    iNodeMesh *node = static_cast<iNodeMesh *>(iNodeManager::getInstance().getNode(_nodeID));
    if (node == nullptr)
    {
        return;
    }

    _textName->setWriteProtected(!node->isChild());
    _textName->setText(node->getName());
    _checkBoxActive->setChecked(node->isActive());
}

void UserControlNode::updateNode()
{
    iNodeMesh *node = static_cast<iNodeMesh *>(iNodeManager::getInstance().getNode(_nodeID));
    if (node == nullptr)
    {
        return;
    }

    node->setName(_textName->getText());
}

void UserControlNode::onNameChanged(const iWidgetPtr source)
{
    updateNode();
    _nameChangedEvent();
}

NameChangedEvent &UserControlNode::getNameChangedEvent()
{
    return _nameChangedEvent;
}

iNodeID UserControlNode::getNodeID() const
{
    return _nodeID;
}