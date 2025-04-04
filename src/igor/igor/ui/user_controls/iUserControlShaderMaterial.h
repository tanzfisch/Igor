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
// (c) Copyright 2012-2025 by Martin Loga
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

namespace igor
{

    /*! User control shader material chooser
     */
    class IGOR_API iUserControlShaderMaterial : public iUserControl
    {

    public:
        /*! ctor initializes member variables

        \param parent the optional parent
        */
        iUserControlShaderMaterial(const iWidgetPtr parent = nullptr);

        /*! clean up
         */
        ~iUserControlShaderMaterial();

        /*! sets the material id

        \param materialID the given material id
        */
        void setID(iResourceID materialID);

        /*! \returns material id
         */
        iResourceID getID() const;

    private:
        /*! material ID
         */
        iResourceID _materialID;

        /*! label material id
         */
        iWidgetLabelPtr _labelID;

        /*! material alias
         */
        iWidgetLabelPtr _labelAlias;

        /*! displays the selected material
         */
        iWidgetPicturePtr _picture;

        /*! initialize gui elements
         */
        void initGUI();

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

    /*! user control shader material chooser pointer definition
     */
    typedef iUserControlShaderMaterial *iUserControlShaderMaterialPtr;

} // namespace igor

#endif // IGOR_USERCONTROL_SHADER_H