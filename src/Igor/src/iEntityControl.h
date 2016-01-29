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

#ifndef __iENTITYCONTROL__
#define __iENTITYCONTROL__

#include <iaSingleton.h>
using namespace IgorAux;

#include <iDefines.h>

namespace Igor
{

    /*! makes the entities move
    */
    class Igor_API iEntityControl : public iaSingleton<iEntityControl>
    {

        friend class iaSingleton<iEntityControl>;

    private:

        /*! true if registered to application handle event
        */
        bool _running = false;

        /*! iterates over all entities and updates their content if needed
        */
        void onHandle();

        /*! does nothing
        */
        iEntityControl();

        /*! unregisters from application handle if needed
        */
        ~iEntityControl();

    };

}

#endif
