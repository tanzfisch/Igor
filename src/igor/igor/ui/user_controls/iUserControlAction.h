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

#ifndef __iUSERCONTROLACTION__
#define __iUSERCONTROLACTION__

#include <igor/ui/actions/iAction.h>
#include <igor/ui/user_controls/iUserControl.h>

namespace igor
{

    class iWidgetLabel;
    typedef iWidgetLabel *iWidgetLabelPtr;
    class iWidgetPicture;
    typedef iWidgetPicture *iWidgetPicturePtr;

    /*! user control which represents an action in eg a menu
    */
    class Igor_API iUserControlAction : public iUserControl
    {

    public:
        /*! ctor initializes member variables

        \param parent the optional parent
        */
        iUserControlAction(const iWidgetPtr parent = nullptr);

        /*! clean up
        */
        ~iUserControlAction() = default;

        /*! \returns the widgets type
        */
        virtual iWidgetType getWidgetType() const override;

        /*! sets the action this widget is associated with

        \param action the action to be set
        */
        void setAction(const iActionPtr action);

        /*! \returns action that is associated with this widget
        */
        iActionPtr getAction() const;

        /*! sets if we reserve space for a picture even if none was defined

        \param value if true space for a picture will be reserved
        */
        void setFixedPictureSize(bool value = true);

        /*! \returns true if we reserve space for a picture even if none was defined
        */
        bool getFixedPictureSize() const;

    private:
        /*! only store the action id
        */
        int64 _actionID;

        /*! handle to text label
        */
        iWidgetLabelPtr _textLabel = nullptr;

        /*! handle to action picture
        */
        iWidgetPicturePtr _picture = nullptr;

        /*! if true we reserve space for a picture even if none was defined
        */
        bool _fixPictureSize = false;

        /*! handles on click event

        \param source the source of this event
        */
        void onClick(const iWidgetPtr source);

        /*! updates the UI
        */
        void update();

        /*! initializes the UI
        */
        void init();
    };

    /*! user control action pointer definition
    */
    typedef iUserControlAction *iUserControlActionPtr;

} // namespace igor
#endif