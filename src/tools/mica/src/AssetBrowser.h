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

#ifndef __ASSET_BROWSER__
#define __ASSET_BROWSER__

#include <igor/igor.h>
using namespace igor;

class AssetBrowser : public iDialog
{
    friend class iWidgetManager;

public:
    /*! init UI
     */
    AssetBrowser();

    /*! does nothing
     */
    ~AssetBrowser() = default;

    /*! sets project

    \param project the project to use
    */
    void setProject(iProjectPtr project);

    /*! \returns current project
     */
    iProjectPtr getProject() const;

private:
    /*! tree view
     */
    iUserControlTreeViewPtr _treeView = nullptr;

    /*! grid view
     */
    iWidgetFixedGridLayoutPtr _gridView = nullptr;

    /*! button to toggle content mode
    */
    iWidgetButtonPtr _showFilesButton = nullptr;

    /*! tree view data
     */
    std::unique_ptr<iItemData> _itemData;

    /*! the current project
     */
    iProjectPtr _project;

    /*! update handle for filesystem updates
     */
    iTimerHandle _updateHandle;

    /*! what data to show in the asset browser
    */
    enum class ContentMode
    {
        Files,
        Assets
    };

    /*! what data to show
    */
    ContentMode _contentMode = ContentMode::Assets;

    /*! init UI
     */
    void initUI();

    void update(const iaTime &time);

    void update(const iaDirectory &dir, iItemPtr item);

    void updateContentModeButton();
    void onClickShowAssetsButton(iWidgetPtr source);

    void onClickTreeView(const iWidgetPtr source);

    /*! updates grid view with given item data

    \param item the given item
    */
    void updateGridView(iItemPtr item);
};

#endif // __ASSET_BROWSER__
