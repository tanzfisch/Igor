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

#ifndef __IGOR_MODEL__
#define __IGOR_MODEL__

#include <igor/resources/iResource.h>
#include <igor/scene/nodes/iNode.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{
    /*! represents model data loaded or generated

    available parameters:

    - inherits all parameters from iResource    
    - subType: type of loader/generator used to load/generate model
    */
    class IGOR_API iModel : public iResource
    {
        friend class iModelFactory;
        friend class iModelDeleter;

    public:
        /*! \returns copy to node tree data
        */
        iNodePtr getNodeCopy();

        /*! \returns pointer to node tree data

        only use this if you know what you are doing
        */
        iNodePtr getNode();

    private:
        /*! node data
		*/
        iNodePtr _node = nullptr;

        /*! set node
		*/
        void setNode(iNodePtr node);

        /*! initializes member variables

        \param parameters the parameters of this model
        */
        iModel(const iParameters &parameters);

        /*! dtor
        */
        virtual ~iModel();

    };

    /*! definition of model pointer
	*/
    typedef std::shared_ptr<iModel> iModelPtr;

} // namespace igor

#endif // __IGOR_MODEL__
