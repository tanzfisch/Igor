//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2014-2020 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: igorgameengine@protonmail.com

#ifndef __USERCONTROLNODE__
#define __USERCONTROLNODE__

#include <igor/igor.h>
using namespace iaux;
using namespace igor;

IGOR_EVENT_DEFINITION(NameChanged, void);

class UserControlNode : public iUserControl
{
public:
    /*! init user control

    \param parent the optional parent widget
    */
    UserControlNode(iNodeID nodeID, const iWidgetPtr parent = nullptr);

    /*! does nothing
    */
    virtual ~UserControlNode() = default;

    /*! \returns name change event
    */
    NameChangedEvent& getNameChangedEvent();

    /*! \returns node id the is represented by this widget
    */
    iNodeID getNodeID() const;

    /*! init ui
    */
    virtual void init();

    /*! update ui with node data
    */
    virtual void update();

    /*! update node with ui data
    */
    virtual void updateNode();

protected:
    /*! main layout
    */
    iWidgetBoxLayoutPtr _layout = nullptr;

private:
    /*! name changed event
    */
    NameChangedEvent _nameChangedEvent;

    /*! text field name 
    */
    iWidgetLineTextEdit *_textName = nullptr;

    /*! active checkbox
    */
    iWidgetCheckBox *_checkBoxActive = nullptr;

    /*! id of node this control is displaying
    */
    iNodeID _nodeID = iNode::INVALID_NODE_ID;

    /*! handled name change

    \param source the source widget that triggered this event
    */
    void onNameChanged(const iWidgetPtr source);

};

#endif