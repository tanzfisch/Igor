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

#ifndef __iFRAMEBUFFER__
#define __iFRAMEBUFFER__

#include <iRenderTarget.h>
#include <iRenderer.h>

namespace Igor
{

	class Igor_API iFrameBuffer : public iRenderTarget
	{

	public:

        iFrameBuffer(uint32 width, uint32 height, iColorFormat format, bool useDepthBuffer);

		virtual ~iFrameBuffer();

        /*! \returns the render target buffer size
        */
        virtual iaVector2i getTargetSize() const;

    protected:

        uint32 _width;
        uint32 _height;

        iColorFormat _format;
        bool _useDepthBuffer;

        uint32 _renderTargetID = iRenderer::INVALID_ID;
	};

};

#endif
 