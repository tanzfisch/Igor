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

#ifndef __WORKSPACE__
#define __WORKSPACE__

#include "CameraArc.h"

#include <memory>

/*! the mica workspace
*/
class Workspace
{

public:
    /*! init members
	*/
    Workspace();

    /*! deinit resources
	*/
    virtual ~Workspace();

    /*! \returns the user root
    */
    iNodePtr getRootUser() const;

    /*! \returns the mica root
    */
    iNodePtr getRootMica() const;

    /*! \returns scene
    */
    iScenePtr getScene() const;

    /*! empties the workspace
	*/
    void clear();

    /*! clear workspace and loads a new one

    \param filename the filename of the model/scene to load
    */
    void loadFile(const iaString &filename);

    /*! imports a file by reference so it does not actually become part of the scene once saved

    \param filename name of the file to be referenced
    */
    void importFileReference(const iaString &filename);

    /*! imports a file and make it part of the scene

    \param filename name of the file to be imported
    */
    void importFile(const iaString &filename);

    /*! \returns list of selected nodes
    */
    const std::vector<iNodeID> &getSelection() const;

    /*! sets the current selection
    */
    void setSelection(const std::vector<iNodeID> &selection);

    /*! clear current selection
    */
    void clearSelection();

    /*! duplicates selected nodes
    */
    void duplicateSelected();

    /*! cut selected nodes
    */
    void cutSelected();

    /*! copy selected nodes
    */
    void copySelected();

    /*! copy selected nodes
    */
    void pasteSelected();

    /*! delete selected nodes
    */
    void deleteSelected();

    /*! \returns the default camera
    */
    CameraArcPtr getCameraArc() const;

private:
    /*! the default camera
    */
    CameraArcPtr _cameraArc = nullptr;

    /*! main scene
	*/
    iScenePtr _scene = nullptr;

    /*! root node of user scene
    */
    iNodePtr _rootUser = nullptr;

    /*! root node of mica scene
    */
    iNodePtr _rootMica = nullptr;

    /*! nodes that have been marked to be cut so they can be pasted later
    */
    std::vector<iNodeID> _cutNodes;

    /*! nodes that have been marked to be copied onces they are pasted later
    */
    std::vector<iNodeID> _copiedNodes;
};

/*! workspace pointer definition
*/
typedef std::shared_ptr<Workspace> WorkspacePtr;

#endif // __WORKSPACE__
