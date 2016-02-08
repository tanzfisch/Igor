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

#ifndef __iNODEPHYSICS__
#define __iNODEPHYSICS__

#include <iNodeVolume.h>
#include <iaMatrix.h>
#include <iPhysicsBody.h>
#include <iMesh.h>

#include <iaVector3.h>
#include <iaMatrix.h>
using namespace IgorAux;

#include <memory>
using namespace std;

namespace Igor
{
    
    class Igor_API iNodePhysics : public iNodeVolume
	{
		
		friend class iNodeVisitorUpdateTransform;
        friend class iNodeFactory;
        friend class iPhysicsManager;
        friend class iPhysicsManagerTask;

    public:

        void clear();
        void addBox(float32 width, float32 height, float32 depth, const iaMatrixf& offset);
        void addSphere(float32 radius, const iaMatrixf& offset);
        void addCone(float32 radius, float32 height, const iaMatrixf& offset);
        void addCapsule(float32 radius, float32 height, const iaMatrixf& offset);
        void addCylinder(float32 radius, float32 height, const iaMatrixf& offset);

        /*! set up mesh for physics mesh

        \param mesh the mesh to make a physics collision from
        \param faceAttribute the faceattribute for the whole mesh
        \param offset off the mesh collision object
        */
        void addMesh(shared_ptr<iMesh> mesh, int64 faceAttribute, const iaMatrixf& offset);

        void addUpVectorJoint(const iaVector3f& upVector);

        float32 getMass() const;
        void setMass(float32 mass);


        void setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate applyForceAndTorqueDelegate);

        void resetForceAndTorqueDelegate();

        uint64 getBody() const;
        
	private:

        struct Box
        {
            float32 _width;
            float32 _height;
            float32 _depth;
            iaMatrixf _offset;
        };

        struct Sphere
        {
            float32 _radius;
            iaMatrixf _offset;
        };

        struct Cone
        {
            float32 _radius;
            float32 _height;
            iaMatrixf _offset;
        };

        struct Capsule
        {
            float32 _radius;
            float32 _height;
            iaMatrixf _offset;
        };

        struct Cylinder
        {
            float32 _radius;
            float32 _height;
            iaMatrixf _offset;
        };

        struct Mesh
        {
            shared_ptr<iMesh> _mesh = nullptr;
            int64 _faceAttribute;
            iaMatrixf _offset;
        };

        vector<Box> _boxes;
        vector<Sphere> _spheres;
        vector<Cone> _cones;
        vector<Capsule> _capsules;
        vector<Cylinder> _cylinders;
        vector<iaVector3f> _upVectorJoints;
        vector<Mesh> _meshs;

        float32 _mass = 0;

        iApplyForceAndTorqueDelegate* _applyForceAndTorqueDelegate = nullptr;


        bool _physicsInitialized = false;

        /*! physics body
        */
        uint64 _bodyID = iPhysicsBody::INVALID_BODY_ID;

        virtual void initPhysics();

        /*! set world matrix

        \param matrix matrix to set
        */
        virtual void onUpdateTransform(iaMatrixf& matrix);

        /*! sets physic body id

        \param bodyID the body id
        */
        void setBody(uint64 bodyID);

        /*! does nothing
        */
        void draw();

        /*! \returns true if physics was already initialized
        */
        bool isInitialized();

        /*! updates physics
        */
        void updatePhysics();

        /*! initializes member variables
        */
        iNodePhysics();

        /*! copy ctor
        */
        iNodePhysics(iNodePhysics* node);

        /*! does nothing
        */
        virtual ~iNodePhysics();

		
	};

};

#endif
