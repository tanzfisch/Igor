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

#ifndef __RENDERENGINE__
#define __RENDERENGINE__

#include <iDefines.h>

#include <vector>
using namespace std;

namespace Igor
{

    class iScene;
    class iNodeCamera;
    class iNode;
    
    /*! does control the render loop
    */
    class iRenderEngine
    {

    public:

        /*! sets the scene to render with

        \param scene scene to render
        */
        void setScene(iScene* scene);

        /*! \returns pointer to scene
        */
        iScene* getScene();

        /*! culls and renders
        */
        void render();

        /*! init
        */
        iRenderEngine();

        /*! deinit
        */
        virtual ~iRenderEngine();

    private:

        /*! cull section id for statistics
        */
        uint32 _cullSectionID = 0;

        /*! draw section id for statistics
        */
        uint32 _drawSectionID = 0;

        /*! pointe to scene

        \todo can't we use an array here?
        */
        iScene* _scene = nullptr;

        /*! temporary list of nodes that where filtered by the culling process
        */
        vector<uint32> _cullResult;

        /*! cull scene relative to specified camera

        \param camera the specified camera
        */
        void cullScene(iNodeCamera* camera);

        /*! draw scene relative to specified camera

        \param camera the specified camera
        */
        void drawScene(iNodeCamera* camera);

    };
}

#endif