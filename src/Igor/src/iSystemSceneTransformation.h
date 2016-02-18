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

#ifndef __iSYSTEMSCENETRANSFORMATION__
#define __iSYSTEMSCENETRANSFORMATION__

#include <iSystem.h>
#include <iNodeTransform.h>

#include <map>
using namespace std;

namespace Igor
{

    class iScene;
    class iSystemPosition;

    class Igor_API iSystemSceneTransformation : public iSystem
    {

    public:

        virtual void registerEntity(uint64 entityID);
        virtual void unregisterEntity(uint64 entityID);
        virtual bool hasEntity(uint64 entityID);
        virtual void handle();

        void setTransformID(uint64 entityID, uint32 transformNodeID);
        uint32 getTransformID(uint64 entityID) const;

        iSystemSceneTransformation(iSystemPosition* systemPosition, iScene* scene);
        virtual ~iSystemSceneTransformation();

    private:

        iSystemPosition* _systemPosition = nullptr;

        /*! map of entity transform nodes
        */
        map<uint64, uint32> _transformNodes;

        /*! scene in use
        */
        iScene* _scene = nullptr;

    };

}

#endif