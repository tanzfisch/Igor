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
// (c) Copyright 2012-2020 by Martin Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __IAUX_MODULE_H__
#define __IAUX_MODULE_H__

#include <igor/iDefines.h>

#include <vector>

namespace igor
{

    /*! module class

    make sure if you derrive from this class that your module will be created during 
    igor::startup() and destroyed during igor::shutdown()
    */
    template <typename T>
    class IGOR_API_TEMPLATE iModule
    {
        friend void destroyModules();
        friend void createModules();
        
    public:
        /*! \returns the instance of this module
        */
        __IGOR_INLINE__ static T &getInstance()
        {
            con_assert(iModule<T>::_instance != nullptr, "module not instanciated");
            return *iModule<T>::_instance;
        }

        /*! returns true if the instance of this is currently instantiated
        */
        static bool isInstantiated()
        {
            return iModule<T>::_instance ? true : false;
        }

    protected:
        /*! default ctor

        does nothing
        */
        iModule() = default;

        /*! dtor
        */
        virtual ~iModule() = default;

    private:
        /*! pointer to the module instance
        */
        static T *_instance;

        /*! creates the single instance of a module
        */
        static void create()
        {
            con_assert_sticky(iModule<T>::_instance == nullptr, "module already initialized");
            iModule<T>::_instance = new T();
        }

        /*! destroys the single instance of a module
        */
        static void destroy()
        {
            con_assert_sticky(iModule<T>::_instance != nullptr, "module already destroyed");
            delete iModule<T>::_instance;
            iModule<T>::_instance = nullptr;
        }

        /*! copy constructor is not allowed to use
        */
        iModule(const iModule &p) = default;
    };

    /*! the actual instance definition of any module
    */
    template <typename T>
    T *iModule<T>::_instance = nullptr;

}; // namespace igor

#endif // __IAUX_MODULE_H__
