//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
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

#ifndef USERCONTROL_RESOURCE_H
#define USERCONTROL_RESOURCE_H

#include <igor/igor.h>
using namespace iaux;
using namespace igor;

class UserControlResource : public iUserControl
{
public:
    /*! init user control

    \param resourceID the resource id to use
    \param parent the optional parent widget
    */
    UserControlResource(iResourceID resourceID, const iWidgetPtr parent = nullptr);

    /*! does nothing
     */
    virtual ~UserControlResource() = default;

    /*! \returns id of node that is represented by this widget
     */
    iResourceID getResourceID() const;

    /*! init ui
     */
    virtual void init();

    /*! update ui with node data
     */
    virtual void update();

    /*! update node with ui data
     */
    virtual void updateResource();

protected:
    /*! \returns main layout so deriving classes can access it
     */
    iWidgetBoxLayoutPtr getLayout();

private:
    /*! resource id
     */
    iResourceID _resourceID;

    /*! main layout
     */
    iWidgetBoxLayoutPtr _layout = nullptr;

    /*! text field type
     */
    iWidgetLineTextEdit *_textType = nullptr;

    /*! text field alias
     */
    iWidgetLineTextEdit *_textAlias = nullptr;

    /*! text field source
     */
    iWidgetLineTextEdit *_textID = nullptr;

    /*! text field source
     */
    iWidgetLineTextEdit *_textSource = nullptr;

    /*! handled alias change

    \param source the source widget that triggered this event
    */
    void onAliasChanged(const iWidgetPtr source);
};

#endif // USERCONTROL_RESOURCE_H