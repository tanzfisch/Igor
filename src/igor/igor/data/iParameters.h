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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_PARAMETERS__
#define __IGOR_PARAMETERS__

#include <igor/iDefines.h>

#include <map>
#include <any>

namespace igor
{

    /*! a key value list of parameters
    */
    class IGOR_API iParameters
    {

    public:

        /*! init members

        \param parameters the parameters
        */
        iParameters(const std::map<iaString, std::any> &parameters)
        {
            _parameters = parameters;
        }

        /*! \returns value for given key

        if key does not exist it returns some default value

        \param key the given key
        \param defaultValue the given default value
        */
        template <typename T>
        T getValue(const iaString &key, const T &defaultValue = T()) const
        {
            auto iter = _parameters.find(key);
            if (iter == _parameters.end())
            {
                return defaultValue;
            }

            try
            {
                return std::any_cast<T>(iter->second);
            }
            catch (const std::exception &e)
            {
                // common mistake is to use a string but not make it an iaString
                con_crit("invalid any cast");
            }

            return T();
        }

        /*! sets value for given key

        \param key the given key
        \param value the value to set
        */
        void setValue(const iaString &key, const std::any value)
        {
            _parameters[key] = value;
        }

    private:

        /*! parameters 
        */
        std::map<iaString, std::any> _parameters;
    };

}

#endif // __IGOR_PARAMETERS__