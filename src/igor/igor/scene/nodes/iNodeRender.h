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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __iRENDERNODE__
#define __iRENDERNODE__

#include <igor/scene/nodes/iNode.h>
#include <iaux/math/iaMatrix.h>
#include <igor/resources/material/iMaterial.h>

#include <memory>

namespace igor
{

    /*! abstract class for nodes that can render
    */
    class Igor_API iNodeRender : public iNode
    {

        friend class iNodeVisitorUpdateTransform;
        friend class iOctree;
        friend class iRenderEngine;
        friend class iNodeManager;

    public:
        /*! \returns true if node was reached by the culling process and not filtered out
        */
        virtual bool wasReached();

        /*! \returns world matrix of node
        */
        const iaMatrixd &getWorldMatrix() const;

        /*! sets material of node by id

        \param materialID material id
        */
        void setMaterial(uint64 materialID);

        /*! \retruns material id
        */
        uint64 getMaterial();

        /*! sets visibility of this node

        \param visible if true node will be rendered
        */
        void setVisible(bool visible = true);

        /*! \returns true if node is visible
        */
        bool isVisible() const;

        /*! abstract method draw
        */
        virtual void draw() = 0;

    protected:
        /*! world matrix
        */
        iaMatrixd _worldMatrix;

        /*! true if if was not filtered by culling process
        */
        bool _reached = false;

        /*! if node will be rendered
        */
        bool _visible = true;

        /*! called by update transform run

        \parma[in, out] matrix current transform matrix
        */
        virtual void onUpdateTransform(iaMatrixd &matrix) override;

        /*! unregisters from scene
        */
        virtual void onPreSetScene() override;

        /*! register to scene
        */
        virtual void onPostSetScene() override;

        /*! initializes member variables
        */
        iNodeRender();

        /*! copy ctor
        */
        iNodeRender(iNodeRender *node);

        /*! clears relation to material groups
        */
        virtual ~iNodeRender();

    private:
        /*! material id to render with
        */
        uint64 _materialID = iMaterial::INVALID_MATERIAL_ID;
    };

}; // namespace igor

#endif
