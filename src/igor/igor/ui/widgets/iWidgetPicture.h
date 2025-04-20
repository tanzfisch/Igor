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

#ifndef __IGOR_WIDGETPICTURE__
#define __IGOR_WIDGETPICTURE__

#include <igor/ui/widgets/iWidget.h>
#include <igor/resources/texture/iTexture.h>

#include <iaux/data/iaString.h>
using namespace iaux;

#include <memory>
#include <limits>

namespace igor
{

    /*! picture widget
     */
    class IGOR_API iWidgetPicture : public iWidget
    {

    public:
        /*! ctor initializes member variables

        \param parent optional parnt
        */
        iWidgetPicture(const iWidgetPtr parent = nullptr);

        /*! does nothing
         */
        virtual ~iWidgetPicture() = default;

        /*! sets the texture to use for the picture

        \param texture the texture to use
        */
        void setTexture(iTexturePtr texture);

        /*! sets the background texture to use for the whole button by name

        \param textureAlias alias or id to texture
        */
        void setTexture(const iaString &textureAlias);

        /*! \returns texture path
         */
        iTexturePtr getTexture() const;

        /*! \returns true if picture has a valid and loaded texture
         */
        bool hasTexture() const;

        /*! sets maximum display size of picture

        \param width max width
        \param height max height
        */
        void setMaxSize(int32 width, int32 height);

        /*! \returns maximum width
         */
        int32 getMaxWidth();

        /*! \returns maximum height
         */
        int32 getMaxHeight();

        /*! sets if the aspect ratio of the picture must be kept

        \param keep if true the aspect ratio must be kept
        */
        void setKeepAspectRatio(bool keep = true);

        /*! \returns true if aspect ratio will be kept
         */
        bool getKeepAspectRatio() const;

        /*! enables/disables checker board
        */
        void setCheckerBoard(bool enable);

        /*! \returns true if checker board is enabled
        */
        bool isCheckerBoardEnabled() const;

    private:
        /*! flag if the aspect ratio must be kept when resized
         */
        bool _keepAspectRatio = true;

        /*! if true draw checker board behind picture
        */
        bool _checkerBoard = false;

        /*! shared pointer to texture
         */
        iTexturePtr _texture;

        /*! maximum display width
         */
        int32 _maxWidth = std::numeric_limits<int32>::max();

        /*! maximum display height
         */
        int32 _maxHeight = std::numeric_limits<int32>::max();

        /*! updates size based on it's content
         */
        void calcMinSize() override;
        
        /*! draws the widget
         */
        void draw() override;
    };

    /*! widget picture pointer definition
    */
    typedef iWidgetPicture *iWidgetPicturePtr;

} // namespace igor

#endif // __IGOR_WIDGETPICTURE__
