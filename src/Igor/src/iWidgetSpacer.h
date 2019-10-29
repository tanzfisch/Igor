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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __iWIDGETSPACER__
#define __iWIDGETSPACER__

#include <iWidget.h>

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

	/*! spacer widget
	*/
	class Igor_API iWidgetSpacer : public iWidget
	{

	public:

		/*! ctor initializes member variables

		\param parent optional parent
		*/
		iWidgetSpacer(const iWidgetPtr parent = nullptr);

		/*! does nothing
		*/
		virtual ~iWidgetSpacer() = default;

        /*! \returns the widgets type
        */
        virtual iWidgetType getWidgetType() const override;

	protected:

		/*! updates size based on it's content
		*/
		void calcMinSize();

		/*! draws the button
		*/
		void draw();

	};

	/*! widget spacer pointer definition
	*/
	typedef iWidgetSpacer* iWidgetSpacerPtr;
}

#endif
