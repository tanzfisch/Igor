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

#ifndef __IGOR_DRAG__
#define __IGOR_DRAG__

#include <igor/ui/widgets/iWidget.h>
#include <igor/data/iMimeData.h>

namespace igor
{

    class IGOR_API iDrag
    {
        public:

            /*! store source id

            \param source source widget
            */
            iDrag(iWidgetPtr source = nullptr);

            /*! sets mime data

            \param mimeData the mime data to set
            */
            void setMimeData(const iMimeData &mimeData);

            /*! \returns the mime data
            */
            const iMimeData& getMimeData();

            /*! \returns id of source widget (only if the source was an igor widget)
            */
            iWidgetID getSource() const;

            /*! execute the drag
            */
            void execute();

        private:

            /*! the mime data
            */
            iMimeData _mimeData;

            /*! source widget id
            */
            iWidgetID _source = iWidget::INVALID_WIDGET_ID;
    };
}

#endif // __IGOR_DRAG__