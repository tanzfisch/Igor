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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef MICA_MAIN_LAYOUT_H
#define MICA_MAIN_LAYOUT_H

#include <igor/igor.h>
using namespace igor;

IGOR_EVENT_DEFINITION_NO_ARGS(CreateProject);
IGOR_EVENT_DEFINITION_NO_ARGS(LoadProject);
IGOR_EVENT_DEFINITION_NO_ARGS(SaveProject);

/*! main dialog
 */
class MainDialog : public iDialog
{
public:
    /*! init
     */
    MainDialog();

    /*! does nothing
     */
    ~MainDialog() = default;

    /*! \returns create project event
     */
    CreateProjectEvent &getEventCreateProject();

    /*! \returns load project event
     */
    LoadProjectEvent &getEventLoadProject();

    /*! \returns save project event
     */
    SaveProjectEvent &getEventSaveProject();

private:
    /*! create project event
     */
    CreateProjectEvent _createProject;

    /*! load project event
     */
    LoadProjectEvent _loadProject;

    /*! save project event
     */
    SaveProjectEvent _saveProject;

    /*! init ui
     */
    void initGUI();

    /*! triggers create project event

    \param source the event source
    */
    void onCreateProject(const iWidgetPtr source);

    /*! triggers load project event

    \param source the event source
    */
    void onLoadProject(const iWidgetPtr source);

    /*! triggers save project event

    \param source the event source
    */
    void onSaveProject(const iWidgetPtr source);

    /*! creates main application menu

    \returns created menu
    */
    iWidgetMenuBarPtr createMenu();
};

#endif // MICA_MAIN_LAYOUT_H
