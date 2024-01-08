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

#ifndef __IGOR_NODEVOLUME__
#define __IGOR_NODEVOLUME__

#include <igor/scene/nodes/iNodeRender.h>

#include <igor/data/iAABox.h>

#include <iaux/data/iaSphere.h>

namespace igor
{

    /*! Node that is a render-able and has a volume therefore relevant for the octree structure
	*/
    class iNodeVolume : public iNodeRender
    {

        friend class iScene;

    public:
        /*! \returns center of sphere in world coordinates
        */
        iaVector3d getCenter() const;

        /*! \returns center of sphere in local coordinates
        */
        const iaVector3d &getRelativeCenter() const;

        /*! \returns bounding sphere
        */
        const IGOR_API iaSphered &getBoundingSphere() const;

        /*! sets bounding box

        \param bbox the bounding box
        */
        void setBoundingBox(const iAABoxd &bbox);

        /*! \returns bounding box

        \todo save actual bbox in volume node
        */
        const IGOR_API iAABoxd &getBoundingBox() const;

    protected:
        /*! called by node visitor update transform

        \param[in, out] matrix current transformation matrix
        */
        virtual IGOR_API void onUpdateTransform(iaMatrixd &matrix) override;

        /*! unregisters from scene
        */
        virtual IGOR_API void onPreSetScene() override;

        /*! registers to scene
        */
        virtual IGOR_API void onPostSetScene() override;

        /*! updates octree if node is within a scene
        */
        void updateTree();

        /*! initializes member variables
        */
        iNodeVolume();

        /*! copy ctor
        */
        iNodeVolume(iNodeVolume *node);

        /*! dtor
        */
        virtual ~iNodeVolume();

    private:
        /*! bbox of volume
        */
        iAABoxd _actualbbox;

        /*! bounding sphere
        */
        iaSphered _sphere;

        /*! bbox of content
        */
        iAABoxd _bbox;

        /*! calculates the bbox based on current scale
        */
        void calcActualBBox();
    };

} // namespace igor

#endif // __IGOR_NODEVOLUME__