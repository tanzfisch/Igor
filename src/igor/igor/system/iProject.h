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
        /*! create project object from project folder

        \param projectFolder the given project folder
        \returns project object
        */
        static iProjectPtr loadProject(const iaString &projectFolder);

    private:
        /*! project folder
         */
        iaString _projectFolder;

        /*! initialize a project based on it's project folder

        \param projectFolder the given project folder
        */
        iProject(const iaString &projectFolder);
    };

} // namespace igor

#endif // __IGOR_PROJECT__
