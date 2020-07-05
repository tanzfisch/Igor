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

#ifndef __WORKSPACE_H__
#define __WORKSPACE_H__

#include <igor/igor.h>
using namespace igor;

class Workspace
{

public:
    /*! nothing to do
	*/
    Workspace();

    /*! deinit resources
	*/
    virtual ~Workspace();

    /*! \returns the workspace root
    */
    iNodePtr getRoot() const;

    /*! \returns scene
    */
    iScenePtr getScene() const;

    /*! empties the workspace
	*/
    void clear();

    /*! \returns list of selected nodes
    */
    const std::vector<iNodeID> &getSelection() const;

    /*! sets the current selection
    */
    void setSelection(const std::vector<iNodeID> &selection);

    /*! clear current selection
    */
    void clearSelection();

private:
    /*! main scene
	*/
    iScenePtr _scene = nullptr;

    /*! currently selected nodes
	*/
    std::vector<iNodeID> _selectedNodes;
};

#endif // __WORKSPACE_H__
