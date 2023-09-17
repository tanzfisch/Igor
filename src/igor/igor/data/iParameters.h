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
#include <memory>

namespace igor
{

    /*! a key value list of parameters
     */
    class IGOR_API iParameters
    {

    public:
        /*! does nothing
         */
        iParameters() = default;

        /*! init members

        \param parameters the parameters
        */
        iParameters(const std::map<iaString, std::any> &parameters)
        {
            _parameters = parameters;
        }

        /*! \returns value for given parameter name

        if name does not exist it returns some default value

        \param name the parameter name
        \param defaultValue the given default value
        */
        template <typename T>
        T getParameter(const iaString &name, const T &defaultValue = T()) const
        {
            auto iter = _parameters.find(name);
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

        /*! \returns true if given key exists

        \param name name of parameter
        */
        bool hasParameter(const iaString &name) const
        {
            return _parameters.find(name) != _parameters.end();
        }

        /*! sets value for given parameter

        \param name name of parameter
        \param value the value to set
        */
        void setParameter(const iaString &name, const std::any value)
        {
            _parameters[name] = value;
        }

    private:
        /*! parameters
         */
        std::map<iaString, std::any> _parameters;
    };


    /*! parameters pointer definition
     */
    typedef std::shared_ptr<iParameters> iParametersPtr;

}

#endif // __IGOR_PARAMETERS__