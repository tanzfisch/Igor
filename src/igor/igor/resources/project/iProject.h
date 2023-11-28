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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_PROJECT__
#define __IGOR_PROJECT__

#include <igor/iDefines.h>

#include <iaux/data/iaString.h>
using namespace iaux;

#include <memory>

namespace igor
{

    /*! project pointer definition
     */
    class iProject;
    typedef std::shared_ptr<iProject> iProjectPtr;

    /*! project definition
     */
    class IGOR_API iProject
    {
    public:
        /*! opens project from project folder

        \param projectFolder the given project folder
        \returns project
        */
        static iProjectPtr loadProject(const iaString &projectFolder);

        /*! creates new project in given project folder

        \param projectFolder given project folder
        \param projectName given project name
        \returns project
        */
        static iProjectPtr createProject(const iaString &projectFolder);

        /*! saves existing project

        \param project the project to save
        */
        static void saveProject(iProjectPtr project);

        /*! unloads project

        \param project the project to unload
        */
        static void unloadProject(iProjectPtr project);

        /*! \returns project folder
         */
        const iaString &getProjectFolder() const;

        /*! \returns project name
         */
        const iaString &getName() const;

        /*! sets project name
         */
        void setName(const iaString &projectName);

    private:
        /*! project folder
         */
        iaString _projectFolder;

        /*! project name
         */
        iaString _projectName;

        /*! loads project
        */
        void load();

        /*! loads project
        */
        void unload();

        /*! saves project
        */
        void save();

        /*! reads project configuration

        \param filename the project configuration file
        */
        bool readConfiguration(const iaString &filename);

        /*! write project configuration

        \param filename the project configuration file
        */
        bool writeConfiguration(const iaString &filename);

        /*! initialize a project based on it's project folder

        \param projectFolder the given project folder
        */
        iProject(const iaString &projectFolder);
    };

} // namespace igor

#endif // __IGOR_PROJECT__
