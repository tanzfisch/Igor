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

#include <unordered_map>
#include <any>
#include <memory>

namespace igor
{

    /*! parameters map definition
    */
    typedef std::unordered_map<iaString, std::any> iParametersMap;

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
        explicit iParameters(const iParametersMap &parameters);

        /*! \returns value for given parameter name

        if name does not exist it returns some default value

        \param name the parameter name
        \param defaultValue the given default value
        */
        template <typename T>
        T getParameter(const iaString &name, const T &defaultValue = T()) const;

        /*! \returns true if given key exists

        \param name name of parameter
        */
        bool hasParameter(const iaString &name) const;

        /*! sets value for given parameter

        \param name name of parameter
        \param value the value to set
        */
        void setParameter(const iaString &name, const std::any value);

        /*! \returns all parameters
        */
        const iParametersMap& getParameters() const;

    private:
        /*! parameters
         */
        iParametersMap _parameters;
    };

#include <igor/data/iParameters.inl>

    /*! stream parameters

    \param stream the destination
    \param parameters the parameters to stream
    \returns the resulting stream
    */
    IAUX_API std::wostream &operator<<(std::wostream &stream, const iParameters &parameters);    

    /*! stream std::any (well as long it's a type that we know about)

    \param stream the stream
    \param any the std::any data to stream
    \return the resulting stream
    */
    IAUX_API std::wostream &operator<<(std::wostream &stream, const std::any &any);

    /*! parameters pointer definition
     */
    typedef std::shared_ptr<iParameters> iParametersPtr;

}

#endif // __IGOR_PARAMETERS__