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

#ifndef __iSYSTEMSCENEINTERACTION__
#define __iSYSTEMSCENEINTERACTION__

#include <iSystem.h>
#include <iNode.h>

#include <iaVector3.h>
using namespace IgorAux;

#include <map>
#include <vector>
using namespace std;

namespace Igor
{

    __IGOR_FUNCTION_POINTER__(InitNodes, __IGOR_DEFAULTCALL__, uint32, (uint64));
    __IGOR_FUNCTION_POINTER__(DeInitNodes, __IGOR_DEFAULTCALL__, void, (uint64, uint32));

    class iScene;

    class Igor_API iSystemSceneInteraction : public iSystem
    {

        struct EntityData
        {
            uint32 _transformNodeID = iNode::INVALID_NODE_ID;
            DeInitNodes _deinitFunction = nullptr;
            InitNodes _initFunction = nullptr;
            bool _initialized = false;
        };

    public:

        virtual void registerEntity(uint64 entityID);
        virtual void unregisterEntity(uint64 entityID);
        virtual bool hasEntity(uint64 entityID);

        void init(uint64 entityID);
        void deinit(uint64 entityID);
        bool isInitialized(uint64 entityID) const;

        void setCallbacks(uint64 entityID, InitNodes initFunction, DeInitNodes deinitFunction);

        iSystemSceneInteraction(iScene* scene);
        virtual ~iSystemSceneInteraction();

    private:

        /*! map of entity transform nodes
        */
        map<uint64, EntityData> _transformNodes;

        /*! scene in use
        */
        iScene* _scene = nullptr;

    };

}

#endif