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

#ifndef __iRENDERTARGET__
#define __iRENDERTARGET__

#include <iDefines.h>

#include <iaVector2.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{

    class iView;

	class Igor_API iRenderTarget
	{

	public:

        iRenderTarget();

		virtual ~iRenderTarget();

        /*! adds view to the window

        \param view the view to be added
        \todo would be nice to manipulate the order of views beeing added
        */
        void addView(iView* view);

        /*! removes view from the window

        \param view the view to be removed
        */
        void removeView(iView* view);

        /*! \returns the render target buffer size
        */
        virtual iaVector2i getTargetSize() const = 0;

        /*! renders the views in to this render target
        */
        virtual void draw() = 0;

        /*! calls a handle on the render target
        */
        virtual void handle() = 0;

    protected:

        /*! list of windows

        \todo why is this a flush list again?
        */
        vector<iView*> _views;

        /*! updates views with target size
        */
        void updateViews();

        /*! draws the views
        */
        void drawViews();

        /*! initialize render target
        */
        void activateRenderTarget();

        /*! deinitialize render target
        */
        void deactivateRenderTarget();

	};

};

#endif
 