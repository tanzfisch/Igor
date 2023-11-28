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

#ifndef __MAIN_LAYOUT__
#define __MAIN_LAYOUT__

#include "Workspace.h"

#include <igor/igor.h>
using namespace igor;

IGOR_EVENT_DEFINITION(CreateProject, void);
IGOR_EVENT_DEFINITION(LoadProject, void);
IGOR_EVENT_DEFINITION(SaveProject, void);
IGOR_EVENT_DEFINITION(LoadFile, void);
IGOR_EVENT_DEFINITION(SaveFile, void);

IGOR_EVENT_DEFINITION(CopyNode, void, uint64);
IGOR_EVENT_DEFINITION(PasteNode, void, uint64);
IGOR_EVENT_DEFINITION(CutNode, void, uint64);

/*! main dialog
 */
class MainDialog : public iDialog
{
public:
    MainDialog(WorkspacePtr workspace);
    ~MainDialog();

    CreateProjectEvent& getEventCreateProject();
    LoadProjectEvent& getEventLoadProject();
    SaveProjectEvent& getEventSaveProject();
    LoadFileEvent& getEventLoadFile();
    SaveFileEvent& getEventSaveFile();
    CopyNodeEvent& getEventCopyNode();
    CutNodeEvent& getEventCutNode();
    PasteNodeEvent& getEventPasteNode();

private:
    /*! the mica workspace
    */
    WorkspacePtr _workspace;

    CreateProjectEvent _createProject;
    LoadProjectEvent _loadProject;
    SaveProjectEvent _saveProject;

    LoadFileEvent _loadFile;
    SaveFileEvent _saveFile;

    CopyNodeEvent _copyNode;
    CutNodeEvent _cutNode;
    PasteNodeEvent _pasteNode;

    void initGUI();
    void deinitGUI();

    void onCreateProject(const iWidgetPtr source);
    void onLoadProject(const iWidgetPtr source);
    void onSaveProject(const iWidgetPtr source);

    void onLoadFile(const iWidgetPtr source);
    void onSaveFile(const iWidgetPtr source);

    void onCopy(const iWidgetPtr source);
    void onPaste(const iWidgetPtr source);
    void onCut(const iWidgetPtr source);    
    void onDelete(const iWidgetPtr source);

    iWidgetMenuBarPtr createMenu();
};

#endif // __MAIN_LAYOUT__
