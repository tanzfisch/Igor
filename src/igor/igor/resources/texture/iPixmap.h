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

#ifndef IGOR_PIXMAP_H
#define IGOR_PIXMAP_H

#include <iaux/data/iaString.h>
#include <iaux/data/iaColor4.h>
#include <iaux/data/iaColor3.h>
using namespace iaux;

#include <igor/iDefines.h>

#include <memory>

namespace igor
{

    class iPixmap;

    /*! pixmap pointer definition
     */
    typedef std::shared_ptr<iPixmap> iPixmapPtr;

    /*! pixmap is an editable texture

    in order to use it for rendering one has to transform it in to a iTexture
    */
    class IGOR_API iPixmap
    {

    public:
        /*! releases resources
         */
        ~iPixmap();

        /*! creates new pixmap with given parameters

        \param width width of pixmap in pixel
        \param height height of pixmap in pixel
        \param colorFormat the color format of the pixmap
        */
        static iPixmapPtr createPixmap(uint32 width, uint32 height, iColorFormat colorFormat, uint8 *data = nullptr);

        /*! loads pixmap from file

        \param filename the file to load
        \returns pixmap
        */
        static iPixmapPtr loadPixmap(const iaString &filename);

        /*! \returns width of pixmap
         */
        uint32 getWidth() const;

        /*! \returns height of pixmap
         */
        uint32 getHeight() const;

        /*! \returns bytes per pixel
         */
        uint32 getBytesPerPixel() const;

        /*! \returns color format in use
         */
        iColorFormat getColorFormat() const;

        /*! \returns raw pixel data
         */
        uint8 *getData() const;

        /*! same as getPixel but returns type iaColor4c

        \param x x coordinate
        \param x y coordinate
        \param[out] color the returned color
        */
        const iaColor4c getPixelRGBA(float64 x, float64 y) const;

        /*! same as getPixel but returns type iaColor3c

        \param x x coordinate
        \param x y coordinate
        \param[out] color the returned color
        */
        const iaColor3c getPixelRGB(float64 x, float64 y) const;

        /*! same as setPixel but returns type iaColor4c

        \param x x coordinate
        \param x y coordinate
        \param[out] color the returned color
        */
        void setPixel(float64 x, float64 y, const iaColor4c &color);

        /*! same as setPixel but returns type iaColor3c

        \param x x coordinate
        \param x y coordinate
        \param[out] color the returned color
        */
        void setPixel(float64 x, float64 y, const iaColor3c &color);

    private:
        /*! pixmap width
         */
        uint32 _width = 0;

        /*! pixmap height
         */
        uint32 _height = 0;

        /*! bytes per pixel
         */
        uint32 _bytesPerPixel = 0;

        /*! color format
         */
        iColorFormat _colorFormat = iColorFormat::Undefined;

        /*! raw data
         */
        uint8 *_data = nullptr;

        /*! does nothing
         */
        iPixmap() = default;

        /*! allocates memory based on size and colorformat

        \param width width of pixmap in pixel
        \param height height of pixmap in pixel
        \param colorFormat the color format of the pixmap
        \param data pixel data
        */
        iPixmap(uint32 width, uint32 height, iColorFormat colorFormat, uint8 *data = nullptr);
    };

}; // namespace igor

#endif // IGOR_PIXMAP_H