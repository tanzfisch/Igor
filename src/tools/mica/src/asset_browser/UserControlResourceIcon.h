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
    /*! ctor

    \param filename the filename of the resource
    \param referencePath the optional reference path within the resource
    \param parent (optional) parent
    */
    UserControlResourceIcon(const iaString &filename, const iaString &referencePath = "", const iWidgetPtr parent = nullptr);

    /*! \returns resource id
    */
    iResourceID getResourceID() const;

    /*! \returns reference path within the resource

    ie a path to a mesh within a model resource
    */
    const iaString& getReferencePath() const;

private:
    /*! the file name
     */
    iaString _filename;

    /*! the reference path
    */
    iaString _referencePath;

    /*! picture (thumbnail)
     */
    iWidgetPicturePtr _picture;

    /*! indicator picture if asset is in dictionary
     */
    iWidgetPicturePtr _dictPicture;

    /*! label with filename
     */
    iWidgetLabelPtr _labelFilename;

    /*! label with filename
     */
    iWidgetLabelPtr _labelReferencePath;

    /*! the context menu
     */
    iDialogMenu _contextMenu;

    /*! id of resource this icon represents
    */
    iResourceID _resourceID;

    /*! init ui
     */
    void initGUI();

    /*! update ui values
    */
    void updateUI();

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

    /*! refresh thumbnail
     */
    void onRefresh() override;

};

#endif // USERCONTROL_RESOURCE_ICON_H