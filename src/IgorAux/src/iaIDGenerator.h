//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __IGOR_AUX_IDGENERATOR__
#define __IGOR_AUX_IDGENERATOR__

#include <iaMutex.h>

#include <vector>


namespace IgorAux
{

    template <class T>
    class IgorAux_API_Template iaIDGenerator
    {
    public:

        /*! creates a new or recycles an old id

        \returns an ID
        */
        T createID();

    private:

        /*! the next id in case the recycled ID list is empty
        */
        T _nextID = static_cast<T>(IGOR_INVALID_ID + 1);

        /*! mutex to make shure the ID generation is uninterruped
        */
        iaMutex _mutex;

    };

    template <class T>
    T iaIDGenerator<T>::createID()
    {
        T result;

        _mutex.lock();
        result = _nextID++;
        _mutex.unlock();

        return result;
    }

    typedef uint64 iaID64;
    typedef uint32 iaID32;
    typedef uint16 iaID16;
    typedef uint8 iaID8;

    typedef iaIDGenerator<iaID64> iaIDGenerator64;
    typedef iaIDGenerator<iaID32> iaIDGenerator32;
    typedef iaIDGenerator<iaID16> iaIDGenerator16;
    typedef iaIDGenerator<iaID8> iaIDGenerator8;

};

#endif

