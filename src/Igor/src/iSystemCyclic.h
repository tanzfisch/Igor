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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iSYSTEM_CYCLIC__
#define __iSYSTEM_CYCLIC__

#include <iSystem.h>

#include <vector>
#include <mutex>
using namespace std;

namespace Igor
{

    /*! system that gets called every frame
    */
    class Igor_API iSystemCyclic : public iSystem
    {

        friend class iEntityManager;

    public:

        /*! does nothing
        */
        iSystemCyclic() = default;

        /*! does nothing
        */
        virtual ~iSystemCyclic() = default;

    protected:

        /*! entity cache of this system
        */
        vector<uint64> _entities;

        /*! called once by entity manager before handle is called
        */
        virtual void init() = 0;

        /*! called once per frame by entity manager
        */
        virtual void handle() = 0;

    private:

        /*! component dependancies
        */
        vector<uint64> _dependencies;

        /*! id of entity
        */
        uint64 _id = INVALID_SYSTEM_ID;

        /*! next id for next generated entity
        */
        static uint64 _nextSystemID;

        /*! mutex to protect entity id generation
        */
        static mutex _mutexID;

    };

}

#endif