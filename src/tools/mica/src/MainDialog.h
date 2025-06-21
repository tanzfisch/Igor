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
IGOR_EVENT_DEFINITION_NO_ARGS(CloseProject);

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
    CreateProjectEvent &getCreateProjectEvent();

    /*! \returns load project event
     */
    LoadProjectEvent &getLoadProjectEvent();

    /*! \returns save project event
     */
    SaveProjectEvent &getSaveProjectEvent();

    /*! \returns close project event
     */
    CloseProjectEvent &getCloseProjectEvent();

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

    /*! save project event
     */
    CloseProjectEvent _closeProject;

    /*! play button
     */
    iWidgetButtonPtr _playStopButton = nullptr;

    /*! init ui
     */
    void onInitUI();

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

    /*! triggers close project event

    \param source the event source
    */
    void onCloseProject(const iWidgetPtr source);

    /*! creates main application menu

    \returns created menu
    */
    iWidgetMenuBarPtr createMenu();

    /*! prints project tree to console

    \param source the event source
     */
    void onPrintProjectTree(const iWidgetPtr source);

    /*! on pre recent project menu open

    \param menu the recent project menu
    */
    void onRecentProjectOpen(iWidgetMenuPtr menu);

    /*! on play button pressed
     */
    void onPlayStop(iWidgetPtr source);

    /*! called on any other event
     */
    bool onEvent(const iEvent &event) override;

    /*! on project loaded event

    \param event the event data
    \returns true if event was consumed
    */
    bool onProjectLoaded(const iEventProjectLoaded &event);

    /*! on project unloaded event

    \param event the event data
    \returns true if event was consumed
    */
    bool onProjectUnloaded(const iEventProjectUnloaded &event);
};

#endif // MICA_MAIN_LAYOUT_H
