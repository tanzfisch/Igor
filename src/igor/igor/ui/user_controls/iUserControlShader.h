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
// (c) Copyright 2012-2026 by Martin A. Loga
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

#ifndef IGOR_USERCONTROL_SHADER_H
#define IGOR_USERCONTROL_SHADER_H

#include <igor/ui/user_controls/iUserControl.h>

#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetPicture.h>
#include <igor/resources/texture/iTexture.h>
#include <igor/resources/material/iMaterial.h>

namespace igor
{

    /*! User control shader shader chooser
     */
    class IGOR_API iUserControlShader : public iUserControl
    {

    public:
        /*! ctor initializes member variables

        \param parent the optional parent
        */
        iUserControlShader(const iWidgetPtr parent = nullptr);

        /*! clean up
         */
        ~iUserControlShader();

        /*! sets the shader id

        this will use default material attributes to generate a preview image

        \param shaderID the given shader id
        */
        void setShader(const iResourceID &shaderID);

        /*! sets the shader id via material

        this will use the material attributes to generate a preview image

        \param material the given material
        */
        void setMaterial(iMaterialPtr material);

        /*! \returns shader id
         */
        const iResourceID &getShaderID() const;

    private:
        /*! shader ID
         */
        iResourceID _shaderID;

        /*! label shader id
         */
        iWidgetLabelPtr _labelID;

        /*! shader alias
         */
        iWidgetLabelPtr _labelAlias;

        /*! displays the selected shader
         */
        iWidgetPicturePtr _picture;

        /*! initialize gui elements
         */
        void onInitUI();

        /*! drag move handle

        \param drag the drag data
        \param mousePos the current mouse pos
        */
        void onDragMove(iDrag &drag, const iaVector2f &mousePos) override;

        /*! drop handle

        \param drag the drag data
        \param mousePos the current mouse pos
        */
        void onDrop(const iDrag &drag, const iaVector2f &mousePos) override;
    };

    /*! user control shader shader chooser pointer definition
     */
    typedef iUserControlShader *iUserControlShaderMaterialPtr;

} // namespace igor

#endif // IGOR_USERCONTROL_SHADER_H