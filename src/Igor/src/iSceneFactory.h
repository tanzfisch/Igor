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
// (c) Copyright 2012-2019 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iSCENEFACTORY__
#define __iSCENEFACTORY__

#include <iDefines.h>

#include <iaSingleton.h>
using namespace IgorAux;

namespace Igor
{

    class iScene;

    /*! class that creates and destroys instances of scenes 
    */
    class Igor_API iSceneFactory : public iaSingleton<iSceneFactory>
	{
        friend class iaSingleton<iSceneFactory>;

	public:

		/*! creates a scene

		\returns pointer to new scene
		*/
		iScene* createScene();

		/*! releases a scene

		\param scene pointer to scene to be released
		*/
		void destroyScene(iScene* scene);

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
}

#endif

