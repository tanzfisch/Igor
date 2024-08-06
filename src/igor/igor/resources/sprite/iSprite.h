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

    /*! used to have 2d sprites
     */
    class IGOR_API iSprite : public iResource
    {

        friend class iSpriteFactory;

    public:
        /*! a frame defines an area within the texture that later can be rendered
         */
        struct iFrame
        {
            /*! position of frame in texture coordinates 0.0-1.0
             */
            iaRectanglef _rect;

            /*! pivot or pivot of frame in texture coordinates 0.0-1.0

            can be used to position on screen
            */
            iaVector2f _pivot;
        };

        /*! \returns the amount of frames
         */
        uint32 getFrameCount() const;

        /*! returns frame for given index

        no range check!

        \param index the frame index to return
        \returns the specified frame
        */
        const iFrame &getFrame(uint32 index) const;

        /*! \returns texture
         */
        const iTexturePtr &getTexture() const;

        /*! sets the texture of this sprite
        */
        void setTexture(iTexturePtr texture);

    private:
        /*! the sprite frames
         */
        std::vector<iFrame> _frames;

        /*! texture in use
         */
        iTexturePtr _texture;

        /*! ctor initializes member variables

        \param parameters the parameters of this sprite
        */
        iSprite(const iParameters &parameters);

        /*! adds a frame to the sprite

        \returns index of the added frame
        \param rect the frame dimensions
        \param pivot the pivot of the frame
        \param pixel if true values are in pixel coordinates if false than 0.0-1.0
        */
        uint32 addFrame(const iaRectanglef &rect, const iaVector2f &pivot, bool pixel = true);

        /*! adds a frame to the sprite

        \returns index of the added frame
        \param pos top left corner of frame
        \param size size of frame
        \param pivot the pivot of the frame
        \param pixel if true values are in pixel coordinates if false than 0.0-1.0
        */
        uint32 addFrame(const iaVector2f &pos, const iaVector2f &size, const iaVector2f &pivot, bool pixel = true);
    };

    /*! sprite pointer definition
     */
    typedef std::shared_ptr<iSprite> iSpritePtr;

}; // namespace igor

#endif // __IGOR_SPRITE__