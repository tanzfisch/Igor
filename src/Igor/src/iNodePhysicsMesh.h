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

#ifndef __iNODEPHYSICSMESH__
#define __iNODEPHYSICSMESH__

#include <iNodeVolume.h>
#include <iaMatrix.h>

#include <memory>
using namespace std;

namespace Igor
{
    
    class Igor_API iNodePhysicsMesh : public iNodeVolume
	{
		
		friend class iNodeVisitorUpdateTransform;
        friend class iNodeFactory;
        friend class iPhysicsManager;

    public:

        void setMesh(shared_ptr<iMesh> mesh, int64 faceAttribute, const iaMatrixf& offset);

        uint64 getBody() const;
        
	protected:

        /*! set world matrix

        \param matrix matrix to set
        */
        virtual void onUpdateTransform(iaMatrixf& matrix);

    private:

        /*!
        */
        iaMatrixf _offset;

        /*! 
        */
        shared_ptr<iMesh> _mesh = nullptr;

        int64 _faceAttribute = 0;

        /*! physics body
        */
        uint64 _bodyID = 0;

        /*! sets physic body id

        \param bodyID the body id
        */
        void setBody(uint64 bodyID);

        /*! does nothing
        */
        virtual void draw();

        /*! setup physics
        */
        void setupPhysics();

        /*! initializes member variables
        */
        iNodePhysicsMesh();

        /*! copy ctor
        */
        iNodePhysicsMesh(iNodePhysicsMesh* node);

        /*! does nothing
        */
		virtual ~iNodePhysicsMesh();
		
	};

};

#endif
