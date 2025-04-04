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
// (c) Copyright 2012-2025 by Martin Loga
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

#ifndef IGOR_DRAG_H
#define IGOR_DRAG_H

#include <igor/ui/widgets/iWidget.h>
#include <igor/data/iMimeData.h>
#include <igor/resources/texture/iTexture.h>

namespace igor
{

    /*! drag states
    */
    enum class IGOR_API iDragState
    {
        Neutral,
        Accepted,
        Rejected
    };

    /*! drag data for drag & drop
     */
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
        const iMimeData &getMimeData() const;

        /*! \returns id of source widget (only if the source was an igor widget)
         */
        iWidgetID getSource() const;

        /*! sets texture displayed along the cursor during drag

        \param texture the texture to display
        */
        void setTexture(iTexturePtr texture);

        /*! \returns the texture set on the drag
        */
        iTexturePtr getTexture() const;

        /*! accept the drag as valid
        */
        void accept();

        /*! reject the drag as invalid
        */
        void reject();

        /*! reset the drag state to neutral
        */
        void clear();

        /*! \returns drag state
        */
        iDragState getDragState() const;

        /*! execute the drag
         */
        void execute();

    private:

        /*! state of drag
        */
        iDragState _dragState = iDragState::Neutral;

        /*! the mime data
         */
        iMimeData _mimeData;

        /*! source widget id
         */
        iWidgetID _source = iWidget::INVALID_WIDGET_ID;

        /*! texture to display while drag
        */
        iTexturePtr _texture;
    };

    /*! drag pointer definition
     */
    typedef iDrag *iDragPtr;
}

#endif // IGOR_DRAG_H