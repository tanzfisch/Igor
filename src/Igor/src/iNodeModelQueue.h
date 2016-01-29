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

#ifndef __iMODELNODEQUEUE__
#define __iMODELNODEQUEUE__

#include <iDefines.h>

#include <vector>
#include <mutex>
using namespace std;

namespace Igor
{

    class iNodeModel;

    /*! a queue of node models that have to be loaded asynchronously

    \todo remove this and use iNodeFactory to add data instead. still a good idea
    */
	class iNodeModelQueue
	{

	private:

        /*! sync with data load workers
        */
        mutex _mutex;

        /*! contains model nodes that just got inserted or changed
        */
        vector<uint32> _loadingQueue;

        /*! contains model nodes to be processed in current frame
        */
        vector<uint32> _processingQueue;

	public:

        /*! adds model to queue
        */
        void addModelNode(iNodeModel* modelNode);

        /*! called indirectly by iScene once per frame to process possible updates in scene
        */
        void process();
        
	};

}

#endif
