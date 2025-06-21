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

#ifndef IGOR_PROJECT_H
#define IGOR_PROJECT_H

#include <igor/resources/iResourceManager.h>
#include <igor/resources/module/iModule.h>

#include <iaux/system/iaEvent.h>
#include <iaux/data/iaString.h>
using namespace iaux;

#include <memory>

namespace igor
{

    /*! project scene added event
     */
    IGOR_EVENT_DEFINITION(iSceneAdded, const iResourceID &);

    /*! project scene removed event
     */
    IGOR_EVENT_DEFINITION(iSceneRemoved, const iResourceID &);

    /*! project pointer definition
     */
    class iProject;
    typedef std::shared_ptr<iProject> iProjectPtr;

    /*! project definition
     */
    class IGOR_API iProject : public iModule<iProject>
    {
        friend class iModule<iProject>;

    public:

        /*! project mode
        */
        enum class iMode
        {
            Edit,
            Runtime
        };  

        /*! opens project

        closes active project if any

        \param path the given project file or folder
        \returns project        
        */
        void load(const iaString &path, iMode mode = iMode::Runtime);

        /*! \returns true if a project currently is loaded
         */
        bool isLoaded() const;

        /*! unloads project
         */
        void unload();

        /*! creates new project in given project folder and loads it

        \param path given project folder
        */
        void create(const iaString &path);

        /*! saves existing project

        saves it in to existing project folder
        */
        void save();

        /*! \returns project folder
         */
        const iaString &getProjectPath() const;

        /*! \returns project file path
         */
        const iaString getProjectFilepath() const;

        /*! \returns project name
         */
        const iaString &getName() const;

        /*! sets project name
         */
        void setName(const iaString &projectName);

        /*! add scene to project's root scene

        \param sceneID the scene to add (aka type prefab)
        \param name name of the scene
        \param active if scene is active when added
        */
        void addScene(const iResourceID &sceneID, const iaString &name = "scene", bool active = true);

        /*! remove scene from project root scene

        \param sceneID the scene/prefab to remove
        */
        void removeScene(const iResourceID &sceneID);

        /*! \returns project scene added event
         */
        iSceneAddedEvent &getSceneAddedEvent();

        /*! \returns project scene removed event
         */
        iSceneRemovedEvent &getSceneRemovedEvent();        

        /*! \returns list of all sub scenes in this project excluding root scene
         */
        const std::vector<iResourceID> &getScenes() const;

        /*! \returns the active root project scene
         */
        iEntityScenePtr getRootScene() const;

        /*! sets project mode

        this changes which copy of the root scene is used from here onwards

        \param mode the mode to switch to
        */
        void setProjectMode(iMode mode);

    private:
        /*! project folder
         */
        iaString _projectFolder;

        /*! project file
        */
        iaString _projectFile;

        /*! project name
         */
        iaString _projectName;

        /*! the root project scene all other scenes get added to as prefabs
         */
        iEntityScenePtr _activeScene = nullptr;

        /*! the edit mode scene
        */
        iEntityScenePtr _editScene = nullptr;

        /*! the runtime mode scene
        */
        iEntityScenePtr _runtimeScene = nullptr;

        /*! true if project is loaded
         */
        bool _isLoaded = false;

        /*! project scene added event
         */
        iSceneAddedEvent _sceneAddedEvent;

        /*! project scene added event
         */
        iSceneRemovedEvent _sceneRemovedEvent;

        /*! scenes referenced by project
         */
        std::vector<iResourceID> _scenes;      

        /*! current project mode
        */
        iMode _mode = iMode::Runtime;

        /*! loads project
         */
        void load();

        /*! reads project configuration

        \param filename the project configuration file
        */
        bool read(const iaString &filename);

        /*! read individual scene from json

        \param sceneJson the given json
        */
        bool readScene(const json &sceneJson);

        /*! write project configuration

        \param filename the project configuration file
        */
        bool write(const iaString &filename);
    };

} // namespace igor

#endif // IGOR_PROJECT_H
