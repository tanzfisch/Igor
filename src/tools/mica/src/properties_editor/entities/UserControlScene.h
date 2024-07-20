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

#ifndef USERCONTROL_SCENE_H
#define USERCONTROL_SCENE_H

#include <igor/igor.h>
using namespace iaux;
using namespace igor;

/*! user control scene
*/
class UserControlScene : public iUserControl
{
public:
    /*! init user control

    \param sceneID scene id
    \param parent the optional parent widget
    */
    UserControlScene(iEntitySceneID sceneID, const iWidgetPtr parent = nullptr);

    /*! does nothing
     */
    virtual ~UserControlScene() = default;

    /*! \returns scene id
     */
    iEntitySceneID getSceneID() const;

    /*! init ui
     */
    virtual void init();

    /*! update ui with node data
     */
    virtual void update();

    /*! update scene
     */
    virtual void updateScene();

private:
    /*! the scene id
     */
    iEntitySceneID _sceneID;

    /*! flag to prevent endless update loop
     */
    bool _ignoreUpdate = false;    

    /*! main layout
     */
    iWidgetBoxLayoutPtr _layout = nullptr;

    /*! text field type
     */
    iWidgetLineTextEdit *_textName = nullptr;
};

#endif // USERCONTROL_ENTITY_H