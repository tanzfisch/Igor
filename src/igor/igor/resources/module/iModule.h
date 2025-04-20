//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IAUX_MODULE_H
#define IAUX_MODULE_H

#include <igor/iDefines.h>

namespace igor
{

    /*! module class

    make sure if you derive from this class that your module will be created during
    igor::startup() and destroyed during igor::shutdown()
    */
    template <typename T>
    class iModule
    {
    public:
        /*! \returns the instance of this module
         */
        IGOR_INLINE static T &getInstance()
        {
            con_assert(iModule<T>::_instance != nullptr, "module not instantiated");
            return *iModule<T>::_instance;
        }

        /*! returns true if the instance of this is currently instantiated
         */
        IGOR_INLINE static bool isInstantiated()
        {
            return iModule<T>::_instance ? true : false;
        }

        /*! creates the single instance of a module

        only call this if you know what you are doing
         */
        static void createInstance()
        {
            con_assert_sticky(iModule<T>::_instance == nullptr, "module already initialized");
            iModule<T>::_instance = new T();
        }

        /*! destroys the single instance of a module

        only call this if you know what you are doing
         */
        static void destroyInstance()
        {
            con_assert_sticky(iModule<T>::_instance != nullptr, "module already destroyed");
            delete iModule<T>::_instance;
            iModule<T>::_instance = nullptr;
        }

    protected:
        /*! does nothing
         */
        iModule() = default;

        /*! does nothing
         */
        virtual ~iModule() = default;

    private:
        /*! pointer to the module instance
         */
        static T *_instance;

        /*! copy constructor is not allowed to be used
         */
        iModule(const iModule &p) = default;
    };

    /*! the actual instance definition of any module
     */
    template <typename T>
    T *iModule<T>::_instance = nullptr;

}; // namespace igor

#endif // IAUX_MODULE_H