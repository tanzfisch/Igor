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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_RENDERERSTATE__
#define __IGOR_RENDERERSTATE__

#include <igor/iDefines.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    /*! render state set holds a list of all render states the renderer can process

	\todo alpha test besides blending would be nice to have
	*/
    class iRenderStateSet
    {

        friend class iRenderer;

    public:
        /*! initializes default values for the state set
		*/
        iRenderStateSet();

        /*! does nothing
		*/
        virtual ~iRenderStateSet() = default;

        /*! defines the value of a specific render state

		\param state render state
		\param value render state value
		*/
        void setRenderState(const iRenderState state, const iRenderStateValue value);

        /*! returns the value of a specific render state

		\param state render state
		\return render state value
		*/
        iRenderStateValue getRenderState(const iRenderState state) const;

    private:
        /*! array with all supported render states
		*/
        iRenderStateValue _renderStates[static_cast<unsigned int>(iRenderState::RenderStateCount)];
    };

#include <igor/renderer/iRenderStateSet.inl>

}; // namespace igor

#endif
