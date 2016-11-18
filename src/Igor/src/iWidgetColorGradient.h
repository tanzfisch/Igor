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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iWIDGETCOLORGRADIENT__
#define __iWIDGETCOLORGRADIENT__

#include <iWidget.h>

#include <iaString.h>
#include <iaGradient.h>
using namespace IgorAux;

namespace Igor
{

    class iTexture;

    /*! color created/added event
    */
    iaEVENT(iColorGradientColorCreatedEvent, iColorGradientColorCreatedDelegate, void, (float32 at, const iaColor4f& color), (at, color));

    /*! color view widget

    \todo highlight selection

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    */
	class Igor_API iWidgetColorGradient : public iWidget
	{

        /*! needs to be friend because it's the factory that creates this widget
        */
        friend class iWidgetManager;

	public:

		/*! sets color gradient
		\param color color value in rgba
		*/
        void setGradient(const iaGradientColor4f& gradient);

		/*! \returns color gradient
		*/
        const iaGradientColor4f& getGradient() const;

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
        shared_ptr<iTexture> _texture = nullptr;

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

        /*! ctor initializes member variables
        */
        iWidgetColorGradient();

        /*! release texture
        */
		virtual ~iWidgetColorGradient();

        /*! creates instance of this widget type
        */
        static iWidget* createInstance();
	};
}

#endif
