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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iPHYSICSMANAGER__
#define __iPHYSICSMANAGER__

#include <iDefines.h>

#include <iSphere.h>

#include <iaSingleton.h>
using namespace IgorAux;

#include <list>
using namespace std;

namespace Igor
{

    class iScene;
    class iNode;

    class Igor_API iPhysicsManager : public iaSingleton<iPhysicsManager>
    {

        friend class iaSingleton<iPhysicsManager>;

    public:

        /*! sets the scene to render with

        \param scene scene to render
        */
        void setScene(iScene* scene);

        /*! \returns pointer to scene
        */
        iScene* getScene();

        /*! updates physics
        */
        void update();

    private:

        /*! pointer to scene
        */
        iScene* _scene = nullptr;

        /*! temporary list of nodes that where filtered by the culling process
        */
        list<iNode*> _cullResult;
        
        /*! cull by sphere
        */
        void cullScene(const iSpheref& sphere);

        /*! does nothing
        */
        iPhysicsManager() = default;

        /*! does nothing
        */
        virtual ~iPhysicsManager() = default;

    };
}

#endif