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
// (c) Copyright 2014-2016 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iNODEVOLUME__
#define __iNODEVOLUME__

#include <iNodeRender.h>
#include <iSphere.h>

namespace Igor
{

	/*! Node that is a renderable and has a volume therefore relevant for the octree structure

    \todo bounding sphere needs to be updated if node was scaled ...
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
        iaVector3d getRelativeCenter() const;

        /*! sets bounding sphere

        \param sphere the sphere to set
        */
        void setBoundingSphere(const iSphered& sphere);

        /*! \returns bounding sphere
        */
        const Igor_API iSphered& getBoundingSphere() const;

	protected:

        /*! called by node visitor update transform

        \param[in, out] matrix current transformation matrix
        */
        virtual void onUpdateTransform(iaMatrixd& matrix);

        /*! unregisters from scene
        */
		virtual void onPreSetScene();

        /*! registers to scene
        */
		virtual void onPostSetScene();

        /*! \todo dirty workaround. need to look in to this once more
        */
        virtual bool onUpdateData();

        /*! updates octree if node is within a scene
        */
		void updateTree();

        /*! initializes memeber variables
        */
		iNodeVolume();

        /*! copy ctor
        */
        iNodeVolume(iNodeVolume* node);

        /*! dtor
        */
		virtual ~iNodeVolume();

    private:

        /*! sphere of volume
        */
        iSphered _sphere;

	};

}

#endif