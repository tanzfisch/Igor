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
// (c) Copyright 2012-2017 by Martin Loga
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

#ifndef __iPHYSICSUSERMESHCOLLISIONHANDLER__
#define __iPHYSICSUSERMESHCOLLISIONHANDLER__

#include <iDefines.h>

#include <iAABox.h>

#include <mutex>
using namespace std;

namespace Igor
{

    class Igor_API iPhysicsUserMeshCollisionHandler
    {
        
    public:

        iPhysicsUserMeshCollisionHandler();

        virtual ~iPhysicsUserMeshCollisionHandler() = default;

        void setAABB(const iaVector3f& min, const iaVector3f& max);

        virtual void collideCallback(void* const collideDesc, const void* const continueCollisionHandle);
        virtual float32 rayHitCallback(void* const rayDesc);
        virtual void destroyCallback();
        virtual void getCollisionInfo(void* const infoRecord);
        virtual int testOverlapAABB(const float64* const box0, const float64* const box1);
        virtual int getFacesInAABB(const float64* p0, const float64* p1, const float64** vertexArray, int* vertexCount, int* vertexStrideInBytes, const int* indexList, int maxIndexCount, const int* userDataList);
        
    protected:

        iAABoxf _bb;

    };
    
}

#endif
