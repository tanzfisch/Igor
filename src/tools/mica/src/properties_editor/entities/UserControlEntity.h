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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef USERCONTROL_ENTITY_H
#define USERCONTROL_ENTITY_H

#include <igor/igor.h>
using namespace iaux;
using namespace igor;

class UserControlEntity : public iUserControl
{
public:
    /*! init user control

    When there is not entityID given the user control represents the scene it self

    \param sceneID scene id
    \param entityID entity id of entity within given scene
    \param parent the optional parent widget
    */
    UserControlEntity(iEntitySceneID sceneID, iEntityID entityID, const iWidgetPtr parent = nullptr);

    /*! does nothing
     */
    ~UserControlEntity() = default;

    /*! \returns scene id
     */
    iEntitySceneID getSceneID() const;

    /*! \returns entity id
     */
    iEntityID getEntityID() const;

    /*! init ui
     */
    void init();

    /*! update ui with node data
     */
    void update();

    /*! update entity
     */
    void updateEntity();

private:
    /*! the scene id
     */
    iEntitySceneID _sceneID = iEntitySceneID::getInvalid();

    /*! the entity id
     */
    iEntityID _entityID = iEntityID::getInvalid();

    /*! flag to prevent endless update loop
     */
    bool _ignoreUpdate = false;    

    /*! components layout
     */
    iWidgetBoxLayoutPtr _componentsLayout = nullptr;

    /*! text field name
     */
    iWidgetLineTextEditPtr _textName = nullptr;

    /*! text field id
     */
    iWidgetLineTextEditPtr _textID = nullptr;    

    /*! check box active
    */
    iWidgetCheckBoxPtr _checkBoxActive = nullptr;

    /*! add button
    */
    iWidgetButtonPtr _addComponent = nullptr;

    /*! called on add component button clicked

    \param source the source widget of the event
    */
    void onAddComponentClicked(iWidgetPtr source);

    /*! called when name was changed

    \param source the source widget of the event
    */
    void onNameChanged(iWidgetPtr source);

    /*! called when active checkbox was clicked

    \param source the source widget of the event
    */
    void onActiveChanged(iWidgetPtr source);
};

#endif // USERCONTROL_ENTITY_H