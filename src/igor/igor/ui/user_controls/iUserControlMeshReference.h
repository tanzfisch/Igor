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

#ifndef IGOR_USERCONTROL_MESH_REFERENCE_H
#define IGOR_USERCONTROL_MESH_REFERENCE_H

#include <igor/ui/user_controls/iUserControl.h>

#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetPicture.h>
#include <igor/resources/texture/iTexture.h>

namespace igor
{

    /*! User control texture chooser
     */
    class IGOR_API iUserControlMeshReference : public iUserControl
    {

    public:
        /*! ctor initializes member variables

        \param parent the optional parent
        */
        iUserControlMeshReference(const iWidgetPtr parent = nullptr);

        /*! clean up
         */
        ~iUserControlMeshReference();

        /*! sets the model id

        \param modelID the given model id
        \param meshPaths the mesh paths to use
        */
        void setReference(const iResourceID &modelID, const std::vector<iaString> &meshPaths);

        /*! \returns texture id
         */        
        iResourceID getModelID() const;

        /*! \returns all mesh paths
        */
        const std::vector<iaString> &getMeshPaths() const;

    private:
        /*! model id
         */
        iResourceID _modelID;

        /*! all mesh paths
        */
        std::vector<iaString> _meshPaths;

        /*! reference path within model
        */
        iWidgetLabelPtr _meshPath;

        /*! label model id
         */
        iWidgetLabelPtr _labelID;

        /*! label alias
         */
        iWidgetLabelPtr _labelAlias;

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

    /*! user control texture chooser pointer definition
     */
    typedef iUserControlMeshReference *iUserControlMeshReferencePtr;

} // namespace igor

#endif // IGOR_USERCONTROL_MESH_REFERENCE_H