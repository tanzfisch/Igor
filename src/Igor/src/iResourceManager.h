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

#ifndef __RESOURCEMANAGER__
#define __RESOURCEMANAGER__

#include <iDefines.h>

#include <iaSingleton.h>
#include <iaDirectory.h>
#include <iaString.h>
using namespace IgorAux;

#include <map>
#include <vector>
#include <mutex>
using namespace std;

namespace Igor
{

    /*! manages locations of resoruces
    */
    class Igor_API iResourceManager : public iaSingleton<iResourceManager>
    {

        friend class iaSingleton<iResourceManager>;

    private:

        /*! mutex to manage access to internal data
        */
        mutex _mutex;

        /*! list of search paths
        */
        vector<iaString> _searchPaths;

        /*! does nothing
        */
        iResourceManager() = default;

        /*! does nothing
        */
        ~iResourceManager() = default;

    public:
		
        /*! adds search path to list

        \param folder search path to add
        */
        void addSearchPath(const iaString& folder);

        /*! removes search path from list

        \param folder search path to remove
        */
        void removeSearchPath(const iaString& folder);

        /*! clears search path list
        */
        void clear();

        /*! \returns full path to a file if it exists

        \param filename file name to search for in search paths
        */
        iaString getPath(const iaString& filename);
        
    };

}

#endif