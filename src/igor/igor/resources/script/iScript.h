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
// (c) Copyright 2012-2026 by Martin A. Loga
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

#ifndef IGOR_SCRIPT_H
#define IGOR_SCRIPT_H

#include <igor/resources/iResource.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

    /*! used to have 2d sprites
     */
    class IGOR_API iScript : public iResource
    {

        friend class iScriptFactory;

    public:
        /*! sets script

        \param script the script to set
        */
        void setScript(const iaString &script);

        /*! \returns script
        */
        const char* getScript() const;

        /*! executes script
         */
        void execute();

    private:
        /*! the script
         */
        std::string _script;

        /*! ctor initializes member variables

        \param parameters the parameters of this script
        */
        iScript(const iParameters &parameters);
    };

    /*! script pointer definition
     */
    typedef std::shared_ptr<iScript> iScriptPtr;

}; // namespace igor

#endif // IGOR_SCRIPT_H