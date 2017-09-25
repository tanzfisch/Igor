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
// (c) Copyright 2012-2017 by Martin Loga
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

#ifndef __iRENDERERSTATE__
#define __iRENDERERSTATE__

#include <iDefines.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    /*! render states
    */
	enum class iRenderState
	{
		DepthTest,
		DepthMask,
		Blend,
		CullFace,
		Texture2D0,
		Texture2D1,
		Texture2D2,
		Texture2D3,
		Texture2D4,
		Texture2D5,
		Texture2D6,
		Texture2D7,
		Wireframe,
		DepthFunc,
		CullFaceFunc,
		BlendFuncSource,
		BlendFuncDestination,
        Instanced,
        InstancedFunc,
		RenderStateCount
	};

    /*! possible values for render states
    */
	enum class iRenderStateValue
	{
		Off = 0,
		On = 1,
		One,
		Zero,
		DestinationColor,
		OneMinusDestinationColor,
		SourceAlpha,
		OneMinusSourceAlpha,
		DestinationAlpha,
		OneMinusDestinationAlpha,
		SourceColor,
		OneMinusSourceColor,
		Never,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		Always,
		Front,
		Back,
		Invalid,
        PositionOrientation,
        Position, //! \todo not implemented
		RenderStateValueCount
	};

	/*! render state set holds a list of all render states the renderer can process

    \todo alpha test besides blending would be nice to have
	*/
    class iRenderStateSet
	{

        friend class iRenderer;

	private:

		/*! array with all supported render states
		*/
		iRenderStateValue _renderStates[static_cast<unsigned int>(iRenderState::RenderStateCount)];

	public:

		/*! defines the value of a specific render state

		\param state render state
		\param value render state value
		*/
        void Igor_API setRenderState(const iRenderState state, const iRenderStateValue value);

		/*! returns the value of a specific render state

		\param state render state
		\return render state value
		*/
        iRenderStateValue Igor_API getRenderStateValue(const iRenderState state);

        /*! initializes default values for the state set
        */
        iRenderStateSet();

        /*! does nothing
        */
        virtual ~iRenderStateSet() = default;

	};

};

#endif
