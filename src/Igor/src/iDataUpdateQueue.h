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

#ifndef __iDATAUPDATEQUEUE__
#define __iDATAUPDATEQUEUE__

#include <iDefines.h>

#include <iaMutex.h>
using namespace IgorAux;

#include <vector>


namespace Igor
{

    class iNode;

    /*! a queue that calls update data on nodes added
    */
	class iDataUpdateQueue
	{

    public:

        /*! adds node to queue

        thread save addind to queue
        */
        void addNode(uint64 nodeID);

        /*! called indirectly by iScene once per frame to process possible updates in scene

        must be called in main thread
        */
        void process();

	private:

        /*! sync with data load workers
        */
        iaMutex _mutex;

        /*! contains model nodes that just got inserted or changed
        */
        std::vector<uint64> _loadingQueue;

        /*! contains model nodes to be processed in current frame
        */
        std::vector<uint64> _processingQueue;
        
	};

}

#endif
