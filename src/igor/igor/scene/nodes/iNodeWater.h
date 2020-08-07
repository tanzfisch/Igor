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

#ifndef __iNODEWATER__
#define __iNODEWATER__

#include <igor/scene/nodes/iNodeRender.h>
#include <igor/resources/texture/iTextureResourceFactory.h>

namespace igor
{

    /*! renders a enless water plane relative to current camera
    */
    class IGOR_API iNodeWater : public iNodeRender
    {

        friend class iNodeManager;

    public:
        /*! \returns true if this node was reached by culling

        the sky box node is always reached!
        */
        virtual bool wasReached();

        /*! draw the sky box
        */
        virtual void draw();

        /*! y position of water plane

        \param yPos the position of the water plane on the Y axis
        */
        void setWaterPosition(float32 yPos);

        /*! \returns y position of water plane
        */
        float32 getWaterPosition() const;

        /*! set ambient color of water plane

        \param color the ambient color
        */
        void setAmbient(const iaColor4f &color);

        /*! \returns ambient color of water plane
        */
        iaColor4f getAmbient() const;

    private:
        /*! y position of water plane
        */
        float32 _yPos = 0;

        /*! ambient color of water plane
        */
        iaColor4f _ambient;

        /*! initializes member variables
        */
        iNodeWater();

        /*! copy ctor
        */
        iNodeWater(iNodeWater *node);

        /*! releases resources
        */
        virtual ~iNodeWater();
    };

} // namespace igor

#endif