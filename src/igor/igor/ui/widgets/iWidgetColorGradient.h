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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef __IGOR_WIDGETCOLORGRADIENT__
#define __IGOR_WIDGETCOLORGRADIENT__

#include <igor/ui/widgets/iWidget.h>
#include <igor/resources/texture/iTexture.h>

#include <iaux/data/iaString.h>
#include <iaux/data/iaKeyFrameGraph.h>
#include <iaux/system/iaEvent.h>
using namespace iaux;

#include <memory>

namespace igor
{

    /*! color created/added event
     */
    IGOR_EVENT_DEFINITION(iColorGradientColorCreated, float32, const iaColor4f &);

    /*! color view widget
     */
    class IGOR_API iWidgetColorGradient : public iWidget
    {

    public:
        /*! ctor initializes member variables

        \param parent optional parent
        */
        iWidgetColorGradient(const iWidgetPtr parent = nullptr);

        /*! release texture
         */
        virtual ~iWidgetColorGradient();

        /*! blocks all outgoing events
         */
        virtual void blockEvents() override;

        /*! unblocks all outgoing events
         */
        virtual void unblockEvents() override;

        /*! sets color gradient
        \param color color value in rgba
        */
        void setGradient(const iaKeyFrameGraphColor4f &gradient);

        /*! \returns color gradient
         */
        const iaKeyFrameGraphColor4f &getGradient() const;

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

        /*! \returns selected color index
         */
        int32 getSelectedIndex() const;

    private:
        /*! color gradient
         */
        iaKeyFrameGraphColor4f _gradient;

        /*! selected color index
         */
        int32 _selectedColorIndex = -1;

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

        /*! handles incoming mouse key down events

        \param event mouse key down event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseKeyDown(iEventMouseKeyDown &event) override;

        /*! handles mouse key up events

        \param event the mouse key up event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseKeyUp(iEventMouseKeyUp &event) override;

        /*! updates size based on it's content
         */
        void calcMinSize() override;

        /*! draws the widget
         */
        void draw() override;
    };

    /*! color gradient widget pointer definition
     */
    typedef iWidgetColorGradient *iWidgetColorGradientPtr;

} // namespace igor

#endif // __IGOR_WIDGETCOLORGRADIENT__
