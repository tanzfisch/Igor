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
// contact: igorgameengine@protonmail.com

#ifndef __iWIDGETCOLORGRADIENT__
#define __iWIDGETCOLORGRADIENT__

#include <igor/ui/widgets/iWidget.h>
#include <igor/resources/texture/iTexture.h>

#include <iaux/data/iaString.h>
#include <iaux/data/iaGradient.h>
using namespace iaux;

#include <memory>

namespace igor
{

    /*! color created/added event
    */
    iaEVENT(iColorGradientColorCreatedEvent, iColorGradientColorCreatedDelegate, void, (float32 at, const iaColor4f &color), (at, color));

    /*! color view widget
    */
    class Igor_API iWidgetColorGradient : public iWidget
    {

    public:
        /*! ctor initializes member variables

		\param parent optional parent
		*/
        iWidgetColorGradient(const iWidgetPtr parent = nullptr);

        /*! release texture
		*/
        virtual ~iWidgetColorGradient();

        /*! \returns the widgets type
        */
        virtual iWidgetType getWidgetType() const override;

        /*! set wether events will be blocked or not

        implementation needs to be overriden by deriving classes to make sure all additional events are blocked too

        \param blockEvents if true events from this widget will be blocked
        */
        virtual void block(bool blockEvents) override;

        /*! sets color gradient
		\param color color value in rgba
		*/
        void setGradient(const iaGradientColor4f &gradient);

        /*! \returns color gradient
		*/
        const iaGradientColor4f &getGradient() const;

        /*! sets if alpha values are displayed
        */
        void setUseAlpha(bool useAlpha = true);

        /*! \returns true if alpha values are displayed
        */
        bool isUsingAlpha() const;

        /*! sets the interactive feature

        \param interactive if true gradient will be interactive
        */
        void setInteractive(bool interactive = true);

        /*! \returns true if gradient is interactive
        */
        bool isInteractive();

        /*! registers delegate to color created event

        \param delegate the delegate to register
        */
        void registerOnColorCreatedEvent(iColorGradientColorCreatedDelegate delegate);

        /*! unregisters delegate from color created event

        \param delegate the delegate to unregister
        */
        void unregisterOnColorCreatedEvent(iColorGradientColorCreatedDelegate delegate);

    private:
        /*! color gradient
        */
        iaGradientColor4f _gradient;

        /*! flag if alpha is displayed
        */
        bool _useAlpha = true;

        /*! flag if gradient is interactive
        */
        bool _interactive = false;

        /*! shared pointer to background texture
        */
        iTexturePtr _texture;

        /*! color created event
        */
        iColorGradientColorCreatedEvent _colorCreated;

        /*! handles incomming mouse key down events

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool handleMouseKeyDown(iKeyCode key);

        /*! updates size based on it's content
        */
        void calcMinSize();

        /*! draws the widget
		*/
        void draw();
    };
} // namespace igor

#endif