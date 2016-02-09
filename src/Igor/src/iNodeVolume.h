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
// (c) Copyright 2014-2015 by Martin Loga
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
	\todo add get/set sphere interface
	*/
	class iNodeVolume : public iNodeRender
	{
		friend class iNodeVisitorUpdateTransform;
		friend class iScene;

    public:

        /*! \returns center of sphere in world coordinates
        */
        iaVector3f getCenter() const;

        /*! \returns center of sphere in local coordinates
        */
        iaVector3f getRelativeCenter() const;

        /*! sets bounding sphere

        \param sphere the sphere to set
        */
        void setBoundingSphere(const iSpheref& sphere);

        /*! \returns bounding sphere
        */
        const Igor_API iSpheref& getBoundingSphere() const;

	protected:

        virtual void onUpdateTransform(iaMatrixf& matrix);

        /*! unregisters from scene
        */
		virtual void onPreSetScene();

        /*! registers to scene
        */
		virtual void onPostSetScene();

        virtual bool onUpdateData();

        /*! updates octree if node is within a scene
        */
		void update();

        /*! initializes memeber variables
        */
		iNodeVolume();

        /*! copy ctor
        */
        iNodeVolume(iNodeVolume* node);

        /*! does nothing
        */
		virtual ~iNodeVolume() = default;

    private:

        /*! sphere of volume
        */
        iSpheref _sphere;

	};

}

#endif