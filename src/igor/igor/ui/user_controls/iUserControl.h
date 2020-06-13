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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __iUSERCONTROL__
#define __iUSERCONTROL__

#include <igor/ui/widgets/iWidget.h>

namespace igor
{

	/*! use control base class

	mostly used as a container to combine some of the standard widgets to apear as one
	*/
	class Igor_API iUserControl : public iWidget
	{

	public:
		/*! does nothing

        \param parent the optional parent
		*/
		iUserControl(const iWidgetPtr parent = nullptr);

		/*! does nothing
		*/
		virtual ~iUserControl() = default;

		/*! \returns the widgets type
        */
		virtual iWidgetType getWidgetType() const override;

	protected:
		/*! updates size based on widgets content

		all widgets have to derive from this
		*/
		virtual void calcMinSize() override;
	};

} // namespace igor

#endif
