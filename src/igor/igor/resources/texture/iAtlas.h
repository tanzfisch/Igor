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

#ifndef __IGOR_SPRITE__
#define __IGOR_SPRITE__

#include <igor/resources/texture/iTexture.h>

#include <iaux/data/iaRectangle.h>
#include <iaux/math/iaVector2.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <memory>
#include <vector>

class TiXmlElement;

namespace igor
{

    class iAtlas;

    /*! atlas pointer definition
     */
    typedef std::shared_ptr<iAtlas> iAtlasPtr;

    /*! used to have 2d sprites
     */
    class IGOR_API iAtlas
    {

    public:
        /*! \returns a newly created atlas

        \param texture atlas texture
        \param filename optional atlas data file
        */
        static iAtlasPtr create(const iTexturePtr &texture, const iaString &filename = "");

        /*! ctor initializes member variables

        \param texture texture used for this sprite
        */
        iAtlas(const iTexturePtr &texture, const iaString &filename = "");

        /*! does nothing
         */
        ~iAtlas() = default;

        /*! a frame defines an area within the texture that later can be rendered
         */
        struct iFrame
        {
            /*! position of frame in texture coordinates 0.0-1.0
             */
            iaRectanglef _rect;

            /*! origin or pivot of frame in texture coordinates 0.0-1.0

            can be used to position on screen
            */
            iaVector2f _origin;
        };

        /*! adds a frame to the atlas

        \returns index of the added frame
        \param rect the frame dimensions
        \param origin the origin of the frame
        \param pixel if true values are in pixel coordinates if false than 0.0-1.0
        */
        uint32 addFrame(const iaRectanglef &rect, const iaVector2f &origin, bool pixel = true);

        /*! adds a frame to the atlas

        \returns index of the added frame
        \param pos top left corner of frame
        \param size size of frame
        \param origin the origin of the frame
        \param pixel if true values are in pixel coordinates if false than 0.0-1.0
        */
        uint32 addFrame(const iaVector2f &pos, const iaVector2f &size, const iaVector2f &origin, bool pixel = true);

        /*! loads frames from file

        \param filename filename to load from
        */
        void loadFrames(const iaString &filename);

        /*! \returns texture
         */
        const iTexturePtr &getTexture() const;

        /*! sets txture used by this atlas

        \param texture the texture to use
        */
        void setTexture(const iTexturePtr &texture);

        /*! \returns the amount of frames
         */
        uint32 getFrameCount() const;

        /*! returns frame for given index

        no range check!

        \param index the frame index to return
        \returns the specified frame
        */
        const iFrame &getFrame(uint32 index) const;

    private:
        /*! the atlas frames
         */
        std::vector<iFrame> _frames;

        /*! texture in use
         */
        iTexturePtr _texture;

        /*! read an atlas from xml file
         */
        void readAtlas(TiXmlElement *atlas);
    };

}; // namespace igor

#endif
