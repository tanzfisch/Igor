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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IGOR_SCENEFACTORY_H__
#define __IGOR_SCENEFACTORY_H__

#include <igor/scene/iScene.h>
#include <igor/resources/module/iModule.h>

namespace igor
{

    /*! class that creates and destroys instances of scenes 
    */
    class IGOR_API iSceneFactory : public iModule<iSceneFactory>
    {
        friend class iModule<iSceneFactory>;

    public:
        /*! creates a scene

		\returns pointer to new scene
		*/
        iScenePtr createScene();

        /*! releases a scene

		\param scene pointer to scene to be released
		*/
        void destroyScene(iScenePtr scene);

    private:
        /*! keeps track on how many scenes where alocated
        */
        uint32 sceneCount = 0;

        /*! does nothing
        */
        iSceneFactory() = default;

        /*! cheks scene counter
        */
        ~iSceneFactory();
    };

} // namespace igor

#endif // __IGOR_SCENEFACTORY_H__