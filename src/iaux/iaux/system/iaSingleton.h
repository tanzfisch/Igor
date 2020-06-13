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
// contact: martinloga@gmx.de

#ifndef __IAUX_SINGLETON__
#define __IAUX_SINGLETON__

#include <iaux/system/iaMutex.h>

namespace iaux
{

#pragma warning(disable : 4100)

    /*! singleton base class

    \todo put here some serious explanation how this works
    */
    template <typename T>
    class IgorAux_API_Template iaSingleton
    {
    public:
        /*! returns the singletons instance

        \return reference to singletion instance
        */
        static T &getInstance()
        {
            _mutex.lock();
            if (nullptr == iaSingleton<T>::_instance)
            {
                iaSingleton<T>::_instance = new T();
            }
            _mutex.unlock();

            return *iaSingleton<T>::_instance;
        }

        /*! deletes singleton instance
        */
        static void destroyInstance()
        {
            if (iaSingleton<T>::_instance != nullptr)
            {
                iaSingleton<T>::_instance->onPreDestroyInstance();
            }

            _mutex.lock();
            if (nullptr != iaSingleton<T>::_instance)
            {
                delete (iaSingleton<T>::_instance);
                iaSingleton<T>::_instance = nullptr;
            }
            _mutex.unlock();
        }

        /*! returns true if the instance of this currently instantiated
        */
        static bool isInstantiated()
        {
            bool result = iaSingleton<T>::_instance ? true : false;
            return result;
        }

    protected:
        /*! pointer to the singleton instance
        */
        static T *_instance;

        /*! default ctor

        does nothing
        */
        iaSingleton() = default;

        /*! dtor

        deletes singleton instance
        */
        virtual ~iaSingleton()
        {
            iaSingleton<T>::_instance = nullptr;
        }

        /*! last chance for the instance to clean up before shut down
        */
        virtual void onPreDestroyInstance(){};

    private:
        /*! mutex to protect instance pointer
        */
        static iaMutex _mutex;

        /*! copy constructor is not allowed to use
        */
        iaSingleton(const iaSingleton &p) = default;
    };

    /*! the actual instance definition of any singleton
    */
    template <typename T>
    T *iaSingleton<T>::_instance = nullptr;

    /*! the mutex of any singleton
    */
    template <typename T>
    iaMutex iaSingleton<T>::_mutex;

#pragma warning(default : 4100)
}; // namespace iaux

#endif
