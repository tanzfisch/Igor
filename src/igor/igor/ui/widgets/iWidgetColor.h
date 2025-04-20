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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef __IGOR_WIDGETCOLOR__
#define __IGOR_WIDGETCOLOR__

#include <igor/ui/widgets/iWidget.h>
#include <igor/resources/texture/iTexture.h>

#include <iaux/data/iaString.h>
#include <iaux/data/iaColor4.h>
using namespace iaux;

#include <memory>

namespace igor
{

    /*! color view widget
    */
    class IGOR_API iWidgetColor : public iWidget
    {

    public:
        /*! ctor initializes member variables

		\param parent optional parent
		*/
        iWidgetColor(const iWidgetPtr parent = nullptr);

        /*! release texture
		*/
        virtual ~iWidgetColor();

        /*! sets color
		\param color color value in rgba
		*/
        void setColor(const iaColor4f &color);

        /*! \returns current color value
		*/
        const iaColor4f &getColor() const;

    private:
        /*! the color to present
        */
        iaColor4f _color;

        /*! shared pointer to background texture
        */
        iTexturePtr _texture;

        /*! updates size based on it's content
        */
        void calcMinSize() override;

        /*! draws the widget
         */
        void draw() override;
    };
} // namespace igor

#endif // __IGOR_WIDGETCOLOR__
