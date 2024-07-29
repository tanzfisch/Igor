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

#ifndef USERCONTROL_RESOURCE_ICON_H
#define USERCONTROL_RESOURCE_ICON_H

#include <igor/igor.h>
using namespace igor;

/*! an icon that represents a resource

adding information about the resources
*/
class UserControlResourceIcon : public iUserControl
{
public:
    UserControlResourceIcon(const iWidgetPtr parent = nullptr);

    /*! sets the resource filename

    \param filename the filename of the resource
    */
    void setFilename(const iaString &filename);

    /*! refresh thumbnail
     */
    void refresh();

    /*! \returns resource id
    */
    iResourceID getResourceID() const;

private:
    /*! the file name
     */
    iaString _filename;

    /*! picture (thumbnail)
     */
    iWidgetPicturePtr _picture;

    /*! indicator picture if asset is in dictionary
     */
    iWidgetPicturePtr _dictPicture;

    /*! label with filename
     */
    iWidgetLabelPtr _label;

    /*! the context menu
     */
    iDialogMenu _contextMenu;

    /*! id of resource this icon represents
    */
    iResourceID _resourceID;

    /*! init ui
     */
    void initGUI();

    /*! create menu based on context menu event

    \param source the calling withget
    */
    void OnContextMenu(iWidgetPtr source);

    /*! adds given resource to dictionary

    \param source the source widget of this event
    */
    void onAddDictionary(iWidgetPtr source);

    /*! unregister asset from dictionary

    \param source the source widget of this event
    */
    void onRemoveDictionary(iWidgetPtr source);

    /*! updates the visual dictionary state
    */
    void updateDictionaryState();

    /*! draws the widget
        */
    void draw() override;

    /*! called when dragged
    */
    void onDrag() override;     
};

#endif // USERCONTROL_RESOURCE_ICON_H