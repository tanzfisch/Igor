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
    IGOR_EVENT_DEFINITION(iProjectSceneAdded, const iResourceID &);

    /*! project scene removed event
     */
    IGOR_EVENT_DEFINITION(iProjectSceneRemoved, const iResourceID &);

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
        /*! opens project

        closes active project if any

        \param path the given project file or folder
        \returns project
        */
        void load(const iaString &path);

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

        /*! \returns scenes folder
        */
        const iaString getScenesPath() const;

        /*! \returns project name
         */
        const iaString &getName() const;

        /*! sets project name
         */
        void setName(const iaString &projectName);

        /*! \returns true if changes been made and not saved
         */
        bool hasChanges() const;

        /*! \returns true if a project currently is loaded
         */
        bool isLoaded() const;

        /*! add scene to project

        \param sceneID the scene to add (aka type prefab)
        \param name name of the scene
        \param active if scene is active when added
        */
        void addScene(const iResourceID &sceneID, const iaString &name = "scene", bool active = true);

        /*! remove scene from project

        \param sceneID the scene/prefab to remove
        */
        void removeScene(const iResourceID &sceneID);

        /*! \returns list of scene references
         */
        const std::vector<iResourceID> &getScenes() const;

        /*! \returns the project scene
         */
        iEntityScenePtr getProjectScene() const;

        /*! \returns project scene added event
         */
        iProjectSceneAddedEvent &getProjectSceneAddedEvent();

        /*! \returns project scene removed event
         */
        iProjectSceneRemovedEvent &getProjectSceneRemovedEvent();

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
        iEntityScenePtr _projectScene = nullptr;

        /*! if true project configuration has changes
         */
        bool _hasChanges = false;

        /*! true if project is loaded
         */
        bool _isLoaded = false;

        /*! project scene added event
         */
        iProjectSceneAddedEvent _projectSceneAddedEvent;

        /*! project scene added event
         */
        iProjectSceneRemovedEvent _projectSceneRemovedEvent;

        /*! scenes references by project
         */
        std::vector<iResourceID> _scenes;

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
